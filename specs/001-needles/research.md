# Research: Needles VST Plugin

**Created**: 2024-11-24
**Feature**: Needles VST Plugin
**Purpose**: Resolve technical unknowns and establish best practices for JUCE-based image-to-audio synthesis

## JUCE Framework for VST Development

**Decision**: Use JUCE 7.x as the primary framework for cross-platform VST development

**Rationale**:

- JUCE provides complete VST3/AU/AAX plugin framework with built-in host compatibility
- Cross-platform support (Windows, macOS, Linux) from single codebase
- Built-in audio processing pipeline with real-time safety guarantees
- Integrated GUI framework optimized for audio applications
- Mature ecosystem with extensive documentation and community support

**Alternatives considered**:

- Raw VST3 SDK (rejected: too low-level, platform-specific complications)
- iPlug2 framework (rejected: less mature, smaller ecosystem)
- Custom framework (rejected: reinventing established solutions)

## Image Processing in Real-Time Audio Context

**Decision**: Use JUCE's Image class with custom scanline processing for real-time audio generation

**Rationale**:

- JUCE Image class provides efficient cross-platform image loading (JPG, PNG, GIF)
- Scanline-based processing minimizes memory allocation in audio thread
- Built-in pixel format conversion and color space handling
- Image caching prevents file I/O during audio processing

**Alternatives considered**:

- OpenCV integration (rejected: heavy dependency, licensing complexity)
- Platform-specific image APIs (rejected: breaks cross-platform compatibility)
- Custom image parsing (rejected: reinventing image format support)

## Audio Thread Safety and Parameter Handling

**Decision**: Use JUCE's AudioProcessorValueTreeState for thread-safe parameter management

**Rationale**:

- Provides automatic thread-safe communication between UI and audio threads
- Built-in parameter smoothing prevents audio artifacts during real-time changes
- Automatic state persistence for DAW project saving/loading
- Host automation compatibility

**Alternatives considered**:

- Manual atomic operations (rejected: complex, error-prone)
- Message queues (rejected: adds latency, complexity)
- Direct shared memory (rejected: thread safety issues)

## RGB to Audio Conversion Algorithm

**Decision**: Use weighted RGB averaging with configurable area sampling and linear interpolation

**Rationale**:

- RGB averaging provides perceptually meaningful audio mapping
- Area-based sampling creates smooth audio transitions
- Linear interpolation between samples prevents audio aliasing
- Configurable weights allow artistic control

**Implementation approach**:

```cpp
// Pseudo-code for RGB to audio conversion
float rgbToAudio(int x, int y, int areaSize) {
    float totalR = 0, totalG = 0, totalB = 0;
    int pixelCount = 0;

    // Sample area around position
    for (int dx = -areaSize/2; dx <= areaSize/2; dx++) {
        for (int dy = -areaSize/2; dy <= areaSize/2; dy++) {
            // Extract RGB, accumulate
            // Handle boundary conditions
        }
    }

    // Convert to audio range [-1.0, 1.0]
    float avgR = totalR / pixelCount / 255.0f;
    float avgG = totalG / pixelCount / 255.0f;
    float avgB = totalB / pixelCount / 255.0f;

    return (avgR + avgG + avgB) / 3.0f * 2.0f - 1.0f;
}
```

**Alternatives considered**:

- HSV color space conversion (considered for future enhancement)
- Frequency domain mapping (rejected: too complex for initial implementation)
- Neural network-based conversion (rejected: overkill, performance impact)

## Path Tracing Implementation

**Decision**: Use iterator-based scanline pattern with configurable speed control

**Rationale**:

- Iterator pattern enables easy extension to different path algorithms
- Scanline (left-right alternating) provides predictable, musical patterns
- Speed control via sample rate scaling maintains audio quality
- Infinite looping via modulo arithmetic on image dimensions

**Implementation approach**:

```cpp
class ImageScanner {
    struct ScanPosition {
        float x, y;
        bool rightToLeft;
        int scanLine;
    };

    void advancePosition(float speed) {
        position.x += rightToLeft ? -speed : speed;

        if (position.x >= imageWidth || position.x < 0) {
            position.scanLine++;
            position.rightToLeft = !position.rightToLeft;
            position.x = rightToLeft ? imageWidth-1 : 0;

            if (position.scanLine >= imageHeight) {
                position.scanLine = 0; // Loop infinitely
            }
        }
    }
};
```

## Performance Optimization Strategies

**Decision**: Implement multi-level optimization for real-time performance

**Strategies**:

1. **Image pre-processing**: Convert to optimal pixel format on load
2. **Audio buffer optimization**: Process samples in chunks to amortize overhead
3. **Parameter smoothing**: Use JUCE's LinearSmoothedValue for artifact-free changes
4. **Memory pool**: Pre-allocate buffers to avoid real-time allocation

**Performance targets**:

- CPU usage: <10% on typical hardware
- Memory usage: <100MB for 2048x2048 images
- Parameter response: <50ms latency
- Audio dropouts: Zero tolerance

## Cross-Platform Compatibility

**Decision**: Use JUCE's built-in cross-platform abstractions exclusively

**Platform-specific considerations**:

- **Windows**: VST3, ASIO driver support
- **macOS**: AU and VST3, Core Audio integration
- **Linux**: VST3, ALSA/JACK audio backends

**Build system**: JUCE Projucer for cross-platform project generation with CMake fallback

## Testing Strategy

**Decision**: Multi-layer testing approach combining unit, integration, and manual testing

**Test categories**:

1. **Unit tests**: Image processing algorithms, parameter handling
2. **Integration tests**: Full plugin workflow, DAW compatibility
3. **Performance tests**: CPU usage, memory allocation profiling
4. **Manual tests**: Real DAW testing across platforms

**Test framework**: Catch2 for C++ unit testing with JUCE's audio testing utilities

## Development Workflow

**Decision**: TDD approach aligned with constitution requirements

**Workflow**:

1. Write failing tests for each component
2. Implement minimal code to pass tests
3. Refactor for performance and maintainability
4. Manual DAW testing for integration validation

**Code quality**: clang-format for formatting, cppcheck for static analysis

## Conclusion

All technical unknowns have been resolved with concrete implementation decisions. The JUCE framework provides a solid foundation for cross-platform VST development, while the chosen algorithms balance simplicity with musical utility. Performance optimizations ensure real-time audio processing requirements are met.
