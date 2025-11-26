/*
 * RGBChannelTest.cpp - RGB channel independence integration tests
 * 
 * Validates that RGB channels can be panned independently and maintain
 * proper separation in complete audio processing pipeline.
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../../Source/StereoProcessor.h"
#include "../../Source/AudioSynthesis.h"

using Catch::Matchers::WithinAbs;

//==============================================================================
TEST_CASE("RGB Channel Independence", "[integration][rgb][independence]")
{
    auto stereoProcessor = createStereoProcessor();
    
    SECTION("Each RGB channel processes independently")
    {
        // Create test RGB values representing distinct color content
        RGB testPixel(255, 128, 64); // Red=255, Green=128, Blue=64
        
        // Extract per-channel audio using existing RGB::toAudioChannel method
        float redAudio = testPixel.toAudioChannel(0);    // Red channel only
        float greenAudio = testPixel.toAudioChannel(1);  // Green channel only
        float blueAudio = testPixel.toAudioChannel(2);   // Blue channel only
        
        // Verify channels produce different audio values
        REQUIRE(redAudio != greenAudio);
        REQUIRE(greenAudio != blueAudio);
        REQUIRE(redAudio != blueAudio);
        
        // Test independent panning
        float redPan = -1.0f;   // Hard left
        float greenPan = 0.0f;  // Center  
        float bluePan = +1.0f;  // Hard right
        
        auto [left, right] = stereoProcessor->processRGBChannels(
            redAudio, greenAudio, blueAudio,
            redPan, greenPan, bluePan
        );
        
        // Red (hard left) should contribute more to left channel
        // Blue (hard right) should contribute more to right channel
        // Green (center) should contribute equally to both
        
        REQUIRE(left != right); // Stereo field should be different due to panning
        REQUIRE(left >= -1.0f);
        REQUIRE(left <= 1.0f);
        REQUIRE(right >= -1.0f);
        REQUIRE(right <= 1.0f);
    }
    
    SECTION("RGB channel audio extraction works correctly")
    {
        // Test that RGB struct provides per-channel audio extraction
        RGB redPixel(255, 0, 0);     // Pure red
        RGB greenPixel(0, 255, 0);   // Pure green
        RGB bluePixel(0, 0, 255);    // Pure blue
        
        float redAudio = redPixel.toAudioChannel(0);
        float redFromGreen = greenPixel.toAudioChannel(0);
        float redFromBlue = bluePixel.toAudioChannel(0);
        
        // Red channel audio should be maximum from red pixel
        REQUIRE(redAudio > redFromGreen);
        REQUIRE(redAudio > redFromBlue);
        
        // Similar tests for other channels
        float greenAudio = greenPixel.toAudioChannel(1);
        float greenFromRed = redPixel.toAudioChannel(1);
        
        REQUIRE(greenAudio > greenFromRed);
    }
}

//==============================================================================
TEST_CASE("RGB Channel Mixing Behavior", "[integration][rgb][mixing]")
{
    auto stereoProcessor = createStereoProcessor();
    
    SECTION("Multiple RGB channels mix without artifacts")
    {
        // Test various RGB combinations to ensure stable mixing
        
        struct TestCase {
            RGB pixel;
            float redPan, greenPan, bluePan;
            std::string description;
        };
        
        std::vector<TestCase> testCases = {
            {{255, 255, 255}, 0.0f, 0.0f, 0.0f, "White pixel, all centered"},
            {{255, 0, 0}, -1.0f, 0.0f, 1.0f, "Red left, green center, blue right"},
            {{128, 128, 128}, -0.5f, 0.0f, 0.5f, "Gray pixel with partial panning"},
            {{255, 128, 64}, 0.25f, -0.25f, 0.75f, "Complex color with varied panning"}
        };
        
        for (const auto& test : testCases) {
            INFO("Testing: " << test.description);
            
            float redAudio = test.pixel.toAudioChannel(0);
            float greenAudio = test.pixel.toAudioChannel(1);
            float blueAudio = test.pixel.toAudioChannel(2);
            
            auto [left, right] = stereoProcessor->processRGBChannels(
                redAudio, greenAudio, blueAudio,
                test.redPan, test.greenPan, test.bluePan
            );
            
            // Ensure no clipping or invalid values
            REQUIRE(std::isfinite(left));
            REQUIRE(std::isfinite(right));
            REQUIRE(left >= -1.0f);
            REQUIRE(left <= 1.0f);
            REQUIRE(right >= -1.0f);
            REQUIRE(right <= 1.0f);
            
            // Ensure audio is produced (not silent)
            float totalEnergy = (left * left) + (right * right);
            if (redAudio != 0.0f || greenAudio != 0.0f || blueAudio != 0.0f) {
                REQUIRE(totalEnergy > 0.0001f); // Should produce some audio
            }
        }
    }
}

//==============================================================================
TEST_CASE("Edge Case RGB Processing", "[integration][rgb][edge-cases]")
{
    auto stereoProcessor = createStereoProcessor();
    
    SECTION("Zero audio input produces zero output")
    {
        float redAudio = 0.0f;
        float greenAudio = 0.0f;
        float blueAudio = 0.0f;
        
        auto [left, right] = stereoProcessor->processRGBChannels(
            redAudio, greenAudio, blueAudio,
            -1.0f, 0.0f, 1.0f  // Various pan positions
        );
        
        REQUIRE_THAT(left, WithinAbs(0.0f, 0.0001f));
        REQUIRE_THAT(right, WithinAbs(0.0f, 0.0001f));
    }
    
    SECTION("Single channel active produces expected stereo positioning")
    {
        // Test red channel only, hard left
        auto [left1, right1] = stereoProcessor->processRGBChannels(
            1.0f, 0.0f, 0.0f,  // Red only
            -1.0f, 0.0f, 0.0f  // Red hard left
        );
        
        REQUIRE_THAT(left1, WithinAbs(1.0f, 0.0001f));
        REQUIRE_THAT(right1, WithinAbs(0.0f, 0.0001f));
        
        // Test blue channel only, hard right  
        auto [left2, right2] = stereoProcessor->processRGBChannels(
            0.0f, 0.0f, 1.0f,  // Blue only
            0.0f, 0.0f, 1.0f   // Blue hard right
        );
        
        REQUIRE_THAT(left2, WithinAbs(0.0f, 0.0001f));
        REQUIRE_THAT(right2, WithinAbs(1.0f, 0.0001f));
    }
}