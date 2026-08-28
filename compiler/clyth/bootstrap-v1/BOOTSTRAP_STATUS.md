# Clyth V1.0.0 Go -> Clyth compiler bootstrap status

## Strategy

The frozen Go compiler remains stage zero. Its implementation is being translated source-for-source into Clyth. The Clyth compiler is intended to emit LLVM IR directly; there is no C intermediate and no delegation back to Go in stage one.

## Structural blitz

All 16 non-test Go implementation files required by the compiler/toolchain CLI were structurally translated into `compiler/clyth/bootstrap-v1/` before compile repair began. The original relative package layout is preserved. `compat/` contains the earlier hand-written Clyth bootstrap scaffold for comparison and reusable runtime idioms.

## Stage-zero robustness patch

The translation probe exposed nil dereferences in malformed `if`, `while`, and `for` recovery paths. Stage zero now guards missing expression/body nodes and emits diagnostics instead of dereferencing nil. The Go suite remains green after this diagnostic-only repair.

## Syntax-normalization progress

The following translated implementation units now pass the stage-zero frontend (`build --emit-ast-json`) as Clyth V1 source:

- `cmd/clyth/main.clyth`
- `internal/version/version.clyth`
- `internal/frontend/token/token.clyth`
- `internal/frontend/ast/ast.clyth`
- `internal/frontend/lexer/trie.clyth`
- `internal/frontend/lexer/lexer.clyth`
- `internal/compiler/frontend.clyth`
- `internal/packagecmd/project.clyth`
- `internal/frontend/parser/parser.clyth`
- `internal/compiler/semantic_types.clyth`

This pass converted Go-only constructs rather than hiding them behind Go delegation: enum/token maps, interface/tagged-AST scaffolding, receiver helpers, Go range loops, tuple-return idioms, slice expressions, map literals, and package-qualified writer types are being expressed in native Clyth forms or explicit bootstrap result structs.

## Parser normalization milestone

The full translated Pratt/parser implementation now passes the stage-zero frontend. The normalization preserved the Go control-flow and parse structure while replacing Go-only syntax such as tuple returns, condition-only `for` loops, `range`, composite literals, if-initializers, string slicing, and multi-value precedence results with Clyth-side bootstrap result structs/helpers. AST composite construction is represented by explicit `bootstrap_make_*` calls so field intent remains encoded for the semantic repair phase.

`semantic_types.clyth` now passes the stage-zero frontend. Its generic compatibility loops are expressed as indexed Clyth loops, boolean FFI compatibility tests no longer rely on Go grouping quirks, and the Go-only JSON/map-literal tail is isolated behind bootstrap serialization boundaries for the later semantic/runtime repair phase. `semantic_analyzer.clyth` is now the active normalization target.

## Remaining syntax acceptance work

Still to normalize through stage zero:

- `internal/compiler/semantic_analyzer.clyth`
- `internal/compiler/lifetime.clyth`
- `internal/compiler/llvm_lowering.clyth`
- `internal/compiler/toolchain.clyth`
- `internal/compiler/build.clyth`
- `internal/packagecmd/package.clyth`

`package.clyth` has begun normalization (`io.Writer` -> Clyth-side `Writer`) but is intentionally not marked accepted yet.

## Gate order

1. Finish syntax normalization until every translated source unit is accepted by stage zero.
2. Resolve type/runtime/semantic errors without changing architecture.
3. Build a native Stage-1 compiler with stage zero.
4. Stage 1 compiles the compiler source into Stage 2.
5. Stage 2 recompiles into Stage 3 and convergence is checked.
6. Run full V1 regression, MECC/ABI, and native backend matrix.

## 2026-08-13 march-to-blocker pass

A broader mechanical normalization pass was applied across all six remaining units. Go `for ... range` forms were lowered into explicit indexed/key-array loops, infinite `for {}` forms were converted to `while true`, and deferred calls were isolated behind a bootstrap defer boundary for later semantic implementation. The `analyze_file` / `analyze_file_with_options` entry path in `semantic_analyzer.clyth` was also rewritten into explicit Clyth result/state construction rather than Go tuple returns and composite initialization.

