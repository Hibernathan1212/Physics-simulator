#!/bin/bash
## -V: create SPIR-V binary
## -x: save binary output as text-based 32-bit hexadecimal numbers
## -o: output file
glslangValidator -V -x -o /Users/nathan/Documents/Coding/Simulator/Simulator/Vendor/imgui/vulkan/glsl_shader.frag.u32 /Users/nathan/Documents/Coding/Simulator/Simulator/Vendor/imgui/vulkan/glsl_shader.frag
glslangValidator -V -x -o /Users/nathan/Documents/Coding/Simulator/Simulator/Vendor/imgui/vulkan/glsl_shader.vert.u32 /Users/nathan/Documents/Coding/Simulator/Simulator/Vendor/imgui/vulkan/glsl_shader.vert
