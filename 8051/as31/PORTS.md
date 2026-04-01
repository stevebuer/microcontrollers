# 8051 Ports

Every port has:

* Latch bit
* The actual pin
* A weak pullup (except P0)

P0 is for external bus interface, so it needs to float not pull up.

Pin current limit: 10-15mA
Port Max: 20 mA?

## The Quasi‑Bidirectional Truth Table

Behavior for P1, P2, P3:

Latch	Pin drives	Meaning
0	Strong 0	Output LOW
1	Weak 1		Input (but looks like HIGH unless pulled down)

Such that

Writing 0 → the pin is a strong output low
Writing 1 → the pin becomes an input with a weak pull‑up

This is why the 8051 is called “quasi‑bidirectional.”

### What this means for LEDs

Case 1: LED from pin → GND
This is the classic 8051 LED wiring:

Code
Pin ----> LED ----> Resistor ----> GND
Behavior:

Write 0 → LED ON (pin sinks current)

Write 1 → LED OFF (weak pull‑up, barely any current)

This works beautifully on P1, P2, P3.

Case 2: LED from Vcc → pin
This is less common but still works:

Code
Vcc ----> LED ----> Resistor ----> Pin
Behavior:

Write 0 → LED ON (pin sinks current)

Write 1 → LED OFF (pin floats high)

Same logic, but reversed wiring.

🔴 Special Case: PORT 0 (P0)
P0 is different:

No internal pull‑ups

If latch = 1 → pin floats (high‑Z)

If latch = 0 → pin drives low

So for LEDs on P0:

You must provide an external pull‑up resistor

Or wire LED to Vcc and let the pin sink current

This is why beginners often think P0 is “broken.”

🟩 Putting it all together with your blinky
If you do:

```
MOV P2, #0FFh   ; all pins high (LEDs off)
CPL P2.0        ; toggle bit 0
```

Then with LED → GND:

CPL toggles between weak high (LED off) and strong low (LED on)

Perfect behavior

🟣 The mental model that makes everything click
Think of each pin like this:

Latch = 0 → “I am an output low.”
Latch = 1 → “I am an input, but I’ll gently pull myself high.”

That’s it.
Once you internalize that, the 8051’s ports stop being mysterious.

If you want, I can sketch out the internal transistor diagram of a port pin or show you the safest LED wiring for each port.
