# Needles VST Plugin

Needles is an VST audio plugin that turns image pixels information directly into sound, based on a prototype I made a long time ago, example sound here: https://audiocookbook.org/more-image-to-sound-techniques-mould-by-nick-froud/.

It's also an excuse to play with spec-driven development. All code here is AI generated.

AI generated after this point.

**Image-to-Audio VST Plugin** - Convert images into continuous audio through configurable scanning patterns and RGB-to-audio synthesis.

## Current Status

✅ **Foundation Complete** - Architecture, interfaces, and tests ready  
🚧 **User Story 1** - MVP implementation starting (instant audio from image loading)

See [PROGRESS_REPORT.md](PROGRESS_REPORT.md) for detailed implementation status.

## Quick Start

### For Developers

1. **Prerequisites**

   - JUCE 7.x framework
   - C++17 compatible compiler
   - Visual Studio 2022 (Windows) / Xcode 14+ (macOS) / GCC 9+ (Linux)

2. **Build Setup**

   ```bash
   # Open in JUCE Projucer
   open NeedlesVST/NeedlesVST.jucer

   # Or use CMake
   cd NeedlesVST
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make
   ```

3. **Run Tests**

   ```bash
   cd NeedlesVST/Tests
   ./setup_catch2.sh
   # Build and run tests in your IDE or build system
   ```

4. **Code Quality**
   ```bash
   cd NeedlesVST
   ./check_code_quality.sh
   ```

### Architecture Overview

```text
Image Loading → Pixel Scanning → RGB-to-Audio → Real-time Output
     ↓              ↓              ↓              ↓
 ImageLoader → ImageScanner → AudioSynthesis → PluginProcessor
```

**Core Features**:

- **Instant Audio**: Immediate playback upon image loading
- **Infinite Looping**: Continuous audio generation without gaps
- **Real-time Parameters**: Adjustable scan speed and area size
- **Advanced Synthesis**: Multiple RGB conversion algorithms
- **DAW Integration**: Full VST3/AU plugin compatibility

## Development Approach

This project follows **spec-driven development** with TDD methodology:

1. **Constitution** → Quality principles and standards
2. **Specification** → User stories and requirements
3. **Planning** → Technical architecture and file structure
4. **Tasks** → 70 actionable implementation steps
5. **Implementation** → Test-first development

**Current Phase**: User Story 1 implementation (Tasks T024-T034)

## User Stories

### 🎯 User Story 1 (P1) - MVP

> "As a music producer, I want to load an image file and immediately hear continuous audio, so I can quickly explore the sonic character of visual content."

**Status**: Tests written ✅ | Implementation starting 🚧

### User Story 2 (P2) - Real-time Control

> "As a music producer, I want to adjust scan speed and area size parameters in real-time while hearing immediate audio changes."

**Status**: Planned 📋

### User Story 3 (P3) - Advanced Synthesis

> "As a music producer, I want to select different RGB conversion formulas and scanning patterns to explore various sonic interpretations."

**Status**: Planned 📋

## Contributing

1. Follow TDD: Write failing tests first, then implement
2. Maintain constitution compliance (code quality, performance, UX)
3. Each user story should be independently testable
4. Run quality checks before committing

## Technical Stack

- **Framework**: JUCE 7.x (C++17)
- **Plugin Format**: VST3 (primary), AU, AAX
- **Testing**: Catch2 with custom JUCE helpers
- **Build**: Projucer + CMake alternative
- **Quality**: clang-format + cppcheck integration

---

**Project Vision**: Transform images into expressive musical content through innovative real-time audio synthesis, providing musicians with a new creative tool for sonic exploration.
