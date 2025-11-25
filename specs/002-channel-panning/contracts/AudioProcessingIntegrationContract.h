/*
 * Audio Processing Integration Contract
 * 
 * End-to-end contract for RGB channel panning in complete audio pipeline
 * Validates integration between components
 */

#pragma once

#include <gtest/gtest.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "../Source/PluginProcessor.h"
#include "../Source/ImageLoader.h"

namespace Contracts {

/**
 * Contract: Complete Audio Pipeline
 * RGB panning must integrate correctly with existing audio synthesis
 */
class AudioProcessingIntegrationContract : public ::testing::Test
{
protected:
    void SetUp() override 
    {
        processor = std::make_unique<NeedlesAudioProcessor>();
        processor->prepareToPlay(44100.0, 512);
        
        // Create test image with distinct RGB content
        createTestImage();
        processor->loadImage(testImagePath);
    }
    
    void TearDown() override 
    {
        processor->releaseResources();
    }
    
    void createTestImage() 
    {
        // Create 10x10 test image with distinct RGB channels
        // Red area: top-left (255,0,0)
        // Green area: top-right (0,255,0)  
        // Blue area: bottom (0,0,255)
        // Implementation details for test image creation...
    }
    
    std::unique_ptr<NeedlesAudioProcessor> processor;
    std::string testImagePath = "/tmp/test_rgb_image.png";
};

TEST_F(AudioProcessingIntegrationContract, ProcessesRGBChannelsIndependently)
{
    // Contract: Each RGB channel can be panned independently
    
    // Set distinct pan positions for each channel
    processor->setParameter("redPan", -100.0f);    // Hard left
    processor->setParameter("greenPan", 0.0f);     // Center
    processor->setParameter("bluePan", 100.0f);    // Hard right
    
    // Create audio buffer for processing
    juce::AudioBuffer<float> buffer(2, 512);  // Stereo, 512 samples
    buffer.clear();
    
    // Process audio with scanning in red area
    processor->setParameter("scanX", 0.2f);  // Left side (red area)
    processor->setParameter("scanY", 0.2f);  // Top (red area)
    
    juce::MidiBuffer midiBuffer;
    processor->processBlock(buffer, midiBuffer);
    
    // Red channel at hard left should produce left channel audio only
    float leftRMS = buffer.getRMSLevel(0, 0, 512);
    float rightRMS = buffer.getRMSLevel(1, 0, 512);
    
    EXPECT_GT(leftRMS, 0.01f) << "Left channel must have audio from red channel";
    EXPECT_LT(rightRMS, 0.001f) << "Right channel must be silent for hard left red pan";
}

TEST_F(AudioProcessingIntegrationContract, MaintainsAudioQualityWithPanning)
{
    // Contract: Panning must not introduce distortion or artifacts
    
    processor->setParameter("redPan", 50.0f);
    processor->setParameter("greenPan", -25.0f);
    processor->setParameter("bluePan", 75.0f);
    
    juce::AudioBuffer<float> buffer(2, 512);
    buffer.clear();
    
    // Process multiple buffers
    juce::MidiBuffer midiBuffer;
    for (int i = 0; i < 10; ++i) {
        processor->processBlock(buffer, midiBuffer);
        
        // Check for clipping or NaN values
        for (int channel = 0; channel < 2; ++channel) {
            for (int sample = 0; sample < 512; ++sample) {
                float value = buffer.getSample(channel, sample);
                
                EXPECT_FALSE(std::isnan(value)) 
                    << "Audio processing must not produce NaN values";
                EXPECT_GE(value, -1.0f) 
                    << "Audio must not clip below -1.0";
                EXPECT_LE(value, 1.0f) 
                    << "Audio must not clip above +1.0";
            }
        }
    }
}

TEST_F(AudioProcessingIntegrationContract, RespondsToParameterAutomation)
{
    // Contract: Parameter changes must be reflected in audio output within 50ms
    
    juce::AudioBuffer<float> buffer(2, 512);
    juce::MidiBuffer midiBuffer;
    
    // Start with center pan
    processor->setParameter("redPan", 0.0f);
    processor->processBlock(buffer, midiBuffer);
    
    float initialLeftLevel = buffer.getRMSLevel(0, 0, 512);
    float initialRightLevel = buffer.getRMSLevel(1, 0, 512);
    
    // Change to hard left pan
    processor->setParameter("redPan", -100.0f);
    
    // Process enough buffers to cover 50ms at 44.1kHz
    // 50ms = 2205 samples = ~5 buffers of 512 samples
    bool foundChange = false;
    for (int i = 0; i < 5 && !foundChange; ++i) {
        buffer.clear();
        processor->processBlock(buffer, midiBuffer);
        
        float currentLeftLevel = buffer.getRMSLevel(0, 0, 512);
        float currentRightLevel = buffer.getRMSLevel(1, 0, 512);
        
        // Look for significant change indicating parameter response
        if (std::abs(currentLeftLevel - initialLeftLevel) > 0.01f ||
            std::abs(currentRightLevel - initialRightLevel) > 0.01f) {
            foundChange = true;
        }
    }
    
    EXPECT_TRUE(foundChange) 
        << "Parameter automation must be reflected in audio within 50ms";
}

TEST_F(AudioProcessingIntegrationContract, MaintainsPerformanceWithAllChannelsPanned)
{
    // Contract: CPU usage must stay within targets when all channels are panned
    
    // Set all channels to different positions
    processor->setParameter("redPan", -75.0f);
    processor->setParameter("greenPan", 25.0f);
    processor->setParameter("bluePan", 90.0f);
    
    juce::AudioBuffer<float> buffer(2, 512);
    juce::MidiBuffer midiBuffer;
    
    // Measure processing time
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Process many buffers to get reliable timing
    for (int i = 0; i < 1000; ++i) {
        buffer.clear();
        processor->processBlock(buffer, midiBuffer);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        endTime - startTime);
    
    // Calculate CPU usage percentage
    // 1000 buffers * 512 samples = 512,000 samples
    // At 44.1kHz: 512,000 samples = ~11.6 seconds of audio
    double audioTimeSeconds = 512000.0 / 44100.0;
    double processingTimeSeconds = duration.count() / 1000000.0;
    double cpuUsagePercent = (processingTimeSeconds / audioTimeSeconds) * 100.0;
    
    EXPECT_LT(cpuUsagePercent, 10.0) 
        << "CPU usage with RGB panning must remain under 10%: " 
        << cpuUsagePercent << "%";
}

} // namespace Contracts