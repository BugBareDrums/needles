/*
 * StereoProcessor.h - Stereo field positioning and RGB channel mixing
 * 
 * Handles constant power panning for individual RGB channels with
 * real-time audio processing constraints and sample-accurate timing.
 */

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <utility>
#include <cmath>
#include <memory>

//==============================================================================
/**
 * Stereo processing interface for RGB channel panning
 * 
 * Provides constant power panning algorithms for positioning individual 
 * RGB color channels across the stereo field while maintaining phase
 * coherence and energy conservation.
 */
class IStereoProcessor
{
public:
    virtual ~IStereoProcessor() = default;
    
    /**
     * Process single audio sample with stereo positioning
     * @param input Audio sample in range [-1.0, +1.0]
     * @param panPosition Pan position: -1.0 (hard left) to +1.0 (hard right)
     * @return Pair of (left, right) stereo samples
     */
    virtual std::pair<float, float> processPan(float input, float panPosition) = 0;
    
    /**
     * Process RGB channels with independent pan positions
     * @param redAudio Red channel audio sample
     * @param greenAudio Green channel audio sample  
     * @param blueAudio Blue channel audio sample
     * @param redPan Red channel pan position [-1.0, +1.0]
     * @param greenPan Green channel pan position [-1.0, +1.0]
     * @param bluePan Blue channel pan position [-1.0, +1.0]
     * @return Pair of (left, right) mixed stereo samples
     */
    virtual std::pair<float, float> processRGBChannels(
        float redAudio, float greenAudio, float blueAudio,
        float redPan, float greenPan, float bluePan) = 0;
};

//==============================================================================
/**
 * Constant power stereo processor implementation
 * 
 * Implements industry-standard constant power panning using sin/cos law
 * for professional audio production. Maintains constant perceived loudness
 * across all pan positions and preserves mono compatibility.
 */
class StereoProcessor : public IStereoProcessor
{
public:
    StereoProcessor() = default;
    ~StereoProcessor() override = default;
    
    // IStereoProcessor interface
    std::pair<float, float> processPan(float input, float panPosition) override;
    std::pair<float, float> processRGBChannels(
        float redAudio, float greenAudio, float blueAudio,
        float redPan, float greenPan, float bluePan) override;
    
private:
    /**
     * Clamp pan position to valid range [-1.0, +1.0]
     * @param pan Input pan position
     * @return Clamped pan position
     */
    static float clampPan(float pan);
    
    /**
     * Apply constant power panning algorithm
     * @param input Audio sample
     * @param pan Normalized pan position [-1.0, +1.0] 
     * @return Pair of (left, right) samples using sin/cos law
     */
    static std::pair<float, float> constantPowerPan(float input, float pan);
    
    // Constants for constant power panning
    static constexpr float PI_4 = 0.7853981633974483f;  // π/4
    static constexpr float SQRT_2_2 = 0.7071067811865476f;  // √2/2 (center level)
};

//==============================================================================
/**
 * Factory function to create StereoProcessor instance
 * @return Unique pointer to IStereoProcessor implementation
 */
std::unique_ptr<IStereoProcessor> createStereoProcessor();