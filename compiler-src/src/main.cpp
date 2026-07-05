#include "common.hpp"
#include "cxxopts/cxxopts.hpp"
#include "scanner.hpp"
#include "clyth_ast.hpp"
#include "clyth_semantic.hpp"
#include "clyth_lowering_plan.hpp"
#include "clyth_llvm_stub.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <vector>

// ANTLR4 runtime includes.
#include "antlr4-runtime.h"

// Generated from ClythV1.g4.
#include "clyth_antlr_files/ClythV1Lexer.h"
#include "clyth_antlr_files/ClythV1Parser.h"

struct CompilerOptions {
    bool debug_mode = false;
    bool dump_ast_json_stdout = false;
    bool dump_ast_bytecode_stdout = false;
    bool dump_codegen_plan = false;
    bool emit_ir_only = false;
    bool print_ir = false;
    bool emit_ir = false;
    bool emit_all = false;

    std::filesystem::path main_file;
    std::filesystem::path ast_json_file;
    std::filesystem::path ast_bytecode_file;
    std::string output_binary_name = "clyth_program.bin";
    std::filesystem::path runtime_root;
    std::filesystem::path compiler_invocation_path;
    std::vector<std::filesystem::path> included_source_paths;
};


static std::string strip_quotes(std::string value) {
    if (value.size() >= 2 &&
        ((value.front() == '"' && value.back() == '"') ||
         (value.front() == '`' && value.back() == '`'))) {
        return value.substr(1, value.size() - 2);
    }
    return value;
}


static std::filesystem::path locate_runtime_root(const std::filesystem::path& main_file, const std::filesystem::path& compiler_invocation_path) {
    std::vector<std::filesystem::path> candidates;
    std::error_code ec;

    if (const char* env_runtime = std::getenv("CLYTH_RUNTIME_DIR")) {
        candidates.push_back(std::filesystem::path(env_runtime));
    }

    if (!compiler_invocation_path.empty()) {
        std::error_code exe_ec;
        const auto exe_path = std::filesystem::weakly_canonical(compiler_invocation_path, exe_ec);
        const auto exe_dir = (exe_ec ? compiler_invocation_path : exe_path).parent_path();
        candidates.push_back(exe_dir / ".." / "share" / "clyth" / "runtime");
        candidates.push_back(exe_dir / "share" / "clyth" / "runtime");
        candidates.push_back(exe_dir / ".." / "clyth-runtime");
    }

    candidates.push_back(std::filesystem::current_path(ec) / "clyth-runtime");
    candidates.push_back(std::filesystem::current_path(ec) / "share" / "clyth" / "runtime");
    candidates.push_back(main_file.parent_path() / "clyth-runtime");
    candidates.push_back(main_file.parent_path() / ".." / "clyth-runtime");
    candidates.push_back(main_file.parent_path() / ".." / "share" / "clyth" / "runtime");

    for (const auto& candidate : candidates) {
        std::error_code candidate_ec;
        std::filesystem::path normalized = std::filesystem::weakly_canonical(candidate, candidate_ec);
        if (!candidate_ec && std::filesystem::exists(normalized)) {
            return normalized;
        }
    }

    return std::filesystem::path{};
}

static std::filesystem::path resolve_include_path(
    const std::filesystem::path& base_dir,
    const std::filesystem::path& runtime_root,
    std::filesystem::path include_path
) {
    if (include_path.extension().empty()) {
        include_path += ".clyth";
    }

    if (include_path.is_absolute()) {
        return include_path;
    }

    const std::filesystem::path local_candidate = base_dir / include_path;
    if (std::filesystem::exists(local_candidate)) {
        return local_candidate;
    }

    if (!runtime_root.empty()) {
        const std::filesystem::path runtime_candidate = runtime_root / include_path;
        if (std::filesystem::exists(runtime_candidate)) {
            return runtime_candidate;
        }

        // Runtime modules are the source of truth for bundled Clyth runtime APIs.
        // A user-facing include such as `include "dma/dma.clyth"` resolves to
        // `clyth-runtime/modules/module-dma/dma.clyth`.
        auto it = include_path.begin();
        if (it != include_path.end()) {
            const std::string module_name = it->string();
            ++it;
            std::filesystem::path remainder;
            for (; it != include_path.end(); ++it) {
                remainder /= *it;
            }
            if (!module_name.empty() && !remainder.empty()) {
                const std::filesystem::path module_candidate =
                    runtime_root / "modules" / ("module-" + module_name) / remainder;
                if (std::filesystem::exists(module_candidate)) {
                    return module_candidate;
                }
            }
        }
    }

    return local_candidate;
}

static bool path_is_inside(const std::filesystem::path& maybe_child, const std::filesystem::path& maybe_parent) {
    if (maybe_child.empty() || maybe_parent.empty()) {
        return false;
    }

    std::error_code ec_child;
    std::error_code ec_parent;
    const std::filesystem::path child = std::filesystem::weakly_canonical(maybe_child, ec_child);
    const std::filesystem::path parent = std::filesystem::weakly_canonical(maybe_parent, ec_parent);
    if (ec_child || ec_parent) {
        return false;
    }

    auto child_it = child.begin();
    auto parent_it = parent.begin();
    for (; parent_it != parent.end(); ++parent_it, ++child_it) {
        if (child_it == child.end() || *child_it != *parent_it) {
            return false;
        }
    }
    return true;
}

