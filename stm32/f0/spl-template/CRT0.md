# 🌱 What “C runtime startup” actually means

Before `main()` can run, the CPU needs a bunch of things set up:

### **1. A valid stack pointer**  
You just mastered this part.

### **2. Initialized `.data` section**  
Copy global variables with initial values from Flash → RAM.

### **3. Zeroed `.bss` section**  
Set all uninitialized globals to 0.

### **4. Optional: call static constructors**  
Only needed in C++.

### **5. Finally, call `main()`**

This whole sequence is what people call **crt0** (“C runtime zero”).

---

# 🧱 The minimal ARM Cortex‑M startup flow

Here’s the conceptual flow, not code yet:

```
Reset_Handler:
    set SP from vector table
    copy .data from Flash to RAM
    zero .bss
    call main()
    loop forever
```

You already understand the first step. Now we add the memory initialization.

---

# 📦 Step 1: The linker script defines the memory layout

Your linker script will define symbols like:

```
_estack
_sdata
_edata
_etext
_sbss
_ebss
```

These are addresses the startup code uses to know:

- where `.data` lives in Flash  
- where `.data` should go in RAM  
- where `.bss` starts and ends  

You don’t need to memorize them — the linker script provides them.

---

# 🔧 Step 2: Copy `.data` from Flash to RAM

In pseudocode:

```c
uint32_t* src = &_etext;   // end of text = start of data in Flash
uint32_t* dst = &_sdata;   // start of data in RAM

while (dst < &_edata)
    *dst++ = *src++;
```

This ensures global variables with initial values are correct.

---

# 🧽 Step 3: Zero `.bss`

```c
uint32_t* b = &_sbss;
while (b < &_ebss)
    *b++ = 0;
```

This ensures uninitialized globals start at zero, as the C standard requires.

---

# 🚀 Step 4: Call `main()`

```c
main();
```

After `main()` returns (it shouldn’t), you typically loop forever.

---

# 🧠 Why this matters

C assumes:

- global variables exist in RAM  
- they have the right initial values  
- the stack is valid  
- memory is zeroed where required  

The CPU does **none** of this for you.  
Your startup code does.
