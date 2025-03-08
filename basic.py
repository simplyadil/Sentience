import sys

from functions.builtinfun import run

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python3 basic.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]
    try:
        with open(filename, "r") as f:
            code = f.read()
    except FileNotFoundError:
        print(f"Error: File '{filename}' not found.")
        sys.exit(1)

    result, error = run(filename, code)
    if error:
        print(error.as_string())
    else:
        print(result)
