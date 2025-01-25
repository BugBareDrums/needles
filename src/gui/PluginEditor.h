#pragma once

#include "../audio/PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>

class AudioPluginEditor : public juce::AudioProcessorEditor {
 public:
  explicit AudioPluginEditor(AudioPluginProcessor&);
  ~AudioPluginEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

 private:
  AudioPluginProcessor& processor_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginEditor)
}; 