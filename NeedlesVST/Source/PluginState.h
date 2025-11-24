#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include "ImageScanner.h"
#include "AudioSynthesis.h"
#include <string>

//==============================================================================
/**
 * Plugin state interface for persistence and state management
 */
class IPluginState
{
public:
    virtual ~IPluginState() = default;
    
    /**
     * Get current state version for backward compatibility
     * @return Version number
     */
    virtual int getStateVersion() const = 0;
    
    /**
     * Get file path of currently loaded image
     * @return Image file path or empty if none
     */
    virtual std::string getImageFilePath() const = 0;
    
    /**
     * Set image file path for persistence
     * @param filePath Path to image file
     */
    virtual void setImageFilePath(const std::string& filePath) = 0;
    
    /**
     * Get current scan position for state saving
     * @return Current scanner position
     */
    virtual Position getScanPosition() const = 0;
    
    /**
     * Set scan position for state restoration
     * @param position Scanner position to restore
     */
    virtual void setScanPosition(const Position& position) = 0;
    
    /**
     * Get scan pattern setting
     * @return Current scan pattern
     */
    virtual ScanPattern getScanPattern() const = 0;
    
    /**
     * Set scan pattern setting
     * @param pattern Scan pattern to use
     */
    virtual void setScanPattern(ScanPattern pattern) = 0;
    
    /**
     * Get conversion formula setting
     * @return Current conversion formula
     */
    virtual ConversionFormula getConversionFormula() const = 0;
    
    /**
     * Set conversion formula setting
     * @param formula Conversion formula to use
     */
    virtual void setConversionFormula(ConversionFormula formula) = 0;
    
    /**
     * Check if auto-load image on project open is enabled
     * @return true if auto-load is enabled
     */
    virtual bool isAutoLoadEnabled() const = 0;
    
    /**
     * Enable or disable auto-load image on project open
     * @param enabled true to enable auto-load
     */
    virtual void setAutoLoadEnabled(bool enabled) = 0;
    
    /**
     * Serialize state to ValueTree for DAW persistence
     * @return ValueTree containing all state data
     */
    virtual juce::ValueTree serialize() const = 0;
    
    /**
     * Deserialize state from ValueTree
     * @param state ValueTree containing state data
     * @return true if deserialization was successful
     */
    virtual bool deserialize(const juce::ValueTree& state) = 0;
};