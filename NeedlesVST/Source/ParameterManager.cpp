#include "ParameterManager.h"
#include <algorithm>

//==============================================================================
float ParameterManager::getScanSpeed() const
{
    return scanSpeed.load();
}

int ParameterManager::getAreaSize() const
{
    return static_cast<int>(areaSize.load());
}

float ParameterManager::getOutputGain() const
{
    return outputGain.load();
}

float ParameterManager::getLeftChannelWeight() const
{
    return leftChannelWeight.load();
}

float ParameterManager::getRightChannelWeight() const
{
    return rightChannelWeight.load();
}

float ParameterManager::getRedPan() const
{
    return redPan.load();
}

float ParameterManager::getGreenPan() const
{
    return greenPan.load();
}

float ParameterManager::getBluePan() const
{
    return bluePan.load();
}

void ParameterManager::updateFromValueTreeState(juce::AudioProcessorValueTreeState& parameters)
{
    // Core parameters
    float newScanSpeed = *parameters.getRawParameterValue("scanSpeed");
    float newAreaSize = static_cast<float>(*parameters.getRawParameterValue("areaSize"));
    float newOutputGain = *parameters.getRawParameterValue("outputGain");
    float newLeftWeight = *parameters.getRawParameterValue("leftWeight");
    float newRightWeight = *parameters.getRawParameterValue("rightWeight");
    
    // RGB pan parameters
    float newRedPan = *parameters.getRawParameterValue("redPan");
    float newGreenPan = *parameters.getRawParameterValue("greenPan");
    float newBluePan = *parameters.getRawParameterValue("bluePan");
    
    // Check for changes
    bool changed = false;
    
    if (scanSpeed.load() != newScanSpeed) {
        scanSpeed.store(newScanSpeed);
        changed = true;
    }
    
    if (areaSize.load() != newAreaSize) {
        areaSize.store(newAreaSize);
        changed = true;
    }
    
    if (outputGain.load() != newOutputGain) {
        outputGain.store(newOutputGain);
        changed = true;
    }
    
    if (leftChannelWeight.load() != newLeftWeight) {
        leftChannelWeight.store(newLeftWeight);
        changed = true;
    }
    
    if (rightChannelWeight.load() != newRightWeight) {
        rightChannelWeight.store(newRightWeight);
        changed = true;
    }
    
    // RGB pan parameter updates
    if (redPan.load() != newRedPan) {
        redPan.store(newRedPan);
        changed = true;
    }
    
    if (greenPan.load() != newGreenPan) {
        greenPan.store(newGreenPan);
        changed = true;
    }
    
    if (bluePan.load() != newBluePan) {
        bluePan.store(newBluePan);
        changed = true;
    }
    
    if (changed) {
        parametersChanged.store(true);
    }
}

bool ParameterManager::hasParametersChanged()
{
    return parametersChanged.exchange(false);
}

//==============================================================================
std::unique_ptr<IParameterManager> createParameterManager()
{
    return std::make_unique<ParameterManager>();
}