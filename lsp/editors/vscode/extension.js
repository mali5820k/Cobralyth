const vscode = require('vscode');
const cp = require('child_process');
const path = require('path');

let proc;
let pending = new Map();
let nextID = 1;
let recv = Buffer.alloc(0);
const diagnostics = vscode.languages.createDiagnosticCollection('clyth');

function toRange(r) {
  return new vscode.Range(r.start.line, r.start.character, r.end.line, r.end.character);
}
function toPos(p) { return { line: p.line, character: p.character }; }
function docID(doc) { return { uri: doc.uri.toString() }; }

function send(msg) {
  const body = Buffer.from(JSON.stringify(msg), 'utf8');
  proc.stdin.write(`Content-Length: ${body.length}\r\n\r\n`);
  proc.stdin.write(body);
}
function notify(method, params) { send({ jsonrpc: '2.0', method, params }); }
function request(method, params) {
  const id = nextID++;
  return new Promise((resolve, reject) => {
    pending.set(id, { resolve, reject });
    send({ jsonrpc: '2.0', id, method, params });
  });
}
function handle(msg) {
  if (Object.prototype.hasOwnProperty.call(msg, 'id')) {
    const p = pending.get(msg.id);
    if (p) {
      pending.delete(msg.id);
      if (msg.error) p.reject(new Error(msg.error.message || JSON.stringify(msg.error)));
      else p.resolve(msg.result);
    }
    return;
  }
  if (msg.method === 'textDocument/publishDiagnostics') {
    const uri = vscode.Uri.parse(msg.params.uri);
    const ds = (msg.params.diagnostics || []).map(d => {
      const x = new vscode.Diagnostic(toRange(d.range), d.message, Math.max(0, (d.severity || 1) - 1));
      x.source = d.source || 'clyth-lsp';
      return x;
    });
    diagnostics.set(uri, ds);
  }
}
function consume(data) {
  recv = Buffer.concat([recv, data]);
  while (true) {
    const split = recv.indexOf('\r\n\r\n');
    if (split < 0) return;
    const header = recv.slice(0, split).toString('ascii');
    const m = /content-length:\s*(\d+)/i.exec(header);
    if (!m) { recv = Buffer.alloc(0); return; }
    const len = Number(m[1]);
    const start = split + 4;
    if (recv.length < start + len) return;
    const body = recv.slice(start, start + len).toString('utf8');
    recv = recv.slice(start + len);
    try { handle(JSON.parse(body)); } catch (e) { console.error('clyth-lsp message error', e); }
  }
}
function bundledServer(context) {
  const platform = process.platform;
  const arch = process.arch;
  const ext = platform === 'win32' ? '.exe' : '';
  return path.join(context.extensionPath, 'bin', `${platform}-${arch}`, `clyth-lsp${ext}`);
}
async function activate(context) {
  const configured = vscode.workspace.getConfiguration('clyth').get('lsp.path', '').trim();
  const command = configured || bundledServer(context);
  proc = cp.spawn(command, [], { stdio: ['pipe', 'pipe', 'pipe'] });
  proc.stdout.on('data', consume);
  proc.stderr.on('data', d => console.error(`[clyth-lsp] ${d.toString()}`));
  proc.on('error', e => vscode.window.showErrorMessage(`Unable to start clyth-lsp: ${e.message}`));
  proc.on('exit', code => { if (code && code !== 0) vscode.window.showErrorMessage(`clyth-lsp exited with code ${code}`); });

  const root = vscode.workspace.workspaceFolders?.[0]?.uri.toString() || null;
  await request('initialize', { processId: process.pid, rootUri: root, capabilities: {} });
  notify('initialized', {});

  const open = doc => {
    if (doc.languageId === 'clyth') notify('textDocument/didOpen', { textDocument: { uri: doc.uri.toString(), languageId: 'clyth', version: doc.version, text: doc.getText() } });
  };
  vscode.workspace.textDocuments.forEach(open);
  context.subscriptions.push(vscode.workspace.onDidOpenTextDocument(open));
  context.subscriptions.push(vscode.workspace.onDidChangeTextDocument(e => {
    if (e.document.languageId === 'clyth') notify('textDocument/didChange', { textDocument: { uri: e.document.uri.toString(), version: e.document.version }, contentChanges: [{ text: e.document.getText() }] });
  }));
  context.subscriptions.push(vscode.workspace.onDidCloseTextDocument(doc => {
    if (doc.languageId === 'clyth') { notify('textDocument/didClose', { textDocument: docID(doc) }); diagnostics.delete(doc.uri); }
  }));

  const sel = { language: 'clyth', scheme: 'file' };
  context.subscriptions.push(diagnostics);

  // Keep this legend in lock-step with clyth-lsp. Semantic tokens make callable
  // type declarations visually distinct from invocation expressions and distinguish
  // package imports from quoted source-file imports.
  const semanticLegend = new vscode.SemanticTokensLegend(
    ['namespace', 'type', 'typeParameter', 'function', 'variable', 'string'],
    ['declaration', 'readonly']
  );
  context.subscriptions.push(vscode.languages.registerDocumentSemanticTokensProvider(sel, {
    async provideDocumentSemanticTokens(doc) {
      const r = await request('textDocument/semanticTokens/full', { textDocument: docID(doc) });
      return new vscode.SemanticTokens(new Uint32Array(r?.data || []));
    }
  }, semanticLegend));
  context.subscriptions.push(vscode.languages.registerCompletionItemProvider(sel, {
    async provideCompletionItems(doc, pos) {
      const r = await request('textDocument/completion', { textDocument: docID(doc), position: toPos(pos) });
      return (r?.items || []).map(i => { const x = new vscode.CompletionItem(i.label, Math.max(0, (i.kind || 1) - 1)); x.detail = i.detail; return x; });
    }
  }, '.', ':'));
  context.subscriptions.push(vscode.languages.registerHoverProvider(sel, {
    async provideHover(doc, pos) {
      const r = await request('textDocument/hover', { textDocument: docID(doc), position: toPos(pos) });
      if (!r) return null;
      const value = typeof r.contents === 'string' ? r.contents : (r.contents?.value || '');
      return new vscode.Hover(new vscode.MarkdownString(value), r.range ? toRange(r.range) : undefined);
    }
  }));
  context.subscriptions.push(vscode.languages.registerDefinitionProvider(sel, {
    async provideDefinition(doc, pos) {
      const r = await request('textDocument/definition', { textDocument: docID(doc), position: toPos(pos) });
      return r ? new vscode.Location(vscode.Uri.parse(r.uri), toRange(r.range)) : null;
    }
  }));
  context.subscriptions.push(vscode.languages.registerReferenceProvider(sel, {
    async provideReferences(doc, pos) {
      const r = await request('textDocument/references', { textDocument: docID(doc), position: toPos(pos), context: { includeDeclaration: true } });
      return (r || []).map(x => new vscode.Location(vscode.Uri.parse(x.uri), toRange(x.range)));
    }
  }));
  context.subscriptions.push(vscode.languages.registerDocumentSymbolProvider(sel, {
    async provideDocumentSymbols(doc) {
      const r = await request('textDocument/documentSymbol', { textDocument: docID(doc) });
      return (r || []).map(x => new vscode.DocumentSymbol(x.name, x.detail || '', Math.max(0, (x.kind || 1) - 1), toRange(x.range), toRange(x.selectionRange)));
    }
  }));
  context.subscriptions.push(vscode.languages.registerRenameProvider(sel, {
    async provideRenameEdits(doc, pos, newName) {
      const r = await request('textDocument/rename', { textDocument: docID(doc), position: toPos(pos), newName });
      if (!r?.changes) return null;
      const edit = new vscode.WorkspaceEdit();
      for (const [uri, edits] of Object.entries(r.changes)) for (const e of edits) edit.replace(vscode.Uri.parse(uri), toRange(e.range), e.newText);
      return edit;
    }
  }));
}
async function deactivate() {
  if (!proc) return;
  try { await request('shutdown', null); notify('exit', null); } catch (_) {}
  proc.kill();
  proc = undefined;
}
module.exports = { activate, deactivate };
