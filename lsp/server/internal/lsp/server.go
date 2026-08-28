package lsp

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io"
	"regexp"
	"sort"
	"strconv"
	"strings"
	"sync"
	"unicode"
)

type request struct {
	JSONRPC string          `json:"jsonrpc"`
	ID      json.RawMessage `json:"id,omitempty"`
	Method  string          `json:"method"`
	Params  json.RawMessage `json:"params,omitempty"`
}
type response struct {
	JSONRPC string          `json:"jsonrpc"`
	ID      json.RawMessage `json:"id,omitempty"`
	Result  any             `json:"result"`
	Error   any             `json:"error,omitempty"`
}
type position struct {
	Line      int `json:"line"`
	Character int `json:"character"`
}
type rng struct {
	Start position `json:"start"`
	End   position `json:"end"`
}
type location struct {
	URI   string `json:"uri"`
	Range rng    `json:"range"`
}
type document struct {
	URI     string
	Text    string
	Version int
}
type symbol struct {
	Name           string
	Kind           int
	Detail         string
	Range          rng
	SelectionRange rng
	URI            string
}
type server struct {
	mu       sync.RWMutex
	docs     map[string]document
	out      io.Writer
	shutdown bool
}

var declRE = regexp.MustCompile(`(?m)^\s*(?:extern\s+C\s+)?(?:(struct|protocol|enum|module)\s+([A-Za-z_]\w*)|(?:[A-Za-z_]\w*(?:\s*\[[^\]]*\])?(?:\s*\*)?\s+)+([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:\{|$))`)
var localRE = regexp.MustCompile(`(?m)^\s*(?:const\s+)?[A-Za-z_]\w*(?:\s*<[^>]+>)?(?:\s*\[[^\]]*\])?(?:\s*\*)?\s+([A-Za-z_]\w*)\s*(?:=|$)`)

var keywords = []string{"auto", "break", "case", "cast", "const", "continue", "default", "else", "enum", "extern", "false", "for", "if", "import", "module", "new", "private", "protocol", "public", "return", "struct", "switch", "this", "true", "while", "C"}
var types = []string{"bool", "char", "float32", "float64", "int8", "int16", "int32", "int64", "string", "uint8", "uint16", "uint32", "uint64", "void"}
var docs = map[string]string{
	"auto": "Infers a variable type from its initializer.", "enum": "Declares a scoped nominal enumeration.", "struct": "Declares a Clyth value type.", "protocol": "Declares a compile-time conformance contract.", "new": "Creates owning heap-backed storage and returns a pointer.",
	"cast": "Explicitly converts a value to the requested destination type.", "private": "Restricts a struct member to the declaring struct.", "public": "Explicitly exposes a struct member; public is the default.", "const": "Marks a value or referenced value as immutable.", "extern": "Declares a function implemented across an ABI boundary.", "import": "Imports a Clyth module.",
	"string": "Clyth string value type.", "int32": "Signed 32-bit integer.", "bool": "Boolean value (`true` or `false`).",
}

var semanticTokenTypes = []string{"namespace", "type", "typeParameter", "function", "variable", "string"}
var semanticTokenModifiers = []string{"declaration", "readonly"}

var moduleImportRE = regexp.MustCompile(`(?m)^\s*import\s+([A-Za-z_][A-Za-z0-9_-]*(?:/[A-Za-z0-9_.-]+)*)`)
var fileImportRE = regexp.MustCompile(`(?m)^\s*import\s+"([^"\n]+)"`)
var callableTypeRE = regexp.MustCompile(`([A-Za-z_]\w*(?:[ \t]*<[^>\n]+>)?)[ \t]*\(([^()\n]*)\)([ \t]*\[\])?[ \t]+([A-Za-z_]\w*)`)
var explicitLambdaRE = regexp.MustCompile(`([A-Za-z_]\w*(?:[ \t]*<[^>\n]+>)?(?:[ \t]*\*)?)[ \t]*\(([^()\n]*)\)[ \t]*=>`)
var contextualLambdaRE = regexp.MustCompile(`\(([^()\n]*)\)[ \t]*=>`)

