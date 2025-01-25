## 1. Product Overview

Product Name: Needles

Description: Needles is a minimalistic VST and standalone synthesizer that converts images into audio signals. It processes pixel data, scanning through an image and mapping color values to audio.

Target Users:

Musicians and producers looking for experimental or generative sound-design tools.
Sound designers and media artists who want a unique way to blend visuals with audio.

## 2. Goals & Objectives

Create a Novel Sound-Design Tool

Leverage image data to generate interesting and unconventional timbres.
Provide immediate and intuitive results with minimal user setup.
Deliver a Smooth User Experience

Simple loading of images (standard file formats: PNG, JPEG, BMP).
Minimal UI, focused on essential parameters to guide the synthesis.
Flexible Parameter Control

Enable users to adjust scan speed, pixel averaging size, and the way RGB values map to audio.
Support Multiple Formats

Provide both a standalone desktop application (Mac/Windows) and a VST plugin (VST3, AU if possible), all built with JUCE.

## 3. Product Scope

The scope covers:

Image loading and display within the plugin/standalone app.
A scanning engine that traverses the image data.
Audio rendering that maps pixel color data to audio waveforms/signals, respecting user-defined parameters.
A minimal UI to configure key parameters and visualize the loaded image.

Out-of-scope (future enhancements may include but are not part of the initial release):

Advanced scanning patterns or shape-based scanning (beyond the default snake-like horizontal pass).
Real-time image manipulation tools (e.g., image filters, color correction).
Advanced modulation or effects beyond the core image-to-audio mapping.

## 4. Features & Functional Requirements

This section details the main features and functional requirements of Needles.

### 4.1 Image Loading

Supported File Formats: At minimum, PNG, JPEG, and BMP.
File Browser Integration: Provide a button (or drag-and-drop region) to load files from the user’s system.
Image Display:
Display the loaded image on the right half of the UI.
If no image is loaded, show a placeholder or “No image loaded” message.

## 4.2 Audio Generation (Synthesis Method)

Scanning Path

Default Path: Start from the top-left, scan left-to-right; then next row scans right-to-left, creating a “zig-zag” or “snake-like” pattern down the image.
Requirement: Implementation should be structured to allow additional paths in future (e.g., purely left-to-right for all rows, or diagonal scan, etc.).
Pixel Color to Audio Mapping

Color Sampling
At any scan position, read the pixel’s R, G, and B values.
User Parameter: “Area Size” – an integer specifying the neighborhood around the current scan point that gets averaged (e.g., 1 = single pixel, 3 = 3x3, etc.).
Compute average R, G, and B from this neighborhood.
Pixel to Audio Formula
Default formula: (R + G + B) / 3, scaled to a suitable audio range (e.g., -1.0 to +1.0).
Requirement: Provide a user-accessible multiplier or scaling factor to adjust the amplitude range.
Stereo Handling: By default, output the same signal to Left and Right channels.
Possible Future Extension: Offer separate mappings for R→Left, G→Right, B→(some transformation) for more complex stereo imaging.
Scan Speed & Playback

Scan Speed Parameter: Controls how quickly the scan position moves through the image:
Range could be from 1 pixel/second up to the entire image in less than a second.
Implementation in audio callback should ensure stable real-time playback without stutters.
Once the end of the image is reached (bottom row), the audio can either stop or loop from the top again.
Looping Option: User can enable or disable looping in advanced preferences.

### 4.3 User Interface & Controls

The UI is divided into two main sections:

#### 4.3.1 Left Section – Parameter Panel

Scan Speed

A slider or rotary dial for controlling how quickly the scan moves through the image.
Display the current numeric value (e.g., “50 px/second”).
Minimum/Maximum values to be determined through testing (e.g., 1 px/sec to 10,000 px/sec).
Area Size

A slider or step-based selector for the size of the averaging area (e.g., 1, 3, 5, … up to a max that is feasible for real-time).
Display the selected size (e.g., 1→1x1, 3→3x3 neighborhood, etc.).
Intensity/Scaling

A slider for controlling the amplitude scaling of the RGB average.
Possibly displayed in decibels or a 0–100% scale.
Show numeric readout or a simple “-inf to +6 dB” range, for example.
Loop Toggle (Optional for v1.0, or can be in advanced settings)

A button or switch that, when enabled, restarts scanning from the top of the image once reaching the bottom.
Audio Output Level

A small VU meter or output meter to monitor the real-time level.
Load Image Button

Prominent button to open file browser. Alternatively, a drag-and-drop area on the interface.

#### 4.3.2 Right Section – Image Display

Loaded Image View

Show the user-loaded image in its original aspect ratio.
Possibly highlight the current scan position in real-time (if performance allows).
No Image Placeholder
If no image is loaded, display a neutral placeholder or instructions.

## 5. Workflow & User Experience

