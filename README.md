---

# 3D Programming OpenGL Project

A comprehensive 3D graphics application written in modern C++ utilizing OpenGL, GLFW, and GLAD. This project demonstrates core principles of 3D rendering, interactive camera control, object manipulation, and model loading, serving as both an educational resource and a foundation for more complex 3D applications.

---

## Table of Contents

* [Features](#features)
* [Getting Started](#getting-started)

    * [Prerequisites](#prerequisites)
    * [Installation](#installation)
* [Project Structure](#project-structure)
* [Usage](#usage)
* [Controls](#controls)
* [Configuration](#configuration)
* [Contributing](#contributing)
* [Acknowledgements](#acknowledgements)

---

## Features

* **Modern OpenGL Pipeline**: Utilizes vertex buffers, shaders, and VAOs.
* **Real-Time 3D Rendering**: Renders 3D objects in real time with interactive feedback.
* **Camera System**: First-person and orbit camera movement with mouse and keyboard controls.
* **OBJ Model Loader**: Load and display custom 3D models (.obj files).
* **Object Manipulation**: Move, scale, and rotate objects interactively.
* **Basic Lighting**: Implements directional and point lighting.
* **Event Handling**: Mouse and keyboard callbacks for smooth interaction.
* **Cross-Platform**: Runs on Windows, macOS, and Linux.
* **Clean, Modular Codebase**: Object-oriented design for extensibility and clarity.

---

## Getting Started

### Prerequisites

* **C++17** or later
* [CMake](https://cmake.org/) (3.10+ recommended)
* [GLFW](https://www.glfw.org/)
* [GLAD](https://glad.dav1d.de/)
* [stb\_image](https://github.com/nothings/stb) (for texture loading)
* [glm](https://github.com/g-truc/glm) (for math operations)

### Installation

1. **Clone the repository:**

   ```sh
   git clone https://github.com/yourusername/opengl-3d-programming-project.git
   cd opengl-3d-programming-project
   ```

2. **Install dependencies:**
   You can use your package manager or submodules for libraries (GLFW, GLAD, stb\_image, glm).

   Example for Ubuntu:

   ```sh
   sudo apt-get install libglfw3-dev libglm-dev
   ```

3. **Build the project:**

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

4. **Run the application:**

   ```sh
   ./OpenGL3DApp
   ```

---

## Project Structure

```text
.
├── assets/             # 3D models, textures, shaders
│   ├── Ball1.obj
│   └── ...
├── src/                # Source files
│   ├── main.cpp
│   ├── App.cpp
│   └── ...
├── shaders/            # GLSL vertex/fragment shaders
│   ├── basic.vert
│   └── basic.frag
├── CMakeLists.txt
├── README.md
```

---

## Usage

After building and running the application:

* By default, the app loads and renders the default model (`assets/Ball1.obj`).
* Use the controls below to move the camera and interact with objects.
* Models and assets can be swapped by changing file paths in the configuration.

---

## Controls

| Action            | Key/Mouse             | Description                |
|-------------------|-----------------------|----------------------------|
| Move Camera       | `W`, `A`, `S`, `D`    | Forward, left, back, right |
| Look Around       | Mouse Drag (Left Btn) | Rotate camera view         |
| Zoom In/Out       | Mouse Scroll          | Zoom camera in and out     |
| Object Manipulate | Arrow Keys / Custom   | Move selected object       |
| Exit Application  | `ESC`                 | Close the window           |

> *Tip: Controls can be customized in `src/App.cpp` or as documented in code comments.*

---

## Configuration

* **Models:** Place .obj models in the `assets/` folder. Update the path in `main.cpp` or `App.cpp` to load different
  models.
* **Shaders:** GLSL shaders can be modified in the `shaders/` directory.
* **Camera settings, window size, and lighting parameters** can be adjusted in source files for custom behavior.

---

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/YourFeature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin feature/YourFeature`)
5. Open a Pull Request

---

## Acknowledgements

* [LearnOpenGL](https://learnopengl.com/) for extensive tutorials and guides.
* [GLFW](https://www.glfw.org/)
* [GLAD](https://glad.dav1d.de/)
* [stb\_image](https://github.com/nothings/stb)
* [glm](https://github.com/g-truc/glm)

---

> For questions, suggestions, or issues,
> please [open an issue](https://github.com/ahmetabdullahgultekin/BilliardShow/issues).

---