func Serve(in io.Reader, out io.Writer) error {
	s := &server{docs: map[string]document{}, out: out}
	r := bufio.NewReader(in)
	for {
		body, err := readMessage(r)
		if err == io.EOF {
			return nil
		}
		if err != nil {
			return err
		}
		var req request
		if err = json.Unmarshal(body, &req); err != nil {
			return err
		}
		if err = s.handle(req); err != nil {
			return err
		}
		if s.shutdown && req.Method == "exit" {
			return nil
		}
	}
}
func readMessage(r *bufio.Reader) ([]byte, error) {
	n := -1
	for {
		line, err := r.ReadString('\n')
		if err != nil {
			return nil, err
		}
		line = strings.TrimRight(line, "\r\n")
		if line == "" {
			break
		}
		if strings.HasPrefix(strings.ToLower(line), "content-length:") {
			v := strings.TrimSpace(strings.SplitN(line, ":", 2)[1])
			n, _ = strconv.Atoi(v)
		}
	}
	if n < 0 {
		return nil, fmt.Errorf("missing Content-Length")
	}
	b := make([]byte, n)
	_, err := io.ReadFull(r, b)
	return b, err
}
func (s *server) send(v any) error {
	b, err := json.Marshal(v)
	if err != nil {
		return err
	}
	_, err = fmt.Fprintf(s.out, "Content-Length: %d\r\n\r\n%s", len(b), b)
	return err
}
func (s *server) reply(id json.RawMessage, result any) error {
	return s.send(response{JSONRPC: "2.0", ID: id, Result: result})
}
func (s *server) notify(method string, params any) error {
	return s.send(map[string]any{"jsonrpc": "2.0", "method": method, "params": params})
}

