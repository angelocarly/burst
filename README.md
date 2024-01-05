# Burst
![build](https://github.com/angelocarly/burst/actions/workflows/cmake.yml/badge.svg)  
Minimal C++ rendering library using Vulkan. Made for personal use for my generative art projects.

## Building
Firstly, install the required system dependencies:
```
sudo apt-get install -y libspdlog-dev libglfw3-dev glslang-tools libglm-dev
```

Secondly, install the [Vulkan SDK](https://vulkan.lunarg.com) and set the path environment variables:
```
export VULKAN_SDK=/path/to/vulkan/sdk
# Required for MoltenVK
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

## Dependencies
- [spdlog](https://github.com/gabime/spdlog) - Fast header only logging library.
- [glfw](https://github.com/glfw/glfw) - Cross platform window and input library.
- [glm](https://github.com/g-truc/glm) - OpenGL Math Library, wonderful to use imo.
- [Vulkan SDK](https://vulkan.lunarg.com) - Low level graphics API.
- [VulkanMemoryAllocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) - Easy to integrate Vulkan memory allocator.
- [VulkanMemoryAllocator-Hpp](https://github.com/YaaZ/VulkanMemoryAllocator-Hpp) - C++ bindings for VulkanMemoryAllocator.
- [Dear Imgui](https://github.com/ocornut/imgui) - Lightweight, feature full gui library.
- [stbi](https://github.com/nothings/stb) - PNG library
- [ImGuiFileDialog](https://github.com/aiekick/ImGuiFileDialog) - File dialog written for Dear Imgui
