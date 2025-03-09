from utils.errors import RTError
from execution.runtime import RTResult
from core.values import Context, Number, SymbolTable, Value


class BaseFunction(Value):
    """Base class for function values.

    This class encapsulates shared functionality for all functions in the language,
    including user-defined and built-in functions. It provides methods for context
    generation, argument checking, and populating a new execution context with argument values.
    """

    def __init__(self, name):
        super().__init__()
        self.name = name or "<anonymous>"

    def generate_new_context(self):
        """Generate a new execution context for the function call."""
        new_context = Context(self.name, self.context, self.pos_start)
        new_context.symbol_table = SymbolTable(new_context.parent.symbol_table)
        return new_context

    def check_args(self, arg_names, args):
        """Check if the number of arguments provided matches the expected number."""
        res = RTResult()

        if len(args) > len(arg_names):
            return res.failure(
                RTError(
                    self.pos_start,
                    self.pos_end,
                    f"{len(args) - len(arg_names)} too many args passed into {self}",
                    self.context,
                )
            )

        if len(args) < len(arg_names):
            return res.failure(
                RTError(
                    self.pos_start,
                    self.pos_end,
                    f"{len(arg_names) - len(args)} too few args passed into {self}",
                    self.context,
                )
            )

        return res.success(None)

    def populate_args(self, arg_names, args, exec_ctx):
        """Populate the function's execution context with the provided arguments."""
        for i in range(len(args)):
            arg_name = arg_names[i]
            arg_value = args[i]
            arg_value.set_context(exec_ctx)
            exec_ctx.symbol_table.set(arg_name, arg_value)

    def check_and_populate_args(self, arg_names, args, exec_ctx):
        """Check argument count and, if valid, populate the function's execution context with arguments."""
        res = RTResult()
        res.register(self.check_args(arg_names, args))
        if res.should_return():
            return res
        self.populate_args(arg_names, args, exec_ctx)
        return res.success(None)