func (s *server) handle(q request) error {
	switch q.Method {
	case "initialize":
		return s.reply(q.ID, map[string]any{"capabilities": map[string]any{"textDocumentSync": 1, "hoverProvider": true, "definitionProvider": true, "referencesProvider": true, "documentSymbolProvider": true, "completionProvider": map[string]any{"triggerCharacters": []string{".", ":"}}, "renameProvider": true, "semanticTokensProvider": map[string]any{"legend": map[string]any{"tokenTypes": semanticTokenTypes, "tokenModifiers": semanticTokenModifiers}, "full": true}}, "serverInfo": map[string]any{"name": "clyth-lsp", "version": "0.1.0"}})
	case "initialized":
		return nil
	case "shutdown":
		s.shutdown = true
		return s.reply(q.ID, nil)
	case "exit":
		return nil
	case "textDocument/didOpen":
		var p struct {
			TextDocument struct {
				URI     string `json:"uri"`
				Text    string `json:"text"`
				Version int    `json:"version"`
			} `json:"textDocument"`
		}
		json.Unmarshal(q.Params, &p)
		s.put(document{p.TextDocument.URI, p.TextDocument.Text, p.TextDocument.Version})
		return s.publishDiagnostics(p.TextDocument.URI)
	case "textDocument/didChange":
		var p struct {
			TextDocument struct {
				URI     string `json:"uri"`
				Version int    `json:"version"`
			} `json:"textDocument"`
			ContentChanges []struct {
				Text string `json:"text"`
			} `json:"contentChanges"`
		}
		json.Unmarshal(q.Params, &p)
		if len(p.ContentChanges) > 0 {
			s.put(document{p.TextDocument.URI, p.ContentChanges[len(p.ContentChanges)-1].Text, p.TextDocument.Version})
			return s.publishDiagnostics(p.TextDocument.URI)
		}
		return nil
	case "textDocument/didClose":
		var p struct {
			TextDocument struct {
				URI string `json:"uri"`
			} `json:"textDocument"`
		}
		json.Unmarshal(q.Params, &p)
		s.mu.Lock()
		delete(s.docs, p.TextDocument.URI)
		s.mu.Unlock()
		return nil
	case "textDocument/documentSymbol":
		return s.documentSymbols(q)
	case "textDocument/hover":
		return s.hover(q)
	case "textDocument/definition":
		return s.definition(q)
	case "textDocument/references":
		return s.references(q)
	case "textDocument/completion":
		return s.completion(q)
	case "textDocument/rename":
		return s.rename(q)
	case "textDocument/semanticTokens/full":
		return s.semanticTokens(q)
	default:
		if len(q.ID) > 0 {
			return s.reply(q.ID, nil)
		}
		return nil
	}
}
func (s *server) put(d document) { s.mu.Lock(); s.docs[d.URI] = d; s.mu.Unlock() }
func (s *server) get(uri string) (document, bool) {
	s.mu.RLock()
	de, ok := s.docs[uri]
	s.mu.RUnlock()
	return de, ok
}
func posAt(text string, off int) position {
	if off > len(text) {
		off = len(text)
	}
	pre := text[:off]
	line := strings.Count(pre, "\n")
	col := off
	if i := strings.LastIndex(pre, "\n"); i >= 0 {
		col = off - i - 1
	}
	return position{line, col}
}
func offsetAt(text string, p position) int {
	lines := strings.Split(text, "\n")
	if p.Line >= len(lines) {
		return len(text)
	}
	off := 0
	for i := 0; i < p.Line; i++ {
		off += len(lines[i]) + 1
	}
	if p.Character > len(lines[p.Line]) {
		p.Character = len(lines[p.Line])
	}
	return off + p.Character
}
func wordAt(text string, p position) (string, rng) {
	off := offsetAt(text, p)
	if off > len(text) {
		off = len(text)
	}
	a, b := off, off
	for a > 0 && isWord(rune(text[a-1])) {
		a--
	}
	for b < len(text) && isWord(rune(text[b])) {
		b++
	}
	return text[a:b], rng{posAt(text, a), posAt(text, b)}
}
func isWord(r rune) bool { return r == '_' || unicode.IsLetter(r) || unicode.IsDigit(r) }
func scanSymbols(d document) []symbol {
	out := []symbol{}
	for _, m := range declRE.FindAllStringSubmatchIndex(d.Text, -1) {
		name := ""
		start := -1
		kind := 12
		detail := "function"
		if m[2] >= 0 {
			kw := d.Text[m[2]:m[3]]
			name = d.Text[m[4]:m[5]]
			start = m[4]
			if kw == "struct" {
				kind = 23
				detail = "struct"
			} else if kw == "module" {
				kind = 2
				detail = "module"
			} else {
				kind = 11
				detail = "protocol"
			}
		} else if m[6] >= 0 {
			name = d.Text[m[6]:m[7]]
			start = m[6]
		}
		if start >= 0 {
			r := rng{posAt(d.Text, start), posAt(d.Text, start+len(name))}
			out = append(out, symbol{name, kind, detail, r, r, d.URI})
		}
	}
	for _, m := range localRE.FindAllStringSubmatchIndex(d.Text, -1) {
		if m[2] >= 0 {
			name := d.Text[m[2]:m[3]]
			r := rng{posAt(d.Text, m[2]), posAt(d.Text, m[3])}
			out = append(out, symbol{name, 13, "variable", r, r, d.URI})
		}
	}
	return out
}
func (s *server) documentSymbols(q request) error {
	var p struct {
		TextDocument struct {
			URI string `json:"uri"`
		} `json:"textDocument"`
	}
	json.Unmarshal(q.Params, &p)
	d, ok := s.get(p.TextDocument.URI)
	if !ok {
		return s.reply(q.ID, []any{})
	}
	sy := scanSymbols(d)
	res := make([]map[string]any, 0, len(sy))
	for _, x := range sy {
		res = append(res, map[string]any{"name": x.Name, "detail": x.Detail, "kind": x.Kind, "range": x.Range, "selectionRange": x.SelectionRange})
	}
	return s.reply(q.ID, res)
}
func (s *server) hover(q request) error {
	uri, p := docPos(q.Params)
	d, ok := s.get(uri)
	if !ok {
		return s.reply(q.ID, nil)
	}
	w, r := wordAt(d.Text, p)
	if w == "" {
		return s.reply(q.ID, nil)
	}
	if txt, ok := docs[w]; ok {
		return s.reply(q.ID, map[string]any{"contents": map[string]any{"kind": "markdown", "value": "**`" + w + "`** — " + txt}, "range": r})
	}
	for _, x := range scanSymbols(d) {
		if x.Name == w {
			return s.reply(q.ID, map[string]any{"contents": map[string]any{"kind": "markdown", "value": "**" + x.Detail + "** `" + w + "`"}, "range": r})
		}
	}
	return s.reply(q.ID, nil)
}
func docPos(raw json.RawMessage) (string, position) {
	var p struct {
		TextDocument struct {
			URI string `json:"uri"`
		} `json:"textDocument"`
		Position position `json:"position"`
	}
	json.Unmarshal(raw, &p)
	return p.TextDocument.URI, p.Position
}
func (s *server) definition(q request) error {
	uri, p := docPos(q.Params)
	d, ok := s.get(uri)
	if !ok {
		return s.reply(q.ID, nil)
	}
	w, _ := wordAt(d.Text, p)
	s.mu.RLock()
	defer s.mu.RUnlock()
	for _, dd := range s.docs {
		for _, x := range scanSymbols(dd) {
			if x.Name == w {
				return s.reply(q.ID, location{x.URI, x.SelectionRange})
			}
		}
	}
	return s.reply(q.ID, nil)
}
func occurrences(d document, w string) []location {
	if w == "" {
		return nil
	}
	re := regexp.MustCompile(`\b` + regexp.QuoteMeta(w) + `\b`)
	ms := re.FindAllStringIndex(d.Text, -1)
	out := make([]location, 0, len(ms))
	for _, m := range ms {
		out = append(out, location{d.URI, rng{posAt(d.Text, m[0]), posAt(d.Text, m[1])}})
	}
	return out
}
func (s *server) references(q request) error {
	uri, p := docPos(q.Params)
	d, ok := s.get(uri)
	if !ok {
		return s.reply(q.ID, []any{})
	}
	w, _ := wordAt(d.Text, p)
	s.mu.RLock()
	defer s.mu.RUnlock()
	var out []location
	for _, dd := range s.docs {
		out = append(out, occurrences(dd, w)...)
	}
	return s.reply(q.ID, out)
}
func (s *server) completion(q request) error {
	uri, _ := docPos(q.Params)
	d, _ := s.get(uri)
	seen := map[string]bool{}
	items := []map[string]any{}
	add := func(v string, k int, detail string) {
		if !seen[v] {
			seen[v] = true
			items = append(items, map[string]any{"label": v, "kind": k, "detail": detail})
		}
	}
	for _, k := range keywords {
		add(k, 14, "Clyth keyword")
	}
	for _, t := range types {
		add(t, 7, "Clyth type")
	}
	for _, x := range scanSymbols(d) {
		add(x.Name, x.Kind, x.Detail)
	}
	sort.Slice(items, func(i, j int) bool { return items[i]["label"].(string) < items[j]["label"].(string) })
	return s.reply(q.ID, map[string]any{"isIncomplete": false, "items": items})
}
func (s *server) rename(q request) error {
	var p struct {
		TextDocument struct {
			URI string `json:"uri"`
		} `json:"textDocument"`
		Position position `json:"position"`
		NewName  string   `json:"newName"`
	}
	json.Unmarshal(q.Params, &p)
	d, ok := s.get(p.TextDocument.URI)
	if !ok {
		return s.reply(q.ID, nil)
	}
	w, _ := wordAt(d.Text, p.Position)
	if w == "" || p.NewName == "" {
		return s.reply(q.ID, nil)
	}
	changes := map[string][]map[string]any{}
	s.mu.RLock()
	defer s.mu.RUnlock()
	for _, dd := range s.docs {
		for _, loc := range occurrences(dd, w) {
			changes[dd.URI] = append(changes[dd.URI], map[string]any{"range": loc.Range, "newText": p.NewName})
		}
	}
	return s.reply(q.ID, map[string]any{"changes": changes})
}

