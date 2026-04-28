# byte-grep

A command-line utility for searching binary files and directories using byte/bit masks.

## Features

- Search files and directories recursively
- Pattern matching with wildcards (X for any bit, _ for byte separator)
- Binary (0b) and hexadecimal (0x) pattern syntax
- Three search modes: first-of, last-of, all-of
- Count matches without listing positions

## Build

### Dependencies

- C++20 compiler
- CMake 3.16+
- Boost.ProgramOptions

### Build with CMake

```bash
git clone https://github.com/DanilaEfimov/Byte-grep.git
cd byte-grep
mkdir build && cd build
cmake ..
make
```

The executable `byte-grep` will be in the `build` directory.

## Usage

byte-grep -i <path> -p "<pattern>" [options]

### Options

| Option | Description |
|--------|-------------|
| `-i, --input` | File or directory to search (required) |
| `-p, --pattern` | Byte/bit mask pattern (required) |
| `-c, --count` | Count matches only, do not list positions |
| `-m, --mode` | Search mode: `first-of`, `last-of`, `all-of` (default: `first-of`) |
| `-h, --help` | Show help message |

### Pattern Syntax

#### Binary Format (`0b` prefix)

- `1` – bit must be 1
- `0` – bit must be 0
- `X` – bit can be either (wildcard)
- `_` – byte separator (optional)

**Examples:**
- `"0b11111111"` – exact byte `0xFF`
- `"0b1XXXXXXX"` – byte with highest bit set
- `"0b0000XXXX_XXXXXXXX"` – first nibble zero, second any

#### Hexadecimal Format (`0x` prefix)

- `0-9`, `A-F` – hex digit (exact match)
- `X` – nibble wildcard

**Examples:**
- `0xFF` – exact byte `0xFF`
- `0xX0` – first nibble any, second nibble zero
- `0x4X5X` – two-byte pattern

### Examples

Search for byte `0x4A` in a single file:
```cmd
byte-grep -i file.bin -p "0x4A"
```

Search recursively for pattern with wildcards:
```cmd
byte-grep -i ./directory -p "0b1100XXXX" -m all-of
```

Count occurrences of a pattern:
```cmd
byte-grep -i firmware.bin -p "0xDEADBEEF" -c
```

## Output Format
```
pattern '...' was found:
3 times
bootloader.bin, 0
firmware.bin, 1024
firmware.bin, 2048
```
