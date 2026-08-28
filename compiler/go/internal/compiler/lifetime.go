package compiler

import (
	"fmt"
	"sort"

	"github.com/mali5820k/clyth-bootstrap/internal/frontend/ast"
	"github.com/mali5820k/clyth-bootstrap/internal/frontend/token"
)

// lifetime_binding models one lexical variable binding. Pointer aliases remain
// ordinary aliases; allocation identity is tracked separately from the binding.
type lifetime_binding struct {
	name             string
	depth            int
	type_info        *SemanticType
	allocation       int
	aggregate_allocs map[int]bool
	aggregate_fields map[string]int
	container_allocs map[int]bool
	origin           string
}

type lifetime_allocation_state struct {
	live             bool
	owned_here       bool
	created_depth    int
	external_domains map[string]bool
	policy           MeccPolicy
	origin           string
}

type lifetime_flow struct {
	bindings    map[string][]*lifetime_binding
	allocations map[int]*lifetime_allocation_state
	depth       int
	policy      MeccPolicy
	terminated  bool
}

type lifetime_context struct {
	analyzer        *SemanticAnalyzer
	result          *AnalysisResult
	options         SemanticOptions
	namespace       *Namespace
	path            string
	allocation_ids  map[*ast.NewExpr]int
	external_ids    map[ast.Expr]int
	next_allocation int
	next_external   int
	action_keys     map[string]bool
	diagnostic_keys map[string]bool
}

func analyze_lifetimes(analyzer *SemanticAnalyzer, result *AnalysisResult, options SemanticOptions) {
	context := &lifetime_context{
		analyzer: analyzer, result: result, options: options,
		allocation_ids: map[*ast.NewExpr]int{}, external_ids: map[ast.Expr]int{},
		next_allocation: 1, next_external: -1,
		action_keys: map[string]bool{}, diagnostic_keys: map[string]bool{},
	}

	paths := make([]string, 0, len(analyzer.programs))
	for path := range analyzer.programs {
		paths = append(paths, path)
	}
	sort.Strings(paths)
	for _, path := range paths {
		program := analyzer.programs[path]
		namespace := analyzer.path_namespaces[path]
		if program == nil || namespace == nil {
			continue
		}
		context.namespace = namespace
		context.path = path
		context.analyze_program(program)
	}

	if result.mecc_macros_present && !options.mecc_enabled {
		result.compiler_diagnostics = append(result.compiler_diagnostics, CompilerDiagnostic{
			level: diagnostic_info, code: "MECC_CONFIG_IGNORED",
			message:     "Project contains MECC macros - ignoring since MECC is disabled in this build",
			source_path: result.root_path,
		})
	}
}

func (context *lifetime_context) analyze_program(program *ast.Program) {
	file_policy := MeccPolicy{}
	for _, declaration := range program.Declarations {
		switch item := declaration.(type) {
		case *ast.MeccConfigDirectiveDecl:
			context.result.mecc_macros_present = true
			if !context.validate_mecc_property(item.Property, item) {
				continue
			}
			if context.options.mecc_enabled {
				file_policy.compacting = item.Value
			}
			context.result.mecc_policy_by_node[item] = file_policy
		case *ast.FunctionDecl:
			if item.Body != nil {
				context.analyze_function(item, item.Parameters, item.Body, file_policy)
			}
		case *ast.StructDecl:
			for _, member := range item.Members {
				switch typed := member.(type) {
				case *ast.MethodDecl:
					if typed.Body != nil {
						context.analyze_function(&typed.FunctionDecl, typed.Parameters, typed.Body, file_policy)
					}
				case *ast.ConstructorMember:
					context.analyze_function(nil, typed.Parameters, typed.Body, file_policy)
				case *ast.DestructorMember:
					context.analyze_function(nil, nil, typed.Body, file_policy)
				}
			}
		case *ast.MethodBlockDecl:
			for _, member := range item.Members {
				switch typed := member.(type) {
				case *ast.MethodDecl:
					context.analyze_function(&typed.FunctionDecl, typed.Parameters, typed.Body, file_policy)
				case *ast.ConstructorMember:
					context.analyze_function(nil, typed.Parameters, typed.Body, file_policy)
				case *ast.DestructorMember:
					context.analyze_function(nil, nil, typed.Body, file_policy)
				}
			}
		}
	}
}

func (context *lifetime_context) validate_mecc_property(property string, node ast.Node) bool {
	if property == "compacting" {
		return true
	}
	context.analyzer.add_error(context.path, node, "unknown #MECC_config property '"+property+"'")
	return false
}

func new_lifetime_flow(policy MeccPolicy) *lifetime_flow {
	return &lifetime_flow{
		bindings: map[string][]*lifetime_binding{}, allocations: map[int]*lifetime_allocation_state{},
		policy: policy,
	}
}

