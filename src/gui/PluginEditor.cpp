#include "PluginEditor.h"

AudioPluginEditor::AudioPluginEditor(AudioPluginProcessor& p)
    : AudioProcessorEditor(&p), processor_(p) {
    // Set initial window size
    setSize(400, 300);
}

AudioPluginEditor::~AudioPluginEditor() {}

void AudioPluginEditor::paint(juce::Graphics& g) {
    // Fill background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Add basic title
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("VST Plugin", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginEditor::resized() {
    // Layout your components here
    auto area = getLocalBounds();
    
    // Example of reserving space for controls:
    // auto controls_area = area.removeFromTop(100);
} 