/*
 * StereoProcessorTest.cpp - Unit tests for StereoProcessor
 * 
 * Validates constant power panning algorithms, parameter range handling,
 * and RGB channel processing according to the contract specifications.
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../../Source/StereoProcessor.h"

using Catch::Matchers::WithinAbs;

//==============================================================================
TEST_CASE("StereoProcessor Pan Range Validation", "[stereo][panning][contract]")
{
    auto processor = createStereoProcessor();
    
    SECTION("Hard left pan produces left channel only")
    {
        float inputSample = 0.5f;
        auto [left, right] = processor->processPan(inputSample, -1.0f);
        
        REQUIRE_THAT(left, WithinAbs(inputSample, 0.0001f));
        REQUIRE_THAT(right, WithinAbs(0.0f, 0.0001f));
    }
    
    SECTION("Hard right pan produces right channel only")
    {
        float inputSample = 0.5f;
        auto [left, right] = processor->processPan(inputSample, +1.0f);
        
        REQUIRE_THAT(left, WithinAbs(0.0f, 0.0001f));
        REQUIRE_THAT(right, WithinAbs(inputSample, 0.0001f));
    }
    
    SECTION("Center pan produces equal levels using constant power law")
    {
        float inputSample = 1.0f;
        auto [left, right] = processor->processPan(inputSample, 0.0f);
        
        // Constant power law: center = √2/2 ≈ 0.7071067811865476f
        float expectedLevel = inputSample * 0.7071067811865476f;
        REQUIRE_THAT(left, WithinAbs(expectedLevel, 0.0001f));
        REQUIRE_THAT(right, WithinAbs(expectedLevel, 0.0001f));
    }
}

//==============================================================================
TEST_CASE("Energy Conservation Across Pan Range", "[stereo][panning][energy]")
{
    auto processor = createStereoProcessor();
    float inputSample = 1.0f;
    float tolerance = 0.001f;
    
    SECTION("Total energy is preserved across pan positions")
    {
        for (float pan = -1.0f; pan <= 1.0f; pan += 0.1f) 
        {
            auto [left, right] = processor->processPan(inputSample, pan);
            float totalEnergy = (left * left) + (right * right);
            float inputEnergy = inputSample * inputSample;
            
            REQUIRE_THAT(totalEnergy, WithinAbs(inputEnergy, tolerance));
        }
    }
}

//==============================================================================
TEST_CASE("Out-of-Range Parameter Handling", "[stereo][panning][validation]")
{
    auto processor = createStereoProcessor();
    float inputSample = 0.5f;
    
    SECTION("Values below -1.0 clamp to -1.0")
    {
        auto [left1, right1] = processor->processPan(inputSample, -2.0f);
        auto [left2, right2] = processor->processPan(inputSample, -1.0f);
        
        REQUIRE_THAT(left1, WithinAbs(left2, 0.0001f));
        REQUIRE_THAT(right1, WithinAbs(right2, 0.0001f));
    }
    
    SECTION("Values above +1.0 clamp to +1.0")
    {
        auto [left1, right1] = processor->processPan(inputSample, +2.0f);
        auto [left2, right2] = processor->processPan(inputSample, +1.0f);
        
        REQUIRE_THAT(left1, WithinAbs(left2, 0.0001f));
        REQUIRE_THAT(right1, WithinAbs(right2, 0.0001f));
    }
}

//==============================================================================
TEST_CASE("RGB Channel Processing", "[stereo][rgb][channels]")
{
    auto processor = createStereoProcessor();
    
    SECTION("RGB channels are processed independently")
    {
        float redAudio = 0.5f;
        float greenAudio = 0.3f;
        float blueAudio = 0.2f;
        
        float redPan = -1.0f;   // Hard left
        float greenPan = 0.0f;  // Center
        float bluePan = +1.0f;  // Hard right
        
        auto [left, right] = processor->processRGBChannels(
            redAudio, greenAudio, blueAudio,
            redPan, greenPan, bluePan
        );
        
        // Red should contribute only to left channel
        REQUIRE(left > right); // Left should be louder due to red contribution
        
        // Verify mix doesn't exceed valid range
        REQUIRE(left >= -1.0f);
        REQUIRE(left <= 1.0f);
        REQUIRE(right >= -1.0f);
        REQUIRE(right <= 1.0f);
    }
    
    SECTION("Mixed RGB channels stay within valid output range")
    {
        // Test with extreme values that could cause clipping
        float maxAudio = 1.0f;
        
        auto [left, right] = processor->processRGBChannels(
            maxAudio, maxAudio, maxAudio,
            0.0f, 0.0f, 0.0f  // All centered
        );
        
        // Output should be clamped to valid range
        REQUIRE(left >= -1.0f);
        REQUIRE(left <= 1.0f);
        REQUIRE(right >= -1.0f);
        REQUIRE(right <= 1.0f);
    }
}