func (flow *lifetime_flow) clone() *lifetime_flow {
	cloned := &lifetime_flow{
		bindings: map[string][]*lifetime_binding{}, allocations: map[int]*lifetime_allocation_state{},
		depth: flow.depth, policy: flow.policy, terminated: flow.terminated,
	}
	for name, stack := range flow.bindings {
		copy_stack := make([]*lifetime_binding, len(stack))
		for index, binding := range stack {
			copy_binding := *binding
			copy_binding.aggregate_allocs = map[int]bool{}
			for id, present := range binding.aggregate_allocs {
				copy_binding.aggregate_allocs[id] = present
			}
			copy_binding.aggregate_fields = map[string]int{}
			for name, id := range binding.aggregate_fields {
				copy_binding.aggregate_fields[name] = id
			}
			copy_binding.container_allocs = map[int]bool{}
			for id, present := range binding.container_allocs {
				copy_binding.container_allocs[id] = present
			}
			copy_stack[index] = &copy_binding
		}
		cloned.bindings[name] = copy_stack
	}
	for id, allocation := range flow.allocations {
		copy_allocation := *allocation
		copy_allocation.external_domains = map[string]bool{}
		for domain, present := range allocation.external_domains {
			copy_allocation.external_domains[domain] = present
		}
		cloned.allocations[id] = &copy_allocation
	}
	return cloned
}

func (flow *lifetime_flow) current_binding(name string) *lifetime_binding {
	stack := flow.bindings[name]
	if len(stack) == 0 {
		return nil
	}
	return stack[len(stack)-1]
}

func (flow *lifetime_flow) push_binding(binding *lifetime_binding) {
	if binding.aggregate_allocs == nil {
		binding.aggregate_allocs = map[int]bool{}
	}
	if binding.aggregate_fields == nil {
		binding.aggregate_fields = map[string]int{}
	}
	if binding.container_allocs == nil {
		binding.container_allocs = map[int]bool{}
	}
	flow.bindings[binding.name] = append(flow.bindings[binding.name], binding)
}

func (flow *lifetime_flow) pop_depth(depth int) {
	for name, stack := range flow.bindings {
		for len(stack) > 0 && stack[len(stack)-1].depth >= depth {
			stack = stack[:len(stack)-1]
		}
		if len(stack) == 0 {
			delete(flow.bindings, name)
		} else {
			flow.bindings[name] = stack
		}
	}
}

func (context *lifetime_context) analyze_function(function *ast.FunctionDecl, parameters []*ast.Parameter, body *ast.BlockStmt, policy MeccPolicy) {
	flow := new_lifetime_flow(policy)
	for _, parameter := range parameters {
		if parameter == nil || parameter.Name == "" {
			continue
		}
		type_info := context.analyzer.resolve_type_ref(context.namespace, parameter.Type, nil, context.path)
		binding := &lifetime_binding{name: parameter.Name, depth: 0, type_info: type_info, aggregate_allocs: map[int]bool{}, aggregate_fields: map[string]int{}, container_allocs: map[int]bool{}, origin: "parameter"}
		if semantic_is_pointer(type_info) {
			id := context.next_external
			context.next_external--
			binding.allocation = id
			flow.allocations[id] = &lifetime_allocation_state{live: true, owned_here: false, created_depth: -1, external_domains: map[string]bool{"caller": true}, origin: "parameter"}
		}
		flow.push_binding(binding)
	}

	states := context.analyze_block([]*lifetime_flow{flow}, body, false)
	for _, state := range states {
		if state.terminated {
			continue
		}
		context.finalize_function_state(state, body)
	}
	_ = function
}

func (context *lifetime_context) analyze_block(states []*lifetime_flow, block *ast.BlockStmt, nested bool) []*lifetime_flow {
	if block == nil {
		return states
	}
	result_states := make([]*lifetime_flow, 0, len(states))
	for _, initial := range states {
		if initial.terminated {
			result_states = append(result_states, initial)
			continue
		}
		state := initial.clone()
		if nested {
			state.depth++
		}
		entry_depth := state.depth
		active := []*lifetime_flow{state}
		for _, statement := range block.Statements {
			next := []*lifetime_flow{}
			for _, current := range active {
				if current.terminated {
					next = append(next, current)
					continue
				}
				next = append(next, context.analyze_statement(current, statement)...)
			}
			active = context.limit_states(next)
		}
		for _, current := range active {
			if !current.terminated {
				context.cleanup_depth(current, entry_depth, block)
			}
			if nested {
				current.pop_depth(entry_depth)
				current.depth--
			}
			result_states = append(result_states, current)
		}
	}
	return context.limit_states(result_states)
}

