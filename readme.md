# WoopWoop 2.0 (C++)

> A C++ port of the [WoopWoop](https://github.com/DanPeled/WoopWoop-Game-Engine) engine
## Huh?
WoopWoop is a open source, easy to use basic game engine, with a simple to use ECS system. 

It was made with the purpouse of making game development easier to read, write, and making it faster to prototype projects.
## Why I moved to C++?

**Short answer:** Changed to Linux, raylib stopped working, couldn't manage to make .NET function.

**Longer answer:** Thought it might be a chance to improve my C++ skills, and also a way to make the engine faster.

## Changes over the C# version:
- Uses [SFML](https://www.sfml-dev.org/) for rendering.
- [ImGui](https://github.com/ocornut/imgui) integration.
- Highly improved performance (from around 200fps to around 5000fps on an empty scene).
- New scene system
- Animation system
- Less generic typing (mostly due to how C++ works)
- More advanced editor

# Making
there are 2 make configurations: `debug` and `release`:
- Make the `debug` using the `debug.sh` file or call `make debug`.
- Make the `release` using the `release.sh` file or call `make`.

## Configuration information: 
- `debug`: Will include all the engine code in the build, and run with the editor.
- `release`: Will not include the editor code and will only include the neccessary stuff for your game.