type semanticTok struct {
	line, start, length, typ, mods int
}

func semanticTypeIndex(name string) int {
	for i, v := range semanticTokenTypes {
		if v == name {
			return i
		}
	}
	return 0
}
func semanticMods(names ...string) int {
	bits := 0
	for _, name := range names {
		for i, v := range semanticTokenModifiers {
			if v == name {
				bits |= 1 << i
			}
		}
	}
	return bits
}
func semanticAdd(out *[]semanticTok, text string, start, end int, typ string, mods int) {
	if start < 0 || end <= start || end > len(text) {
		return
	}
	p := posAt(text, start)
	// LSP semantic tokens cannot span lines. All tokens emitted here are identifiers/paths on one line.
	if strings.Contains(text[start:end], "\n") {
		return
	}
	*out = append(*out, semanticTok{p.Line, p.Character, end - start, semanticTypeIndex(typ), mods})
}
func typeTokenKind(name string) string {
	name = strings.TrimSpace(name)
	if len(name) == 1 && name[0] >= 'A' && name[0] <= 'Z' {
		return "typeParameter"
	}
	return "type"
}
func addTypeIdentifiers(out *[]semanticTok, text string, base int, fragment string) {
	re := regexp.MustCompile(`[A-Za-z_]\w*`)
	for _, m := range re.FindAllStringIndex(fragment, -1) {
		name := fragment[m[0]:m[1]]
		if name == "const" {
			continue
		}
		semanticAdd(out, text, base+m[0], base+m[1], typeTokenKind(name), 0)
	}
}
func scanSemanticTokens(text string) []semanticTok {
	out := []semanticTok{}
	// Imports deliberately distinguish package identity from source-file paths.
	for _, m := range moduleImportRE.FindAllStringSubmatchIndex(text, -1) {
		if m[2] >= 0 {
			semanticAdd(&out, text, m[2], m[3], "namespace", 0)
		}
	}
	for _, m := range fileImportRE.FindAllStringSubmatchIndex(text, -1) {
		if m[2] >= 0 {
			semanticAdd(&out, text, m[2]-1, m[3]+1, "string", 0)
		}
	}
	// Fully typed lambdas use ReturnType(TypedParameters...) => { ... }.
	// Highlight the signature as type information and the named parameters as declarations.
	for _, match := range explicitLambdaRE.FindAllStringSubmatchIndex(text, -1) {
		if match[2] < 0 || match[4] < 0 {
			continue
		}
		addTypeIdentifiers(&out, text, match[2], text[match[2]:match[3]])
		parameter_fragment := text[match[4]:match[5]]
		parameter_base := match[4]
		parameter_re := regexp.MustCompile(`(?:const\s+)?([A-Za-z_]\w*(?:\s*<[^>]+>)?(?:\s*\*)?)\s+([A-Za-z_]\w*)`)
		for _, parameter_match := range parameter_re.FindAllStringSubmatchIndex(parameter_fragment, -1) {
			addTypeIdentifiers(&out, text, parameter_base+parameter_match[2], parameter_fragment[parameter_match[2]:parameter_match[3]])
			semanticAdd(&out, text, parameter_base+parameter_match[4], parameter_base+parameter_match[5], "variable", semanticMods("declaration"))
		}
	}
	// Contextual lambdas may contain either typed parameters or bare names whose types
	// come from the receiving callable signature. Highlight the final identifier in
	// each parameter slot as a parameter declaration.
	for _, match := range contextualLambdaRE.FindAllStringSubmatchIndex(text, -1) {
		if match[2] < 0 {
			continue
		}
		fragment := text[match[2]:match[3]]
		fragment_base := match[2]
		segment_start := 0
		for _, segment := range strings.Split(fragment, ",") {
			trimmed := strings.TrimSpace(segment)
			if trimmed == "" {
				segment_start += len(segment) + 1
				continue
			}
			identifiers := regexp.MustCompile(`[A-Za-z_]\w*`).FindAllStringIndex(segment, -1)
			if len(identifiers) > 0 {
				last := identifiers[len(identifiers)-1]
				semanticAdd(&out, text, fragment_base+segment_start+last[0], fragment_base+segment_start+last[1], "variable", semanticMods("declaration"))
			}
			segment_start += len(segment) + 1
		}
	}
	// Callable types use ReturnType(ParameterTypes...) name.  The following identifier
	// makes this a declaration/type context rather than an invocation expression.
	for _, m := range callableTypeRE.FindAllStringSubmatchIndex(text, -1) {
		if m[2] < 0 || m[4] < 0 || m[8] < 0 {
			continue
		}
		addTypeIdentifiers(&out, text, m[2], text[m[2]:m[3]])
		params := text[m[4]:m[5]]
		addTypeIdentifiers(&out, text, m[4], params)
		semanticAdd(&out, text, m[8], m[9], "variable", semanticMods("declaration"))
	}
	// Ordinary function declarations/calls are left as function tokens. Type tokens above
	// take precedence when editors merge semantic and lexical highlighting.
	funcRE := regexp.MustCompile(`\b([A-Za-z_]\w*)\s*\(`)
	for _, m := range funcRE.FindAllStringSubmatchIndex(text, -1) {
		name := text[m[2]:m[3]]
		isType := false
		for _, t := range types {
			if name == t {
				isType = true
				break
			}
		}
		if isType {
			continue
		}
		kind := "function"
		if len(name) > 0 && name[0] >= 'A' && name[0] <= 'Z' {
			kind = "type"
		}
		semanticAdd(&out, text, m[2], m[3], kind, 0)
	}
	// Sort and de-duplicate overlapping starts, preferring the earlier-added structural token.
	sort.SliceStable(out, func(i, j int) bool {
		if out[i].line != out[j].line {
			return out[i].line < out[j].line
		}
		return out[i].start < out[j].start
	})
	filtered := out[:0]
	lastLine, lastEnd := -1, -1
	for _, t := range out {
		if t.line == lastLine && t.start < lastEnd {
			continue
		}
		filtered = append(filtered, t)
		lastLine, lastEnd = t.line, t.start+t.length
	}
	return filtered
}
func encodeSemanticTokens(tokens []semanticTok) []int {
	data := make([]int, 0, len(tokens)*5)
	prevLine, prevStart := 0, 0
	for i, t := range tokens {
		dl := t.line - prevLine
		ds := t.start
		if i > 0 && dl == 0 {
			ds = t.start - prevStart
		}
		data = append(data, dl, ds, t.length, t.typ, t.mods)
		prevLine, prevStart = t.line, t.start
	}
	return data
}
func (s *server) semanticTokens(q request) error {
	var p struct {
		TextDocument struct {
			URI string `json:"uri"`
		} `json:"textDocument"`
	}
	json.Unmarshal(q.Params, &p)
	d, ok := s.get(p.TextDocument.URI)
	if !ok {
		return s.reply(q.ID, map[string]any{"data": []int{}})
	}
	return s.reply(q.ID, map[string]any{"data": encodeSemanticTokens(scanSemanticTokens(d.Text))})
}