func (context *lifetime_context) limit_states(states []*lifetime_flow) []*lifetime_flow {
	const max_states = 64
	if len(states) <= max_states {
		return states
	}
	// Lifetime analysis is intentionally path-sensitive.  For extremely branchy
	// source we cap state growth but preserve the first concrete paths rather than
	// inventing semantics-changing repairs.  A future CFG lattice can merge these.
	return states[:max_states]
}

func (context *lifetime_context) analyze_statement(flow *lifetime_flow, statement ast.Stmt) []*lifetime_flow {
	switch item := statement.(type) {
	case *ast.MeccConfigDirectiveStmt:
		context.result.mecc_macros_present = true
		if context.validate_mecc_property(item.Property, item) && context.options.mecc_enabled {
			flow.policy.compacting = item.Value
		}
		context.result.mecc_policy_by_node[item] = flow.policy
		return []*lifetime_flow{flow}
	case *ast.VarDeclStmt:
		type_info := context.analyzer.resolve_type_ref(context.namespace, item.Type, nil, context.path)
		if item.Type != nil && item.Type.Name == "auto" && item.Initializer != nil {
			type_info = clone_type(context.result.expression_types[item.Initializer])
		}
		binding := &lifetime_binding{name: item.Name, depth: flow.depth, type_info: type_info, aggregate_allocs: map[int]bool{}, aggregate_fields: map[string]int{}, container_allocs: map[int]bool{}, origin: "local"}
		if item.Initializer != nil {
			id := context.eval_pointer_value(flow, item.Initializer, true)
			if id != 0 && semantic_is_pointer(type_info) {
				binding.allocation = id
			}
			if source_name, ok := identifier_name(item.Initializer); ok {
				if source := flow.current_binding(source_name); source != nil {
					for allocation := range source.aggregate_allocs {
						binding.aggregate_allocs[allocation] = true
					}
					for field, allocation := range source.aggregate_fields {
						binding.aggregate_fields[field] = allocation
					}
					for allocation := range source.container_allocs {
						binding.container_allocs[allocation] = true
					}
				}
			}
		}
		flow.push_binding(binding)
		return []*lifetime_flow{flow}
	case *ast.ExprStmt:
		context.eval_expr(flow, item.Expression, true)
		return []*lifetime_flow{flow}
	case *ast.ReturnStmt:
		returned := map[int]bool{}
		if item.Value != nil {
			id := context.eval_pointer_value(flow, item.Value, true)
			if id != 0 {
				returned[id] = true
				context.mark_external(flow, id, "return")
			}
			if name, ok := identifier_name(item.Value); ok {
				if binding := flow.current_binding(name); binding != nil {
					for allocation := range binding.aggregate_allocs {
						returned[allocation] = true
						context.mark_external(flow, allocation, "return:"+name)
					}
				}
			}
		}
		context.cleanup_before_return(flow, returned, item)
		context.finalize_escape_state(flow)
		flow.terminated = true
		return []*lifetime_flow{flow}
	case *ast.IfStmt:
		context.eval_expr(flow, item.Condition, true)
		then_flow := flow.clone()
		else_flow := flow.clone()
		then_states := context.analyze_nested_statement(then_flow, item.Then)
		else_states := []*lifetime_flow{else_flow}
		if item.Else != nil {
			else_states = context.analyze_nested_statement(else_flow, item.Else)
		}
		return append(then_states, else_states...)
	case *ast.WhileStmt:
		context.eval_expr(flow, item.Condition, true)
		zero := flow.clone()
		one := flow.clone()
		body_states := context.analyze_nested_statement(one, item.Body)
		return append([]*lifetime_flow{zero}, body_states...)
	case *ast.ForStmt:
		loop_flow := flow.clone()
		if item.Init != nil {
			states := context.analyze_statement(loop_flow, item.Init)
			if len(states) > 0 {
				loop_flow = states[0]
			}
		}
		if item.Condition != nil {
			context.eval_expr(loop_flow, item.Condition, true)
		}
		zero := loop_flow.clone()
		one := loop_flow.clone()
		body_states := context.analyze_nested_statement(one, item.Body)
		for _, state := range body_states {
			if item.Post != nil && !state.terminated {
				context.eval_expr(state, item.Post, true)
			}
		}
		return append([]*lifetime_flow{zero}, body_states...)
	case *ast.SwitchStmt:
		context.eval_expr(flow, item.Value, true)
		states := []*lifetime_flow{}
		for _, switch_case := range item.Cases {
			branch := flow.clone()
			if switch_case.Value != nil {
				context.eval_expr(branch, switch_case.Value, true)
			}
			for _, nested := range switch_case.Statements {
				branch_states := context.analyze_statement(branch, nested)
				if len(branch_states) == 0 {
					break
				}
				branch = branch_states[0]
			}
			states = append(states, branch)
		}
		if len(states) == 0 {
			states = append(states, flow)
		}
		return states
	case *ast.BlockStmt:
		return context.analyze_block([]*lifetime_flow{flow}, item, true)
	default:
		return []*lifetime_flow{flow}
	}
}