static std::vector<std::filesystem::path> runtime_link_inputs_for(
    const std::filesystem::path& runtime_root,
    const std::vector<std::filesystem::path>& included_paths
) {
    std::vector<std::filesystem::path> inputs;
    if (runtime_root.empty()) {
        return inputs;
    }

    struct RuntimeArchiveRule {
        std::filesystem::path module_source;
        std::filesystem::path archive;
        std::filesystem::path legacy_archive;
    };

    const std::vector<RuntimeArchiveRule> rules {
        {
            runtime_root / "modules" / "module-dma" / "dma.clyth",
            runtime_root / "modules" / "module-dma" / "x86_64" / "libclyth_dma.a",
            runtime_root / "c-bindings" / "dma" / "libclyth_dma.a"
        },
        {
            runtime_root / "modules" / "module-file-io" / "file-io.clyth",
            runtime_root / "modules" / "module-file-io" / "x86_64" / "libclyth_file_io.a",
            runtime_root / "c-bindings" / "file-io" / "x86_64" / "libclyth_file_io.a"
        },
        {
            runtime_root / "modules" / "module-hash" / "hash.clyth",
            runtime_root / "modules" / "module-hash" / "x86_64" / "libclyth_rapidhash.a",
            runtime_root / "c-bindings" / "rapidhash" / "x86_64" / "libclyth_rapidhash.a"
        },
        {
            runtime_root / "modules" / "module-router" / "router.clyth",
            runtime_root / "modules" / "module-router" / "x86_64" / "libclyth_http.a",
            runtime_root / "c-bindings" / "http" / "x86_64" / "libclyth_http.a"
        },
        {
            runtime_root / "modules" / "module-http" / "http.clyth",
            runtime_root / "modules" / "module-http" / "x86_64" / "libclyth_http.a",
            runtime_root / "c-bindings" / "http" / "x86_64" / "libclyth_http.a"
        },
        {
            runtime_root / "modules" / "module-https" / "https.clyth",
            runtime_root / "modules" / "module-https" / "x86_64" / "libclyth_http.a",
            runtime_root / "c-bindings" / "http" / "x86_64" / "libclyth_http.a"
        },
        {
            runtime_root / "modules" / "module-json" / "json.clyth",
            runtime_root / "modules" / "module-json" / "x86_64" / "libclyth_json.a",
            runtime_root / "c-bindings" / "yyjson" / "x86_64" / "libclyth_json.a"
        },
        {
            runtime_root / "modules" / "module-concurrency" / "concurrency.clyth",
            runtime_root / "modules" / "module-concurrency" / "x86_64" / "libclyth_concurrency.a",
            runtime_root / "c-bindings" / "concurrency" / "x86_64" / "libclyth_concurrency.a"
        }
    };

    std::set<std::filesystem::path> pushed;
    for (const auto& rule : rules) {
        std::error_code module_ec;
        const auto module_canonical = std::filesystem::weakly_canonical(rule.module_source, module_ec);
        if (module_ec) {
            continue;
        }

        bool included_module = false;
        for (const auto& included : included_paths) {
            std::error_code included_ec;
            const auto included_canonical = std::filesystem::weakly_canonical(included, included_ec);
            if (!included_ec && included_canonical == module_canonical) {
                included_module = true;
                break;
            }
        }

        if (!included_module) {
            continue;
        }

        std::filesystem::path selected_archive;
        if (std::filesystem::exists(rule.archive)) {
            selected_archive = rule.archive;
        } else if (std::filesystem::exists(rule.legacy_archive)) {
            selected_archive = rule.legacy_archive;
        }

        if (selected_archive.empty()) {
            fmt::print(stderr, "ERROR: runtime module '{}' requires missing archive '{}'. Run ./build_compiler.sh first.\n",
                       rule.module_source.string(), rule.archive.string());
            continue;
        }

        std::error_code archive_ec;
        const auto archive_canonical = std::filesystem::weakly_canonical(selected_archive, archive_ec);
        const auto archive_key = archive_ec ? selected_archive.lexically_normal() : archive_canonical;
        if (pushed.insert(archive_key).second) {
            inputs.push_back(selected_archive);
        }
    }

    return inputs;
}


static bool is_runtime_package_include_name(const std::string& include_target) {
    if (include_target.empty()) {
        return false;
    }
    if (include_target.find('/') != std::string::npos || include_target.find('\\') != std::string::npos) {
        return false;
    }
    if (include_target.find('.') != std::string::npos) {
        return false;
    }
    return true;
}

static std::vector<std::string> parse_json_string_array_field(const std::string& json, const std::string& field_name) {
    std::vector<std::string> values;
    const std::regex field_pattern("\\\"" + field_name + "\\\"\\s*:\\s*\\[([^\\]]*)\\]");
    std::smatch field_match;
    if (!std::regex_search(json, field_match, field_pattern)) {
        return values;
    }

    const std::string body = field_match[1].str();
    const std::regex string_pattern("\"([^\"]+)\"");
    for (auto it = std::sregex_iterator(body.begin(), body.end(), string_pattern);
         it != std::sregex_iterator(); ++it) {
        values.push_back((*it)[1].str());
    }
    return values;
}

static bool collect_runtime_module_sources(
    const std::filesystem::path& runtime_root,
    const std::string& module_name,
    std::set<std::string>& active_modules,
    std::set<std::string>& resolved_modules,
    std::vector<std::filesystem::path>& source_paths
) {
    if (runtime_root.empty()) {
        fmt::print(stderr, "ERROR: Runtime root is not available while resolving module '{}'.\n", module_name);
        return false;
    }
    if (resolved_modules.count(module_name) > 0) {
        return true;
    }
    if (active_modules.count(module_name) > 0) {
        fmt::print(stderr, "ERROR: Runtime module dependency cycle detected at '{}'.\n", module_name);
        return false;
    }

    const std::filesystem::path module_dir = runtime_root / "modules" / ("module-" + module_name);
    const std::filesystem::path manifest_path = module_dir / "module.json";
    if (!std::filesystem::exists(manifest_path)) {
        fmt::print(stderr, "ERROR: Runtime module '{}' was not found at '{}'.\n", module_name, manifest_path.string());
        return false;
    }

    Scanner manifest_scanner;
    if (!manifest_scanner.read_file(manifest_path.string())) {
        fmt::print(stderr, "ERROR: Failed to read runtime module manifest: {}\n", manifest_path.string());
        return false;
    }

    active_modules.insert(module_name);
    const std::string manifest = manifest_scanner.to_string();
    for (const auto& dependency : parse_json_string_array_field(manifest, "dependencies")) {
        if (!collect_runtime_module_sources(runtime_root, dependency, active_modules, resolved_modules, source_paths)) {
            return false;
        }
    }

    const auto exports = parse_json_string_array_field(manifest, "exports");
    if (exports.empty()) {
        fmt::print(stderr, "ERROR: Runtime module '{}' has no exports in '{}'.\n", module_name, manifest_path.string());
        return false;
    }

    for (const auto& export_file : exports) {
        std::filesystem::path export_path = module_dir / export_file;
        if (export_path.extension().empty()) {
            export_path += ".clyth";
        }
        if (!std::filesystem::exists(export_path)) {
            fmt::print(stderr, "ERROR: Runtime module '{}' export '{}' was not found at '{}'.\n",
                       module_name, export_file, export_path.string());
            return false;
        }
        source_paths.push_back(export_path);
    }

    active_modules.erase(module_name);
    resolved_modules.insert(module_name);
    return true;
}

static bool read_clyth_source_with_includes_impl(
    const std::filesystem::path& file,
    const std::filesystem::path& runtime_root,
    std::set<std::filesystem::path>& active_stack,
    std::set<std::filesystem::path>& already_included,
    std::vector<std::filesystem::path>& included_paths,
    std::ostringstream& output
) {
    std::error_code ec;
    const std::filesystem::path canonical = std::filesystem::weakly_canonical(file, ec);
    const std::filesystem::path normalized = ec ? file.lexically_normal() : canonical;

    if (active_stack.count(normalized) > 0) {
        fmt::print(stderr, "ERROR: include cycle detected at {}\n", normalized.string());
        return false;
    }

    if (already_included.count(normalized) > 0) {
        return true;
    }

    Scanner scanner;
    if (!scanner.read_file(normalized.string())) {
        fmt::print(stderr, "ERROR: Failed to read source file: {}\n", normalized.string());
        return false;
    }

    active_stack.insert(normalized);
    already_included.insert(normalized);
    included_paths.push_back(normalized);

    const std::filesystem::path base_dir = normalized.parent_path();
    const std::regex include_pattern(R"(^\s*include\s+([^;\s]+)\s*;?\s*$)");
    std::istringstream input(scanner.to_string());
    std::string line;

    output << "// begin include unit: " << normalized.string() << "\n";

    while (std::getline(input, line)) {
        std::smatch match;
        if (std::regex_match(line, match, include_pattern)) {
            std::string include_target = strip_quotes(match[1].str());

            std::filesystem::path local_probe = base_dir / std::filesystem::path(include_target);
            if (local_probe.extension().empty()) {
                local_probe += ".clyth";
            }
            const bool local_include_exists = std::filesystem::exists(local_probe);
            const bool runtime_package_exists = !runtime_root.empty()
                && std::filesystem::exists(runtime_root / "modules" / ("module-" + include_target) / "module.json");

            if (!local_include_exists && is_runtime_package_include_name(include_target) && runtime_package_exists) {
                std::set<std::string> active_modules;
                std::set<std::string> resolved_modules;
                std::vector<std::filesystem::path> module_sources;
                if (!collect_runtime_module_sources(runtime_root, include_target, active_modules, resolved_modules, module_sources)) {
                    return false;
                }
                for (const auto& module_source : module_sources) {
                    if (!read_clyth_source_with_includes_impl(module_source, runtime_root, active_stack, already_included, included_paths, output)) {
                        return false;
                    }
                }
                continue;
            }

            std::filesystem::path include_path = resolve_include_path(base_dir, runtime_root, std::filesystem::path(include_target));

            if (!read_clyth_source_with_includes_impl(include_path, runtime_root, active_stack, already_included, included_paths, output)) {
                return false;
            }
            continue;
        }

        output << line << "\n";
    }

    output << "// end include unit: " << normalized.string() << "\n";
    active_stack.erase(normalized);
    return true;
}


