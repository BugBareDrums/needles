/*
 * RGB Channel Parameter Contract
 * 
 * Defines parameter behavior contracts for RGB channel panning
 * Ensures proper DAW automation and UI integration
 */

#pragma once

#include <gtest/gtest.h>
#include "../Source/ParameterManager.h"

namespace Contracts {

/**
 * Contract: Parameter Range Validation
 * RGB pan parameters must maintain valid ranges and proper defaults
 */
class RGBChannelParameterContract : public ::testing::Test
{
protected:
    void SetUp() override 
    {
        paramManager = createParameterManager();
    }
    
    std::unique_ptr<IParameterManager> paramManager;
};

TEST_F(RGBChannelParameterContract, InitializesWithCenterDefaults)
{
    // Contract: All RGB pan parameters default to center (0.0)
    EXPECT_FLOAT_EQ(paramManager->getRedPan(), 0.0f) 
        << "Red pan must default to center position";
    EXPECT_FLOAT_EQ(paramManager->getGreenPan(), 0.0f) 
        << "Green pan must default to center position";
    EXPECT_FLOAT_EQ(paramManager->getBluePan(), 0.0f) 
        << "Blue pan must default to center position";
}

TEST_F(RGBChannelParameterContract, AcceptsValidRangeValues)
{
    // Contract: Parameters accept full valid range [-100.0, +100.0]
    
    // Test extreme values
    paramManager->setRedPan(-100.0f);
    paramManager->setGreenPan(+100.0f);
    paramManager->setBluePan(0.0f);
    
    EXPECT_FLOAT_EQ(paramManager->getRedPan(), -100.0f);
    EXPECT_FLOAT_EQ(paramManager->getGreenPan(), +100.0f);
    EXPECT_FLOAT_EQ(paramManager->getBluePan(), 0.0f);
    
    // Test intermediate values
    paramManager->setRedPan(-50.5f);
    paramManager->setGreenPan(25.8f);
    paramManager->setBluePan(-33.3f);
    
    EXPECT_FLOAT_EQ(paramManager->getRedPan(), -50.5f);
    EXPECT_FLOAT_EQ(paramManager->getGreenPan(), 25.8f);
    EXPECT_FLOAT_EQ(paramManager->getBluePan(), -33.3f);
}

TEST_F(RGBChannelParameterContract, ClampsOutOfRangeValues)
{
    // Contract: Out-of-range values are clamped to valid bounds
    
    paramManager->setRedPan(-150.0f);   // Below minimum
    paramManager->setGreenPan(200.0f);  // Above maximum
    
    EXPECT_FLOAT_EQ(paramManager->getRedPan(), -100.0f) 
        << "Values below -100.0 must clamp to -100.0";
    EXPECT_FLOAT_EQ(paramManager->getGreenPan(), 100.0f) 
        << "Values above +100.0 must clamp to +100.0";
}

/**
 * Contract: Thread Safety
 * Parameter access must be thread-safe for real-time audio
 */
TEST_F(RGBChannelParameterContract, SupportsThreadSafeAccess)
{
    // Contract: Simultaneous read/write operations don't cause data races
    
    std::atomic<bool> testComplete{false};
    std::atomic<int> readErrors{0};
    
    // Writer thread (simulates UI thread)
    std::thread writer([this, &testComplete]() {
        float value = -100.0f;
        while (!testComplete) {
            paramManager->setRedPan(value);
            paramManager->setGreenPan(value + 50.0f);
            paramManager->setBluePan(value + 100.0f);
            value += 1.0f;
            if (value > 100.0f) value = -100.0f;
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });
    
    // Reader thread (simulates audio thread)
    std::thread reader([this, &testComplete, &readErrors]() {
        while (!testComplete) {
            float red = paramManager->getRedPan();
            float green = paramManager->getGreenPan();
            float blue = paramManager->getBluePan();
            
            // Values should always be in valid range
            if (red < -100.0f || red > 100.0f ||
                green < -100.0f || green > 100.0f ||
                blue < -100.0f || blue > 100.0f) {
                readErrors++;
            }
        }
    });
    
    // Run test for 100ms
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    testComplete = true;
    
    writer.join();
    reader.join();
    
    EXPECT_EQ(readErrors.load(), 0) 
        << "Thread-safe access must never return invalid values";
}

/**
 * Contract: Parameter Smoothing
 * Parameter changes must be smoothed to prevent audio artifacts
 */
TEST_F(RGBChannelParameterContract, SmoothsParameterChanges)
{
    // Contract: Large parameter jumps are smoothed over time
    
    paramManager->setRedPan(-100.0f);
    float initialValue = paramManager->getRedPan();
    
    // Simulate large parameter change
    paramManager->setRedPan(+100.0f);
    
    // During smoothing period, values should transition gradually
    bool foundIntermediateValue = false;
    for (int i = 0; i < 1000; ++i) {  // Simulate audio buffer processing
        float currentValue = paramManager->getRedPan();
        if (currentValue > initialValue && currentValue < 100.0f) {
            foundIntermediateValue = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    
    EXPECT_TRUE(foundIntermediateValue) 
        << "Parameter changes must be smoothed, not instantaneous";
}

} // namespace Contracts