# 🏁 Startup File Overview (startup_stm32f0xx.s)

The startup file is the very first code that runs after reset. It performs the low‑level steps required before any C code can execute.

### **1. Set the initial stack pointer**
The first entry in the vector table is the top of RAM.  
The CPU loads this into the stack pointer automatically on reset.

### **2. Provide the vector table**
A table of function pointers for:
- Reset handler  
- Core exceptions (NMI, HardFault, etc.)  
- Peripheral interrupts  

The CPU reads this table from address `0x08000000` at boot.

### **3. Implement the Reset_Handler**
This is the real entry point of the program. It:
- copies the `.data` section from flash to RAM  
- zeros the `.bss` section  
- optionally sets the vector table offset  
- calls `SystemInit()`  
- calls `main()`  

After this, normal C execution begins.

### **4. Provide weak default handlers**
All interrupt handlers are defined as weak symbols.  
If the user defines their own handler in C, it overrides the weak one.

### **5. End in an infinite loop for unhandled interrupts**
Default handlers typically loop forever so unexpected faults are easy to catch during debugging.

---

# 📌 Vector Table in C vs Assembly

On Cortex‑M microcontrollers, the vector table can be defined in **either**:

- the **startup assembly file**, or  
- a **C source file**, usually called `vector_table.c` or included in `startup.c`

Both approaches are valid. The CPU doesn’t care — it just reads a table of addresses at the start of flash.

---

# 🧭 Why define the vector table in C?

Defining the vector table in C has a few advantages:

- **Easier to read** — function pointers instead of assembly directives  
- **Easier to override handlers** — just define a function with the same name  
- **Easier to maintain** — no need to edit assembly when adding/removing IRQs  
- **Easier to debug** — handlers show up as symbols in your debugger  

Many hobbyists and even some professional templates prefer this style.

---

# 🧱 What changes when the vector table is in C?

If you move the vector table into C:

1. The startup assembly file becomes *very* small  
   It only needs to set the stack pointer and jump to `Reset_Handler`.

2. The C file provides:
   - the vector table array  
   - weak default handlers  
   - the Reset_Handler implementation  

3. The linker script must place the vector table at the start of flash  
   Usually via:

   ```
   .isr_vector :
   {
       KEEP(*(.isr_vector))
   } > FLASH
   ```

---

# 📄 README‑ready summary (copy/paste)

Here’s a clean section you can drop straight into your README:

---

## Vector Table Location (Assembly or C)

The Cortex‑M0 requires a vector table at the beginning of flash. This table contains the initial stack pointer and the addresses of all exception and interrupt handlers.

There are two valid ways to provide this table:

### **1. Vector table in the startup assembly file**
- Traditional STM32 approach  
- The table is written using `.word` directives  
- Reset_Handler and default handlers are also defined in assembly  

### **2. Vector table in a C source file**
- Modern, easier‑to‑read approach  
- The table is an array of function pointers placed in the `.isr_vector` section  
- Handlers are weak symbols, so user code can override them  
- The startup assembly file only sets the stack pointer and jumps to `Reset_Handler`

Both methods produce the same result. The choice is purely about readability and maintainability.
