# Needles - image to sound synth

This is an idea for a synth I've had for a while. I don't have time to learn JUCE at home so I'm using Cursor AI IDE to do it for me.

## Prerequisites

- CMake (3.15 or higher)
- C++ compiler supporting C++14 or higher
  - Windows: Visual Studio 2019 or higher
  - macOS: Xcode 11.0 or higher
  - Linux: GCC 7+ or Clang 6+
- JUCE framework (will be automatically downloaded via CMake)

## Building the Project

### 1. Clone the Repository

```bash
git clone <repository-url>
cd <repository-name>
```

### 2. Get JUCE

Either clone JUCE as a submodule:

```bash
git submodule add https://github.com/juce-framework/JUCE.git
git submodule update --init --recursive
```

Or download it manually and place it in the project root directory.

### 3. Build the Project

#### On Windows:

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

#### On macOS/Linux:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Installation

### Windows

The VST3 plugin will be built to:

```
build/VST_Plugin_artefacts/Release/VST3/VST_Plugin.vst3
```

Copy this to:

```
C:\Program Files\Common Files\VST3
```

### macOS

The VST3 plugin will be built to:

```
build/VST_Plugin_artefacts/Release/VST3/VST_Plugin.vst3
```

Copy this to:

```
/Library/Audio/Plug-Ins/VST3
```

like this:

```
sudo cp -r "build/VST_Plugin_artefacts/Release/VST3/VST_Plugin.vst3" "/Library/Audio/Plug-Ins/VST3/"
```

### Linux

The VST3 plugin will be built to:

```

build/VST_Plugin_artefacts/Release/VST3/VST_Plugin.vst3

```

Copy this to:

```

/usr/local/lib/vst3

```

## Usage

After installation, the plugin can be loaded in any VST3-compatible Digital Audio Workstation (DAW) such as:

- Ableton Live
- FL Studio
- Reaper
- Bitwig Studio
- Studio One

## Development

### Project Structure

```

.
├── CMakeLists.txt
├── JUCE/
├── src/
│ ├── audio/
│ │ ├── PluginProcessor.cpp
│ │ └── PluginProcessor.h
│ ├── gui/
│ │ ├── PluginEditor.cpp
│ │ └── PluginEditor.h
│ └── utils/
├── tests/
└── resources/

```

### Adding New Features

1. Audio processing code goes in `src/audio/PluginProcessor.cpp`
2. GUI-related code goes in `src/gui/PluginEditor.cpp`
3. Utility functions should be placed in `src/utils/`

## License

[Your chosen license]

## Contributing

[Your contribution guidelines]

```

```
