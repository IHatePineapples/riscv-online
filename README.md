# RISC-V Online
![RP2040 crossbuild workflow](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-RP2040-ARM-Release.yml/badge.svg)
![Host-only workflow](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-ubuntu-x86_64-Release.yml/badge.svg)
![Host-only workflow](https://github.com/IHatePineapples/riscv-online/actions/workflows/codeql.yml/badge.svg)

RISC-V Assembly Web Interpreter written in C++ using Wt and deployable on a Raspberry Pi Pico W.

## Building from source

### Requirements
* CMake
* Make
* GCC & G++ or equivalent (supports c++17)
* Raspberry Pi Pico W (optional)

### Configure:

```bash
cmake .
```
### Build:

```bash
make -j$(nproc)
```

### Running:
```bash
./app/app
```

### Testing:
```bash
make test
```



