# ARM Assembly Notes

## Labels

* 0-9 are local scratch labels that exist between instruction that reference them
* branch suffix *b* or *f* mean to branch forwarward or back to local reference. e.g.

```
1:  subs r0, r0, #1
    bne 1b        @ branch back to label 1

    movs r1, #10
2:  subs r1, r1, #1
    bne 2b        @ branch back to label 2
```

## ARM Architecture Procedure Call Standard

## **Registers**
- **r0–r3** — args + return; caller‑saved  
- **r4–r11** — callee‑saved  
- **r12** — scratch  
- **sp (r13)** — stack pointer  
- **lr (r14)** — return address  
- **pc (r15)** — program counter  

---

## **Arguments**
- 1st–4th args → **r0–r3**  
- Extra args → **stack**  

---

## **Return Values**
- Main return → **r0**  
- Secondary → **r1**  

---

## **Caller‑Saved**
- **r0–r3**, **r12**, **lr** (if needed)  

---

## **Callee‑Saved**
- **r4–r11** must be restored if modified  

---

## **Stack Rules**
- Full descending  
- 4‑byte aligned always  
- 8‑byte aligned at call sites  

---

## **Minimal ABI‑Correct Function**
```asm
delay_custom:
    cmp r0, #0
    beq 2f
1:  subs r0, r0, #1
    bne 1b
2:  bx lr
```