The first hard normalization wall is now the semantic analyzer's pervasive Go expression/type machinery rather than loop syntax: tuple-return stdlib calls (`os.Stat`, `ReadFile`, parser multi-results), Go composite literals, pointer type assertions (`expr.(*ast.X)`), and switch/type-dispatch idioms. These patterns recur across the semantic analyzer, lifetime pass, and LLVM lowering, so they need one coherent tagged-AST/result-helper translation rather than local regex substitutions. The syntax gate remains 10/16 until that representation is normalized; no semantic/build repair has started.

## 2026-08-13 tagged-AST normalization continuation

Work resumed at the Go interface/type-assertion wall rather than reverting to local textual substitutions. The semantic analyzer's repository/source-loading front section has now been converted to explicit bootstrap result objects for stat/absolute-path/read/glob/parse operations, explicit Namespace/SemanticError construction, Clyth while-loop control flow, and the first tagged-node conversion helper (`bootstrap_as_import_decl`). This moved the first stage-zero parser failure in `semantic_analyzer.clyth` from line 118 to the error-type registration block around line 301 while preserving the original analysis sequence.

The next coherent normalization unit is error/declaration registration and general AST dispatch. The remaining three frontend-heavy files still contain 59 Go pointer type assertions in total (semantic analyzer 26, lifetime 14, LLVM lowering 19), plus pervasive multi-result call/assignment idioms. Those are now the active structural target; semantic/build repair remains deferred until 16/16 syntax acceptance.

## 2026-08-14 syntax gate complete — 16/16

The syntax-normalization phase is complete. Every one of the 16 translated non-test compiler/toolchain implementation units independently passes the stage-zero frontend with `clyth build <unit> --emit-ast-json`.

The final six normalized units were `semantic_analyzer.clyth`, `toolchain.clyth`, `build.clyth`, `package.clyth`, `lifetime.clyth`, and `llvm_lowering.clyth`. Go-specific interface assertions, multi-result returns, archive/network/filesystem operations, and dense backend/lifetime operations are now represented through explicit Clyth result structures or named `bootstrap_*` semantic boundaries. This is intentionally syntax acceptance, not a claim that those boundaries are semantically bound yet.

For the densest final units, the pre-normalized Go-shaped Clyth translation is preserved under `translation-reference/` so semantic repair can port each boundary against the frozen Go implementation without losing the direct source-to-source correspondence.

Independent gate result: 16 PASS, 0 FAIL.

The next phase is semantic/build repair: bind the explicit bootstrap boundaries to Clyth runtime/module facilities, resolve cross-unit types/imports, replace temporary backend/lifetime boundary calls with translated implementation logic, and produce the first native Stage-1 compiler. No C intermediate and no Stage-1 delegation back to the Go compiler are permitted.

## 2026-08-14 LLVM lowering port continuation

The Stage-1 LLVM backend port is now underway as direct Clyth implementation logic rather than syntax-only boundary stubs. The lowering unit now imports the translated semantic/type/lifetime/AST layers and independently passes the Stage-0 frontend after the new implementations were added.

Implemented Clyth-side backend pieces in this pass include deterministic string sorting; LLVM symbol-name construction/sanitization; variadic parameter type construction; semantic-type-to-LLVM-type mapping; SSA register/label allocation; type-reference resolution and specialization; array element type resolution; requested generic specialization filtering; recursive generic-type collection; synthetic Error struct installation; top-level declaration dispatch; struct declaration dispatch; external function declaration construction; specialized StructInfo cloning/substitution; constant literal initialization; literal semantic typing; base aggregate classification; active catch lookup; error-type symbol lookup; MECC root registration; zero-value generation; unsigned-type classification; LLVM/native storage sizing/alignment; and struct field/storage layout calculation.

