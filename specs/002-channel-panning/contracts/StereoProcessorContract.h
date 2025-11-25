/*
 * StereoProcessor Interface Contract
 * 
 * Defines the contract for stereo field positioning and RGB channel mixing
 * Used for TDD development and component testing
 */

#pragma once

#include <gtest/gtest.h>
#include "../Source/StereoProcessor.h"

namespace Contracts {

/**
 * Contract: Pan Range Validation
 * All pan values must be processed correctly across full range
 */
class StereoProcessorPanRangeContract : public ::testing::Test
{
protected:
    void SetUp() override 
    {
        processor = std::make_unique<StereoProcessor>();
    }
    
    std::unique_ptr<StereoProcessor> processor;
};

TEST_F(StereoProcessorPanRangeContract, ProcessesHardLeftPanCorrectly)
{
    // Contract: Hard left pan (-1.0) produces left channel only
    float inputSample = 0.5f;
    auto [left, right] = processor->processPan(inputSample, -1.0f);
    
    EXPECT_FLOAT_EQ(left, inputSample) << "Hard left pan must route to left channel";
    EXPECT_FLOAT_EQ(right, 0.0f) << "Hard left pan must silence right channel";
}

TEST_F(StereoProcessorPanRangeContract, ProcessesHardRightPanCorrectly)
{
    // Contract: Hard right pan (+1.0) produces right channel only  
    float inputSample = 0.5f;
    auto [left, right] = processor->processPan(inputSample, +1.0f);
    
    EXPECT_FLOAT_EQ(left, 0.0f) << "Hard right pan must silence left channel";
    EXPECT_FLOAT_EQ(right, inputSample) << "Hard right pan must route to right channel";
}

TEST_F(StereoProcessorPanRangeContract, ProcessesCenterPanCorrectly)
{
    // Contract: Center pan (0.0) produces equal levels on both channels
    float inputSample = 1.0f;
    auto [left, right] = processor->processPan(inputSample, 0.0f);
    
    // Constant power law: center = √2/2 ≈ 0.7071
    float expectedLevel = inputSample * 0.7071067811865476f;
    EXPECT_NEAR(left, expectedLevel, 0.0001f) << "Center pan must use constant power law";
    EXPECT_NEAR(right, expectedLevel, 0.0001f) << "Center pan must produce equal levels";
}

/**
 * Contract: Energy Conservation
 * Total energy must be preserved across all pan positions
 */
TEST_F(StereoProcessorPanRangeContract, PreservesEnergyAcrossPanRange)
{
    float inputSample = 1.0f;
    float tolerance = 0.001f;
    
    // Test energy conservation across pan range
    for (float pan = -1.0f; pan <= 1.0f; pan += 0.1f) 
    {
        auto [left, right] = processor->processPan(inputSample, pan);
        float totalEnergy = (left * left) + (right * right);
        float inputEnergy = inputSample * inputSample;
        
        EXPECT_NEAR(totalEnergy, inputEnergy, tolerance) 
            << "Energy not preserved at pan position: " << pan;
    }
}

/**
 * Contract: Parameter Range Handling
 * Out-of-range parameters must be handled safely
 */
TEST_F(StereoProcessorPanRangeContract, HandlesOutOfRangeParameters)
{
    float inputSample = 0.5f;
    
    // Test extreme values beyond normal range
    auto [left1, right1] = processor->processPan(inputSample, -2.0f);
    auto [left2, right2] = processor->processPan(inputSample, -1.0f);
    
    EXPECT_FLOAT_EQ(left1, left2) << "Values below -1.0 must clamp to -1.0";
    EXPECT_FLOAT_EQ(right1, right2) << "Values below -1.0 must clamp to -1.0";
    
    auto [left3, right3] = processor->processPan(inputSample, +2.0f);
    auto [left4, right4] = processor->processPan(inputSample, +1.0f);
    
    EXPECT_FLOAT_EQ(left3, left4) << "Values above +1.0 must clamp to +1.0";
    EXPECT_FLOAT_EQ(right3, right4) << "Values above +1.0 must clamp to +1.0";
}

} // namespace Contracts