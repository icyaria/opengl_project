This program implements Phong lighting in a 3D environment using OpenGL. The sphere dynamically lights up the face as it rotates, and the user can control the camera.

In case there is a build issue, an executable file is available in the corresponding folder.

**Requirements**  
The project is implemented in C++ and requires the following:  
- CMake (3.10+)
- OpenGL
- g++ or MinGW (on Windows)

**Build & Execution**

**Windows (MinGW)**  
```
mkdir -p build  
cd build  
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER="C:/msys64/ucrt64/bin/gcc.exe" -DCMAKE_CXX_COMPILER="C:/msys64/ucrt64/bin/g++.exe" ..  
cmake --build . --config Debug  
cd ..  
.\graphics2425_opengl_project.exe  # Run from the root directory!
```

**Linux**  
```
mkdir -p build  
cd build  
cmake ..  
make -j$(nproc)  
cd ..  
./graphics2425_opengl_project  # Run from the root directory!
```

**Build Output**  
The executable file is created in the root directory:  
```
graphics_project_2425/
|--- graphics2425_opengl_project.exe (Windows)  
|--- graphics2425_opengl_project (Linux)  
|--- build/  (Folder with temporary build files)
```

**Implementation**  
**main.cpp**  
Handles window creation with GLFW, shader and model loading, camera management, and sphere movement.  
Includes Phong lighting, which is implemented in `face-shader.fs`.  

**Functions**  
Sphere Rotation:  
- "P" → Pause  
- "U" → Start  
- "H" / "J" → Decrease / Increase speed  

Camera Handling:  
- W, A, S, D → Movement  
- Mouse → Rotation  

Model Display:  
- The face is loaded from an `.obj` file using Assimp, while the sphere is created dynamically.  

**sphere.cpp**  
Creates the sphere using triangular approximations.  
`void Sphere::setupSphere()`  
- Initializes GPU memory with `glGenBuffers()` and `glBufferData()`.  
- Manages VAO and VBO for the vertices.  
- Sets attribute pointers for position, normalized vectors, and texture coordinates.  

`void Sphere::Draw()`  
- Uses `glDrawElements()` to render the sphere.  
- Enables Wireframe Mode (`glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)`) when needed.

All files and functions are based on LearnOpenGL.

---

