# CooEngine
Welcome to the Coo Engine source code!

Coo Engine is a custom Game Engine developed using Direct3D 11 and C++ 17. 
The engine supports both 3D and 2D games development. 

## Main Features
### AI
- AIWorld that contains and manages all the Entities, Agents, Obstacles and Walls.
- Finite State Machine that allows the addition of custom states and actions on Agents and switch between states.
- Goal Module that hierarchically determines Agents' action according to subgoal and strategic top-level goal.
- Path Finding that uses policy pattern for customization pathing methods in a graph.
- Perception of Agent that senses the given environment and memorizes information for some amount of time.
- Steering Behavior that can be added to Agents and control of the weight of each behaviour while combining. Implementated behaviors including Seek, Flee, Arrive, Wander, Pursuit, Evade, Obstacle Avoidance, Separation, Alignment and Cohesion.
  
### Core
- Memory Management:
  - BlockAllocator that allocates, access and free fixed-size memory blocks to avoid memory fragmentation.
  - Handle Pool that manages accessibility and checks the validation of Handles.
- StreamReader and StreamWriter that read and write data into MemoryStream. 
- Meta system that saves information of source code which can be used on run time. General Meta type including:
  - Meta Array
  - Meta Class
  - Meta Field
  - Meta Pointer
  
### Editor
- Component-based game object system that can:
  - Add Game Objects and Service in World
  - Load JSON template for object creation

### Graphics
- Animation:
  - Animator that plays, pauses animation and manages the transition between animation clips.
  - Skeleton/Skinning animation support.
  - Tool for animation file import and convert to a custom animation file.
- DebugUI that provides an interface between CooEngine and Dear Imgui.
- Mesh:
  - Basic geometry builder includes Cube, Cone, Cylinder, Plane and Sphere.
  - Mesh data save and load.
  - MeshBuffer for shader use.
- Simple Draw that is used for visual debugging.
- Sprite Renderer:
  - Batches sprites for efficiency.
  - Draw sprite with different textures, positions, rotations, scales and pivots.

### Math
- Math types:
  - *Vector2, Vector3, Vector4*
  - *Matrix3, Matrix4*
  - *Quaternion*
  - 2D: *Circle, LineSegment*
  - 3D: *AABB, Ray*
  - *PerlinNoise*
- Some useful constants and helper fucntions.
- Random number generation using std::random_device.

### Network
- LinkingContext that refer objects instance across a network
- TCP socket and UDP socket

### Physics
- PhysicsWorld that contains and manages all the Particles and Constraints
- Particles that updates using Verlet integration
- Constraints that affects particles including:
  - Fixed Constraints
  - Spring Constraints

## External Source Used
- [Assimp](https://github.com/assimp/assimp)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [DirectXTK](https://github.com/microsoft/DirectXTK)
- [RapidJSON](https://rapidjson.org/)
- [Mixamo](https://www.mixamo.com/#/)
