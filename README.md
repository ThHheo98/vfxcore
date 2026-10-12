# vfxcore

A small C++20 project for exploring core programming concepts used in VFX.

## Build and test

Configure, build, and run the debug tests (with AddressSanitizer and
UndefinedBehaviorSanitizer enabled):

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Build and test the optimized release configuration with:

```sh
cmake --preset release
cmake --build --preset release
ctest --preset release
```

The `vfxcore_demo` executable demonstrates `Vec3::dot`:

```sh
./build/debug/vfxcore_demo
```

It prints `dot = 32`.
