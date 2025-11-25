# Research: RGB Channel Panning Implementation

**Feature**: 002-channel-panning  
**Date**: November 25, 2024  
**Research Phase**: Phase 0 - Technical Unknowns Resolution

## Research Questions Addressed

### 1. Stereo Panning Algorithm Implementation in JUCE

**Question**: What is the optimal stereo panning algorithm for real-time audio processing that maintains phase coherence and prevents center image collapse?

**Decision**: Constant Power Panning (sin/cos law)

- **Formula**:
  - Left channel = audio × cos(panAngle × π/4)
  - Right channel = audio × sin(panAngle × π/4)
  - Pan range: -1.0 (hard left) to +1.0 (hard right)

**Rationale**:

- Maintains constant perceived loudness across pan positions
- Preserves phase relationship for mono compatibility
- Industry standard for professional audio applications
- Computationally efficient for real-time processing
- JUCE recommendation in audio processing documentation

**Alternatives considered**:

- Linear panning: Rejected due to center image loudness dip (-3dB at center)
- Equal power (square root law): Rejected due to higher CPU cost
- Custom curve: Rejected due to complexity and lack of standardization

### 2. Parameter Range and Resolution for Professional Use

**Question**: What parameter range and resolution provides professional-grade control while maintaining efficient automation?

**Decision**: Float range [-100.0, +100.0] with 0.1 step resolution

- **Mapping**: User percentage directly maps to internal [-1.0, +1.0] range
- **Automation**: Sample-accurate parameter smoothing with 10ms attack time

**Rationale**:

- Matches industry standard DAW pan control conventions
- Provides sufficient resolution for precise positioning (1001 discrete steps)
- Direct percentage mapping eliminates user confusion
- 10ms smoothing prevents audio artifacts while maintaining responsiveness

**Alternatives considered**:

- Integer percentage: Rejected due to reduced automation precision
- [-1.0, +1.0] user range: Rejected due to user experience concerns
- Instant parameter changes: Rejected due to audio artifacts (clicks/pops)

### 3. RGB Channel Separation Architecture

**Question**: How should per-channel audio generation integrate with existing RGB-to-audio conversion while maintaining performance?

**Decision**: Pre-pan channel separation with cached RGB extraction

- **Architecture**:
  1. Extract RGB values from image pixel (existing)
  2. Generate separate audio for each channel: toAudioChannel(0/1/2)
  3. Apply pan positioning to each channel independently
  4. Mix channels to stereo output

**Rationale**:

- Leverages existing `RGB::toAudioChannel(int channel)` method
- Minimal CPU overhead (3 channel conversions vs 1 mixed conversion)
- Maintains separation for future enhancements (EQ, filtering per channel)
- Clean architectural separation of concerns

**Alternatives considered**:

- Post-processing pan on mixed signal: Rejected, loses RGB separation
- Full RGB duplication per channel: Rejected due to memory overhead
- Complex channel routing: Rejected due to architectural complexity

### 4. Thread-Safe Parameter Access for Real-Time Audio

**Question**: How should RGB pan parameters integrate with existing thread-safe parameter management?

**Decision**: Extend existing atomic parameter pattern in ParameterManager

- **Implementation**:
  ```cpp
  std::atomic<float> redPan{0.0f};
  std::atomic<float> greenPan{0.0f};
  std::atomic<float> bluePan{0.0f};
  ```
- **Access**: Lock-free reads from audio thread, atomic writes from UI thread

**Rationale**:

- Consistent with existing parameter architecture
- Maintains real-time audio thread constraints (no locks/blocking)
- Simple integration with JUCE AudioProcessorValueTreeState
- Proven pattern in existing codebase

**Alternatives considered**:

- Separate parameter manager: Rejected due to architectural inconsistency
- Lock-based synchronization: Rejected due to real-time thread requirements
- Message passing: Rejected due to latency concerns

### 5. UI Integration Best Practices

**Question**: What UI control type provides optimal user experience for pan parameters in VST plugin context?

**Decision**: JUCE Slider with rotary knob style and center detent

- **Configuration**:
  - RotaryVerticalDrag interaction
  - Center detent at 0% for quick center positioning
  - Bipolar color scheme (negative = blue, positive = red)
  - Text box for precise value entry

**Rationale**:

- Familiar to audio professionals (matches DAW pan controls)
- Center detent enables quick return to center position
- Visual feedback matches semantic meaning (left/right positioning)
- Efficient screen space usage for 3 RGB controls

**Alternatives considered**:

- Linear slider: Rejected due to space requirements for 3 controls
- Custom pan widget: Rejected due to development complexity
- Knob without detent: Rejected due to center positioning difficulty

## Implementation Strategy Summary

**Core Architecture**:

1. Extend existing RGB synthesis with per-channel processing
2. Add StereoProcessor component for constant power panning
3. Integrate pan parameters through existing ParameterManager pattern
4. Maintain real-time performance through lock-free atomic operations

**Performance Validation**:

- Benchmark CPU usage with all RGB channels at extreme pan positions
- Verify <50ms parameter response time with automation
- Test sample-accurate timing with rapid parameter changes

**Risk Mitigation**:

- Use proven JUCE patterns throughout implementation
- Extensive unit testing of pan algorithms
- Performance regression testing against existing CPU targets
- Professional audio testing with common DAW automation scenarios