struct StructInlineDesugarResult {
    bool ok = true;
    std::string source;
};

static bool is_ident_start(char ch) {
    return std::isalpha(static_cast<unsigned char>(ch)) || ch == '_';
}

static bool is_ident_continue(char ch) {
    return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_';
}

static std::size_t skip_ws(const std::string& source, std::size_t pos) {
    while (pos < source.size() && std::isspace(static_cast<unsigned char>(source[pos]))) {
        ++pos;
    }
    return pos;
}

static bool is_position_in_comment_or_string(const std::string& source, std::size_t pos) {
    bool in_string = false;
    bool escaped = false;
    bool in_line_comment = false;
    bool in_block_comment = false;

    for (std::size_t i = 0; i < pos && i < source.size(); ++i) {
        const char ch = source[i];
        const char next = (i + 1 < source.size()) ? source[i + 1] : '\0';

        if (in_line_comment) {
            if (ch == '\n' || ch == '\r') {
                in_line_comment = false;
            }
            continue;
        }

        if (in_block_comment) {
            if (ch == '*' && next == '/') {
                in_block_comment = false;
                ++i;
            }
            continue;
        }

        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }

        if (ch == '/' && next == '/') {
            in_line_comment = true;
            ++i;
            continue;
        }
        if (ch == '/' && next == '*') {
            in_block_comment = true;
            ++i;
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
    }

    return in_string || in_line_comment || in_block_comment;
}

static bool starts_keyword_at(const std::string& source, std::size_t pos, const std::string& keyword) {
    if (pos + keyword.size() > source.size() || source.compare(pos, keyword.size(), keyword) != 0) {
        return false;
    }
    if (is_position_in_comment_or_string(source, pos)) {
        return false;
    }
    const bool left_ok = pos == 0 || !is_ident_continue(source[pos - 1]);
    const bool right_ok = pos + keyword.size() >= source.size() || !is_ident_continue(source[pos + keyword.size()]);
    return left_ok && right_ok;
}

static std::size_t find_matching_brace(const std::string& source, std::size_t open_pos) {
    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (std::size_t i = open_pos; i < source.size(); ++i) {
        const char ch = source[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '{') {
            ++depth;
        } else if (ch == '}') {
            if (depth == 0) {
                return std::string::npos;
            }
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }

    return std::string::npos;
}

static std::string trim_copy(std::string text) {
    auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), not_space));
    text.erase(std::find_if(text.rbegin(), text.rend(), not_space).base(), text.end());
    return text;
}

static std::vector<std::string> split_struct_body_items(const std::string& body) {
    std::vector<std::string> items;
    std::size_t start = 0;
    std::size_t depth_paren = 0;
    std::size_t depth_brace = 0;
    std::size_t depth_bracket = 0;
    bool in_string = false;
    bool escaped = false;

    auto push_item_until = [&](std::size_t end_exclusive) {
        std::string item = trim_copy(body.substr(start, end_exclusive - start));
        if (!item.empty()) {
            items.push_back(item);
        }
        start = end_exclusive;
    };

    for (std::size_t i = 0; i < body.size(); ++i) {
        const char ch = body[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '(') { ++depth_paren; continue; }
        if (ch == ')' && depth_paren > 0) { --depth_paren; continue; }
        if (ch == '[') { ++depth_bracket; continue; }
        if (ch == ']' && depth_bracket > 0) { --depth_bracket; continue; }
        if (ch == '{') { ++depth_brace; continue; }
        if (ch == '}' && depth_brace > 0) {
            --depth_brace;
            if (depth_brace == 0 && depth_paren == 0 && depth_bracket == 0) {
                push_item_until(i + 1);
            }
            continue;
        }

        if (depth_paren == 0 && depth_brace == 0 && depth_bracket == 0) {
            if (ch == ';' || ch == ',') {
                push_item_until(i + 1);
                continue;
            }

            if (ch == '\n') {
                // Clyth permits semicolon-less field declarations. A field inside
                // a struct is usually a single top-level line like `int32 value` or
                // `private int32 value`. Method signatures include `(` and are
                // completed when their body-closing brace is seen above.
                std::string candidate = trim_copy(body.substr(start, i - start));
                if (!candidate.empty() && candidate.find('(') == std::string::npos) {
                    items.push_back(candidate);
                    start = i + 1;
                }
            }
        }
    }

    std::string tail = trim_copy(body.substr(start));
    if (!tail.empty()) {
        items.push_back(tail);
    }

    return items;
}

static bool looks_like_inline_method(const std::string& item) {
    const auto paren = item.find('(');
    const auto brace = item.find('{');
    if (paren == std::string::npos || brace == std::string::npos || paren > brace) {
        return false;
    }
    return item.find('}') != std::string::npos;
}

static std::string strip_visibility_prefix(std::string item) {
    std::string trimmed = trim_copy(item);
    if (starts_keyword_at(trimmed, 0, "public")) {
        return trim_copy(trimmed.substr(6));
    }
    if (starts_keyword_at(trimmed, 0, "private")) {
        // Alpha 0.2 accepts the spelling so code can be written in the intended
        // style. Visibility enforcement is intentionally a semantic pass later.
        return trim_copy(trimmed.substr(7));
    }
    return item;
}

static std::size_t find_matching_paren(const std::string& source, std::size_t open_pos);

static std::size_t count_top_level_parameters_text(const std::string& params) {
    const std::string trimmed = trim_copy(params);
    if (trimmed.empty()) {
        return 0;
    }
    std::size_t count = 1;
    int depth = 0;
    for (char ch : trimmed) {
        if (ch == '<' || ch == '(' || ch == '[') { ++depth; continue; }
        if (ch == '>' || ch == ')' || ch == ']') { --depth; continue; }
        if (ch == ',' && depth == 0) { ++count; }
    }
    return count;
}

static std::string normalize_constructor_item(std::string item) {
    std::string trimmed = trim_copy(item);
    if (starts_keyword_at(trimmed, 0, "constructor")) {
        const std::size_t open = trimmed.find('(');
        const std::size_t close = open == std::string::npos ? std::string::npos : find_matching_paren(trimmed, open);
        const std::size_t arity = (open == std::string::npos || close == std::string::npos)
            ? 0
            : count_top_level_parameters_text(trimmed.substr(open + 1, close - open - 1));
        if (arity == 0) {
            return "void " + trimmed;
        }
        return "void constructor__" + std::to_string(arity) + trimmed.substr(open);
    }
    if (starts_keyword_at(trimmed, 0, "destructor")) {
        return "void " + trimmed;
    }
    return item;
}