func (s *server) publishDiagnostics(uri string) error {
	d, ok := s.get(uri)
	if !ok {
		return nil
	}
	diags := diagnose(d.Text)
	return s.notify("textDocument/publishDiagnostics", map[string]any{"uri": uri, "diagnostics": diags})
}
func diagnose(text string) []map[string]any {
	type st struct {
		r   byte
		off int
	}
	stack := []st{}
	pairs := map[byte]byte{')': '(', ']': '[', '}': '{'}
	open := map[byte]bool{'(': true, '[': true, '{': true}
	out := []map[string]any{}
	inStr := false
	esc := false
	lineComment := false
	blockComment := false
	blockStart := 0
	for i := 0; i < len(text); i++ {
		c := text[i]
		if lineComment {
			if c == '\n' {
				lineComment = false
			}
			continue
		}
		if blockComment {
			if i+1 < len(text) && c == '*' && text[i+1] == '/' {
				blockComment = false
				i++
			}
			continue
		}
		if inStr {
			if esc {
				esc = false
				continue
			}
			if c == '\\' {
				esc = true
				continue
			}
			if c == '"' {
				inStr = false
			}
			continue
		}
		if i+1 < len(text) && c == '/' && text[i+1] == '/' {
			lineComment = true
			i++
			continue
		}
		if i+1 < len(text) && c == '/' && text[i+1] == '*' {
			blockComment = true
			blockStart = i
			i++
			continue
		}
		if c == '"' {
			inStr = true
			continue
		}
		if open[c] {
			stack = append(stack, st{c, i})
			continue
		}
		if want, ok := pairs[c]; ok {
			if len(stack) == 0 || stack[len(stack)-1].r != want {
				out = append(out, diag(text, i, i+1, 1, "unmatched closing delimiter"))
			} else {
				stack = stack[:len(stack)-1]
			}
		}
	}
	for _, x := range stack {
		out = append(out, diag(text, x.off, x.off+1, 1, "unclosed delimiter"))
	}
	if inStr {
		off := len(text) - 1
		if off < 0 {
			off = 0
		}
		out = append(out, diag(text, off, len(text), 1, "unterminated string literal"))
	}
	if blockComment {
		out = append(out, diag(text, blockStart, blockStart+2, 1, "unterminated block comment"))
	}
	return out
}
func diag(text string, a, b, sev int, msg string) map[string]any {
	return map[string]any{"range": rng{posAt(text, a), posAt(text, b)}, "severity": sev, "source": "clyth-lsp", "message": msg}
}
