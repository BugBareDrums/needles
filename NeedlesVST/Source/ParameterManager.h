#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <atomic>

//==============================================================================
/**
 * Parameter manager interface for thread-safe parameter access
 */
class IParameterManager
{
public:
    virtual ~IParameterManager() = default;
    
    /**
     * Get scan speed parameter value
     * @return Speed multiplier (0.1 to 10.0)
     */
    virtual float getScanSpeed() const = 0;
    
    /**
     * Get area size parameter value
     * @return Area size in pixels (1 to 50)
     */
    virtual int getAreaSize() const = 0;
    
    /**
     * Get output gain parameter value
     * @return Gain multiplier (0.0 to 2.0)
     */
    virtual float getOutputGain() const = 0;
    
    /**
     * Get left channel weight
     * @return Weight value (0.0 to 1.0)
     */
    virtual float getLeftChannelWeight() const = 0;
    
    /**
     * Get right channel weight
     * @return Weight value (0.0 to 1.0)
     */
    virtual float getRightChannelWeight() const = 0;
    
    /**
     * Get red channel pan position
     * @return Pan position (-100.0 to +100.0, 0.0 = center)
     */
    virtual float getRedPan() const = 0;
    
    /**
     * Get green channel pan position
     * @return Pan position (-100.0 to +100.0, 0.0 = center)
     */
    virtual float getGreenPan() const = 0;
    
    /**
     * Get blue channel pan position
     * @return Pan position (-100.0 to +100.0, 0.0 = center)
     */
    virtual float getBluePan() const = 0;
    
    /**
     * Update parameter values from ValueTreeState (called from audio thread)
     * @param parameters JUCE parameter state
     */
    virtual void updateFromValueTreeState(juce::AudioProcessorValueTreeState& parameters) = 0;
    
    /**
     * Check if any parameters have changed since last check
     * @return true if parameters were updated
     */
    virtual bool hasParametersChanged() = 0;
};

//==============================================================================
/**
 * Concrete parameter manager implementation with thread-safe atomic access
 */
class ParameterManager : public IParameterManager
{
public:
    ParameterManager() = default;
    ~ParameterManager() override = default;
    
    // IParameterManager interface
    float getScanSpeed() const override;
    int getAreaSize() const override;
    float getOutputGain() const override;
    float getLeftChannelWeight() const override;
    float getRightChannelWeight() const override;
    float getRedPan() const override;
    float getGreenPan() const override;
    float getBluePan() const override;
    void updateFromValueTreeState(juce::AudioProcessorValueTreeState& parameters) override;
    bool hasParametersChanged() override;
    
private:
    // Atomic parameters for thread-safe access
    std::atomic<float> scanSpeed{1.0f};
    std::atomic<float> areaSize{5.0f};
    std::atomic<float> outputGain{1.0f};
    std::atomic<float> leftChannelWeight{0.5f};
    std::atomic<float> rightChannelWeight{0.5f};
    
    // RGB pan parameters (002-channel-panning feature)
    std::atomic<float> redPan{0.0f};
    std::atomic<float> greenPan{0.0f};
    std::atomic<float> bluePan{0.0f};
    
    // Change tracking
    std::atomic<bool> parametersChanged{false};
};

//==============================================================================
/**
 * Factory function to create ParameterManager instance
 * @return Unique pointer to IParameterManager implementation
 */
std::unique_ptr<IParameterManager> createParameterManager();