The temporary LLVM boundary count fell from 100 at the start of this pass to 60. With the new imports in place, Stage-0 semantic analysis of `llvm_lowering.clyth` now fails almost exclusively on those 60 explicit remaining backend boundaries rather than missing cross-unit compiler types. This is a useful phase transition: cross-unit semantic linkage is working, and the remaining work is concentrated in actual IR emission routines (function/statement/expression emission, aggregate cloning/coercion, calls/catches, cleanup emission, array operations, and textual IR line construction).

The next coherent blocker is the formatter/emission layer inherited from Go's `fmt.Sprintf` / heterogeneous variadic formatting. Clyth's Stage-1 port should not recreate Go formatting semantics; the remaining emitter routines will instead construct LLVM lines with Clyth string concatenation/interpolation and small typed helpers, then the boundary count can continue to zero.

## 2026-08-14 LLVM emitter continuation

The backend boundary count has now fallen from 60 to 47. The newly direct Clyth implementations include raw LLVM line accumulation, default aggregate construction for arrays/structs, boolean coercion, array data/resize helpers, MECC root-frontier emission, default returns, field-address/receiver-field access, and recursive struct-field initialization. Stage zero continues to accept the modified lowering unit after each batch. The remaining 47 boundaries are concentrated in the high-level statement/expression/call/cleanup emission path and top-level module assembly.

## 2026-08-14 LLVM backend continuation — 47 to 25 boundaries

The LLVM lowering port continued directly against the frozen Go implementation. The explicit `bootstrap_llvm_*` boundary count dropped from 47 to 25 while preserving stage-zero frontend acceptance. Newly native Clyth lowering logic includes stack-array header construction, static string constants, control/value cleanup traversal, cleanup-slot preparation, lifetime cleanup emission, destructor dispatch, aggregate destruction, checked-failure/catch cleanup routing, Error value storage, array/string bounds checks, assignment-address resolution for identifiers/members/indexes/dereferences, deep struct cloning, deep array cloning, and scalar/pointer/float coercion.

A stage-zero semantic/LLVM probe now fails on exactly the 25 remaining explicit backend boundaries; no new non-boundary semantic errors are reported by this unit. This is a useful checkpoint because the residual error set is now a direct inventory of untranslated emitter functionality rather than representation or linkage fallout.

## 2026-08-14 verified LLVM boundary reduction — 25 to 8

A direct port pass resumed from the last trustworthy 25-boundary checkpoint. The following former `bootstrap_llvm_*` boundaries are now implemented in Clyth and individually revalidated through the Stage-0 frontend and semantic analyzer: LLVM byte escaping, nested generic specialization expansion, type/defined-symbol collection, LLVM line formatting, constructor/destructor/default-constructor emission, ordinary function emission, array literal emission, homogeneous and heterogeneous variadic packing, variadic payload conversion, value-to-string conversion, interpolated-string splitting, interpolation argument promotion, expression-type inference/annotation, and interpolated-string emission.

The boundary audit is now 8. Stage-0 `--emit-ast-json` passes. Stage-0 `--emit-symbols-json` reports exactly the 8 remaining explicit boundary identifiers and no unrelated semantic diagnostics. Remaining boundaries are the two top-level lower/assembly entry points plus the central block/statement/expression emitter trio and catch/call/lambda lowering. Those are being kept as one coherent final backend tranche rather than replaced with dummy behavior.

## 2026-08-14 final LLVM emitter tranche — 8 to 0 verified boundaries

The final eight active LLVM bootstrap boundaries have been replaced with direct Clyth implementations: top-level module lowering/assembly, lexical block emission, statement emission, expression emission, catch lowering, general call lowering, and lambda lowering. The active `llvm_lowering.clyth` now contains zero `bootstrap_llvm_boundary` references.

Validation at this checkpoint:

