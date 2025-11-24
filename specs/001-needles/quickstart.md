# Quickstart Guide: Needles VST Plugin

**Version**: 1.0
**Created**: 2024-11-24
**Audience**: Developers, testers, and end users

## For Developers

### Prerequisites

- **Development Environment**: Visual Studio 2022 (Windows), Xcode 14+ (macOS), or GCC 9+ (Linux)
- **JUCE Framework**: Download JUCE 7.x from https://juce.com/
- **VST3 SDK**: Optional (JUCE includes VST3 headers)
- **Git**: For version control
- **CMake 3.20+**: Alternative build system (optional)

### Development Setup

1. **Clone Repository**

   ```bash
   git clone <repository-url>
   cd NeedlesVST
   ```

2. **Install JUCE Framework**

   - Download JUCE from official website
   - Extract to a known location (e.g., `~/JUCE` or `C:\JUCE`)
   - Note the path for Projucer configuration

3. **Configure Projucer Project**

   ```bash
   # Open JUCE Projucer
   # Load NeedlesVST.jucer file
   # Set JUCE library path in Project Settings
   # Generate platform-specific build files
   ```

4. **Build Plugin**

   **Windows (Visual Studio)**:

   ```cmd
   # Open Builds/VisualStudio2022/NeedlesVST.sln
   # Build in Release configuration
   # Plugin will be in Builds/VisualStudio2022/x64/Release/VST3/
   ```

   **macOS (Xcode)**:

   ```bash
   cd Builds/Xcode
   xcodebuild -configuration Release
   # Plugin will be in ~/Library/Audio/Plug-Ins/VST3/
   ```

   **Linux (Makefile)**:

   ```bash
   cd Builds/LinuxMakefile
   make CONFIG=Release
   # Plugin will be in Builds/LinuxMakefile/build/
   ```

5. **Install for Testing**

   **Windows**: Copy `.vst3` file to `C:\Program Files\Common Files\VST3\`
   **macOS**: Plugin auto-installs to `~/Library/Audio/Plug-Ins/VST3/`
   **Linux**: Copy to `~/.vst3/`

### Running Tests

```bash
# Unit tests
cd Tests
./run_unit_tests.sh

# Integration tests (requires DAW)
./run_integration_tests.sh

