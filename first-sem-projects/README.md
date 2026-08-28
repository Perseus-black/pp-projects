# pp-projects
C++ practice programs and small projects for personal practice.

## First-person room inspection

`room_inspection.cpp` is a standalone C++ terminal simulation. It uses a
simple ray-caster to render a first-person view of a room and lets you inspect
objects by facing them.

```bash
g++ -std=c++17 -Wall -Wextra -pedantic room_inspection.cpp -o room_inspection
./room_inspection
```

Use `W`/`S` to move, `A`/`D` to turn, `E` to inspect the centered object, and
`Q` to quit. It targets POSIX terminals because it uses `termios` for
real-time keyboard input.