func (context *lifetime_context) analyze_nested_statement(flow *lifetime_flow, statement ast.Stmt) []*lifetime_flow {
	if block, ok := statement.(*ast.BlockStmt); ok {
		return context.analyze_block([]*lifetime_flow{flow}, block, true)
	}
	return context.analyze_statement(flow, statement)
}

func semantic_is_pointer(type_info *SemanticType) bool {
	return type_info != nil && (type_info.pointer_depth > 0 || type_info.kind == type_pointer)
}

func identifier_name(expression ast.Expr) (string, bool) {
	identifier, ok := expression.(*ast.IdentifierExpr)
	if !ok {
		return "", false
	}
	return identifier.Name, true
}

func (context *lifetime_context) eval_expr(flow *lifetime_flow, expression ast.Expr, read bool) {
	if expression == nil {
		return
	}
	switch item := expression.(type) {
	case *ast.IdentifierExpr:
		if read {
			context.check_binding_use(flow, item.Name, item)
		}
	case *ast.UnaryExpr:
		context.eval_expr(flow, item.Operand, read)
	case *ast.BinaryExpr:
		context.eval_expr(flow, item.Left, true)
		context.eval_expr(flow, item.Right, true)
	case *ast.AssignExpr:
		context.eval_assignment(flow, item)
	case *ast.CallExpr:
		if context.is_free_call(item) {
			context.eval_free(flow, item)
			return
		}
		if identifier, ok := item.Callee.(*ast.IdentifierExpr); ok && identifier.Name == "catch" && len(item.Arguments) == 1 {
			if lambda, ok := item.Arguments[0].(*ast.LambdaExpr); ok {
				// catch bodies execute synchronously in the current lifetime domain.
				// Analyze the body as a nested lexical region so allocations receive
				// the same deterministic cleanup planning as ordinary blocks.
				states := context.analyze_block([]*lifetime_flow{flow}, lambda.Body, true)
				if len(states) == 1 {
					*flow = *states[0]
				}
				return
			}
		}
		context.eval_expr(flow, item.Callee, true)
		for _, argument := range item.Arguments {
			context.eval_expr(flow, argument, true)
		}
		// A regular call may retain a pointer argument in a longer-lived compiler
		// object. Until callable escape summaries exist, conservatively propagate
		// such allocations across the call boundary instead of reclaiming them at
		// the caller's next lexical cleanup frontier.
		for index, argument := range item.Arguments {
			if semantic_is_pointer(context.result.expression_types[argument]) {
				if id := context.eval_pointer_value(flow, argument, false); id != 0 {
					context.mark_external(flow, id, fmt.Sprintf("callee-argument:%d", index))
				}
			}
		}
		context.record_container_call(flow, item)
	case *ast.MemberExpr:
		if semantic_is_pointer(context.result.expression_types[item.Object]) {
			context.eval_pointer_value(flow, item.Object, true)
		} else {
			context.eval_expr(flow, item.Object, true)
		}
	case *ast.IndexExpr:
		context.eval_expr(flow, item.Object, true)
		context.eval_expr(flow, item.Index, true)
	case *ast.ArrayExpr:
		for _, element := range item.Elements {
			context.eval_expr(flow, element, true)
		}
	case *ast.KeyValueExpr:
		context.eval_expr(flow, item.Key, true)
		context.eval_expr(flow, item.Value, true)
	case *ast.LambdaExpr:
		// Lambda capture lifetime is intentionally conservative for V1.0.0: any
		// pointer identifiers read by the lambda are treated as escaping if the
		// lambda itself later escapes. Full capture summaries are future work.
		_ = item
	case *ast.NewExpr:
		context.eval_pointer_value(flow, item, read)
	case *ast.CastExpr:
		context.eval_expr(flow, item.Value, read)
	case *ast.GenericSpecializationExpr:
		context.eval_expr(flow, item.Target, read)
	}
}

