# Ray Traced Sandbox

## Overview
This project is a fully ray-traced sandbox built with Vulkan. It supports loading of meshes using Assimp and includes a BVH constructor for optimized path tracing. The project also features a custom entity component system and resource manager for further optimization.

## Features
- **Ray Tracing**: Full ray tracing capabilities for any object.
- **Mesh Loading**: Load meshes using Assimp.
- **BVH Constructor**: Optimized path tracing with a Bounding Volume Hierarchy.
- **Entity Component System**: Custom ECS for efficient management of entities.
- **Resource Manager**: Optimized resource management.
- **Real-Time Physics Simulator**: 
    - Rigid body mechanics
    - Conservation of energy and momentum
    - Transference of forces
    - Angular momentum and torque
    - Newtonian gravitation simulator
    - Collision detection, resolution, and solving of collisions

## How It Works

### Ray Tracing
The project utilizes Vulkan's ray tracing capabilities to render realistic images by simulating the way light interacts with objects. This involves tracing the path of light rays as they bounce around the scene, calculating reflections, refractions, and shadows to produce high-quality visuals.

### Mesh Loading
Meshes are loaded using the Assimp library, which supports a wide range of 3D model formats. This allows for easy integration of complex models into the sandbox. Assimp handles the import and export of these models, providing a seamless workflow for adding new assets.

### BVH Constructor
A Bounding Volume Hierarchy (BVH) is used to optimize the path tracing process. The BVH groups objects in the scene into a hierarchy of bounding volumes, allowing the ray tracer to quickly eliminate large portions of the scene that do not intersect with a given ray. This significantly improves rendering performance.

### Entity Component System
The custom Entity Component System (ECS) is designed for efficient management of game entities. Entities are composed of various components that define their behavior and properties. The ECS architecture allows for flexible and scalable entity management, making it easy to add new features and systems to the game.

### Resource Manager
The resource manager is responsible for loading, storing, and managing game assets such as textures, models, and shaders. It ensures that resources are efficiently utilized and prevents redundant loading of assets. This helps in maintaining optimal performance and reducing memory usage.

### Real-Time Physics Simulator
The real-time physics simulator adds a layer of realism to the sandbox by simulating physical interactions between objects. It includes:
- **Rigid Body Mechanics**: Simulates the motion and interaction of solid objects.
- **Conservation of Energy and Momentum**: Ensures that physical interactions adhere to the laws of physics.
- **Transference of Forces**: Models how forces are transferred between objects upon collision.
- **Angular Momentum and Torque**: Simulates rotational dynamics and the effects of torque.
- **Newtonian Gravitation Simulator**: Models gravitational forces between objects based on Newton's law of universal gravitation.
- **Collision Detection, Resolution, and Solving of Collisions**: Detects collisions between objects, resolves interpenetrations, and calculates the resulting forces and changes in motion.

These features work together to create a dynamic and immersive sandbox environment where users can experiment with various physical and graphical scenarios.