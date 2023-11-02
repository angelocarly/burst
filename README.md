# Burst
![build](https://github.com/angelocarly/burst/actions/workflows/cmake.yml/badge.svg)  
Minimal C++ rendering library using Vulkan.

## Philosophy
Burst is a new iteration of [Vakarta](https://github.com/angelocarly/vakarta), my previous attempt at creating a comprehensible Vulkan graphics engine.
Since Vakarta grew a bit out of proportion and was not enjoyable to use, I am trying to apply the concepts I've learned in order to producer an easier to use API.

Burst is intented to:
- Be easy to use, at the cost of creating many abstractions around the Vulkan API.
- Have a clear separation between library and implementation.
- Stay relatively simple and documented.

## Building
Firstly, install the required system dependencies:
```
sudo apt-get install -y libspdlog-dev libglfw3-dev
```

Secondly, install the [Vulkan SDK](https://vulkan.lunarg.com) and set the path environment variables:
```
export VULKAN_SDK=/path/to/vulkan/sdk
export VK_ICD_FILENAMES=/path/to/vulkan/sdk/etc/vulkan/icd.d
```

Then build Burst:
```
git clone https://github.com/angelocarly/burst.git
git submodule update --init --recursive
cd burst && mkdir build && cd build
cmake ..
make
example/Example
```

## Roadmap
- [x] ~~Basic dependency setup and documentation~~
- [x] ~~Integration with a first test implementation~~ See [Borst](https://github.com/angelocarly/borst).
- [x] ~~Window instantiation~~
- [x] ~~Vulkan device instantiation~~
- [x] ~~Vulkan memory allocator~~
- [ ] Connect swapchain to the window
- [ ] ImGui
- [ ] Test framework (delayed, probably want to have Vulkan working first )

## Dependencies
- [spdlog](https://github.com/gabime/spdlog) - Fast header only logging library.
- [glfw](https://github.com/glfw/glfw) - Cross platform window and input library.
- [Vulkan SDK](https://vulkan.lunarg.com) - Low level graphics API.
- [VulkanMemoryAllocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) - Easy to integrate Vulkan memory allocator.
- [VulkanMemoryAllocator-Hpp](https://github.com/YaaZ/VulkanMemoryAllocator-Hpp) - C++ bindings for VulkanMemoryAllocator.
