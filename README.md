# 🎮 Raylib Game Dev Template

A lightweight C++ game development template built with **Raylib** and **ImGui**, designed for rapid prototyping and learning game development fundamentals.

## ✨ Features

- **Raylib Integration** - Fast, simple 2D/3D graphics rendering
- **ImGui Support** - Intuitive UI toolkit for in-game menus and debugging
- **Delta Time Movement** - Frame-rate independent game logic
- **Cross-Platform** - Built with CMake for Windows, macOS, and Linux
- **Clean Architecture** - Separated game layer and platform code
- **Visual Studio Ready** - Pre-configured for MSVC builds

## 📋 Prerequisites

- **CMake** 3.15+
- **C++17 or later**
- **Visual Studio** 2019+ (Windows) or GCC/Clang (Linux/macOS)

## 🚀 Quick Start

### 1. Clone & Build

```bash
cd game
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

### 2. Run the Game

```bash
./Debug/mygame.exe   # Windows
./mygame             # Linux/macOS
```

### 3. Controls

| Key   | Action     |
| ----- | ---------- |
| **W** | Move Up    |
| **A** | Move Left  |
| **S** | Move Down  |
| **D** | Move Right |

## 📁 Project Structure

```
.
├── src/
│   ├── gamelayer/
│   │   ├── gameMain.cpp      # Game logic
│   │   └── gameMain.h        # Game interface
│   └── platform/
│       └── main.cpp          # Platform entry point
├── thirdparty/
│   ├── raylib-6.0/           # Graphics library
│   ├── imgui-docking/        # UI framework
│   └── rlimgui/              # Raylib-ImGui binding
├── resources/                 # Game assets
├── CMakeLists.txt            # Build configuration
└── build/                    # Build output
```

## 🎯 Core Concepts

### Delta Time

Movement is calculated using **delta time** to ensure consistent gameplay speed regardless of frame rate:

```cpp
float deltaTime = GetFrameTime();
if (IsKeyDown(KEY_A)) {
    gameData.posX -= 200.f * deltaTime;  // 200 pixels/second
}
```

- `200.f` = movement speed in pixels per second
- `deltaTime` = time elapsed since last frame
- **Result:** Smooth, frame-rate independent movement

### Game Architecture

**initGame()** - Initialize game state  
**updateGame()** - Called every frame; handles input and rendering  
**closeGame()** - Cleanup and shutdown

## 🔧 Customization

### Change Player Speed

Edit [src/gamelayer/gameMain.cpp](src/gamelayer/gameMain.cpp#L25):

```cpp
200.f * deltaTime  // Change 200 to any speed you want
```

### Change Player Size

```cpp
int playerHeight = 20;
int playerWidth = 20;
// Adjust these values
```

### Change Player Color

```cpp
Color c;
c.r = 0;    // Red (0-255)
c.g = 255;  // Green (0-255)
c.b = 200;  // Blue (0-255)
c.a = 255;  // Alpha (0-255)
```

## 📚 Learning Resources

- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [Raylib Examples](https://github.com/raysan5/raylib/tree/master/examples)
- [ImGui Demo](https://github.com/ocornut/imgui)
- [Game Development with Raylib (YouTube)](https://www.youtube.com/results?search_query=raylib+tutorial)

## 🐛 Debugging

- **Enable Debug Build:**

  ```bash
  cmake --build . --config Debug
  ```

- **Check Console Output:**
  Game logs to `std::cout` - watch the console for debug messages

## 📦 Dependencies

| Library | Purpose        | Version          |
| ------- | -------------- | ---------------- |
| Raylib  | 2D/3D Graphics | 6.0              |
| ImGui   | UI Toolkit     | Latest (docking) |
| CMake   | Build System   | 3.15+            |

## 🎨 Next Steps

1. **Add Sprites** - Load PNG/JPG textures with Raylib
2. **Implement Collision** - Add collision detection logic
3. **Create Enemies** - Spawn and manage NPCs
4. **Add Sound** - Integrate audio with Raylib
5. **Build UI** - Use ImGui for menus and HUD

## 🤝 Contributing

Feel free to fork, modify, and enhance this template!

## 📄 License

This template is open for educational and commercial use.

---

**Happy Game Developing!** 🎮✨
