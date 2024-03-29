# RISC-V Online
![1](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-RP2040-ARM-Release.yml/badge.svg)
![2](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-ubuntu-x86_64-Release.yml/badge.svg)
![3](https://github.com/IHatePineapples/riscv-online/actions/workflows/codeql.yml/badge.svg)
![3](https://github.com/IHatePineapples/riscv-online/actions/workflows/devskim.yml/badge.svg)


RISC-V Assembly Web Interpreter written in C++ for the Raspberry Pi Pico W.

## Building from source

### Requirements
* CMake
* Make
* GCC/G++ or equivalent (supports c++17)
* Raspberry Pi Pico W

### Configure:

```bash
cmake .
```

### Build:

```bash
make -j$(nproc)
```

### Testing:
```bash
cd src/test
cmake .
make
make test
```