Launch the Plugin/Standalone
User sees a clean UI with parameter sliders on the left, empty placeholder on the right.
Load Image
User clicks “Load Image” button or drags a file onto the UI.
The chosen image is displayed on the right.
Configure Parameters
User adjusts scan speed, area size, scaling, etc., to shape the resulting sound.
Audio Playback
In a DAW (VST version), the plugin will receive transport and buffer calls normally. As scanning proceeds, audio is output in real time.
In standalone mode, user may have a “Play”/“Stop” button or the scanning starts automatically upon image load.

Monitor Output

User observes the VU meter for level, modifies scaling/area size to prevent distortion or to achieve certain sonic textures.

Loop or Stop
At the bottom of the image, if loop is enabled, scanning restarts from top; if loop is disabled, playback ends.

## 6. Non-Functional Requirements

### Performance

Must run in real-time without audio dropouts.
Efficiently handle images up to a certain resolution (e.g., 4096 × 4096) without significant latency or CPU spikes.

### Stability

Prevent crashes when loading invalid or corrupted image files.
Provide clear error handling and user feedback if a file cannot be opened.

### Cross-Platform Compatibility

Must run on macOS and Windows, supporting the latest OS versions feasible.
Provide both standalone and VST (VST3) plugin binaries.

### Scalability

The scanning engine should allow for alternative or more complex scanning algorithms in future.

### Security

No external connections or network usage required.
Handle image processing locally and securely.

## 7. Technical Implementation Notes

### JUCE Framework

Use the JUCE AudioPlugin framework for shared code between standalone and VST.
Leverage JUCE’s image loading classes (juce::ImageFileFormat) for file import.
Use JUCE’s AudioProcessor & AudioProcessorEditor classes to manage audio generation and UI.

### Real-Time Audio

The scanning logic will be called within the audio callback.
Carefully manage image scanning increments per audio buffer to ensure accurate playback speed.
Consider using a FIFO or ring buffer system for large images to avoid excessive real-time memory access overhead.

### Parameter Handling

Expose adjustable parameters as AudioProcessorValueTreeState or an equivalent structure to ensure DAW automation is possible for scan speed, area size, scaling, etc.
Make sure parameter changes do not cause abrupt audio artifacts.
UI Rendering

The right-half image display will be drawn in the Editor’s paint routine.
If real-time scanning position feedback is required, track the current line/pixel in an atomic or thread-safe manner to avoid race conditions.
Testing & Validation

Test across multiple OS/hardware configurations.
Validate performance with large vs. small images.
Check for memory leaks or excessive CPU usage. 8. Milestones & Deliverables
Alpha Version

Basic image loading and scanning.
Parameter changes reflected in audio (scan speed, area size, scaling).
Minimal UI with no final polish.
Beta Version

Full UI layout with final design aesthetic.
Stabilized scanning engine, with optional real-time position indicator.
VST and standalone builds tested.
Release Candidate (RC)

Bug fixes from Beta, performance optimizations.
Cross-platform testing completed (macOS, Windows).
Documentation of features and user guide drafted.
Version 1.0 Release

Final UI/UX pass, branding elements.
Distribution packaging (signed installers on Windows, macOS notarization). 9. Risks & Assumptions
Performance Bottlenecks

Large images or complex scanning algorithms could degrade performance.
Mitigation: Use efficient data structures and possibly limit maximum resolution or implement downscaling options.
Audio Quality

Generating audio directly from pixels could be harsh or unpredictable.
Mitigation: Provide a scaling or smoothing parameter to reduce abrupt changes. Possibly integrate a simple filter if necessary.
UI Complexity

If real-time scanning position is displayed over the image, the added overhead might disrupt audio.
Mitigation: Provide a toggle for real-time overlay or keep the scanning indicator minimal.
Dependency on JUCE and Third-Party Libraries

Must keep up to date with JUCE releases for OS compatibility.
Mitigation: Regular updates and testing in the development cycle. 10. Conclusion & Next Steps
Needles promises a unique approach to sound design by bridging the gap between visual media and audio generation. The initial implementation will focus on a minimal, reliable core feature set (zig-zag scanning, pixel averaging, basic color-to-audio mapping). Once this foundation is solid, more sophisticated scanning patterns and advanced stereo or multichannel output options could be considered for future updates.

Immediate Next Steps

Finalize UI mockups and parameter ranges.
Implement core scanning engine and basic color-to-audio conversion.
Develop test harness and QA process for image loading and real-time audio output.
Iterate on performance optimizations before Beta release.
Appendix: Parameter & UI Mockup (Example)
Parameter Panel (Left)

Scan Speed: Slider (1–10,000 px/sec)
Area Size: Dropdown/Slider (1, 3, 5, 7, 9)
Intensity/Scaling: Slider (e.g., -inf to +6 dB or 0–200%)
Loop: Toggle (On/Off)
Load Image: Button
Image Display (Right)

Main area displays the loaded image.
(Optional) A thin horizontal line or highlight shows the current scanning row.