static bool has_top_level_colon(const std::string& text) {
    std::size_t depth_paren = 0;
    std::size_t depth_brace = 0;
    std::size_t depth_bracket = 0;
    bool in_string = false;
    bool escaped = false;

    for (char ch : text) {
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '(') { ++depth_paren; continue; }
        if (ch == ')' && depth_paren > 0) { --depth_paren; continue; }
        if (ch == '{') { ++depth_brace; continue; }
        if (ch == '}' && depth_brace > 0) { --depth_brace; continue; }
        if (ch == '[') { ++depth_bracket; continue; }
        if (ch == ']' && depth_bracket > 0) { --depth_bracket; continue; }
        if (ch == ':' && depth_paren == 0 && depth_brace == 0 && depth_bracket == 0) {
            return true;
        }
    }

    return false;
}


static std::size_t find_matching_paren(const std::string& source, std::size_t open_pos) {
    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (std::size_t i = open_pos; i < source.size(); ++i) {
        const char ch = source[i];
        if (in_string) {
            if (escaped) { escaped = false; }
            else if (ch == '\\') { escaped = true; }
            else if (ch == '"') { in_string = false; }
            continue;
        }
        if (ch == '"') { in_string = true; continue; }
        if (ch == '(') { ++depth; }
        else if (ch == ')') {
            if (depth == 0) { return std::string::npos; }
            --depth;
            if (depth == 0) { return i; }
        }
    }
    return std::string::npos;
}

static std::size_t find_matching_bracket(const std::string& source, std::size_t open_pos) {
    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (std::size_t i = open_pos; i < source.size(); ++i) {
        const char ch = source[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '[') {
            ++depth;
        } else if (ch == ']') {
            if (depth == 0) {
                return std::string::npos;
            }
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }

    return std::string::npos;
}


static std::size_t find_matching_angle(const std::string& source, std::size_t open_pos) {
    std::size_t depth = 0;
    bool in_string = false;
    bool escaped = false;

    for (std::size_t i = open_pos; i < source.size(); ++i) {
        const char ch = source[i];
        if (in_string) {
            if (escaped) {
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            continue;
        }
        if (ch == '<') {
            ++depth;
        } else if (ch == '>') {
            if (depth == 0) {
                return std::string::npos;
            }
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }

    return std::string::npos;
}

static bool looks_like_generic_parameter_list(const std::string& text) {
    std::string trimmed = trim_copy(text);
    if (trimmed.empty()) {
        return false;
    }

    std::size_t start = 0;
    while (start < trimmed.size()) {
        start = skip_ws(trimmed, start);
        if (start >= trimmed.size() || !is_ident_start(trimmed[start])) {
            return false;
        }
        while (start < trimmed.size() && is_ident_continue(trimmed[start])) {
            ++start;
        }
        start = skip_ws(trimmed, start);
        if (start >= trimmed.size()) {
            return true;
        }
        if (trimmed[start] != ',') {
            return false;
        }
        ++start;
    }
    return false;
}



struct GenericStructTemplateInfo {
    std::string name;
    std::vector<std::string> parameters;
    std::string body;
};

struct ConcreteGenericUseInfo {
    std::string template_name;
    std::vector<std::string> arguments;
};

static std::vector<std::string> split_generic_parameter_names(const std::string& text) {
    std::vector<std::string> params;
    std::string current;
    for (char ch : text) {
        if (ch == ',') {
            const std::string trimmed = trim_copy(current);
            if (!trimmed.empty()) {
                params.push_back(trimmed);
            }
            current.clear();
            continue;
        }
        current.push_back(ch);
    }
    const std::string trimmed = trim_copy(current);
    if (!trimmed.empty()) {
        params.push_back(trimmed);
    }
    return params;
}

static std::vector<GenericStructTemplateInfo> collect_generic_struct_templates(const std::string& source) {
    std::vector<GenericStructTemplateInfo> templates;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (!starts_keyword_at(source, pos, "struct")) {
            ++pos;
            continue;
        }

        std::size_t cursor = skip_ws(source, pos + 6);
        if (cursor >= source.size() || !is_ident_start(source[cursor])) {
            ++pos;
            continue;
        }

        const std::size_t name_start = cursor;
        while (cursor < source.size() && is_ident_continue(source[cursor])) {
            ++cursor;
        }
        const std::string struct_name = source.substr(name_start, cursor - name_start);

        cursor = skip_ws(source, cursor);
        if (cursor >= source.size() || source[cursor] != '<') {
            ++pos;
            continue;
        }

        const std::size_t angle_open = cursor;
        const std::size_t angle_close = find_matching_angle(source, angle_open);
        if (angle_close == std::string::npos) {
            ++pos;
            continue;
        }

        const std::string params_text = source.substr(angle_open + 1, angle_close - angle_open - 1);
        if (!looks_like_generic_parameter_list(params_text)) {
            ++pos;
            continue;
        }

        cursor = skip_ws(source, angle_close + 1);
        if (cursor >= source.size() || source[cursor] != '{') {
            ++pos;
            continue;
        }

        const std::size_t open_brace = cursor;
        const std::size_t close_brace = find_matching_brace(source, open_brace);
        if (close_brace == std::string::npos) {
            ++pos;
            continue;
        }

        templates.push_back(GenericStructTemplateInfo{
            struct_name,
            split_generic_parameter_names(params_text),
            source.substr(open_brace + 1, close_brace - open_brace - 1),
        });

        pos = close_brace + 1;
    }

    return templates;
}

static bool is_known_generic_template_name(
    const std::vector<GenericStructTemplateInfo>& templates,
    const std::string& name,
    std::size_t* parameter_count = nullptr
) {
    for (const auto& templ : templates) {
        if (templ.name == name) {
            if (parameter_count != nullptr) {
                *parameter_count = templ.parameters.size();
            }
            return true;
        }
    }
    return false;
}

static std::string join_strings(const std::vector<std::string>& values, const std::string& separator) {
    std::ostringstream out;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            out << separator;
        }
        out << values[i];
    }
    return out.str();
}


static std::string sanitize_generic_type_fragment_for_symbol(std::string value) {
    for (char& ch : value) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            ch = '_';
        }
    }
    return value;
}

static std::vector<std::string> split_top_level_generic_arguments_main(const std::string& text) {
    std::vector<std::string> result;
    std::string current;
    int depth = 0;
    bool in_string = false;

    for (char ch : text) {
        if (in_string) {
            current.push_back(ch);
            if (ch == '"') {
                in_string = false;
            }
            continue;
        }
        if (ch == '"') {
            in_string = true;
            current.push_back(ch);
            continue;
        }
        if (ch == '<' || ch == '[' || ch == '(') {
            ++depth;
            current.push_back(ch);
            continue;
        }
        if (ch == '>' || ch == ']' || ch == ')') {
            --depth;
            current.push_back(ch);
            continue;
        }
        if ((ch == ',' || ch == ':') && depth == 0) {
            const std::string trimmed = trim_copy(current);
            if (!trimmed.empty()) {
                result.push_back(trimmed);
            }
            current.clear();
            continue;
        }
        current.push_back(ch);
    }

    const std::string trimmed = trim_copy(current);
    if (!trimmed.empty()) {
        result.push_back(trimmed);
    }
    return result;
}