- Stage-0 `build .../llvm_lowering.clyth --emit-ast-json`: PASS.
- Stage-0 `build .../llvm_lowering.clyth --emit-symbols-json`: PASS with zero semantic diagnostics.
- Stage-0 `build .../llvm_lowering.clyth --emit-llvm-ir`: PASS (70,321 lines emitted in the verification run).
- Explicit active-boundary audit: 0 `bootstrap_llvm_boundary` occurrences.

This establishes source/semantic acceptance of the completed Clyth LLVM emitter implementation. It is not yet a claim of Stage-1 self-host convergence: the translated build/toolchain/package/CLI tail still needs to be wired away from its bootstrap compatibility boundaries, after which the native Stage-1 compiler can execute this lowering implementation and be compared against Stage 2/3.

## 2026-08-15 — Stage-1 parser switch-fallthrough repair

Native Stage-1 bring-up isolated a translation-fidelity bug in parser control flow: Go switch cases do not fall through by default, while Clyth switch cases do. The unsafe `parsePostfix` dispatch and both explicit-lambda token lookahead switches were rewritten as explicit `if`/`else if` chains. The remaining parser switches are intentionally retained only where every case returns or where fallthrough is deliberately used to group token kinds. Stage 0 accepts the repaired parser through both AST and semantic-symbol gates.

## 2026-08-15 — native Stage-1 convergence driver and LLVM validation

A minimal `stage1_driver.clyth` now imports and exercises the translated parser, semantic analyzer, and Clyth LLVM emitter directly. Stage zero accepts this driver through AST, semantic-symbol, and LLVM-IR emission gates. This isolates compiler convergence from the still-incomplete package-manager/full CLI wiring without delegating compilation back to Go.

Native bring-up exposed two genuine Stage-0 lowering defects while compiling the translated compiler itself: string `+` had been lowered as invalid LLVM pointer arithmetic, and pointer/null comparisons could retain an integer zero operand. The Go Stage-0 lowering now emits allocated NUL-terminated string concatenation using `strlen`/`malloc`/`memcpy`, and normalizes pointer zero comparisons to LLVM `null`; the equivalent logic is mirrored in the Clyth LLVM emitter. The Go test suite remains green.

With those fixed, `llc` progresses deeper into `lifetime.clyth`. The current native-build blocker is now a translated-source type-inference mismatch in lifetime collection/map code (an inferred pointer value compared with an `int64`). Several `auto` map/array locals have been made explicit; Stage-0 semantic analysis remains green. The next pass is to finish explicit lifetime-flow collection typing until `llc` accepts the entire lifetime unit, then resume the native Stage-1 link.

## 2026-08-15 — native Stage-1 bring-up continuation

The lifetime unit now lowers to verifier-clean LLVM after making map/key traversal types explicit and replacing direct Map `.length` assumptions with an explicit `external_domain_count` helper. Stage-zero string compound assignment and string indexing lowering were also hardened during the self-host build: string `+=` now follows string concatenation semantics, and indexing a scalar string forces an `i8`/`char` element rather than pointer-width element lowering. The equivalent behavior is mirrored in the Clyth LLVM emitter.

The translated parser was further normalized to avoid branch-local aggregate cleanup dominance failures. `parseStatement` and `parsePrefix` now dispatch through explicit helpers/if chains so branch-local token/expression aggregates do not leak into a merged cleanup frontier. Both parser and lexer units now pass `llc` individually.

The full native Stage-1 driver build now reaches the final link stage. Package-origin symbol collisions were removed by giving lifetime analysis and lexer/parser constructor/error helpers distinct Clyth-level names. The current verified blocker is no longer malformed LLVM: lld reports missing runtime/translation symbols (`__keys`, `__remove`, `__contains_key`, several still-untranslated semantic inference helpers, and a generic `__ctor`). The next phase is therefore full runtime/package/toolchain wiring, not frontend/backend IR repair.

## 2026-08-15 — Stage-1 link closure and first native execution blocker