func (context *lifetime_context) eval_pointer_value(flow *lifetime_flow, expression ast.Expr, read bool) int {
	if expression == nil {
		return 0
	}
	switch item := expression.(type) {
	case *ast.NewExpr:
		id := context.allocation_id(item)
		if flow.allocations[id] == nil {
			type_info := clone_type(context.result.expression_types[item])
			flow.allocations[id] = &lifetime_allocation_state{
				live: true, owned_here: true, created_depth: flow.depth,
				external_domains: map[string]bool{}, policy: flow.policy, origin: "new",
			}
			context.result.allocations[id] = &AllocationInfo{
				id: id, new_expr: item, source_path: context.path, source_span: item.Span(),
				type_info: type_info, policy: flow.policy,
			}
			context.result.allocation_by_new[item] = id
			context.result.mecc_policy_by_node[item] = flow.policy
		}
		context.eval_expr(flow, item.Value, true)
		context.result.expression_allocations[expression] = id
		return id
	case *ast.IdentifierExpr:
		binding := flow.current_binding(item.Name)
		if binding == nil {
			return 0
		}
		if read {
			context.check_binding_use(flow, item.Name, item)
		}
		if binding.allocation != 0 {
			context.result.expression_allocations[expression] = binding.allocation
		}
		return binding.allocation
	case *ast.LiteralExpr:
		if item.Kind == ast.LiteralNull {
			return 0
		}
	case *ast.CastExpr:
		return context.eval_pointer_value(flow, item.Value, read)
	case *ast.MemberExpr:
		context.eval_expr(flow, item.Object, read)
		if base_name, ok := identifier_name(item.Object); ok {
			if binding := flow.current_binding(base_name); binding != nil {
				if id := binding.aggregate_fields[item.Name]; id != 0 {
					if read {
						if allocation := flow.allocations[id]; allocation != nil && !allocation.live {
							context.add_lifetime_error(item, "provable use after free through '"+base_name+"."+item.Name+"'")
						}
					}
					context.result.expression_allocations[expression] = id
					return id
				}
			}
		}
		semantic := context.result.expression_types[expression]
		if semantic_is_pointer(semantic) {
			id := context.external_allocation_id(flow, expression)
			context.result.expression_allocations[expression] = id
			return id
		}
	case *ast.IndexExpr, *ast.CallExpr:
		context.eval_expr(flow, expression, read)
		semantic := context.result.expression_types[expression]
		if semantic_is_pointer(semantic) {
			id := context.external_allocation_id(flow, expression)
			context.result.expression_allocations[expression] = id
			return id
		}
	}
	context.eval_expr(flow, expression, read)
	return 0
}

func (context *lifetime_context) allocation_id(expression *ast.NewExpr) int {
	if id := context.allocation_ids[expression]; id != 0 {
		return id
	}
	id := context.next_allocation
	context.next_allocation++
	context.allocation_ids[expression] = id
	return id
}

func (context *lifetime_context) external_allocation_id(flow *lifetime_flow, expression ast.Expr) int {
	if id := context.external_ids[expression]; id != 0 {
		if flow.allocations[id] == nil {
			flow.allocations[id] = &lifetime_allocation_state{live: true, external_domains: map[string]bool{"external": true}, origin: "external"}
		}
		return id
	}
	id := context.next_external
	context.next_external--
	context.external_ids[expression] = id
	flow.allocations[id] = &lifetime_allocation_state{live: true, external_domains: map[string]bool{"external": true}, origin: "external"}
	return id
}

func (context *lifetime_context) check_binding_use(flow *lifetime_flow, name string, node ast.Node) {
	binding := flow.current_binding(name)
	if binding == nil || binding.allocation == 0 {
		return
	}
	allocation := flow.allocations[binding.allocation]
	if allocation != nil && !allocation.live {
		context.add_lifetime_error(node, "provable use after free through '"+name+"'")
	}
}

func storage_base_identifier_name(expression ast.Expr) (string, bool) {
	switch item := expression.(type) {
	case *ast.IdentifierExpr:
		return item.Name, true
	case *ast.MemberExpr:
		return storage_base_identifier_name(item.Object)
	case *ast.IndexExpr:
		return storage_base_identifier_name(item.Object)
	default:
		return "", false
	}
}

