# Burst
Simple C++ rendering library using Vulkan.

## Philosophy
Burst is a new iteration of [Vakarta](https://github.com/angelocarly/vakarta), a previous rendering engine I tried to make using Vulkan.
Vakarta has grown out of proportion, and with Burst I try to apply the concepts I've learned during previous development.
From this come the following design principles I will try to uphold:

- **Simple to use**
  - Vakarta was very unclear and complicated to use. Development was generally 80% editing the engine and 20% creating the implementation. I'd like to limit the scope of the library and have 20% of the effort in the engine instead.

- **Clear documentation**
  - Spaghetti code is an issue. It's easy to tinker with something until it works without understanding what it does. I'd like to have clear and consistent documentation of the codebase.

- **An actual library**
  - Vakarta contained all it's implementations in the same repository. This made it hard to alter the engine without breaking the implementation. I'd like to have a clear separation between the engine and it's implementations.

These goals are ambitious, but this will be a best effort.

## Roadmap
- [ ] Dependency setup and documentation
- [ ] Integration with a first test implementation
- [ ] Vulkan + Window instantiation
- [ ] Test framework

## Dependencies
- [spdlog](https://github.com/gabime/spdlog) - A fast header only logging library