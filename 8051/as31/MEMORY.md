# 8051 Memory Layout

## Internal Memory

### Lower 128 bytes: 0x00 → 0x7F (bytes 0 → 127)

Register Banks: x00 → 0x1F (bytes 0 → 31)

* Bank 0: 0x00–0x07
* Bank 1: 0x08–0x0F
* Bank 2: 0x10–0x17
* Bank 3: 0x18–0x1F

0x20 → 0x2F → Bit‑addressable RAM (bytes 32 → 47 = 16 bytes / 128 bits)

0x30 → x7F → General‑purpose RAM (bytes 48 → 127)

### 8052 Upper RAM

0x80–0xFF → General‑purpose RAM (bytes 127 → 255)

Only accessible via indirect addressing.

```
MOV R0, 0x80
MOV A, @R0
```

## Programming Notes