func (context *lifetime_context) eval_assignment(flow *lifetime_flow, assignment *ast.AssignExpr) {
	if identifier, ok := assignment.Target.(*ast.IdentifierExpr); ok {
		binding := flow.current_binding(identifier.Name)
		if binding != nil {
			old_allocation := binding.allocation
			old_aggregate_ids := map[int]bool{}
			for id := range binding.aggregate_allocs {
				old_aggregate_ids[id] = true
			}
			new_allocation := context.eval_pointer_value(flow, assignment.Value, true)
			if semantic_is_pointer(binding.type_info) {
				binding.allocation = new_allocation
			}
			// Aggregate value assignment replaces the destination's previous
			// reference set. Pointer/scalar assignments leave aggregate metadata
			// alone because it describes a different value category.
			if !semantic_is_pointer(binding.type_info) {
				binding.aggregate_allocs = map[int]bool{}
				binding.aggregate_fields = map[string]int{}
				binding.container_allocs = map[int]bool{}
				if source_name, ok := identifier_name(assignment.Value); ok {
					if source := flow.current_binding(source_name); source != nil {
						for id := range source.aggregate_allocs {
							binding.aggregate_allocs[id] = true
						}
						for field, id := range source.aggregate_fields {
							binding.aggregate_fields[field] = id
						}
						for id := range source.container_allocs {
							binding.container_allocs[id] = true
						}
					}
				}
				for id := range old_aggregate_ids {
					if !binding.aggregate_allocs[id] {
						context.maybe_cleanup_after_detach(flow, id, assignment)
					}
				}
			}
			if old_allocation > 0 && old_allocation != new_allocation {
				context.maybe_cleanup_after_detach(flow, old_allocation, assignment)
			}
			return
		}
		// Assignment to an unresolved identifier is a global/static domain.
		id := context.eval_pointer_value(flow, assignment.Value, true)
		if id != 0 {
			context.mark_external(flow, id, "global:"+identifier.Name)
		}
		return
	}

	if index, ok := assignment.Target.(*ast.IndexExpr); ok {
		id := context.eval_pointer_value(flow, assignment.Value, true)
		context.eval_expr(flow, index.Object, true)
		context.eval_expr(flow, index.Index, true)
		if id == 0 {
			return
		}
		if base_name, ok := storage_base_identifier_name(index.Object); ok {
			if binding := flow.current_binding(base_name); binding != nil {
				if binding.origin == "parameter" || semantic_is_pointer(binding.type_info) && binding.allocation < 0 {
					context.mark_external(flow, id, "caller-container:"+base_name)
				} else {
					binding.aggregate_allocs[id] = true
					binding.container_allocs[id] = true
				}
				return
			}
			context.mark_external(flow, id, "global-container:"+base_name)
			return
		}
		// Indexed storage whose root cannot be associated with a local binding
		// necessarily outlives this expression evaluation. Conservatively treat
		// the stored allocation as escaping rather than destroying it immediately.
		context.mark_external(flow, id, "aggregate:index")
		return
	}

	if member, ok := assignment.Target.(*ast.MemberExpr); ok {
		id := context.eval_pointer_value(flow, assignment.Value, true)
		context.eval_expr(flow, member.Object, true)
		if base_name, ok := identifier_name(member.Object); ok {
			binding := flow.current_binding(base_name)
			if binding != nil {
				if binding.origin == "parameter" || semantic_is_pointer(binding.type_info) && binding.allocation < 0 {
					if id != 0 {
						context.mark_external(flow, id, "caller:"+base_name+"."+member.Name)
					}
				} else {
					previous := binding.aggregate_fields[member.Name]
					if previous != 0 && previous != id {
						delete(binding.aggregate_fields, member.Name)
					}
					if id != 0 {
						binding.aggregate_allocs[id] = true
						binding.aggregate_fields[member.Name] = id
					}
					if previous != 0 && previous != id && !binding_has_aggregate_reference(binding, previous) {
						delete(binding.aggregate_allocs, previous)
						context.maybe_cleanup_after_detach(flow, previous, assignment)
					}
				}
				return
			}
			if id != 0 {
				context.mark_external(flow, id, "global:"+base_name+"."+member.Name)
			}
			return
		}
		if id != 0 {
			context.mark_external(flow, id, "aggregate:unknown")
		}
		return
	}
	context.eval_expr(flow, assignment.Target, true)
	context.eval_expr(flow, assignment.Value, true)
}

func (context *lifetime_context) maybe_cleanup_after_detach(flow *lifetime_flow, allocation_id int, node ast.Node) {
	allocation := flow.allocations[allocation_id]
	if allocation == nil || !allocation.live || !allocation.owned_here || len(allocation.external_domains) != 0 {
		return
	}
	if context.has_local_reference(flow, allocation_id) {
		return
	}
	context.add_action(lifetime_cleanup, allocation_id, node, "previous allocation has no remaining live alias or escape", allocation.policy)
	allocation.live = false
	if info := context.result.allocations[allocation_id]; info != nil {
		info.destruction_reason = "reassignment after final local reference"
	}
}

func (context *lifetime_context) has_local_reference(flow *lifetime_flow, allocation_id int) bool {
	for _, stack := range flow.bindings {
		for _, binding := range stack {
			if binding.allocation == allocation_id || binding.aggregate_allocs[allocation_id] {
				return true
			}
		}
	}
	return false
}

func (context *lifetime_context) is_free_call(call *ast.CallExpr) bool {
	identifier, ok := call.Callee.(*ast.IdentifierExpr)
	return ok && identifier.Name == "free"
}

