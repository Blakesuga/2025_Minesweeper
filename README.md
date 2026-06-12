# Minesweeper

A C++ implementation of the classic Minesweeper game built with SFML 2.6.1.


## Requirements

- C++ compiler (g++, MSVC, or MinGW)
- SFML 2.6.1
- CLion (recommended) or any C++ IDE


## Installing SFML

Download SFML 2.6.1 from https://www.sfml-dev.org/download.php and follow
the setup guide for your IDE:

- CLion on Mac: https://www.youtube.com/watch?v=BILAqQ-ZFDA
- CLion on Windows: https://dev.to/danielmelendezz/how-to-get-smfl-to-work-on-windows-using-clion-2bef


## Building & Running

### CLion
Open the project folder in CLion and run via the built-in build system.

### Terminal
g++ src/*.cpp -o project3.out -lsfml-graphics -lsfml-window -lsfml-system
./project3.out


## File Structure

The following must be present before running:

```
/files
  /images         <- all .png sprites
  board_config.cfg
  font.ttf
  leaderboard.txt
 ```

## Configuration

Edit files/board_config.cfg to change board settings.
The three lines represent columns, rows, and mine count respectively.

Example:

```
25
16
50
```

This produces an 800x600 board with 50 mines.


## How to Play

- Left-click a tile to reveal it
- Right-click a tile to place or remove a flag
- Reveal all non-mine tiles to win
- Clicking a mine ends the game


## Controls

```
Smiley face   —   Reset the board
Debug button  —   Toggle mine visibility
Pause/Play    —   Pause or resume the timer
Leaderboard   —   View the top 5 fastest times
```

## Notes

- Do not modify the files/ folder structure or the game will crash on launch
- The leaderboard stores only the top 5 fastest times
- Board dimensions and mine count can be freely changed in board_config.cfg
