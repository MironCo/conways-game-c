# Conway's Game of Life

A high-performance implementation of Conway's Game of Life built in C using raylib, featuring efficient multithreading and sparse grid representation.

## Features

### ✅ Completed
- **Infinite plane** - Cells can exist at any coordinates with no boundary limits
- **Sparse grid representation** - Efficient memory usage using hash tables (UTHash)
- **Multithreaded computation** - 4-thread parallel processing with local result merging
- **Optimized rendering** - Viewport-based culling and thread-safe drawing
- **Camera controls** - Pan (WASD) and zoom (mouse wheel) to explore the infinite plane
- **Configurable threading** - Toggle between single-threaded and multithreaded modes
- **Race condition protection** - Mutex-protected grid access and rendering

### 🚧 Future Improvements
- **Interactive editing** - Click to add/remove cells during simulation
- **Pattern loading** - Load common Conway's Life patterns (gliders, oscillators, etc.)
- **Performance monitoring** - Real-time FPS and generation statistics

## Technical Details

- **Infinite plane**: No grid boundaries - cells can exist anywhere within integer coordinate limits
- **Initial generation**: Random cells seeded in a 1000x1000 area centered at origin
- **Memory usage**: Only alive cells are stored, memory scales with pattern complexity not grid size
- **Threading**: Automatically uses single-threaded mode for <100 alive cells
- **Memory management**: Thread-local hashmaps prevent race conditions
- **Rendering**: Synchronized with mutex locks to prevent visual artifacts

## Getting Started

This project uses C and raylib for graphics rendering.

### Building

```bash
# Install dependencies (raylib)
# Clone and build the project
make
```

### Controls

- **WASD**: Pan around the simulation
- **Mouse wheel**: Zoom in/out (0.5x to 2.0x)
- **Space**: Pause/resume simulation

## About Conway's Game of Life

Conway's Game of Life is a cellular automaton where cells live, die, or reproduce based on simple rules:
- Any live cell with 2-3 live neighbors survives
- Any dead cell with exactly 3 live neighbors becomes alive
- All other cells die or remain dead

Despite these simple rules, complex and beautiful patterns emerge!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.