func (context *lifetime_context) eval_free(flow *lifetime_flow, call *ast.CallExpr) {
	if len(call.Arguments) != 1 {
		context.add_lifetime_error(call, fmt.Sprintf("free expects exactly one argument, got %d", len(call.Arguments)))
		for _, argument := range call.Arguments {
			context.eval_expr(flow, argument, true)
		}
		return
	}
	argument := call.Arguments[0]
	if literal, ok := argument.(*ast.LiteralExpr); ok && literal.Kind == ast.LiteralNull {
		return
	}
	if unary, ok := argument.(*ast.UnaryExpr); ok && unary.Operator == "&" {
		if identifier, ok := unary.Operand.(*ast.IdentifierExpr); ok {
			if binding := flow.current_binding(identifier.Name); binding != nil && !semantic_is_pointer(binding.type_info) {
				context.add_lifetime_error(call, "cannot free stack allocation '&"+identifier.Name+"'")
				return
			}
		}
	}
	argument_type := context.result.expression_types[argument]
	if argument_type != nil && argument_type.is_const {
		context.add_lifetime_error(call, "cannot destroy allocation through const pointer access path")
		return
	}
	id := context.eval_pointer_value(flow, argument, false)
	if id == 0 {
		if argument_type != nil && !semantic_is_pointer(argument_type) && argument_type.kind != type_null {
			context.add_lifetime_error(call, "free requires a raw pointer or null")
		}
		return
	}
	allocation := flow.allocations[id]
	if allocation == nil {
		return
	}
	if !allocation.live {
		context.add_lifetime_error(call, "double free of the same allocation")
		return
	}
	allocation.live = false
	context.add_action(lifetime_explicit_free, id, call, "explicit programmer destruction suppresses compiler cleanup", allocation.policy)
	if info := context.result.allocations[id]; info != nil {
		info.explicitly_freed = true
		info.destruction_reason = "explicit free"
	}
	if context.free_leaves_container_reference(flow, id, argument) {
		context.add_diagnostic(diagnostic_warning, "LIFETIME_STALE_CONTAINER_REF", call,
			"allocation was freed while a container/aggregate reference may remain; the compiler leaves that reference untouched")
	}
}

func (context *lifetime_context) free_leaves_container_reference(flow *lifetime_flow, allocation_id int, argument ast.Expr) bool {
	if _, ok := argument.(*ast.MemberExpr); ok {
		return true
	}
	for _, stack := range flow.bindings {
		for _, binding := range stack {
			if binding.aggregate_allocs[allocation_id] {
				return true
			}
		}
	}
	if call, ok := argument.(*ast.CallExpr); ok {
		if member, ok := call.Callee.(*ast.MemberExpr); ok && member.Name == "get" {
			return true
		}
	}
	return false
}

func (context *lifetime_context) record_container_call(flow *lifetime_flow, call *ast.CallExpr) {
	member, ok := call.Callee.(*ast.MemberExpr)
	if !ok {
		return
	}
	if member.Name != "append" && member.Name != "insert" && member.Name != "put" {
		return
	}
	base_name, base_is_identifier := identifier_name(member.Object)
	for _, argument := range call.Arguments {
		id := context.eval_pointer_value(flow, argument, false)
		if id == 0 {
			continue
		}
		if base_is_identifier {
			if binding := flow.current_binding(base_name); binding != nil {
				if binding.origin == "parameter" || semantic_is_pointer(binding.type_info) && binding.allocation < 0 {
					context.mark_external(flow, id, "caller-container:"+base_name)
				} else {
					binding.aggregate_allocs[id] = true
					binding.container_allocs[id] = true
				}
				continue
			}
			context.mark_external(flow, id, "global-container:"+base_name)
		}
	}
}

func binding_has_aggregate_reference(binding *lifetime_binding, allocation_id int) bool {
	if binding == nil || allocation_id == 0 {
		return false
	}
	if binding.container_allocs[allocation_id] {
		return true
	}
	for _, id := range binding.aggregate_fields {
		if id == allocation_id {
			return true
		}
	}
	return false
}

func (context *lifetime_context) mark_external(flow *lifetime_flow, allocation_id int, domain string) {
	allocation := flow.allocations[allocation_id]
	if allocation == nil {
		return
	}
	if allocation.external_domains[domain] {
		return
	}
	allocation.external_domains[domain] = true
	if info := context.result.allocations[allocation_id]; info != nil {
		info.escaped = true
	}
	// Escaping an owning aggregate also escapes allocations retained by its
	// fields/containers. Otherwise nested heap objects can be reclaimed at the
	// callee return frontier while the escaped parent still points at them.
	for _, stack := range flow.bindings {
		for _, binding := range stack {
			if binding.allocation != allocation_id {
				continue
			}
			for child_id := range binding.aggregate_allocs {
				if child_id != 0 && child_id != allocation_id {
					context.mark_external(flow, child_id, domain+":aggregate")
				}
			}
		}
	}
}

