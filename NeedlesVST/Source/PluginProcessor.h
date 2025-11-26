#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

// Include core interfaces
#include "ImageLoader.h"
#include "ImageScanner.h"
#include "AudioSynthesis.h"
#include "ParameterManager.h"
#include "PluginState.h"
#include "StereoProcessor.h"

//==============================================================================
/**
 * Main audio processor for Needles VST Plugin
 * 
 * Handles real-time audio processing by converting image pixel data to audio
 * through configurable scanning patterns and RGB-to-audio synthesis.
 * 
 * Key responsibilities:
 * - Audio processing loop with real-time constraints
 * - Parameter management with thread-safe access
 * - Integration of image scanning and audio synthesis components
 * - State persistence for DAW project compatibility
 */
class NeedlesAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NeedlesAudioProcessor();
    ~NeedlesAudioProcessor() override;

    //==============================================================================
    // AudioProcessor interface implementation
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // Editor management
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    // Plugin metadata
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    // Program management
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    // State persistence
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Parameter access for editor
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    const juce::AudioProcessorValueTreeState& getParameters() const { return parameters; }

    //==============================================================================
    // Image loading integration (for editor to call)
    bool loadImage(const juce::String& filePath);
    
    // Error handling - get last error message for UI display
    const juce::String& getLastError() const { return lastErrorMessage; }
    
    // Parameter access for UI
    juce::AudioProcessorValueTreeState& getParameterTreeState() { return parameters; }

private:
    //==============================================================================
    // Parameter management
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Core processing components - interfaces ready, implementations in user story phases
    std::unique_ptr<IImageLoader> imageLoader;
    std::unique_ptr<IImageScanner> imageScanner;
    std::unique_ptr<IAudioSynthesis> audioSynthesis;
    std::unique_ptr<IParameterManager> parameterManager;
    std::unique_ptr<IPluginState> pluginState;
    std::unique_ptr<IStereoProcessor> stereoProcessor;

    // Audio processing state
    double currentSampleRate {44100.0};
    int currentBlockSize {512};
    bool isProcessingActive {false};
    
    // Thread safety
    mutable juce::CriticalSection imageMutex;
    
    // Error tracking
    juce::String lastErrorMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeedlesAudioProcessor)
};

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter();