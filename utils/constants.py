# Tokens

import string


TT_INT = "INT"
TT_FLOAT = "FLOAT"
TT_STRING = "STRING"
TT_IDENTIFIER = "IDENTIFIER"
TT_KEYWORD = "KEYWORD"
TT_PLUS = "PLUS"
TT_MINUS = "MINUS"
TT_MUL = "MUL"
TT_DIV = "DIV"
TT_POW = "POW"
TT_EQ = "EQ"
TT_LPAREN = "LPAREN"
TT_RPAREN = "RPAREN"
TT_LSQUARE = "LSQUARE"
TT_RSQUARE = "RSQUARE"
TT_EE = "EE"
TT_NE = "NE"
TT_LT = "LT"
TT_GT = "GT"
TT_LTE = "LTE"
TT_GTE = "GTE"
TT_COMMA = "COMMA"
TT_ARROW = "ARROW"
TT_NEWLINE = "NEWLINE"
TT_EOF = "EOF"
TT_EMBED = "EMBED"
TT_WITH = "WITH"
TT_AI = "AI"
TT_PIPE = "PIPE"
TT_VEC = "VEC"
TT_DOT = "DOT"
TT_COSINE = "COSINE"

KEYWORDS = [
    "VAR",
    "AND",
    "OR",
    "NOT",
    "IF",
    "ELIF",
    "ELSE",
    "FOR",
    "TO",
    "STEP",
    "WHILE",
    "FUN",
    "THEN",
    "END",
    "RETURN",
    "CONTINUE",
    "BREAK",
    "EMBED",
    "WITH",
    "AI",
    "PIPE",
    "VEC",
    "DOT",
    "COSINE"
]


#######################################
# CONSTANTS
#######################################

DIGITS = "0123456789"
LETTERS = string.ascii_letters
LETTERS_DIGITS = LETTERS + DIGITS

WHITESPACE = {" ", "\t"}
NUM_CHARS = set(DIGITS + ".")