static std::string stable_generic_concrete_name(const std::string& template_name, const std::vector<std::string>& args) {
    std::ostringstream out;
    out << template_name;
    for (const auto& arg : args) {
        out << "__" << sanitize_generic_type_fragment_for_symbol(arg);
    }
    return out.str();
}

static std::string substitute_generic_parameters(
    std::string text,
    const std::vector<std::string>& params,
    const std::vector<std::string>& args
) {
    const std::size_t count = std::min(params.size(), args.size());
    for (std::size_t i = 0; i < count; ++i) {
        const std::regex param_pattern("\\b" + params[i] + "\\b");
        text = std::regex_replace(text, param_pattern, args[i]);
    }
    return text;
}

static bool generic_args_are_template_parameters(
    const GenericStructTemplateInfo& templ,
    const std::vector<std::string>& args
) {
    for (const auto& arg : args) {
        for (const auto& param : templ.parameters) {
            if (trim_copy(arg) == param) {
                return true;
            }
        }
    }
    return false;
}

static const GenericStructTemplateInfo* find_generic_template(
    const std::vector<GenericStructTemplateInfo>& templates,
    const std::string& name
) {
    for (const auto& templ : templates) {
        if (templ.name == name) {
            return &templ;
        }
    }
    return nullptr;
}

static std::vector<ConcreteGenericUseInfo> collect_concrete_generic_uses(
    const std::string& source,
    const std::vector<GenericStructTemplateInfo>& templates
) {
    std::vector<ConcreteGenericUseInfo> uses;
    std::set<std::string> seen;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (!is_ident_start(source[pos])) {
            ++pos;
            continue;
        }

        const std::size_t name_start = pos;
        while (pos < source.size() && is_ident_continue(source[pos])) {
            ++pos;
        }
        const std::string name = source.substr(name_start, pos - name_start);

        const GenericStructTemplateInfo* templ = find_generic_template(templates, name);
        if (templ == nullptr) {
            continue;
        }

        std::size_t cursor = skip_ws(source, pos);
        if (cursor >= source.size() || source[cursor] != '<') {
            continue;
        }

        const std::size_t close = find_matching_angle(source, cursor);
        if (close == std::string::npos) {
            continue;
        }

        const std::string inner = source.substr(cursor + 1, close - cursor - 1);
        std::vector<std::string> args = split_top_level_generic_arguments_main(inner);
        if (args.size() != templ->parameters.size()) {
            pos = close + 1;
            continue;
        }

        for (auto& arg : args) {
            arg = trim_copy(arg);
        }

        // A template declaration such as `struct Box<T>` or a generic template body
        // reference such as `List<T>` is not a concrete instantiation. Concrete
        // monomorphization starts only when all generic parameters have been
        // replaced by real types, e.g. `Box<int32>`.
        if (generic_args_are_template_parameters(*templ, args)) {
            pos = close + 1;
            continue;
        }

        const std::string key = name + "<" + join_strings(args, ",") + ">";
        if (seen.insert(key).second) {
            uses.push_back(ConcreteGenericUseInfo{name, args});
        }
        pos = close + 1;
    }

    return uses;
}

static std::string build_generic_instantiation_manifest_comment(
    const std::vector<GenericStructTemplateInfo>& templates,
    const std::vector<ConcreteGenericUseInfo>& uses
) {
    if (uses.empty()) {
        return {};
    }

    std::ostringstream out;
    out << "\n// Alpha 0.4 pass 5A generic instantiation manifest.\n";
    out << "// Concrete runtime generic uses detected before template stripping.\n";

    for (const auto& use : uses) {
        const GenericStructTemplateInfo* templ = nullptr;
        for (const auto& candidate : templates) {
            if (candidate.name == use.template_name) {
                templ = &candidate;
                break;
            }
        }
        if (templ == nullptr) {
            continue;
        }

        const std::string concrete_name = stable_generic_concrete_name(use.template_name, use.arguments);
        out << "// instantiate " << use.template_name << "<" << join_strings(use.arguments, ", ")
            << "> -> " << concrete_name << "\n";

        const std::string substituted_body = substitute_generic_parameters(templ->body, templ->parameters, use.arguments);
        std::istringstream body_stream(substituted_body);
        std::string line;
        out << "// preview struct " << concrete_name << " {\n";
        while (std::getline(body_stream, line)) {
            out << "// " << line << "\n";
        }
        out << "// }\n";
    }

    return out.str();
}

static std::string append_generic_instantiation_manifest(const std::string& source) {
    const auto templates = collect_generic_struct_templates(source);
    if (templates.empty()) {
        return source;
    }
    const auto uses = collect_concrete_generic_uses(source, templates);
    const std::string manifest = build_generic_instantiation_manifest_comment(templates, uses);
    if (manifest.empty()) {
        return source;
    }
    return source + manifest;
}


static std::string defer_generic_struct_templates_for_legacy_backend(const std::string& source);

static std::string replace_generic_type_applications_with_concrete_names(
    const std::string& source,
    const std::vector<GenericStructTemplateInfo>& templates,
    const std::vector<ConcreteGenericUseInfo>& uses
) {
    if (templates.empty() || uses.empty()) {
        return source;
    }

    std::map<std::string, std::string> concrete_names;
    for (const auto& use : uses) {
        concrete_names[use.template_name + "<" + join_strings(use.arguments, ",") + ">"] =
            stable_generic_concrete_name(use.template_name, use.arguments);
    }

    std::ostringstream out;
    std::size_t pos = 0;
    while (pos < source.size()) {
        if (!is_ident_start(source[pos])) {
            out << source[pos++];
            continue;
        }

        const std::size_t name_start = pos;
        while (pos < source.size() && is_ident_continue(source[pos])) {
            ++pos;
        }
        const std::string name = source.substr(name_start, pos - name_start);

        const GenericStructTemplateInfo* templ = find_generic_template(templates, name);
        if (templ == nullptr) {
            out << name;
            continue;
        }

        std::size_t cursor = skip_ws(source, pos);
        if (cursor >= source.size() || source[cursor] != '<') {
            out << name;
            continue;
        }

        const std::size_t close = find_matching_angle(source, cursor);
        if (close == std::string::npos) {
            out << name;
            continue;
        }

        const std::string inner = source.substr(cursor + 1, close - cursor - 1);
        std::vector<std::string> args = split_top_level_generic_arguments_main(inner);
        if (args.size() != templ->parameters.size()) {
            out << name;
            continue;
        }
        for (auto& arg : args) {
            arg = trim_copy(arg);
        }

        if (generic_args_are_template_parameters(*templ, args)) {
            out << name;
            continue;
        }

        const std::string key = name + "<" + join_strings(args, ",") + ">";
        const auto found = concrete_names.find(key);
        if (found == concrete_names.end()) {
            out << name;
            continue;
        }

        out << found->second;
        pos = close + 1;
    }

    return out.str();
}


