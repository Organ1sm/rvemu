# Rvemu

RVEmu is a RISC-V emulator written in C++.

## Compile

```
mkdir build
cd build
cmake ..
make
```

## Usage

```
./rvemu test_file.bin
```

## To-Do List

- [x] RV32I
- [x] RV64I
- [ ] Zifencei extension
- [ ] Zicsr extension
- [ ] M extension
- [ ] A extension
- [ ] F extension
- [ ] D extension
- [x] Machine mode CSRs
- [x] User mode
- [x] Supervisor mode and CSRs 
- [ ] CLINT (core local interrupt controller)
- [ ] PLIC (platform level interrupt controller)
- [ ] MMU and virtual-memory system
- [X] Debugger
- [ ] JIT
