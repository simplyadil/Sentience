#######################################
# RUNTIME RESULT
#######################################


class RTResult:
    """
    Encapsulates the result of runtime execution, tracking the value, errors, and control flow signals.

    Attributes:
        value (any): The result value if execution was successful.
        error (Error or None): Any error encountered during execution.
        func_return_value (any): A function's return value, if a return statement was executed.
        loop_should_continue (bool): Flag indicating if the current loop should continue.
        loop_should_break (bool): Flag indicating if the current loop should break."
    """

    def __init__(self):
        self.reset()

    def reset(self):
        """Reset the internal state of the RTResult to its default values."""
        self.value = None
        self.error = None
        self.func_return_value = None
        self.loop_should_continue = False
        self.loop_should_break = False

    def register(self, res):
        """Register the result from a sub-expression.

        This method propagates error and control signals (like function return, continue, and break)
        from the given result, and returns its value.
        """
        self.error = res.error
        self.func_return_value = res.func_return_value
        self.loop_should_continue = res.loop_should_continue
        self.loop_should_break = res.loop_should_break
        return res.value

    def success(self, value):
        """Mark the result as a successful execution with a given value."""
        self.reset()
        self.value = value
        return self

    def success_return(self, value):
        """Mark the result as a function return with a given value."""
        self.reset()
        self.func_return_value = value
        return self

    def success_continue(self):
        """Mark the result as signaling a loop continue."""
        self.reset()
        self.loop_should_continue = True
        return self

    def success_break(self):
        """Mark the result as signaling a loop break."""
        self.reset()
        self.loop_should_break = True
        return self

    def failure(self, error):
        """Mark the result as a failure with an error."""
        self.reset()
        self.error = error
        return self

    def should_return(self):
        """Check whether the current result should cause an immediate return from the current execution context.

        This is True if any of the following is set:
            - An error occurred.
            - A function return value was set.
            - A loop should continue.
            - A loop should break.
        """
        return (
            self.error
            or self.func_return_value
            or self.loop_should_continue
            or self.loop_should_break
        )
