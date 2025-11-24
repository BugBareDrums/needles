# Data Model: Needles VST Plugin

**Created**: 2024-11-24
**Feature**: Needles VST Plugin
**Purpose**: Define core entities and their relationships for the plugin architecture

## Core Entities

### ImageSource

**Purpose**: Represents a loaded image file with associated processing state

**Attributes**:

- `imageData`: JUCE::Image - The loaded image data in native JUCE format
- `width`: int - Image width in pixels
- `height`: int - Image height in pixels
- `filePath`: std::string - Source file path for display and reload
- `isLoaded`: bool - Whether image is successfully loaded and ready for processing
- `pixelFormat`: JUCE::Image::PixelFormat - Internal pixel format (RGB, ARGB, etc.)

**Validation Rules**:

- Image dimensions must be > 0 and < 4096 pixels
- Supported formats: JPG, PNG, GIF only
- File size must be < 50MB to prevent memory issues
- Must validate file integrity before loading

**State Transitions**:

- Empty → Loading → Loaded (success) or Failed (error)
- Loaded → Empty (when new image loaded or cleared)

### ScanPath

**Purpose**: Represents current position and scanning pattern through the image

**Attributes**:

- `currentX`: float - Current horizontal position (sub-pixel precision)
- `currentY`: float - Current vertical position (sub-pixel precision)
- `scanDirection`: enum { LeftToRight, RightToLeft } - Current scan direction
- `scanLine`: int - Current scanline number (0 to height-1)
- `isLooping`: bool - Whether scanning should loop infinitely
- `pathType`: enum { Horizontal, Vertical, Diagonal, Spiral } - Scanning pattern

**Validation Rules**:

- Position coordinates must be within image bounds
- ScanLine must be valid for current image height
- Path type must be supported by scanner implementation

**State Transitions**:

- Each audio sample advances position according to scan speed
- Line completion triggers direction reversal and line increment
- Image completion triggers loop restart if enabled

### AudioParameters

**Purpose**: User-configurable settings that affect audio generation

**Attributes**:

- `scanSpeed`: float - Speed multiplier for path advancement (0.1 to 10.0)
- `areaSize`: int - Pixel area size for RGB averaging (1 to 50)
- `conversionFormula`: enum { RGBAverage, WeightedRGB, Luminance } - RGB to audio method
- `outputGain`: float - Master output level (0.0 to 2.0)
- `leftChannelWeight`: float - Left channel mix amount (0.0 to 1.0)
- `rightChannelWeight`: float - Right channel mix amount (0.0 to 1.0)

**Validation Rules**:

- All float values must be within specified ranges
- Area size must be odd number for centered sampling
- Weights must sum to reasonable values to prevent clipping

**Relationships**:

- Linked to JUCE::AudioProcessorValueTreeState for thread-safe updates
- Changes trigger parameter smoothing to prevent audio artifacts

### SynthesisEngine

**Purpose**: Coordinates conversion from image pixels to audio stream

**Attributes**:

- `sampleRate`: double - Current audio sample rate from host
- `bufferSize`: int - Audio buffer size from host
- `isProcessing`: bool - Whether audio generation is active
- `currentSample`: uint64_t - Running sample counter for position tracking
- `smoothedParameters`: Map of parameter smoothing objects
- `audioBuffer`: JUCE::AudioBuffer<float> - Working audio buffer

**Validation Rules**:

- Sample rate must be standard audio rate (44.1kHz - 192kHz)
- Buffer size must be power of 2 and reasonable (32-8192 samples)
- Must maintain real-time processing constraints

**Relationships**:

- Uses ImageSource for pixel data access
- Uses ScanPath for position tracking
- Uses AudioParameters for synthesis control
- Outputs to host audio pipeline

### PluginState

**Purpose**: Manages persistence and state synchronization

**Attributes**:

- `stateVersion`: int - Version for backward compatibility
- `imageFilePath`: std::string - Last loaded image path
- `parameterStates`: JUCE::ValueTree - All parameter values
- `scanPosition`: ScanPath - Current scanner state
- `isImageAutoLoad`: bool - Whether to reload image on project open

**Validation Rules**:

- State version must be compatible
- File paths must be validated before reload attempts
- Parameter states must match current parameter definitions

**State Transitions**:

- Save: Serialize current state to host-provided data
- Load: Deserialize state and restore plugin condition
- Reset: Return to default state

## Entity Relationships

### ImageSource ↔ ScanPath

- **Relationship**: ScanPath depends on ImageSource dimensions
- **Constraint**: ScanPath position must stay within ImageSource bounds
- **Update behavior**: When new image loaded, ScanPath resets to origin

### AudioParameters ↔ SynthesisEngine

- **Relationship**: SynthesisEngine reads AudioParameters for real-time processing
- **Constraint**: Thread-safe access via JUCE's parameter system
- **Update behavior**: Parameter changes trigger smooth value transitions

### ScanPath ↔ SynthesisEngine

- **Relationship**: SynthesisEngine advances ScanPath position each audio sample
- **Constraint**: Position updates must maintain scan pattern integrity
- **Update behavior**: Speed changes affect position advancement rate

### PluginState ↔ All Entities

- **Relationship**: PluginState persists and restores all entity states
- **Constraint**: State format must remain backward compatible
- **Update behavior**: Save/load operations synchronize all entity data

## Data Flow

### Image Loading Flow

1. User selects image file
2. ImageSource validates and loads file
3. ScanPath resets to image origin
4. SynthesisEngine begins audio generation
5. PluginState updates with new image path

### Audio Generation Flow

1. Host requests audio buffer
2. SynthesisEngine reads current AudioParameters
3. For each sample:
   - SynthesisEngine gets pixel data from ImageSource at ScanPath position
   - RGB values converted using AudioParameters formula
   - ScanPath position advanced by speed parameter
   - Audio sample written to output buffer
4. Buffer returned to host

### Parameter Update Flow

1. User adjusts UI control
2. AudioParameters receives new value
3. JUCE framework ensures thread-safe delivery to SynthesisEngine
4. SynthesisEngine applies smoothed parameter change
5. PluginState marks state as dirty for next save

## Performance Considerations

### Memory Management

- ImageSource caches pixel data for fast access
- SynthesisEngine pre-allocates working buffers
- Avoid memory allocation in audio thread

### Thread Safety

- ImageSource: Read-only after loading (thread-safe)
- ScanPath: Modified only in audio thread
- AudioParameters: Thread-safe via JUCE framework
- SynthesisEngine: Audio thread only for processing

### Optimization Opportunities

- Pre-compute area sampling offsets for common area sizes
- Cache RGB conversion results for repeated pixels
- Use SIMD instructions for bulk pixel processing
