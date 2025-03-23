from functions.basefun import BaseFunction
from utils.errors import RTError
from execution.runtime import RTResult
from core.values import List, Number, String
from utils.constants import (
    TT_DIV,
    TT_EE,
    TT_GT,
    TT_GTE,
    TT_KEYWORD,
    TT_LT,
    TT_LTE,
    TT_MINUS,
    TT_MUL,
    TT_NE,
    TT_PLUS,
    TT_POW,
)
from core.ai_runtime import AIManager, EmbeddingManager


class Interpreter:
    def __init__(self):
        self.ai_manager = AIManager()
        self.embedding_manager = EmbeddingManager()

    def visit(self, node, context):
        """
        Dispatch the appropriate visitor method for a given AST node.

        Determines the method name dynamically based on the node's class
        (e.g., 'visit_NumberNode') and invokes that method with the node
        and execution context.
        """
        method_name = f"visit_{type(node).__name__}"
        method = getattr(self, method_name, self.no_visit_method)
        return method(node, context)

    def no_visit_method(self, node, context):
        """
        Handle the case when a visitor method for a given node type is not defined.

        This method is called by 'visit()' if the specific visitor method is missing,
        raising an exception to indicate that the node type is not supported.
        """
        raise Exception(f"No visit_{type(node).__name__} method defined")

    ###################################

    def visit_NumberNode(self, node, context):
        """Visit a number literal node and return its numeric value wrapped in an RTResult."""
        return RTResult().success(
            Number(node.tok.value)
            .set_context(context)
            .set_pos(node.pos_start, node.pos_end)
        )

    def visit_StringNode(self, node, context):
        """Visit a string literal node and return its string value wrapped in an RTResult."""
        return RTResult().success(
            String(node.tok.value)
            .set_context(context)
            .set_pos(node.pos_start, node.pos_end)
        )

    def visit_ListNode(self, node, context):
        """Visit a list literal node and recursively evaluate its elements.

        Iterates over each element node in the list, evaluates them, and collects the resulting values.
        Returns a List value with the evaluated elements, setting its context and positional info.
        """
        res = RTResult()
        elements = []

        for element_node in node.element_nodes:
            elements.append(res.register(self.visit(element_node, context)))
            if res.should_return():
                return res

        return res.success(
            List(elements).set_context(context).set_pos(node.pos_start, node.pos_end)
        )

    def visit_VarAccessNode(self, node, context):
        """Visit a variable access node, retrieve the variable's value from the symbol table, and return it."""
        res = RTResult()
        var_name = node.var_name_tok.value
        value = context.symbol_table.get(var_name)

        if not value:
            return res.failure(
                RTError(
                    node.pos_start,
                    node.pos_end,
                    f"'{var_name}' is not defined",
                    context,
                )
            )

        value = value.copy().set_pos(node.pos_start, node.pos_end).set_context(context)
        return res.success(value)

    def visit_VarAssignNode(self, node, context):
        """Visit a variable assignment node, evaluate the assigned expression, and update the symbol table."""
        res = RTResult()
        var_name = node.var_name_tok.value
        value = res.register(self.visit(node.value_node, context))
        if res.should_return():
            return res

        context.symbol_table.set(var_name, value)
        return res.success(value)

    def visit_BinOpNode(self, node, context):
        """Visit a binary operation node, evaluate both operands, and perform the operation."""
        res = RTResult()
        left = res.register(self.visit(node.left_node, context))
        if res.should_return():
            return res
        right = res.register(self.visit(node.right_node, context))
        if res.should_return():
            return res

        if node.op_tok.type == TT_PLUS:
            result, error = left.added_to(right)
        elif node.op_tok.type == TT_MINUS:
            result, error = left.subbed_by(right)
        elif node.op_tok.type == TT_MUL:
            result, error = left.multed_by(right)
        elif node.op_tok.type == TT_DIV:
            result, error = left.dived_by(right)
        elif node.op_tok.type == TT_POW:
            result, error = left.powed_by(right)
        elif node.op_tok.type == TT_EE:
            result, error = left.get_comparison_eq(right)
        elif node.op_tok.type == TT_NE:
            result, error = left.get_comparison_ne(right)
        elif node.op_tok.type == TT_LT:
            result, error = left.get_comparison_lt(right)
        elif node.op_tok.type == TT_GT:
            result, error = left.get_comparison_gt(right)
        elif node.op_tok.type == TT_LTE:
            result, error = left.get_comparison_lte(right)
        elif node.op_tok.type == TT_GTE:
            result, error = left.get_comparison_gte(right)
        elif node.op_tok.matches(TT_KEYWORD, "AND"):
            result, error = left.anded_by(right)
        elif node.op_tok.matches(TT_KEYWORD, "OR"):
            result, error = left.ored_by(right)

        if error:
            return res.failure(error)
        else:
            return res.success(result.set_pos(node.pos_start, node.pos_end))

    def visit_UnaryOpNode(self, node, context):
        """Visit a unary operation node, evaluate its operand
        (such as minus (negation) or logical NOT), and apply the unary operator."""
        res = RTResult()
        number = res.register(self.visit(node.node, context))
        if res.should_return():
            return res

        error = None

        if node.op_tok.type == TT_MINUS:
            number, error = number.multed_by(Number(-1))
        elif node.op_tok.matches(TT_KEYWORD, "NOT"):
            number, error = number.notted()

        if error:
            return res.failure(error)
        else:
            return res.success(number.set_pos(node.pos_start, node.pos_end))

    def visit_IfNode(self, node, context):
        """Visit an if-statement node and execute the corresponding branch.

        Evaluates each condition in the if/elif cases sequentially. If a condition is true, its
        corresponding expression (or statement block) is evaluated and returned. If no conditions are true,
        the else branch (if present) is evaluated.
        """
        res = RTResult()

        for condition, expr, should_return_null in node.cases:
            condition_value = res.register(self.visit(condition, context))
            if res.should_return():
                return res

            if condition_value.is_true():
                expr_value = res.register(self.visit(expr, context))
                if res.should_return():
                    return res
                return res.success(Number.null if should_return_null else expr_value)

        if node.else_case:
            expr, should_return_null = node.else_case
            expr_value = res.register(self.visit(expr, context))
            if res.should_return():
                return res
            return res.success(Number.null if should_return_null else expr_value)

        return res.success(Number.null)

    def visit_ForNode(self, node, context):
        """Visit a for-loop node, iterating over a range of values and evaluating the loop body.

        Evaluates the start, end, and optional step expressions to determine the loop range.
        For each iteration, assigns the current value to the loop variable and evaluates the body.
        Accumulates the results (if required) and handles control flow (continue/break).
        """
        res = RTResult()
        elements = []

        start_value = res.register(self.visit(node.start_value_node, context))
        if res.should_return():
            return res

        end_value = res.register(self.visit(node.end_value_node, context))
        if res.should_return():
            return res

        if node.step_value_node:
            step_value = res.register(self.visit(node.step_value_node, context))
            if res.should_return():
                return res
        else:
            step_value = Number(1)

        i = start_value.value

        if step_value.value >= 0:
            condition = lambda: i < end_value.value
        else:
            condition = lambda: i > end_value.value

        while condition():
            context.symbol_table.set(node.var_name_tok.value, Number(i))
            i += step_value.value

            value = res.register(self.visit(node.body_node, context))
            if (
                res.should_return()
                and res.loop_should_continue == False
                and res.loop_should_break == False
            ):
                return res

            if res.loop_should_continue:
                continue

            if res.loop_should_break:
                break

            elements.append(value)

        return res.success(
            Number.null
            if node.should_return_null
            else List(elements)
            .set_context(context)
            .set_pos(node.pos_start, node.pos_end)
        )

    def visit_WhileNode(self, node, context):
        """Visit a while-loop node, repeatedly evaluating its condition and executing the body.

        Continuously evaluates the loop's condition. If true, it evaluates the loop body, accumulating
        results if applicable, and handles control flow signals (continue/break). When the condition becomes false,
        returns the accumulated results.
        """
        res = RTResult()
        elements = []

        while True:
            condition = res.register(self.visit(node.condition_node, context))
            if res.should_return():
                return res

            if not condition.is_true():
                break

            value = res.register(self.visit(node.body_node, context))
            if (
                res.should_return()
                and res.loop_should_continue == False
                and res.loop_should_break == False
            ):
                return res

            if res.loop_should_continue:
                continue

            if res.loop_should_break:
                break

            elements.append(value)

        return res.success(
            Number.null
            if node.should_return_null
            else List(elements)
            .set_context(context)
            .set_pos(node.pos_start, node.pos_end)
        )

    def visit_FuncDefNode(self, node, context):
        """Visit a function definition node and create a function value.

        Evaluates the function definition by extracting its name, parameters, and body.
        Creates a Function object with these attributes, sets its context and position, and registers it
        in the symbol table if it has a name.
        """
        res = RTResult()

        func_name = node.var_name_tok.value if node.var_name_tok else None
        body_node = node.body_node
        arg_names = [arg_name.value for arg_name in node.arg_name_toks]
        func_value = (
            Function(func_name, body_node, arg_names, node.should_auto_return)
            .set_context(context)
            .set_pos(node.pos_start, node.pos_end)
        )

        if node.var_name_tok:
            context.symbol_table.set(func_name, func_value)

        return res.success(func_value)

    def visit_CallNode(self, node, context):
        """Visit a function call node, evaluate the callable and its arguments, and execute the function.

        Evaluates the node representing the function to be called, then evaluates each argument.
        Executes the function with the evaluated arguments in a new execution context,
        and returns the function's result.
        """
        res = RTResult()
        args = []

        value_to_call = res.register(self.visit(node.node_to_call, context))
        if res.should_return():
            return res
        value_to_call = value_to_call.copy().set_pos(node.pos_start, node.pos_end)

        for arg_node in node.arg_nodes:
            args.append(res.register(self.visit(arg_node, context)))
            if res.should_return():
                return res

        return_value = res.register(value_to_call.execute(args))
        if res.should_return():
            return res
        return_value = (
            return_value.copy()
            .set_pos(node.pos_start, node.pos_end)
            .set_context(context)
        )
        return res.success(return_value)

    def visit_ReturnNode(self, node, context):
        """Visit a return node and signal a function return.

        Evaluates the expression (if any) following the return keyword, and wraps it in an RTResult
        that indicates a function return.
        """
        res = RTResult()

        if node.node_to_return:
            value = res.register(self.visit(node.node_to_return, context))
            if res.should_return():
                return res
        else:
            value = Number.null

        return res.success_return(value)

    def visit_ContinueNode(self, node, context):
        """Visit a continue node and signal that the current loop should continue to its next iteration."""
        return RTResult().success_continue()

    def visit_BreakNode(self, node, context):
        """Visit a break node and signal that the current loop should be exited."""
        return RTResult().success_break()

    def visit_EmbedNode(self, node, context):
        """Visit an embedding node and generate embeddings for the text."""
        res = RTResult()
        
        text = res.register(self.visit(node.text_node, context))
        if res.should_return():
            return res
            
        if not isinstance(text, String):
            return res.failure(RTError(
                node.pos_start, node.pos_end,
                "First argument must be a string",
                context
            ))
            
        model_name = 'default'
        if node.model_node:
            model = res.register(self.visit(node.model_node, context))
            if res.should_return():
                return res
            model_name = model.value
            
        try:
            embedding = self.embedding_manager.embed(text.value, model_name)
            return res.success(embedding)
        except Exception as e:
            return res.failure(RTError(
                node.pos_start, node.pos_end,
                f"Error generating embedding: {str(e)}",
                context
            ))

    def visit_AICallNode(self, node, context):
        """Visit an AI call node and execute the model with given arguments."""
        res = RTResult()
        
        args = []
        for arg_node in node.args:
            arg = res.register(self.visit(arg_node, context))
            if res.should_return():
                return res
            args.append(arg.value)
            
        result, error = self.ai_manager.call_model(
            node.model_name.value,
            args,
            node.pos_start,
            node.pos_end,
            context
        )
        
        if error:
            return res.failure(error)
        return res.success(result)

    def visit_PipeNode(self, node, context):
        """Visit a pipe node and chain operations."""
        res = RTResult()
        
        left = res.register(self.visit(node.left_node, context))
        if res.should_return():
            return res
            
        if isinstance(node.right_node, VarAccessNode):
            # Handle function reference
            func = context.symbol_table.get(node.right_node.var_name_tok.value)
            if not isinstance(func, BaseFunction):
                return res.failure(RTError(
                    node.right_node.pos_start,
                    node.right_node.pos_end,
                    "Expected a function",
                    context
                ))
            args = [left]
        else:
            # Handle direct function
            func = res.register(self.visit(node.right_node, context))
            if res.should_return():
                return res
            args = [left]
            
        result = res.register(func.execute(args))
        if res.should_return():
            return res
            
        return res.success(result)