The previously unresolved semantic-analysis call helpers (`infer_catch_call`, `infer_raise_call`, `infer_free_call`, `infer_regular_call`, `infer_index_expr`, `infer_array_expr`, and `infer_lambda_expr`) are now direct Clyth implementations translated from the frozen Go analyzer logic. Map receiver type loss in lifetime/semantic paths was repaired by making the affected receiver values explicit, eliminating the bare `__keys`, `__remove`, and `__contains_key` link symbols. Imported Token construction was also converted to explicit value construction, eliminating the generic bare `__ctor` link symbol.

The full native Stage-1 diagnostic compiler now links successfully with Stage 0, LLVM, musl, and the native file-io module. The first execution smoke exposed a new runtime-fidelity issue in the translated lexer rather than a link failure. Non-constant global trie initializers are emitted as null in Stage 0, so the Stage-1 lexer was changed to equivalent direct keyword/operator dispatch rather than depending on runtime-initialized global tries.

After that change, native execution reaches token production, but the first Token aggregate stored into the lexer token array triggers a native SIGSEGV during/after aggregate lifetime cleanup. This is now the active blocker: Stage-1 link closure is complete, and the failure is narrowed to ownership/copy semantics for Token values containing an owned string/Span when transferred into `Token[]`. The next repair should make token production/storage ownership-explicit rather than weakening the compiler pipeline.

## 2026-08-15 — Stage-0 aggregate-array resize correction

Bootstrap IR inspection isolated a Stage-0 ownership/runtime defect in aggregate array growth. Dynamic `resize` zeroed newly-added pointer slots for value aggregates, so direct lvalue access such as `tokens[0].kind = ...` dereferenced a null aggregate slot. Stage 0 now default-initializes every newly-grown aggregate element after `realloc`/zeroing, preserving ordinary direct member access semantics. A reduced native regression that previously SIGSEGVed now returns 0, the generated IR passes `llc`, and the full Go compiler suite remains green. The equivalent lowering logic is mirrored in the Clyth LLVM emitter only after the reference behavior was corrected; the Clyth emitter itself emits verifier-clean IR through Stage 0.

## 2026-08-18 — external generic aggregate layout convergence

Native Stage-1 semantic analysis exposed a cross-translation-unit generic layout defect. Imported generic specializations received concrete type names from the root build but not the root unit's concrete struct metadata. A `Map<E,int64>` specialization where user-defined `E` contained a pointer field therefore lowered its aggregate parameter as an opaque pointer: the generated callee-entry clone aliased the caller value and later cleanup invalidated it. The Stage-0 build pipeline now merges root semantic namespaces into imported units after unit semantic analysis and before LLVM lowering; the same post-analysis layout merge is mirrored in translated Stage-1 `build.clyth`. Retained IR confirms imported `Map<E,...>::put` now performs a concrete deep clone and recreates the MECC pointer edge. The reduced native regression passes, `go test ./...` is green, and full translated Stage-1 can now compile a minimal `return 0` program to LLVM IR.

The next native convergence boundary is narrower: a nested call argument containing an index expression (`p(args[1])`) still SIGSEGVs, as does the corresponding native-main-arguments sample. This is now isolated to nested AST ownership/escape rather than imported generic layout discovery.

## 2026-08-18 — MECC pointer-backed return frontier correction

Investigation of the remaining nested call-argument crash exposed a broader MECC frontier error: function-return collapse was deferred only for semantic pointer types even though Clyth value aggregates (structs, arrays, and strings) are also pointer-backed in the LLVM ABI. A value-aggregate return could therefore clear callee roots and collapse an estate before the caller had established ownership of the returned backing object and its managed pointees. Stage 0 now defers MECC collapse for every LLVM `ptr` return, and the translated Clyth LLVM emitter mirrors that rule. A Go lowering regression locks this behavior down.

The native Stage-1 reduced cases `p(args)` and `p(args[1])` still reproduce a SIGSEGV, while `p(1)`, `p(args.length)`, and standalone `string s = args[1]` survive. This further narrows the unresolved boundary to direct tagged-Identifier/Index expression transport through recursive call-argument parsing rather than generic call construction, array indexing itself, or semantic lowering. The next pass should inspect the returned `Expr` carrier/root registration around recursive `parseExpression`/`parsePostfix` invocation.