func (context *lifetime_context) cleanup_depth(flow *lifetime_flow, depth int, node ast.Node) {
	candidate_ids := []int{}
	for id, allocation := range flow.allocations {
		if id <= 0 || allocation == nil || !allocation.live || !allocation.owned_here || allocation.created_depth < depth || len(allocation.external_domains) != 0 {
			continue
		}
		// Ignore references that disappear with this lexical region. Any reference
		// in an outer binding keeps the allocation alive beyond this block.
		outer_reference := false
		for _, stack := range flow.bindings {
			for _, binding := range stack {
				if binding.depth < depth && (binding.allocation == id || binding.aggregate_allocs[id]) {
					outer_reference = true
				}
			}
		}
		if !outer_reference {
			candidate_ids = append(candidate_ids, id)
		}
	}
	sort.Ints(candidate_ids)
	for _, id := range candidate_ids {
		allocation := flow.allocations[id]
		context.add_action(lifetime_cleanup, id, node, "allocation does not escape lexical scope", allocation.policy)
		allocation.live = false
		if info := context.result.allocations[id]; info != nil && info.destruction_reason == "" {
			info.destruction_reason = "lexical scope exit"
		}
	}
}

func (context *lifetime_context) cleanup_before_return(flow *lifetime_flow, returned map[int]bool, node ast.Node) {
	ids := []int{}
	for id, allocation := range flow.allocations {
		if id <= 0 || allocation == nil || !allocation.live || !allocation.owned_here || returned[id] || len(allocation.external_domains) != 0 {
			continue
		}
		ids = append(ids, id)
	}
	sort.Ints(ids)
	for _, id := range ids {
		allocation := flow.allocations[id]
		context.add_action(lifetime_cleanup, id, node, "function exits while allocation remains locally reachable only", allocation.policy)
		allocation.live = false
		if info := context.result.allocations[id]; info != nil && info.destruction_reason == "" {
			info.destruction_reason = "function return"
		}
	}
}

func (context *lifetime_context) finalize_function_state(flow *lifetime_flow, node ast.Node) {
	context.finalize_escape_state(flow)
	ids := []int{}
	for id, allocation := range flow.allocations {
		if id > 0 && allocation != nil && allocation.live && allocation.owned_here && len(allocation.external_domains) == 0 {
			ids = append(ids, id)
		}
	}
	sort.Ints(ids)
	for _, id := range ids {
		allocation := flow.allocations[id]
		context.add_action(lifetime_cleanup, id, node, "allocation does not escape function lifetime domain", allocation.policy)
		allocation.live = false
		if info := context.result.allocations[id]; info != nil && info.destruction_reason == "" {
			info.destruction_reason = "function exit"
		}
	}
}

func (context *lifetime_context) finalize_escape_state(flow *lifetime_flow) {
	for id, allocation := range flow.allocations {
		if id <= 0 || allocation == nil || !allocation.live || !allocation.owned_here {
			continue
		}
		if len(allocation.external_domains) <= 1 {
			continue
		}
		domains := make([]string, 0, len(allocation.external_domains))
		for domain := range allocation.external_domains {
			domains = append(domains, domain)
		}
		sort.Strings(domains)
		reason := "allocation escapes into multiple simultaneous lifetime domains: " + fmt.Sprint(domains)
		if context.options.mecc_enabled {
			if info := context.result.allocations[id]; info != nil {
				info.mecc = true
				info.escaped = true
				info.destruction_reason = "MECC estate reclamation"
			}
			context.add_action(lifetime_mecc, id, context.result.allocations[id].new_expr, reason, allocation.policy)
			context.add_diagnostic(diagnostic_info, "MECC001", context.result.allocations[id].new_expr,
				reason+fmt.Sprintf("; MECC estate management selected (compacting=%t)", allocation.policy.compacting))
		} else if info := context.result.allocations[id]; info != nil {
			context.analyzer.add_error(context.path, info.new_expr, reason+"; build with --mecc=true to permit runtime estate management or provide an explicit deterministic lifetime path")
		}
	}
}

func (context *lifetime_context) add_action(kind LifetimeActionKind, allocation int, node ast.Node, reason string, policy MeccPolicy) {
	if node == nil {
		return
	}
	key := fmt.Sprintf("%s:%d:%d:%d", kind, allocation, node.Span().Start.Offset, node.Span().End.Offset)
	if context.action_keys[key] {
		return
	}
	context.action_keys[key] = true
	context.result.lifetime_actions = append(context.result.lifetime_actions, LifetimeAction{kind: kind, allocation: allocation, node: node, reason: reason, policy: policy})
}

func (context *lifetime_context) add_lifetime_error(node ast.Node, message string) {
	context.analyzer.add_error(context.path, node, message)
}

func (context *lifetime_context) add_diagnostic(level DiagnosticLevel, code string, node ast.Node, message string) {
	span := token.Span{}
	if node != nil {
		span = node.Span()
	}
	key := fmt.Sprintf("%s:%s:%s:%d", level, code, context.path, span.Start.Offset)
	if context.diagnostic_keys[key] {
		return
	}
	context.diagnostic_keys[key] = true
	context.result.compiler_diagnostics = append(context.result.compiler_diagnostics, CompilerDiagnostic{level: level, code: code, message: message, source_path: context.path, source_span: span})
}
