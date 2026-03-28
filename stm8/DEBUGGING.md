# STM8 Debugging

You can debug an STM8 using **OpenOCD + GDB**, but only with the **community‑maintained STM8 support**, not the mainstream OpenOCD/GDB releases. The good news is that this support is mature enough for real debugging: breakpoints, stepping, register inspection, and flashing all work when set up correctly.

---

# Debugging STM8 with OpenOCD and GDB

## Supported Debugging Workflow
A working toolchain for STM8 debugging typically includes:

- **OpenOCD** with STM8 SWIM support  
- **stm8-gdb** (a patched GDB target for STM8)  
- **SDCC** or another compiler that emits DWARF2 debug info  

This setup has been demonstrated on STM8S and STM8L devices using inexpensive ST‑Link V2 clones.

### Confirmed working features
- Flashing ELF binaries to STM8 targets   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)  
- Running, halting, and continuing execution   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)  
- Setting breakpoints and stepping through code   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)  
- Reading registers and inspecting state   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)  

These capabilities make STM8 debugging quite usable once the environment is configured.

---

# Requirements and Setup Notes

## OpenOCD with STM8 SWIM Support
OpenOCD must be built from a version that includes STM8 SWIM patches. The STM8 target scripts (e.g., `stm8s.cfg`, `stm8l.cfg`) are available in the patched OpenOCD source tree.   [stm8-binutils-gdb.sourceforge.io](https://stm8-binutils-gdb.sourceforge.io/)

A custom `st-link.cfg` is often required to ensure SWIM transport is recognized by OpenOCD.   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)

## stm8-gdb
A patched GDB with STM8 target support is required. This comes from the **stm8-binutils-gdb** project, which adds:

- A new STM8 target to GDB  
- Remote debugging enhancements for smoother IDE integration  
- Compatibility with OpenOCD’s STM8 server on port 3333   [stm8-binutils-gdb.sourceforge.io](https://stm8-binutils-gdb.sourceforge.io/)  

## Example of a Working Setup
A complete working example using:

- SDCC  
- stm8-gdb  
- OpenOCD  
- ST‑Link V2 clone  
- STM8L101F3U6 target  

is documented and verified on macOS and Windows.   [Github](https://github.com/DeqingSun/Debug-STM8-with-SDCC-and-VSCode)

This example shows that once OpenOCD is running, GDB can attach and break at `main()` reliably.

---

# Practical Considerations

## Stability
The STM8 debugging support is functional but not as polished as ARM Cortex‑M debugging. Some limitations exist:

- GDB TUI features beyond basic stepping may not work   [GitHub Gist](https://gist.github.com/TG9541/79aa3bb1a56a8220eac8f57ab21302e1)  
- OpenOCD builds may require manual patching or configuration tweaks  
- Path handling quirks exist on some platforms (e.g., STM8‑gdb escaping slashes)   [Github](https://github.com/DeqingSun/Debug-STM8-with-SDCC-and-VSCode/blob/main/README.md)  

## Device Support
OpenOCD includes target configs for several STM8 families:

- `stm8s.cfg`  
- `stm8l.cfg`  
- `stm8l152.cfg`  
- `stm8s003.cfg`  
- `stm8s105.cfg`   [stm8-binutils-gdb.sourceforge.io](https://stm8-binutils-gdb.sourceforge.io/)  

This covers most common STM8S and STM8L devices.

---

# Summary
Debugging STM8 with OpenOCD and GDB **is absolutely possible**, but requires:

- The STM8‑patched OpenOCD  
- The STM8‑patched GDB (stm8-gdb)  
- Correct ST‑Link configuration  
- An ELF file with DWARF2 debug info (SDCC supports this)

Once set up, you can step through code, inspect registers, and debug much like on ARM microcontrollers.
