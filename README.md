# Burst
![build](https://github.com/angelocarly/burst/actions/workflows/cmake.yml/badge.svg)  
Simple C++ rendering library using Vulkan.

## Philosophy
Burst is a new iteration of [Vakarta](https://github.com/angelocarly/vakarta), my previous attempt at creating a comprehensible Vulkan graphics engine.
Since Vakarta grew a bit out of proportion and was not enjoyable to use, I am trying to apply the concepts I've learned in order to producer an easier to use API.

Burst is intented to be
- easy to use, at the cost of creating many abstractions around the Vulkan API
- have a clear separation between library and implementation
- Stay relatively simple and documented

## Building
```
mkdir build && cd build
cmake ..
make
example/Example
```

## Roadmap
- [ ] Dependency setup and documentation
- [x] ~~Integration with a first test implementation~~ See [Borst](https://github.com/angelocarly/borst).
- [ ] Vulkan + Window instantiation
- [ ] Test framework

## Dependencies
Following dependencies have to be installed on your system:
- [spdlog](https://github.com/gabime/spdlog) - A fast header only logging library