class Function(BaseFunction):
    """Represents a user-defined function in the language."""

    def __init__(self, name, body_node, arg_names, should_auto_return):
        """Initialize a new Function instance."""

        super().__init__(name)
        self.body_node = body_node
        self.arg_names = arg_names
        self.should_auto_return = should_auto_return

    def execute(self, args):
        """Execute the function with the provided arguments.

        This method performs the following steps:
          1. Creates a new execution context for the function.
          2. Checks and populates the arguments into the context.
          3. Uses an Interpreter instance to visit and evaluate the function body.
          4. Determines the return value based on the auto-return flag, the interpreter's return value,
             or defaults to Number.null.
        """
        res = RTResult()
        interpreter = Interpreter()
        exec_ctx = self.generate_new_context()

        res.register(self.check_and_populate_args(self.arg_names, args, exec_ctx))
        if res.should_return():
            return res

        value = res.register(interpreter.visit(self.body_node, exec_ctx))
        if res.should_return() and res.func_return_value == None:
            return res

        ret_value = (
            (value if self.should_auto_return else None)
            or res.func_return_value
            or Number.null
        )
        return res.success(ret_value)

    def copy(self):
        """Create a copy of the function instance."""
        copy = Function(
            self.name, self.body_node, self.arg_names, self.should_auto_return
        )
        copy.set_context(self.context)
        copy.set_pos(self.pos_start, self.pos_end)
        return copy

    def __repr__(self):
        """Return the string representation of the function."""
        return f"<function {self.name}>"
