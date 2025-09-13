# Conway's Game of Life

A high-performance implementation of Conway's Game of Life built in C using raylib, featuring efficient multithreading and sparse grid representation.

## Features

### ✅ Completed
- **Sparse grid representation** - Efficient memory usage using hash tables (UTHash)
- **Multithreaded computation** - 4-thread parallel processing with local result merging
- **Optimized rendering** - Viewport-based culling and thread-safe drawing
- **Large grid support** - Handles grids up to 1500x1500 cells efficiently
- **Configurable threading** - Toggle between single-threaded and multithreaded modes
- **Race condition protection** - Mutex-protected grid access and rendering

### 🚧 Future Improvements
- **Interactive editing** - Click to add/remove cells during simulation
- **Pattern loading** - Load common Conway's Life patterns (gliders, oscillators, etc.)
- **Performance monitoring** - Real-time FPS and generation statistics

## Technical Details

- **Grid size**: Configurable via `GRID_SIZE` constant (currently 1000x1000)
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

- **WASD**: pan around the simulation

## About Conway's Game of Life

Conway's Game of Life is a cellular automaton where cells live, die, or reproduce based on simple rules:
- Any live cell with 2-3 live neighbors survives
- Any dead cell with exactly 3 live neighbors becomes alive
- All other cells die or remain dead

Despite these simple rules, complex and beautiful patterns emerge!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.