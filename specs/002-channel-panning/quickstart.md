# Quick Start: RGB Channel Panning Implementation

**Feature**: 002-channel-panning  
**Target**: Developers implementing RGB channel stereo positioning  
**Prerequisites**: Completed 001-needles (core image-to-audio functionality)

## Development Setup

### 1. Branch Setup

```bash
# Ensure you're on the correct feature branch
git checkout 002-channel-panning
git pull origin 002-channel-panning

# Verify base functionality is available
# (001-needles should be merged into this branch)
```

### 2. Build Environment

```bash
# Navigate to project
cd NeedlesVST/

# Clean build (recommended for new feature)
rm -rf build/
mkdir build && cd build

# Configure with testing enabled
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON

# Build with contracts
make -j8
```

### 3. Run Contract Tests (TDD Start)

```bash
# Run initial contract tests (should fail - Red/Green/Amber TDD)
./Tests/ContractTests

# Expected output: All pan-related tests should fail
# This confirms contracts are properly defined before implementation
```

## Implementation Workflow

### Phase 1: Core Components (TDD Order)

#### 1.1 StereoProcessor Component

**Location**: `NeedlesVST/Source/StereoProcessor.h/cpp`
**Purpose**: Constant power panning algorithm

```cpp
// Start with failing contract test:
// StereoProcessorContract::ProcessesCenterPanCorrectly()

// Implement basic interface:
class StereoProcessor {
public:
    std::pair<float, float> processPan(float input, float panPosition);
private:
    float clampPan(float pan);
    std::pair<float, float> constantPowerPan(float input, float pan);
};
```

**Test-First Development**:

1. Run contract test → RED (fails)
2. Implement minimal code → GREEN (passes)
3. Refactor for performance → REFACTOR
4. Repeat for next contract

#### 1.2 Parameter Manager Extension

**Location**: `NeedlesVST/Source/ParameterManager.h/cpp`
**Purpose**: Add RGB pan parameters with thread-safe access

```cpp
// Extend existing interface with:
class ParameterManager : public IParameterManager {
private:
    std::atomic<float> redPan{0.0f};
    std::atomic<float> greenPan{0.0f};
    std::atomic<float> bluePan{0.0f};

public:
    float getRedPan() const override;
    float getGreenPan() const override;
    float getBluePan() const override;
};
```

#### 1.3 Audio Processing Integration

**Location**: `NeedlesVST/Source/PluginProcessor.cpp`
**Purpose**: Integrate pan processing into audio pipeline

```cpp
// In processBlock():
void NeedlesAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&) {
    // 1. Get RGB pixel data (existing functionality)
    RGB pixel = imageScanner->getCurrentPixel();

    // 2. Generate per-channel audio (extend existing)
    float redAudio = pixel.toAudioChannel(0);    // Use existing method
    float greenAudio = pixel.toAudioChannel(1);
    float blueAudio = pixel.toAudioChannel(2);

    // 3. Apply stereo positioning (NEW)
    auto [redLeft, redRight] = stereoProcessor->processPan(redAudio, paramManager->getRedPan());
    auto [greenLeft, greenRight] = stereoProcessor->processPan(greenAudio, paramManager->getGreenPan());
    auto [blueLeft, blueRight] = stereoProcessor->processPan(blueAudio, paramManager->getBluePan());

    // 4. Mix channels to stereo output (NEW)
    float leftOutput = redLeft + greenLeft + blueLeft;
    float rightOutput = redRight + greenRight + blueRight;

    // 5. Write to output buffer (modify existing)
    buffer.setSample(0, sample, leftOutput);   // Left channel
    buffer.setSample(1, sample, rightOutput);  // Right channel
}
```

### Phase 2: UI Integration

#### 2.1 Parameter Registration

**Location**: `NeedlesVST/Source/PluginProcessor.cpp` (constructor)

```cpp
// Add to AudioProcessorValueTreeState parameters:
parameters.createAndAddParameter("redPan", "Red Pan", "%",
    juce::NormalisableRange<float>(-100.0f, 100.0f), 0.0f);
parameters.createAndAddParameter("greenPan", "Green Pan", "%",
    juce::NormalisableRange<float>(-100.0f, 100.0f), 0.0f);
parameters.createAndAddParameter("bluePan", "Blue Pan", "%",
    juce::NormalisableRange<float>(-100.0f, 100.0f), 0.0f);
```

#### 2.2 UI Controls

**Location**: `NeedlesVST/Source/PluginEditor.cpp`

```cpp
// Add rotary sliders for RGB pan controls:
class PluginEditor : public AudioProcessorEditor {
private:
    juce::Slider redPanSlider;
    juce::Slider greenPanSlider;
    juce::Slider bluePanSlider;

    juce::SliderParameterAttachment redPanAttachment;
    juce::SliderParameterAttachment greenPanAttachment;
    juce::SliderParameterAttachment bluePanAttachment;
};
```

## Testing Strategy

### Unit Tests

```bash
# Test individual components
./Tests/UnitTests --filter="StereoProcessor*"
./Tests/UnitTests --filter="RGBChannel*"
./Tests/UnitTests --filter="Parameter*"
```

### Integration Tests

```bash
# Test complete audio pipeline
./Tests/IntegrationTests --filter="AudioProcessing*"
```

### Performance Tests

```bash
# Verify CPU usage targets
./Tests/PerformanceTests --filter="PanPerformance*"

# Expected: <2% additional CPU usage over baseline
```

## Debugging Tips

### Common Issues & Solutions

**Issue**: Pan doesn't affect audio output

```cpp
// Debug: Check parameter flow
float redPan = paramManager->getRedPan();
DBG("Red pan value: " << redPan);

// Verify: Parameter should be in [-1.0, +1.0] range internally
// UI shows [-100, +100], but processing uses normalized values
```

**Issue**: Audio artifacts during pan automation

```cpp
// Debug: Check parameter smoothing
// Add smoothing to parameter reads:
float smoothedPan = parameterSmoother.getNextValue();
```

**Issue**: Center pan not equal in both channels

```cpp
// Debug: Verify constant power law implementation
// Center should be: left = right = input * √2/2
// Check: std::sqrt(2.0f) / 2.0f ≈ 0.7071067811865476f
```

### Performance Debugging

```bash
# Profile with audio-specific tools
# Use Instruments (macOS) or similar for real-time profiling
# Focus on processBlock() method timing
```

## Success Criteria Validation

### Acceptance Test Checklist

- [ ] Each RGB channel pans independently
- [ ] Parameter automation works smoothly in DAW
- [ ] CPU usage remains <10% total plugin usage
- [ ] Audio quality maintained (no distortion/artifacts)
- [ ] UI controls respond within 50ms
- [ ] Default center position (0%) on plugin load

### Final Integration Test

```bash
# Load into DAW (Reaper, Logic, etc.)
# 1. Load colorful image
# 2. Set RGB pans to different positions
# 3. Automate pan parameters
# 4. Verify stereo field movement matches automation
# 5. Check CPU usage in DAW's performance meter
```

## Next Steps After Implementation

1. **Code Review**: Submit PR with contract test results
2. **Documentation**: Update user manual with RGB pan controls
3. **Release Prep**: Add to changelog and version notes
4. **User Testing**: Validate with real-world music production scenarios

## Getting Help

- **Architecture Questions**: Reference `research.md` for design decisions
- **Contract Failures**: Check `contracts/` for expected behavior
- **Performance Issues**: Review constitution.md performance requirements
- **Integration Issues**: Consult existing 001-needles implementation patterns
