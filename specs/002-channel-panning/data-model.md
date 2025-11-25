# Data Model: RGB Channel Panning

**Feature**: 002-channel-panning  
**Date**: November 25, 2024  
**Phase**: 1 - Data Model Design

## Core Entities

### StereoChannelParams

**Purpose**: Represents stereo positioning parameters for individual RGB channels
**Lifecycle**: Created during parameter initialization, updated via DAW automation

| Field    | Type  | Range            | Default | Description                                  |
| -------- | ----- | ---------------- | ------- | -------------------------------------------- |
| redPan   | float | [-100.0, +100.0] | 0.0     | Red channel stereo position (% left/right)   |
| greenPan | float | [-100.0, +100.0] | 0.0     | Green channel stereo position (% left/right) |
| bluePan  | float | [-100.0, +100.0] | 0.0     | Blue channel stereo position (% left/right)  |

**Validation Rules**:

- All pan values must be within [-100.0, +100.0] range
- Values outside range are clamped to valid bounds
- Default center position (0.0) for all channels on plugin initialization

**State Transitions**:

- UI Change: User adjustment → Parameter update → Audio thread notification
- Automation: DAW automation → Parameter smooth → Audio processing
- Preset Load: Saved state → Parameter restoration → Audio configuration

### RGBChannelData

**Purpose**: Individual RGB channel audio data before stereo processing
**Lifecycle**: Generated per audio buffer, processed for stereo positioning

| Field      | Type  | Range        | Description                |
| ---------- | ----- | ------------ | -------------------------- |
| redAudio   | float | [-1.0, +1.0] | Red channel audio sample   |
| greenAudio | float | [-1.0, +1.0] | Green channel audio sample |
| blueAudio  | float | [-1.0, +1.0] | Blue channel audio sample  |
| isValid    | bool  | true/false   | Data validity flag         |

**Validation Rules**:

- Audio samples must be in normalized range [-1.0, +1.0]
- Invalid data (isValid=false) produces silent output
- Generated from existing RGB pixel data using toAudioChannel() method

### StereoOutput

**Purpose**: Final stereo audio output with pan positioning applied
**Lifecycle**: Calculated per audio sample, written to output buffer

| Field        | Type  | Range        | Description                |
| ------------ | ----- | ------------ | -------------------------- |
| leftChannel  | float | [-1.0, +1.0] | Left speaker audio sample  |
| rightChannel | float | [-1.0, +1.0] | Right speaker audio sample |

**Validation Rules**:

- Output samples must remain within [-1.0, +1.0] to prevent clipping
- Mix of RGB channels must not exceed output range
- Constant power panning maintains total energy across pan positions

## Relationships

### Parameter Flow

```
UI Control → ParameterManager → AudioProcessor → StereoProcessor
     ↓              ↓                ↓              ↓
StereoChannelParams → Atomic Storage → Thread-safe Read → Pan Processing
```

### Audio Processing Chain

```
RGB Pixel → RGBChannelData → Pan Processing → StereoOutput → Audio Buffer
    ↓            ↓              ↓              ↓            ↓
Image Scan → Channel Separation → Stereo Position → Channel Mix → DAW Output
```

### State Persistence

```
Plugin State → StereoChannelParams → DAW Project → Restored Parameters
     ↓               ↓                    ↓              ↓
XML/Binary → Parameter Values → Saved Project → Plugin Initialization
```

## Extended Entities (Integration)

### Enhanced ParameterManager Interface

**Extension**: Adds RGB pan parameter management to existing interface

**New Methods**:

```cpp
virtual float getRedPan() const = 0;
virtual float getGreenPan() const = 0;
virtual float getBluePan() const = 0;
```

**Thread Safety**: Maintains existing atomic access pattern

### Enhanced RGB Processing

**Extension**: Leverages existing RGB::toAudioChannel() for channel separation

**Processing Flow**:

1. Existing: `RGB pixel → toAudioChannel(0/1/2) → float samples`
2. New: `Channel samples → Pan processing → Stereo positioning`
3. Integration: `Stereo channels → Buffer mixing → Audio output`

## Constraints and Invariants

### Real-time Constraints

- Parameter reads must be lock-free (atomic operations only)
- Pan calculations must complete within audio buffer deadline
- Memory allocations prohibited in audio thread

### Audio Quality Constraints

- Total harmonic distortion <0.01% with extreme panning
- Phase coherence maintained across all pan positions
- Mono compatibility preserved (no phase cancellation)

### Performance Constraints

- RGB pan processing adds <2% to existing CPU usage
- Parameter smoothing completes within 10ms
- Memory overhead <100 bytes for pan state storage

### Data Integrity Constraints

- Pan parameters always within valid range [-100.0, +100.0]
- Audio samples never exceed [-1.0, +1.0] output range
- Invalid RGB data produces deterministic silent output
- Parameter changes are atomic (no partial updates)

## Implementation Notes

**Memory Layout**: All pan parameters stored as contiguous atomic floats for cache efficiency

**Numerical Precision**: Float precision sufficient for audio applications (32-bit IEEE 754)

**Error Handling**: Invalid parameters clamped to valid range with error logging

**Testing Strategy**: Unit tests for each entity, integration tests for complete data flow