static std::string normalize_generic_instantiated_body_for_backend(const std::string& body) {
    // The frontend accepts visibility markers inside structs, but the current
    // legacy struct-field parser consumes concrete struct bodies after generic
    // instantiation. Normalize instantiated template bodies the same way the
    // inline-struct desugar path normalizes ordinary struct members: visibility
    // is a semantic concern later, not syntax the current backend should lower.
    std::ostringstream out;
    std::istringstream in(body);
    std::string line;
    while (std::getline(in, line)) {
        const std::string trimmed = trim_copy(line);
        if (trimmed.empty()) {
            out << "\n";
            continue;
        }
        const std::string normalized = strip_visibility_prefix(trimmed);
        out << "    " << normalized << "\n";
    }
    return out.str();
}

static std::string instantiate_generic_struct_templates_for_current_backend(const std::string& source) {
    const auto templates = collect_generic_struct_templates(source);
    if (templates.empty()) {
        return source;
    }

    std::vector<ConcreteGenericUseInfo> uses = collect_concrete_generic_uses(source, templates);
    std::set<std::string> seen;
    for (const auto& use : uses) {
        seen.insert(use.template_name + "<" + join_strings(use.arguments, ",") + ">");
    }

    bool changed = true;
    while (changed) {
        changed = false;
        const std::size_t current_count = uses.size();
        for (std::size_t i = 0; i < current_count; ++i) {
            const auto& use = uses[i];
            const GenericStructTemplateInfo* templ = find_generic_template(templates, use.template_name);
            if (templ == nullptr) {
                continue;
            }
            const std::string substituted_body = substitute_generic_parameters(templ->body, templ->parameters, use.arguments);
            auto nested_uses = collect_concrete_generic_uses(substituted_body, templates);
            for (const auto& nested : nested_uses) {
                const std::string key = nested.template_name + "<" + join_strings(nested.arguments, ",") + ">";
                if (seen.insert(key).second) {
                    uses.push_back(nested);
                    changed = true;
                }
            }
        }
    }

    std::ostringstream generated;
    generated << "\n// Alpha 0.4 primitive-first generic instantiation output.\n";

    for (const auto& use : uses) {
        const GenericStructTemplateInfo* templ = find_generic_template(templates, use.template_name);
        if (templ == nullptr) {
            continue;
        }

        const std::string concrete_name = stable_generic_concrete_name(use.template_name, use.arguments);
        std::string substituted_body = substitute_generic_parameters(templ->body, templ->parameters, use.arguments);
        substituted_body = replace_generic_type_applications_with_concrete_names(substituted_body, templates, uses);
        const std::string backend_body = normalize_generic_instantiated_body_for_backend(substituted_body);
        generated << "\nstruct " << concrete_name << " {\n";
        generated << backend_body;
        if (!backend_body.empty() && backend_body.back() != '\n') {
            generated << "\n";
        }
        generated << "}\n";
    }

    std::string rewritten = replace_generic_type_applications_with_concrete_names(source, templates, uses);
    rewritten = defer_generic_struct_templates_for_legacy_backend(rewritten);

    if (uses.empty()) {
        return rewritten;
    }
    return generated.str() + "\n" + rewritten;
}

static std::string defer_generic_struct_templates_for_legacy_backend(const std::string& source) {
    // Alpha 0.4 transition bridge: generic templates are now a semantic/AST
    // concept. The current LLVM backend is still concrete-only, so generic
    // template declarations are deferred after concrete instantiations are
    // generated. This is the remaining legacy bridge, not the long-term generic
    // architecture.
    std::ostringstream out;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (!starts_keyword_at(source, pos, "struct")) {
            out << source[pos++];
            continue;
        }

        const std::size_t struct_start = pos;
        std::size_t cursor = skip_ws(source, pos + 6);
        if (cursor >= source.size() || !is_ident_start(source[cursor])) {
            out << source[pos++];
            continue;
        }

        const std::size_t name_start = cursor;
        while (cursor < source.size() && is_ident_continue(source[cursor])) {
            ++cursor;
        }
        const std::string struct_name = source.substr(name_start, cursor - name_start);

        cursor = skip_ws(source, cursor);
        if (cursor >= source.size() || source[cursor] != '<') {
            out << source[pos++];
            continue;
        }

        const std::size_t angle_open = cursor;
        const std::size_t angle_close = find_matching_angle(source, angle_open);
        if (angle_close == std::string::npos) {
            out << source[pos++];
            continue;
        }

        const std::string params = trim_copy(source.substr(angle_open + 1, angle_close - angle_open - 1));
        cursor = skip_ws(source, angle_close + 1);
        if (cursor >= source.size() || source[cursor] != '{') {
            out << source[pos++];
            continue;
        }

        const std::size_t close_brace = find_matching_brace(source, cursor);
        if (close_brace == std::string::npos) {
            out << source[pos++];
            continue;
        }

        std::size_t after = close_brace + 1;
        if (after < source.size() && source[after] == ';') {
            ++after;
        }

        out << "\n// generic template accepted for runtime instantiation: struct "
            << struct_name << "<" << params << ">\n";
        pos = after;
        (void)struct_start;
    }

    return out.str();
}

static std::string normalize_keyed_array_literals(const std::string& source) {
    std::ostringstream out;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (source[pos] != '[') {
            out << source[pos++];
            continue;
        }

        const std::size_t close = find_matching_bracket(source, pos);
        if (close == std::string::npos) {
            out << source[pos++];
            continue;
        }

        const std::string inner = source.substr(pos + 1, close - pos - 1);
        if (has_top_level_colon(inner)) {
            // Parser-compatible internal representation for keyed arrays.
            // User-facing source remains `[key: value, ...]`; the parser already
            // understands `{key: value, ...}` as a neutral key/value collection.
            out << '{' << inner << '}';
        } else {
            out << '[' << inner << ']';
        }
        pos = close + 1;
    }

    return out.str();
}

