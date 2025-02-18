# RISC-V Online
![1](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-RP2040-ARM-Release.yml/badge.svg)
![2](https://github.com/IHatePineapples/riscv-online/actions/workflows/cmake-ubuntu-x86_64-Release.yml/badge.svg)
![3](https://github.com/IHatePineapples/riscv-online/actions/workflows/codeql.yml/badge.svg)
![4](https://github.com/IHatePineapples/riscv-online/actions/workflows/devskim.yml/badge.svg)
![5](https://github.com/IHatePineapples/riscv-online/actions/workflows/doxygen.yml/badge.svg)

<div style="text-align: center"> 
  <img width="30%" src="https://repository-images.githubusercontent.com/740292996/fc09680a-050d-4285-9090-8d38a7002559"/>
</div>



RISC-V Assembly Web Interpreter written in C++ for the Raspberry Pi Pico W.

## Building from source

### Requirements
* CMake
* Make
* GCC/G++ or equivalent (supports c++17)
* Raspberry Pi Pico W
* Boost (Header-only)

### Configure:

```bash
cmake -G Ninja .
```

### Build:

```bash
ninja
```

### Testing:
```bash
cd src/test
cmake .
make
make test
```
## How to use:

### Setup Wi-Fi

The board will try to connect by default to an open Wi-Fi named "RISC-V Online".
This can be best setup using a phone's network tethering ability, such as on
Android. Create this Wi-Fi network, name it "RISC-V Online" and make it open.

### Flashing 

There should now be a `*.uf2` file located in you binary tree : `src/*.uf2`

After locating it, plug in the MCU while holding the button on board, drag and
drop that file.

### Access web UI

You now need to find the IP address of the board, which is best done just
checking the list of connection in your AP. If using android tethering, this
should be in your settings next to the AP settings.

### Syntax

The builtin web interface support very basic assembly parsing:

```python
  lui x5, 2
  add x5, 1
  jal x0, -32 # Repeat add forever
```
### load/store

Loading storing from memory is supported.
To allocate stack memory, you would usually use `.data` directives in assembly
language. To achieve zeroed memory regions, you have to leave some lines empty,
to act as padding.

```python

  lui x1, 0x2
  sw x1, 0(x0)
```

### Direct web-API access

RISC-V Online has two API endpoints:

```bash
POST /submit
POST /return
```
To submit an emulation state/job, the payload would look like this:

```
id=26&ram=0|cs7|&ra=0&sp=0&gp=0&tp=cn4&t0=0&t1=0&t2=0&fp=0&s1=0&a0=0&a1=0&a2=0&a3=0&a4=0&a5=0&a6=0&a7=0&s2=0&s3=0&s4=0&s5=0&s6=0&s7=0&s8=0&s9=0&s10=0&s11=0&t3=0&t4=0&t5=0&t6=0&pc=w&prog=0&
```

We send the register states, a random ID and program encoded in the ram.

To encode the RAM, you need to encode each 32-bit memory regions as an integer,
then, represented base 36. Each of these memory regions will be suffixed by the `|`
delimiter. Concatenating all of these, and sending them within the `ram` parameter/field
via POST. Everything should be sent to `/submit`.

To retrieve back the result, just `POST` the original ID

```
id=26
```
to `/return`.

This will return a JSON object with all the original fields updated. On error,
from submit or during execution, this will return an empty object, `{ }`.



## TODO: 

- Fix GitHub workflows, Ubuntu doesn't automatically pick up latest compiler,
  need to download it from ARM website.
