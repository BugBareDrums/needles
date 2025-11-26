#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "PluginProcessor.h"

//==============================================================================
/**
 * GUI editor for Needles VST Plugin
 * 
 * Provides user interface for image loading, parameter control, and visual feedback
 * including image display with scan position visualization.
 * 
 * Key responsibilities:
 * - Image file loading dialog and display
 * - Real-time parameter controls (scan speed, area size, conversion formula, etc.)
 * - Visual feedback for current scan position
 * - Responsive layout for different plugin host environments
 */
class NeedlesAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeedlesAudioProcessorEditor(NeedlesAudioProcessor&);
    ~NeedlesAudioProcessorEditor() override;

    //==============================================================================
    // Component interface
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Reference to processor for parameter access
    NeedlesAudioProcessor& audioProcessor;

    // UI Components (will be implemented in user story phases)
    juce::Label titleLabel;
    
    // Parameter controls
    juce::Slider scanSpeedSlider;
    juce::Slider areaSizeSlider;
    
    // RGB Pan controls
    juce::Slider redPanSlider;
    juce::Slider greenPanSlider;
    juce::Slider bluePanSlider;
    
    juce::Label scanSpeedLabel;
    juce::Label areaSizeLabel;
    juce::Label redPanLabel;
    juce::Label greenPanLabel;
    juce::Label bluePanLabel;

    // Image display (Phase 3 - User Story 1)
    juce::TextButton loadImageButton;
    juce::Label imageInfoLabel;
    juce::Image currentImage;
    bool imageLoaded;
    
    // Image display component
    class ImageDisplayComponent : public juce::Component
    {
    public:
        void setImage(const juce::Image& img) { image = img; repaint(); }
        void paint(juce::Graphics& g) override;
        
    private:
        juce::Image image;
    };
    
    ImageDisplayComponent imageDisplay;



    // Parameter attachments for thread-safe UI updates
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> scanSpeedAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> areaSizeAttachment;
    
    // RGB Pan attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> redPanAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> greenPanAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bluePanAttachment;

private:
    // Image loading functionality
    void loadImageFile();
    void updateImageDisplay();
    
    // Parameter setup
    void setupParameterControls();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeedlesAudioProcessorEditor)
};