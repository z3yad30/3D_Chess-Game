# 3D_Chess-Game
A 3D chess game implemented with C++ OpenGL (glut)
# ♟️ 3D Chess Game (OpenGL/GLUT)

A fully interactive 3D chess game created using **C++**, **OpenGL**, and **FreeGLUT** — built as a university project while exploring computer graphics.

> ✅ Part of my **“Old Projects Recap”** series — collecting past builds to reflect growth & improvements.

---

## 🧠 Core Concepts

- 🔲 8x8 Chessboard with alternating 3D tiles
- 🧱 Modeled chess pieces with GLU (cylinders, spheres, cones)
- 🎮 Mouse click interaction to move pieces
- 🔄 Camera orbit control via keyboard (WASD + angle orbit)
- 📜 Valid chess move logic for all major pieces (pawn, rook, knight...)

---

## 🔥 Features

- Fully 3D-rendered chessboard
- Each piece styled using geometric primitives
- Camera movement: orbit with angle / up-down vertical movement
- Move validation (custom logic for each piece)
- Highlight selection with visual indicator
- Click to move from one square to another
- 2D overlay board blended on top for aesthetics

---

## 🛠️ Tech Stack

- **Language:** C++
- **Graphics:** OpenGL (FreeGLUT + GLEW)
- **Modeling:** GLUquadric, Teapot primitives
- **OS Tested On:** Windows (GLUT/FreeGLUT 2.1.0)

---

## 📸 Screenshots

![image](https://github.com/user-attachments/assets/1a239cb0-3bd2-44ba-8da7-0bff6c0f3c41)
![image](https://github.com/user-attachments/assets/44de5f72-31b3-4844-b233-1e91ae88563f)

---

## ▶️ How To Run

Make sure you have:
- OpenGL libraries (GLEW + FreeGLUT)
- Visual Studio or any C++ compiler that supports OpenGL

```
bash
g++ main.cpp -lfreeglut -lglu32 -lopengl32 -o ChessGame
./ChessGame
```

OR compile via your IDE of choice with OpenGL/GLUT linked.

👨‍💻 Notes
Made during my university computer graphics course

I was the project leader, working with others to bring the board and logic to life

Move logic is simplified for demonstration (can be expanded to include castling, en passant, check/checkmate)

📦 Future Improvements
Add full check/checkmate detection

Drag-and-drop UI

Multiplayer logic

Game history and replay

AI opponent using minimax algorithm

✨ Credits & Journey
Built with ❤️ for fun and learning

Shared now as part of my "Old Projects Recap" on LinkedIn

📬 Let’s Connect
linked in

https://www.linkedin.com/in/zeyadrefaey?utm_source=share&utm_campaign=share_via&utm_content=profile&utm_medium=android_app