static StructInlineDesugarResult desugar_inline_struct_methods(const std::string& source) {
    StructInlineDesugarResult result;
    std::ostringstream out;
    std::size_t pos = 0;

    while (pos < source.size()) {
        if (!starts_keyword_at(source, pos, "struct")) {
            out << source[pos++];
            continue;
        }

        const std::size_t struct_start = pos;
        std::size_t cursor = skip_ws(source, pos + 6);
        if (cursor >= source.size() || !is_ident_start(source[cursor])) {
            out << source[pos++];
            continue;
        }

        const std::size_t name_start = cursor;
        while (cursor < source.size() && is_ident_continue(source[cursor])) {
            ++cursor;
        }
        const std::string struct_name = source.substr(name_start, cursor - name_start);
        cursor = skip_ws(source, cursor);

        // Generic struct templates are now owned by the AST/semantic template
        // pipeline. Do not run the legacy inline-method desugar over them: that
        // path only understands concrete `struct Name { ... }` declarations and
        // will corrupt `struct Name<T> { ... }` by streaming one character at a
        // time until the final `}` is left behind for the parser.
        //
        // Generic template methods will be supported by the template
        // instantiation pipeline later. For now, keep the entire generic struct
        // declaration unchanged so the parser can produce GenericStructDecl.
        if (cursor < source.size() && source[cursor] == '<') {
            const std::size_t angle_close = find_matching_angle(source, cursor);
            if (angle_close == std::string::npos) {
                result.ok = false;
                result.source = source;
                return result;
            }

            std::size_t generic_cursor = skip_ws(source, angle_close + 1);
            if (generic_cursor >= source.size() || source[generic_cursor] != '{') {
                out << source[pos++];
                continue;
            }

            const std::size_t generic_close_brace = find_matching_brace(source, generic_cursor);
            if (generic_close_brace == std::string::npos) {
                result.ok = false;
                result.source = source;
                return result;
            }

            std::size_t after_generic = generic_close_brace + 1;
            if (after_generic < source.size() && source[after_generic] == ';') {
                ++after_generic;
            }

            out << source.substr(pos, after_generic - pos);
            pos = after_generic;
            continue;
        }

        if (cursor >= source.size() || source[cursor] != '{') {
            out << source[pos++];
            continue;
        }

        const std::size_t open_brace = cursor;
        const std::size_t close_brace = find_matching_brace(source, open_brace);
        if (close_brace == std::string::npos) {
            result.ok = false;
            result.source = source;
            return result;
        }

        std::size_t after = close_brace + 1;
        if (after < source.size() && source[after] == ';') {
            ++after;
        }

        const std::string body = source.substr(open_brace + 1, close_brace - open_brace - 1);
        const std::vector<std::string> items = split_struct_body_items(body);
        std::vector<std::string> fields;
        std::vector<std::string> methods;

        for (std::string item : items) {
            item = strip_visibility_prefix(item);
            item = normalize_constructor_item(item);
            if (looks_like_inline_method(item)) {
                methods.push_back(item);
            } else if (!trim_copy(item).empty()) {
                fields.push_back(trim_copy(item));
            }
        }

        out << "struct " << struct_name << " {\n";
        for (const auto& field : fields) {
            out << "    " << field;
            const std::string trimmed = trim_copy(field);
            if (!trimmed.empty() && trimmed.back() != ';' && trimmed.back() != ',') {
                out << ";";
            }
            out << "\n";
        }
        out << "}\n";

        if (!methods.empty()) {
            out << "\n" << struct_name << " {\n";
            for (const auto& method : methods) {
                out << method << "\n";
            }
            out << "}\n";
        }

        pos = after;
        (void)struct_start;
    }

    result.source = out.str();
    return result;
}

static bool read_clyth_source_with_includes(
    const std::filesystem::path& main_file,
    const std::filesystem::path& runtime_root,
    std::vector<std::filesystem::path>& included_paths,
    std::string& source_code
) {
    std::set<std::filesystem::path> active_stack;
    std::set<std::filesystem::path> already_included;
    std::ostringstream output;

    if (!read_clyth_source_with_includes_impl(main_file, runtime_root, active_stack, already_included, included_paths, output)) {
        return false;
    }

    source_code = output.str();
    return true;
}


static std::string normalize_multiline_template_string_literals(const std::string& source) {
    std::string out;
    out.reserve(source.size());

    bool in_template = false;
    bool escaped = false;

    for (char c : source) {
        if (in_template) {
            if (escaped) {
                out.push_back(c);
                escaped = false;
                continue;
            }

            if (c == '\\') {
                out.push_back(c);
                escaped = true;
                continue;
            }

            if (c == '`') {
                out.push_back(c);
                in_template = false;
                continue;
            }

            if (c == '\r') {
                continue;
            }

            if (c == '\n') {
                out += "\\n";
                continue;
            }

            if (c == '\t') {
                out += "\\t";
                continue;
            }

            out.push_back(c);
            continue;
        }

        out.push_back(c);
        if (c == '`') {
            in_template = true;
            escaped = false;
        }
    }

    return out;
}