# Manual DAW testing
# Load plugin in Reaper, Logic Pro, Cubase, etc.
# Follow test scenarios in Tests/manual_test_plan.md
```

### Development Workflow

1. **Write tests first** (TDD approach per constitution)
2. **Implement minimal code** to pass tests
3. **Refactor** for performance and maintainability
4. **Manual DAW testing** for integration validation
5. **Code review** following constitution quality standards

## For Testers

### Installation

1. **Download Plugin**

   - Get appropriate build for your platform (Windows .vst3, macOS .vst3/.au, Linux .vst3)

2. **Install Plugin**

   - **Windows**: Place in `C:\Program Files\Common Files\VST3\`
   - **macOS**: Place in `/Library/Audio/Plug-Ins/VST3/` or `~/Library/Audio/Plug-Ins/VST3/`
   - **Linux**: Place in `~/.vst3/`

3. **Verify Installation**
   - Open your DAW (Reaper, Logic Pro, Cubase, FL Studio, etc.)
   - Rescan plugins
   - Look for "Needles" in instrument/effect list

### Basic Testing Workflow

1. **Load Plugin**

   ```
   DAW → Insert New Instrument → Needles
   ```

2. **Load Test Image**

   ```
   Plugin UI → Load Image Button → Select JPG/PNG/GIF
   Expected: Image appears, audio starts immediately
   ```

3. **Verify Audio Output**

   ```
   DAW → Check audio meters → Should show stereo output
   Expected: Continuous audio that loops infinitely
   ```

4. **Test Parameter Control**

   ```
   Adjust Scan Speed → Audio tempo changes in real-time
   Adjust Area Size → Audio character changes
   Expected: Immediate response, no audio dropouts
   ```

5. **Test State Persistence**
   ```
   Save DAW project → Close DAW → Reopen project
   Expected: Image and parameters restored correctly
   ```

### Test Scenarios

**Scenario 1: Basic Functionality**

- Load various image formats (JPG, PNG, GIF)
- Verify audio generation starts immediately
- Confirm infinite looping behavior
- Test with different image sizes (small, medium, large)

**Scenario 2: Performance Testing**

- Load 2048x2048 pixel image
- Monitor CPU usage (should be <10%)
- Adjust parameters rapidly
- Verify no audio dropouts or glitches

**Scenario 3: Edge Cases**

- Load corrupted image file
- Load extremely small image (10x10 pixels)
- Set scan speed to minimum/maximum values
- Set area size to minimum/maximum values

**Scenario 4: DAW Integration**

- Test in multiple DAWs (Reaper, Logic, Cubase)
- Verify VST3 automation works
- Test plugin bypass functionality
- Save/load project with plugin

### Expected Behavior

✅ **Correct Behavior**:

- Image loads within 1 second
- Audio starts within 100ms of image loading
- Continuous looping without gaps or clicks
- Real-time parameter response (<50ms)
- Stable CPU usage under 10%
- Clean audio output without artifacts

❌ **Report These Issues**:

- Audio dropouts or glitches
- Crashes when loading images
- Parameter changes cause audio artifacts
- High CPU usage (>15%)
- Plugin fails to load in DAW
- State not preserved between sessions

## For End Users

### Installation

1. **Download** the plugin from the releases page
2. **Install** to your system's VST3 directory:

   - Windows: Usually handled by installer
   - macOS: Drag to Applications or use installer
   - Linux: Follow provided installation script

3. **Restart your DAW** or rescan plugins

### Quick Start

1. **Load the Plugin**

   - Open your DAW
   - Create a new track
   - Add "Needles" as an instrument

2. **Load Your First Image**

   - Click "Load Image" in the plugin
   - Choose any JPG, PNG, or GIF image
   - Audio will start playing immediately!

3. **Experiment with Controls**

   - **Scan Speed**: Controls how fast the plugin reads through your image
   - **Area Size**: Changes the audio character by averaging more/fewer pixels
   - Listen to how your image sounds as music!

4. **Creative Tips**
   - Try different types of images (photos, artwork, textures)
   - Adjust scan speed to match your song's tempo
   - Use the plugin to create ambient textures or rhythmic patterns
   - Combine with effects (reverb, delay, filters) for richer sounds

### Compatible DAWs

✅ **Tested and Supported**:

- Reaper 6.0+
- Logic Pro X/Logic Pro (macOS)
- Cubase 11+
- Studio One 5+
- FL Studio 20+
- Ableton Live 11+

ℹ️ **Should Work** (VST3 compatible):

- Pro Tools 2021+
- Reason 12+
- Bitwig Studio 4+
- Most other modern DAWs with VST3 support

### Troubleshooting

**Plugin doesn't appear in DAW:**

- Verify installation location
- Rescan plugins in your DAW
- Check DAW supports VST3 format

**No audio output:**

- Check DAW's mixer - ensure track isn't muted
- Verify image is loaded (should see image in plugin)
- Check plugin output gain isn't at zero

**Audio glitches:**

- Try smaller image file (<2MB)
- Reduce area size parameter
- Check DAW buffer size settings

**Plugin crashes:**

- Update to latest version
- Try different image file
- Check system requirements

### System Requirements

**Minimum**:

- Windows 10, macOS 10.15, or Linux Ubuntu 20.04
- 4GB RAM
- Dual-core 2.5GHz processor
- VST3 compatible DAW

**Recommended**:

- 8GB+ RAM
- Quad-core 3GHz+ processor
- SSD storage
- Professional audio interface

## Support

- **Documentation**: Full documentation in `/docs` folder
- **Issues**: Report bugs via GitHub issues
- **Community**: Join our Discord/forum for tips and examples
- **Updates**: Check releases page for latest version
