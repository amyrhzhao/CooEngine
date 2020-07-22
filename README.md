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
  - BlockAllocator that allocates fixed-size memory blocks on compile-time and manages them on run time.
  - Handle Pool that manages accessibility and check the validation of Handles.
- StreamReader and StreamWriter that read and write data into MemoryStream. 
- Meta system that save information of source code which can be use on run time. General Meta type including:
  - Meta Array
  - Meta Class
  - Meta Field
  - Meta Pointer
  
### Graphics
- Animation
- Camera
- DebugUI
- Material
- Mesh
- Simple Draw
- Sprite Renderer

### Math
- Math types:
  - *Vector2, Vector3, Vector4*
  - *Matrix3, Matrix4*
  - *Quaternion*
  - 2D: *Circle, LineSegment*
  - 3D: *AABB, Ray*

## External Source Used
- [Assimp](https://github.com/assimp/assimp)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [DirectXTK](https://github.com/microsoft/DirectXTK)
- [RapidJSON](https://rapidjson.org/)
- [Mixamo](https://www.mixamo.com/#/)
