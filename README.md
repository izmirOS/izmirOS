# Building

## cmake

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -B build
cd build
make
```

# Running

```bash
qemu-system-i386 -fda ./build/image.bin
```

# TODO

- [ ] tss for 64 bit booting
