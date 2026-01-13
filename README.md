# bitfield

A comprehensive C header library for efficient bitfield manipulation with type-safe macros and optimized operations.

## Features

- **Type Definitions**: `bit8_t`, `bit16_t`, `bit32_t`, `bit64_t`
- **Single-bit operations**: Set, clear, toggle, test individual bits
- **Multi-bit operations**: Extract, set, and clear bit ranges
- **Bit counting**: Count set/unset bits using compiler builtins when available
- **Binary string parsing**: Convert binary strings to integers
- **Zero dependencies**: Just include `<stdint.h>`

## Installation

Copy `include/bitfield.h` to your project and include it:

```c
#include "bitfield.h"
```

## Quick Reference

### Basic Bit Operations

| Macro | Description | Example |
|-------|-------------|---------|
| `BIT(i)` | Create bitmask with bit i set | `BIT(3)` → `0b1000` |
| `SETBIT(bf, i)` | Set bit i to 1 | `SETBIT(flags, 5)` |
| `CLEARBIT(bf, i)` | Clear bit i to 0 | `CLEARBIT(flags, 2)` |
| `NULLBIT(bf, i)` | Alias for CLEARBIT | `NULLBIT(flags, 2)` |
| `TOGGLEBIT(bf, i)` | Flip bit i | `TOGGLEBIT(flags, 4)` |
| `ISSET(bf, i)` | Check if bit i is set | `if (ISSET(flags, 3))` |
| `ISNULL(bf, i)` | Check if bit i is clear | `if (ISNULL(flags, 1))` |
| `GETBIT(bf, i)` | Get bit value (0 or 1) | `int val = GETBIT(flags, 7)` |

### Multi-bit Operations

| Macro | Description | Example |
|-------|-------------|---------|
| `BITMASK(n)` | Create mask with n bits set | `BITMASK(4)` → `0b1111` |
| `GETBITS(bf, start, len)` | Extract bit range | `GETBITS(val, 2, 4)` |
| `SETBITS(bf, start, len, val)` | Set bit range to value | `SETBITS(flags, 0, 3, 0b101)` |
| `CLEARBITS(bf, start, len)` | Clear bit range | `CLEARBITS(flags, 4, 2)` |

### Bit Counting

| Macro | Description | Example |
|-------|-------------|---------|
| `ONES(bf)` | Count set bits | `ONES(0b10110)` → `3` |
| `ZEROS(bf)` | Count clear bits | `ZEROS(0xFF)` → `0` (for 8-bit) |

### String Parsing

| Macro | Description | Example |
|-------|-------------|---------|
| `BITS(bf, str)` | Parse binary string | `BITS(val, "10110")` |

## Usage Examples

### Basic Operations

```c
#include "bitfield.h"

int main() {
    bit32_t flags = 0;
    
    // Set some bits
    SETBIT(flags, 0);   // flags = 0b00001
    SETBIT(flags, 3);   // flags = 0b01001
    SETBIT(flags, 5);   // flags = 0b101001
    
    // Check bits
    if (ISSET(flags, 3)) {
        printf("Bit 3 is set\n");
    }
    
    // Toggle a bit
    TOGGLEBIT(flags, 0);  // flags = 0b101000
    
    // Clear a bit
    CLEARBIT(flags, 5);   // flags = 0b001000
    
    // Count set bits
    int count = ONES(flags);  // count = 1
    
    return 0;
}
```

### Multi-bit Fields

```c
bit32_t status = 0;

// Set a 4-bit field at position 8 to value 13 (0b1101)
SETBITS(status, 8, 4, 13);

// Extract the 4-bit field
int value = GETBITS(status, 8, 4);  // value = 13

// Clear a range of bits
CLEARBITS(status, 8, 4);
```

### Binary String Parsing

```c
bit16_t pattern;
BITS(pattern, "1010110011");
// pattern = 0b1010110011 = 691
```

### Flag Management

```c
// Define flag meanings
#define FLAG_READY    0
#define FLAG_RUNNING  1
#define FLAG_ERROR    2
#define FLAG_COMPLETE 3

bit8_t state = 0;

// Set state flags
SETBIT(state, FLAG_READY);
SETBIT(state, FLAG_RUNNING);

// Check multiple flags
if (ISSET(state, FLAG_READY) && !ISSET(state, FLAG_ERROR)) {
    // Safe to proceed
}

// Count active flags
int active = ONES(state);
```

## Performance

- **ONES/ZEROS**: Uses `__builtin_popcount` on GCC/Clang for optimal performance (~1 CPU instruction)
- **Single-bit ops**: Compile to individual bit instructions (BTS, BTR, BTC, BT)
- **Multi-bit ops**: Efficient mask-and-shift operations

## Compiler Compatibility

- **GCC/Clang**: Full optimization with compiler builtins
- **MSVC**: Standard C operations (no builtins used)
- **C99+**: Required for `<stdint.h>` types

## License

See [LICENSE](LICENSE) file for details.
