#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeedlesAudioProcessorEditor::NeedlesAudioProcessorEditor(NeedlesAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set plugin window size
    setSize(600, 400);

    // Setup basic title label
    titleLabel.setText("Needles - Image to Audio VST", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);

    // UI components will be added in respective user story phases:
    // Phase 3 (US1): Image loading and display
    // Phase 4 (US2): Parameter controls with sliders  
    // Phase 5 (US3): Advanced control dropdowns
}

NeedlesAudioProcessorEditor::~NeedlesAudioProcessorEditor()
{
    // Cleanup will be added as components are implemented
}

//==============================================================================
void NeedlesAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw border
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);

    // Temporary placeholder text for development
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    
    juce::Rectangle<int> textArea = getLocalBounds().reduced(20);
    textArea.removeFromTop(60); // Space for title
    
    g.drawMultiLineText(
        "Needles VST Plugin - Development Version\n\n"
        "Phase 1: Setup ✓ (Basic structure ready)\n"
        "Phase 2: Foundational (Next - Core interfaces)\n"
        "Phase 3: User Story 1 (Image loading & audio generation)\n"
        "Phase 4: User Story 2 (Real-time parameter control)\n"
        "Phase 5: User Story 3 (Advanced synthesis control)\n"
        "Phase 6: Polish (Optimization & documentation)\n\n"
        "This placeholder UI will be replaced with functional\n"
        "controls as each user story is implemented.",
        textArea.getX(),
        textArea.getY(),
        textArea.getWidth(),
        juce::Justification::topLeft
    );
}

void NeedlesAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    
    // Title at top
    titleLabel.setBounds(area.removeFromTop(50).reduced(10));
    
    // Layout will be expanded as components are added:
    // - Image display area (left side, User Story 1)
    // - Parameter controls (right side, User Story 2)
    // - Advanced controls (bottom, User Story 3)
}