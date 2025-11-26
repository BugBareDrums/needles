/*
 * StereoProcessor.cpp - Stereo field positioning implementation
 * 
 * Implements constant power panning algorithms for RGB channel positioning
 * with optimized real-time audio processing performance.
 */

#include "StereoProcessor.h"
#include <algorithm>

//==============================================================================
std::pair<float, float> StereoProcessor::processPan(float input, float panPosition)
{
    float clampedPan = clampPan(panPosition);
    return constantPowerPan(input, clampedPan);
}

//==============================================================================
std::pair<float, float> StereoProcessor::processRGBChannels(
    float redAudio, float greenAudio, float blueAudio,
    float redPan, float greenPan, float bluePan)
{
    // Process each RGB channel independently
    auto [redLeft, redRight] = processPan(redAudio, redPan);
    auto [greenLeft, greenRight] = processPan(greenAudio, greenPan);
    auto [blueLeft, blueRight] = processPan(blueAudio, bluePan);
    
    // Mix RGB channels to final stereo output
    float leftOutput = redLeft + greenLeft + blueLeft;
    float rightOutput = redRight + greenRight + blueRight;
    
    // Ensure output remains within valid range (prevent clipping)
    leftOutput = std::clamp(leftOutput, -1.0f, 1.0f);
    rightOutput = std::clamp(rightOutput, -1.0f, 1.0f);
    
    return {leftOutput, rightOutput};
}

//==============================================================================
float StereoProcessor::clampPan(float pan)
{
    return std::clamp(pan, -1.0f, 1.0f);
}

//==============================================================================
std::pair<float, float> StereoProcessor::constantPowerPan(float input, float pan)
{
    // Convert pan position to angle for sin/cos calculation
    // Pan range [-1.0, +1.0] maps to angle range [0, π/2]
    float angle = (pan + 1.0f) * PI_4;  // Maps [-1,+1] to [0, π/2]
    
    // Constant power panning using sin/cos law
    // Left channel uses cosine (stronger at left pan positions)
    // Right channel uses sine (stronger at right pan positions)
    float leftGain = std::cos(angle);
    float rightGain = std::sin(angle);
    
    return {input * leftGain, input * rightGain};
}

//==============================================================================
std::unique_ptr<IStereoProcessor> createStereoProcessor()
{
    return std::make_unique<StereoProcessor>();
}