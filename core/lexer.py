from utils.errors import ExpectedCharError, IllegalCharError
from utils.constants import (
    DIGITS,
    KEYWORDS,
    LETTERS,
    LETTERS_DIGITS,
    NUM_CHARS,
    TT_ARROW,
    TT_COMMA,
    TT_DIV,
    TT_EE,
    TT_EOF,
    TT_EQ,
    TT_FLOAT,
    TT_GT,
    TT_GTE,
    TT_IDENTIFIER,
    TT_INT,
    TT_KEYWORD,
    TT_LPAREN,
    TT_LSQUARE,
    TT_LT,
    TT_LTE,
    TT_MINUS,
    TT_MUL,
    TT_NE,
    TT_NEWLINE,
    TT_PLUS,
    TT_POW,
    TT_RPAREN,
    TT_RSQUARE,
    TT_STRING,
    WHITESPACE,
)


class Token:
    """Represents a token in the source code.
    It contains its type, value, and positional information."""

    def __init__(self, type_, value=None, pos_start=None, pos_end=None):
        """Initialize a new Token instance."""
        self.type = type_
        self.value = value

        if pos_start:
            self.pos_start = pos_start.copy()
            self.pos_end = pos_start.copy()
            self.pos_end.advance()

        if pos_end:
            self.pos_end = pos_end.copy()

    def matches(self, type_, value):
        """Check whether the token matches a specific type and value."""
        return self.type == type_ and self.value == value

    def __repr__(self):
        """Return the string representation of the token."""
        if self.value:
            return f"{self.type}:{self.value}"
        return f"{self.type}"


class Position:
    """Represents a specific location in the source code."""

    def __init__(self, idx, ln, col, fn, ftxt):
        self.idx = idx
        self.ln = ln
        self.col = col
        self.fn = fn
        self.ftxt = ftxt

    def advance(self, current_char=None):
        """Advance the position by one character."""
        self.idx += 1
        self.col += 1

        if current_char == "\n":
            self.ln += 1
            self.col = 0

        return self

    def copy(self):
        """Create a copy of the current position."""
        return Position(self.idx, self.ln, self.col, self.fn, self.ftxt)