static int parse_clyth_file(CompilerOptions& opts) {
    std::string source_code;

    if (!read_clyth_source_with_includes(opts.main_file, opts.runtime_root, opts.included_source_paths, source_code)) {
        return 1;
    }

    if (source_code.empty()) {
        fmt::print(stderr, "ERROR: Source file is empty: {}\n", opts.main_file.string());
        return 1;
    }

    source_code = normalize_multiline_template_string_literals(source_code);

    // Runtime packages are loaded only through explicit include statements such as
    // `include "collections"`. The compiler no longer injects List/Set/Map sources
    // by scanning for container names.
    source_code = normalize_keyed_array_literals(source_code);
    // Keep diagnostic generic-instantiation previews out of the parse/desugar stream.
    // The manifest contains commented `struct ... { ... }` snippets; the legacy
    // text rewriters intentionally operate before ANTLR, so feeding those comments
    // back into the source stream can make them look like real declarations.
    source_code = instantiate_generic_struct_templates_for_current_backend(source_code);

    StructInlineDesugarResult desugared = desugar_inline_struct_methods(source_code);
    if (!desugared.ok) {
        fmt::print(stderr, "ERROR: Failed to desugar inline struct methods/constructors.\n");
        return 1;
    }
    source_code = normalize_keyed_array_literals(desugared.source);

    antlr4::ANTLRInputStream input(source_code);
    ClythV1Lexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    ClythV1Parser parser(&tokens);

    clyth::DiagnosticBag diagnostics;
    clyth::ClythSyntaxErrorListener lexer_errors(diagnostics);
    clyth::ClythSyntaxErrorListener parser_errors(diagnostics);

    lexer.removeErrorListeners();
    parser.removeErrorListeners();

    lexer.addErrorListener(&lexer_errors);
    parser.addErrorListener(&parser_errors);

    ClythV1Parser::ProgramContext *tree = parser.program();

    if (diagnostics.has_errors() || parser.getNumberOfSyntaxErrors() > 0) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Parsing failed with {} syntax error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Parse succeeded for {}\n", opts.main_file.string());

    if (opts.debug_mode) {
        fmt::print("\nParse tree:\n{}\n", tree->toStringTree(&parser));
    }

    clyth::ClythAST ast_builder(diagnostics);
    clyth::ast::ProgramPtr program_ast = ast_builder.build(tree);

    if (diagnostics.has_errors() || program_ast == nullptr) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: AST generation failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    if (opts.debug_mode) {
        fmt::print("\nClyth AST:\n");
        clyth::ast::print_ast(program_ast);
    }

    if (opts.dump_ast_json_stdout) {
        fmt::print("\n{}\n", clyth::ast::to_json(program_ast));
    }

    if (!opts.ast_json_file.empty()) {
        if (!clyth::ast::write_json_file(program_ast, opts.ast_json_file)) {
            fmt::print(stderr, "ERROR: Failed to write AST JSON file: {}\n", opts.ast_json_file.string());
            return 1;
        }

        fmt::print("Wrote AST JSON: {}\n", opts.ast_json_file.string());
    }

    if (opts.dump_ast_bytecode_stdout) {
        fmt::print("\n{}\n", clyth::ast::to_clyth_bytecode(program_ast));
    }

    if (!opts.ast_bytecode_file.empty()) {
        if (!clyth::ast::write_bytecode_file(program_ast, opts.ast_bytecode_file)) {
            fmt::print(stderr, "ERROR: Failed to write AST bytecode file: {}\n", opts.ast_bytecode_file.string());
            return 1;
        }

        fmt::print("Wrote AST bytecode: {}\n", opts.ast_bytecode_file.string());
    }

    clyth::semantic::ClythSemanticPipeline semantic_pipeline(diagnostics);
    clyth::semantic::SemanticResult semantic_result = semantic_pipeline.analyze(program_ast);

    if (!semantic_result.ok || diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Semantic analysis failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Semantic analysis completed for {}\n", opts.main_file.string());

    if (opts.debug_mode) {
        fmt::print("\n");
        semantic_result.print_summary();
    }

    clyth::lowering::ClythLoweringPlan lowering_plan =
        clyth::lowering::ClythLoweringPlanBuilder::build(program_ast, semantic_result, diagnostics);

    if (diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: Lowering-plan generation failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    if (opts.debug_mode) {
        fmt::print("\n");
        lowering_plan.print();
    }

    clyth::llvm_stub::ClythLLVMCodegen codegen(diagnostics);
    clyth::llvm_stub::CodegenConfig codegen_config;
    codegen_config.module_name = opts.main_file.stem().string();
    codegen_config.dump_codegen_plan = opts.dump_codegen_plan || opts.debug_mode;
    codegen_config.output_binary_path = opts.output_binary_name;
    std::filesystem::path ir_output_path = opts.output_binary_name;
    ir_output_path.replace_extension(".ll");
    codegen_config.output_ir_path = ir_output_path;
    codegen_config.compile_to_executable = !opts.emit_ir_only;
    codegen_config.print_ir_to_stdout = opts.print_ir || opts.debug_mode;
    codegen_config.preserve_ir_file = opts.emit_ir_only || opts.emit_ir || opts.emit_all || opts.debug_mode;
    codegen_config.extra_link_inputs = runtime_link_inputs_for(opts.runtime_root, opts.included_source_paths);

    if (!codegen.emit(lowering_plan, semantic_result, codegen_config) || diagnostics.has_errors()) {
        diagnostics.print_to_stderr(opts.main_file);
        fmt::print(stderr, "ERROR: LLVM codegen stub failed with {} error(s).\n", diagnostics.error_count());
        return 1;
    }

    fmt::print("Frontend pipeline completed. LLVM IR generation completed.\n");
    return 0;
}

int main(int argc, char** argv) {
    std::string compiler_version("Clyth Compiler: Version 0.4.0-alpha (Runtime Foundation Release)");
    cxxopts::Options options("clyth", compiler_version.c_str());

    CompilerOptions opts;
    if (argc > 0 && argv[0]) {
        opts.compiler_invocation_path = argv[0];
    }

    options.add_options()
        ("d,debug", "Debug Mode enabled flag", cxxopts::value<bool>()->default_value("false"))
        ("c,compile", "Program Entrypoint", cxxopts::value<std::string>()->default_value("main.clyth"))
        ("o,output", "Output Binary Name", cxxopts::value<std::string>()->default_value("clyth_program.bin"))
        ("ast-json", "Write human-readable AST JSON to file", cxxopts::value<std::string>()->default_value(""))
        ("dump-ast-json", "Print human-readable AST JSON to stdout", cxxopts::value<bool>()->default_value("false"))
        ("ast-bytecode", "Write Clyth AST bytecode/debug format to file", cxxopts::value<std::string>()->default_value(""))
        ("dump-ast-bytecode", "Print Clyth AST bytecode/debug format to stdout", cxxopts::value<bool>()->default_value("false"))
        ("dump-codegen-plan", "Print LLVM codegen activity", cxxopts::value<bool>()->default_value("false"))
        ("emit-ir-only", "Write LLVM IR but do not link an executable", cxxopts::value<bool>()->default_value("false"))
        ("emit-ir", "Preserve generated LLVM IR alongside the executable", cxxopts::value<bool>()->default_value("false"))
        ("emit-all", "Preserve intermediate compiler outputs currently supported by this backend", cxxopts::value<bool>()->default_value("false"))
        ("print-ir", "Print generated LLVM IR to stdout", cxxopts::value<bool>()->default_value("false"))
        ("show-licenses", "Print Clyth and bundled third-party license text", cxxopts::value<bool>()->default_value("false"))
        ("v,version", "Version of compiler", cxxopts::value<bool>()->default_value("false"))
        ("h,help", "Display supported commands", cxxopts::value<bool>()->default_value("false"));

    try {
        auto result = options.parse(argc, argv);

        if (result["help"].as<bool>()) {
            fmt::print("{}\n", options.help());
            return 0;
        }

        if (result["version"].as<bool>()) {
            fmt::print("{}\n", compiler_version);
            return 0;
        }

        if (result["show-licenses"].as<bool>()) {
            const std::array<std::filesystem::path, 11> license_candidates {
                "../LICENSE",
                "../compiler-src/EXTERNAL_LIBRARIES_LICENSES.md",
                "../clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md",
                "LICENSE",
                "EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/CLYTH_LICENSE",
                "licenses/EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/RUNTIME_EXTERNAL_LIBRARIES_LICENSES.md",
                "licenses/ANTLR4_RUNTIME_LICENSE.txt",
                "licenses/FMT_LICENSE",
                "licenses/LLVM_LICENSE.TXT"
            };

            bool printed_any = false;
            for (const auto& path : license_candidates) {
                if (!std::filesystem::exists(path)) {
                    continue;
                }

                std::ifstream file(path);
                if (!file) {
                    continue;
                }

                fmt::print("\n===== {} =====\n", path.string());
                fmt::print("{}", std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
                fmt::print("\n");
                printed_any = true;
            }

            if (!printed_any) {
                fmt::print(stderr, "No license files were found near the compiler binary.\n");
                return 1;
            }

            return 0;
        }

        opts.debug_mode = result["debug"].as<bool>();
        opts.output_binary_name = result["output"].as<std::string>();
        opts.main_file = result["compile"].as<std::string>();
        opts.dump_ast_json_stdout = result["dump-ast-json"].as<bool>();
        opts.dump_ast_bytecode_stdout = result["dump-ast-bytecode"].as<bool>();
        opts.dump_codegen_plan = result["dump-codegen-plan"].as<bool>();
        opts.emit_ir_only = result["emit-ir-only"].as<bool>();
        opts.emit_ir = result["emit-ir"].as<bool>();
        opts.emit_all = result["emit-all"].as<bool>();
        opts.print_ir = result["print-ir"].as<bool>();

        const std::string ast_json = result["ast-json"].as<std::string>();
        const std::string ast_bytecode = result["ast-bytecode"].as<std::string>();

        if (!ast_json.empty()) {
            opts.ast_json_file = ast_json;
        }

        if (!ast_bytecode.empty()) {
            opts.ast_bytecode_file = ast_bytecode;
        }

    } catch (const std::exception& e) {
        fmt::print(stderr, "ERROR occurred: {}\nExiting compilation\n", e.what());
        return 1;
    }

    if (!std::filesystem::exists(opts.main_file)) {
        fmt::print(stderr, "ERROR: Source file does not exist: {}\n", opts.main_file.string());
        return 1;
    }

    if (opts.main_file.extension() != ".clyth") {
        fmt::print(stderr, "ERROR: Ensure the main program file contains the '.clyth' file extension.\n");
        return 1;
    }

    opts.runtime_root = locate_runtime_root(opts.main_file, opts.compiler_invocation_path);
    if (opts.runtime_root.empty()) {
        fmt::print(stderr, "WARN: clyth-runtime directory was not found near the current working directory or source file. Runtime includes may fail.\n");
    }

    fmt::print("Compiling {} -> {}\n", opts.main_file.string(), opts.output_binary_name);

    return parse_clyth_file(opts);
}
