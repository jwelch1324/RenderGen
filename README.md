# RenderGen
This is a personal project to learn how raytracers work by building one from scratch in C++

The project uses CMAKE and imports the benchmarks third party library.

# Current Features
- Full Vector/Matrix Math Library with partial quaternion support
- Basic Procedrual Shape Support
- Basic Lighting Support
- AntiAliasing Support
- Slow Single Threaded Renderer

# Near Term Goals
- Create an Actor Based rendering subsystem that will distribute the ray tracing over multiple cores. We will use actors since they can be spun up as either seperate threads on the same device, or as remote actors across a cluster. 
- Add support for triangulated meshes

# Long Term Goals
- Support Ray Marching of Fractal structures -- the real goal here was to build a tool inspired by Mandlebulb3D (which is written in Delphi) but in a language that more developers can work in. 
