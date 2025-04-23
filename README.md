# Godot Node Tracker Extension
C++ extension for tracking Node2D objects with context management

## Features
- Track/untrack Node2D objects
- Attach arbitrary context data
- Automatic cleanup on node deletion
- Signal-based notifications

## Installation
1. Add as submodule:
```bash
git submodule add https://github.com/yourusername/godot-node-tracker.git
```
2. Recompile Godot with this module

## Usage
```gdscript
var tracker = GDTracker.new()
tracker.track_node($Player, {"speed": 200})
```

## Building
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
