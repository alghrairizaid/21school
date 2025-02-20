# SimpleBashUtils

This project contains implementations of Bash text utilities.

## Build Instructions

1. Navigate to the source directory:
```bash
cd src
```

2. Build the project using make:
```bash
make
```

## Available Utilities

### cat

The cat utility displays file contents.

Usage:
```bash
./cat [OPTION] [FILE]...
```

Options:
- -b (number non-empty lines)
- -e (show ends)
- -n (number all lines)
- -s (squeeze blank lines)
- -t (show tabs)

### grep

The grep utility searches for patterns in text.

Usage:
```bash
./grep [OPTION]... PATTERN [FILE]...
```

Options:
- -e PATTERN (use PATTERN for matching)
- -i (ignore case)
- -v (select non-matching lines)
- -c (output count of matching lines)
- -l (output matching files)
- -n (output line number with matching lines)
- -h (suppress filename prefix)
- -s (suppress error messages)
- -f file (obtain patterns from file)
- -o (show only the matched parts)

## Examples

```bash
# Display file with line numbers
./cat -n filename.txt

# Search for a pattern
./grep "pattern" filename.txt
```
