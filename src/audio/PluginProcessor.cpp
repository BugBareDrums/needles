#include "PluginProcessor.h"
#include "../gui/PluginEditor.h"

AudioPluginProcessor::AudioPluginProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {
}

AudioPluginProcessor::~AudioPluginProcessor() {}

void AudioPluginProcessor::prepareToPlay(double sample_rate, 
                                       int samples_per_block) {
    // Initialize processing state here
}

void AudioPluginProcessor::releaseResources() {
    // Free any resources when playback stops
}

void AudioPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                      juce::MidiBuffer& midi_messages) {
    juce::ScopedNoDenormals noDenormals;
    auto total_num_channels = getTotalNumOutputChannels();

    // Clear any unused channels
    for (auto i = getTotalNumInputChannels(); i < total_num_channels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    // Process audio here
}

juce::AudioProcessorEditor* AudioPluginProcessor::createEditor() {
    return new AudioPluginEditor(*this);
}

bool AudioPluginProcessor::hasEditor() const { return true; }

const juce::String AudioPluginProcessor::getName() const {
    return JucePlugin_Name;
}

bool AudioPluginProcessor::acceptsMidi() const { return false; }
bool AudioPluginProcessor::producesMidi() const { return false; }
bool AudioPluginProcessor::isMidiEffect() const { return false; }
double AudioPluginProcessor::getTailLengthSeconds() const { return 0.0; }

int AudioPluginProcessor::getNumPrograms() { return 1; }
int AudioPluginProcessor::getCurrentProgram() { return 0; }
void AudioPluginProcessor::setCurrentProgram(int index) {}
const juce::String AudioPluginProcessor::getProgramName(int index) { return {}; }
void AudioPluginProcessor::changeProgramName(int index, const juce::String& new_name) {}

void AudioPluginProcessor::getStateInformation(juce::MemoryBlock& dest_data) {
    // Store plugin state here
}

void AudioPluginProcessor::setStateInformation(const void* data, int size_in_bytes) {
    // Restore plugin state here
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioPluginProcessor();
} 