# Vizior - A simple 2D renderer for basic graphics

The goal of Vizior is to bring to C++ the experience of using [p5js](https://p5js.org/) by providing simple funtions to draw shapes, lines, points, text, and sprites to a window.
Bring visuals to your apps with a small library and just a few lines of code.

## How to install

1. Make sure you have cloned the repository recursively (`git clone --recursive ...`)
2. Run the following commands to build and install the library
```sh
mkdir build
cd build
cmake ..
cmake --build .
sudo cmake --install .
```
3. Vizior should now be ready system wide, see this [sample project(TODO)] for an example on how to use the library 


## Features on the Roadmap

 - [ ] Make camera optional
 - [ ] Add a UI mode (where coord are screen, not world)
 - [ ] 3d API
 - [ ] Record image being drawn
 - [ ] Record video (would probably need ffmpeg so ideally optional at build-time )

### Maybe some-day
 - [ ] ?Custom Shaders?
 - [ ] Triangulation for polygon (non-convex-hull)