class Lexer:
    """A lexical analyzer (lexer) for converting source code into a list of tokens."""

    def __init__(self, fn, text):
        """Initialize the lexer with a filename and source text"""
        self.fn = fn
        self.text = text
        self.pos = Position(-1, 0, -1, fn, text)
        self.current_char = None
        self.advance()

    def advance(self):
        """Advance the lexer's position by one character, updating the current character."""
        self.pos.advance(self.current_char)
        self.current_char = (
            self.text[self.pos.idx] if self.pos.idx < len(self.text) else None
        )

    def make_tokens(self):
        """Process the entire source text and convert it into a list of tokens."""
        tokens = []

        while self.current_char is not None:
            if self.current_char in WHITESPACE:
                self.advance()
            elif self.current_char == "#":
                self.skip_comment()
            elif self.current_char in ";\n":
                tokens.append(Token(TT_NEWLINE, pos_start=self.pos))
                self.advance()
            elif self.current_char in DIGITS:
                tokens.append(self.make_number())
            elif self.current_char in LETTERS:
                tokens.append(self.make_identifier())
            elif self.current_char == '"':
                tokens.append(self.make_string())
            elif self.current_char == "+":
                tokens.append(Token(TT_PLUS, pos_start=self.pos))
                self.advance()
            elif self.current_char == "-":
                tokens.append(self.make_minus_or_arrow())
            elif self.current_char == "*":
                tokens.append(Token(TT_MUL, pos_start=self.pos))
                self.advance()
            elif self.current_char == "/":
                tokens.append(Token(TT_DIV, pos_start=self.pos))
                self.advance()
            elif self.current_char == "^":
                tokens.append(Token(TT_POW, pos_start=self.pos))
                self.advance()
            elif self.current_char == "(":
                tokens.append(Token(TT_LPAREN, pos_start=self.pos))
                self.advance()
            elif self.current_char == ")":
                tokens.append(Token(TT_RPAREN, pos_start=self.pos))
                self.advance()
            elif self.current_char == "[":
                tokens.append(Token(TT_LSQUARE, pos_start=self.pos))
                self.advance()
            elif self.current_char == "]":
                tokens.append(Token(TT_RSQUARE, pos_start=self.pos))
                self.advance()
            elif self.current_char == "!":
                token, error = self.make_not_equals()
                if error:
                    return [], error
                tokens.append(token)
            elif self.current_char == "=":
                tokens.append(self.make_equals())
            elif self.current_char == "<":
                tokens.append(self.make_less_than())
            elif self.current_char == ">":
                tokens.append(self.make_greater_than())
            elif self.current_char == ",":
                tokens.append(Token(TT_COMMA, pos_start=self.pos))
                self.advance()
            else:
                pos_start = self.pos.copy()
                char = self.current_char
                self.advance()
                return [], IllegalCharError(pos_start, self.pos, "'" + char + "'")

        tokens.append(Token(TT_EOF, pos_start=self.pos))
        return tokens, None

    def make_number(self):
        """Parse a number (integer or float) from the source text."""
        num_str = ""
        dot_count = 0
        pos_start = self.pos.copy()

        while self.current_char is not None and self.current_char in NUM_CHARS:
            if self.current_char == ".":
                if dot_count == 1:
                    break
                dot_count += 1
            num_str += self.current_char
            self.advance()

        if dot_count == 0:
            return Token(TT_INT, int(num_str), pos_start, self.pos)
        else:
            return Token(TT_FLOAT, float(num_str), pos_start, self.pos)

    def make_string(self):
        """Parse a string litteral from the source text."""
        string = ""
        pos_start = self.pos.copy()
        escape_character = False
        self.advance()

        escape_characters = {"n": "\n", "t": "\t", '"': '"'}

        while self.current_char is not None and (
            self.current_char != '"' or escape_character
        ):
            if escape_character:
                string += escape_characters.get(self.current_char, self.current_char)
            else:
                if self.current_char == "\\":
                    escape_character = True
                    self.advance()
                    continue
                else:
                    string += self.current_char
            self.advance()
            escape_character = False

        self.advance()
        return Token(TT_STRING, string, pos_start, self.pos)

    def make_identifier(self):
        """Parse an identifier or keyword from the source text."""
        id_str = ""
        pos_start = self.pos.copy()

        while (
            self.current_char is not None and self.current_char in LETTERS_DIGITS + "_"
        ):
            id_str += self.current_char
            self.advance()

        tok_type = TT_KEYWORD if id_str in KEYWORDS else TT_IDENTIFIER
        return Token(tok_type, id_str, pos_start, self.pos)

    def make_minus_or_arrow(self):
        """Distinguish between a minus operator and an arrow token ('->')."""
        tok_type = TT_MINUS
        pos_start = self.pos.copy()
        self.advance()

        if self.current_char == ">":
            self.advance()
            tok_type = TT_ARROW

        return Token(tok_type, pos_start=pos_start, pos_end=self.pos)

    def make_not_equals(self):
        """Parse the '!=' operator."""
        pos_start = self.pos.copy()
        self.advance()

        if self.current_char == "=":
            self.advance()
            return Token(TT_NE, pos_start=pos_start, pos_end=self.pos), None

        self.advance()
        return None, ExpectedCharError(pos_start, self.pos, "'=' (after '!')")

    def make_equals(self):
        """Parse the '=' and/or '==' operator."""
        tok_type = TT_EQ
        pos_start = self.pos.copy()
        self.advance()

        if self.current_char == "=":
            self.advance()
            tok_type = TT_EE

        return Token(tok_type, pos_start=pos_start, pos_end=self.pos)

    def make_less_than(self):
        """Parse the '<' and/or '<=' operator."""
        tok_type = TT_LT
        pos_start = self.pos.copy()
        self.advance()

        if self.current_char == "=":
            self.advance()
            tok_type = TT_LTE

        return Token(tok_type, pos_start=pos_start, pos_end=self.pos)

    def make_greater_than(self):
        """Parse the '>' and/or '>=' operator."""
        tok_type = TT_GT
        pos_start = self.pos.copy()
        self.advance()

        if self.current_char == "=":
            self.advance()
            tok_type = TT_GTE

        return Token(tok_type, pos_start=pos_start, pos_end=self.pos)

    def skip_comment(self):
        """Skip over a comment in the source text."""
        self.advance()

        while self.current_char != "\n":
            self.advance()

        self.advance()