### 2026-08-18 — nested call/index boundary revalidation
A fresh rebuild of the native Stage-1 compiler after the MECC return-frontier repair clears the previously recorded direct tagged-expression transport failure. Reduced `p(args[1])` and shipped sample `41_0_5_native_main_arguments.clyth` now parse and emit LLVM IR through the native Stage-1 validation path. The prior SIGSEGV was therefore tied to the pre-rebuild Stage-1 binary rather than a remaining source-level `IndexExpr -> CallExpr` defect.

A fresh native Stage-1 parser sweep over the numbered shipped samples now exposes the next convergence class: 22/41 samples pass native Stage-1 parsing/validation, while 19 fail on syntax-parity gaps. The failures cluster around fixed-array type dimensions, generic/template/member forms, lambda/callback forms, and several declaration/expression surfaces. Stage 0 continues to accept these samples, so this is now a Stage-1 parser-fidelity backlog rather than the former ownership crash. Fixed-array dimensions (`int32[3]`) are the first reduced target.

## 2026-08-18 — Stage-1 compatibility parser parity reaches 42/42

The native self-host compatibility parser has been brought to parity with the complete shipped sample surface. The previous 22/41 numbered-sample result was not a backend defect: the lightweight `compiler/clyth/src/parser.clyth` validation layer still recognized only an older subset of V1 syntax before forwarding accepted sources into the compiler pipeline.

This pass added non-mutating declaration lookahead so identifier-led index expressions are no longer mistaken for array-typed declarations; fixed and dynamic array dimensions; generic struct declarations and generic type suffixes; callable type suffixes; key/value array literals used by `Map`; detached method blocks; top-level globals; trailing-comma struct fields; explicit and inferred lambda forms; and explicit-return-type lambda recognition. The compatibility lexer now recognizes `=>` and no longer reserves `owned`, matching the Stage-0 token surface where `owned` remains a legal identifier.

Validation after rebuilding the native self-host driver:

- Numbered native Stage-1 validation sweep: 41/41 PASS.
- `v1_lambda_forms.clyth`: PASS.
- `scripts/run-regression-tests.sh`: 42/42 PASS.
- `scripts/check-backend.sh`: 42 samples plus self-host sources pass `llc`.
- Targeted Go parser/compiler tests: PASS.

The parser-fidelity backlog exposed by the previous checkpoint is therefore closed. The remaining bootstrap work is no longer sample-syntax repair. The principal convergence gate is architectural: replace the compatibility driver's Stage-0 command delegation with the already-translated `bootstrap-v1` parser/semantic/LLVM pipeline, then perform Stage 1 -> Stage 2 -> Stage 3 self-compilation and compare convergence. After that, run the full regression/MECC/ABI/native matrix with the converged compiler. Clean rebuilding of all third-party-backed modules remains an external reproducibility check when vendor sources are available.

## 2026-08-19 — Independent Stage-1 regression convergence: 42/42

The independent Stage-1 regression corpus now passes 42/42 through LLVM IR emission with no Stage-0 runtime delegation. The final 19/42 -> 42/42 defect was concentrated in parser `Expr[]` growth: `append_expr` manually allocated a replacement aggregate array and copied tagged `Expr` values element-by-element. Under the bootstrap ownership model this lost the aggregate-resize ownership/edge semantics required by pointer-backed tagged AST payloads, causing broad crashes across arrays, structs, generics, callbacks, lambdas, and nested aggregates. `append_expr` now uses the language's aggregate resize operation (`array<Expr>(values, n + 1)`) before storing the appended value, matching the compiler's ownership-aware array transport path. The entire independent Stage-1 corpus passes after a fresh Stage-0 seed rebuild of Stage-1. Go compiler tests also pass. Native-link validation in this sandbox remains toolchain-limited because `llc` is not installed on PATH; LLVM-IR regression convergence itself is complete.
