# Terminal Chess Simulator

## Description
A two‑player terminal-based chess simulator written in C++ using object‑oriented design. This is not a chess engine—moves are validated and enforced by your implementation of the rules, but there is no AI opponent.

## Overview
The core design centers on an abstract base class `Pieces`, which declares:

- **Protected attributes**  
  - `int name;` &nbsp;&nbsp;// piece identifier integer
  - `bool colour;` &nbsp;// white = 0, black = 1  
  - `bool alive;`  
  - `bool is_first_move;`  
  - `bool is_last_square_move;`  
  - `pair<int,int> position;`

- **Public methods**  
  - Getters: `getName()`, `getColour()`, `getPosition()`, `isAlive()`, `isFirstMove()`, `isLastSquareMove()`  
  - `void Kill()` — mark the piece as captured  
  - **Pure virtuals** (must be overridden by each piece subclass):  
    - `bool is_valid_move(pair<int,int> to, vector<vector<Pieces*>> &board)`  
    - `bool check_obstacle(pair<int,int> to, vector<vector<Pieces*>> &board)`  
    - `void move_position(pair<int,int> to, vector<vector<Pieces*>> &board)`  
    - `Pieces* clone() const` — for deep‑copying the board  

- **Static helper functions**  
  - `static bool check_boundry(pair<int,int> pos)` — returns `false` if `pos` is within [0..7]  
  - `static pair<string,bool> which_piece(int id)` — maps numeric IDs to piece names & colors  
  - `static vector<vector<Pieces*>> deepCopyBoard(...)` — clone an entire board for “what‑if” move validation  
  - `static void deleteBoard(...)` — clean up dynamically allocated pieces  
  - `protected static void handleEnPesantFalse(...)` — reset `is_last_square_move` flags after each turn  

Each concrete piece class (`King`, `Queen`, `Bishop`, `Knight`, `Rook`, `Pawn`) inherits from `Pieces` and overrides these methods.  
- **King** also implements:  
  - `is_check()`, `is_checkmate()`, `is_stalemate()`, `can_castle()`  

- **Pawn** also implements:  
  - Promotion rules  
  - En passant capture & tracking via `is_last_square_move`  

The `Chess` class maintains a dynamic 8×8 `vector<vector<Pieces*>>` board, takes user input (`(row, col) - co-ordinate` style), validates moves (via deep‑copy simulation), checks for check/checkmate/stalemate/insufficient material, and announces game state.

## Object‑Oriented Concepts

| Concept           | Usage in Project                                                                                                                                              |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Encapsulation** | Piece attributes are `protected` or `private` with getters/setters; users cannot arbitrarily change a piece’s state from outside its class.                 |
| **Abstraction**   | `Pieces` is an abstract interface declaring all move‑related behaviors; concrete subclasses implement the details, while `Chess` only invokes the interface. |
| **Inheritance**   | All piece types derive from `Pieces`, reusing and extending its interface.                                                                                     |
| **Polymorphism**  | Board stores `Pieces*` and invokes overridden methods (`is_valid_move()`, `move_position()`) at runtime. Method overloading of `is_check()` shows compile‑time polymorphism. |
| **Static Methods**| Utility routines (`check_boundry`, `which_piece`, `deepCopyBoard`, `deleteBoard`, `handleEnPesantFalse`) are shared across all instances without needing an object. |

Additionally, the code uses:
- **`this->`** for clarity in member access  
- **`const` correctness** and `clone() const` for safe copying  
- **Constructor chaining** to initialize piece state  
- **Deep vs. shallow copy** of the board to simulate and validate moves before committing  
- **Dynamic memory allocation** and proper **destructors**  

## Features
- Full movement rules for all pieces (including castling, en passant, promotion)  
- Real‑time check, checkmate, stalemate and insufficient‑material detection  
- Move‑by‑move user input with clear invalid‑move feedback  
- Clean separation between game logic (`Chess` class) and piece behavior  

## Technologies Used
- **Language**: C++17  
- **Build system**: `g++` (or Clang)  

## Installation

1. **Clone the repo**  
   ```bash
   git clone https://github.com/ParthSorathiya1210/Chess_Simulation.git
   cd Chess_Simulation
