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