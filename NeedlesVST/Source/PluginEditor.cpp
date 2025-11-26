#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeedlesAudioProcessorEditor::NeedlesAudioProcessorEditor(NeedlesAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), imageLoaded(false)
{
    // Set plugin window size
    setSize(500, 500);

    // Setup basic title label
    titleLabel.setText("Needles - Image to Audio VST", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(20.0f).withStyle("bold")));
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Setup load image button
    loadImageButton.setButtonText("Load Image");
    loadImageButton.onClick = [this] { loadImageFile(); };
    addAndMakeVisible(loadImageButton);
    
    // Setup image info label
    imageInfoLabel.setText("No image loaded", juce::dontSendNotification);
    imageInfoLabel.setJustificationType(juce::Justification::centred);
    imageInfoLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    addAndMakeVisible(imageInfoLabel);
    
    // Setup image display
    addAndMakeVisible(imageDisplay);
    
    // Setup parameter controls
    setupParameterControls();
}

NeedlesAudioProcessorEditor::~NeedlesAudioProcessorEditor()
{
    // Cleanup will be added as components are implemented
}

//==============================================================================
void NeedlesAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background gradient
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    // Draw subtle gradient
    juce::ColourGradient gradient(juce::Colour(0xff2a2a2a), 0, 0,
                                  juce::Colour(0xff1a1a1a), 0, getHeight(), false);
    g.setGradientFill(gradient);
    g.fillAll();

    // Draw border
    g.setColour(juce::Colour(0xff404040));
    g.drawRect(getLocalBounds(), 1);
}

void NeedlesAudioProcessorEditor::setupParameterControls()
{
    // Setup scan speed slider
    scanSpeedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    scanSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    scanSpeedSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff4CAF50));
    addAndMakeVisible(scanSpeedSlider);
    
    scanSpeedLabel.setText("Scan Speed", juce::dontSendNotification);
    scanSpeedLabel.setJustificationType(juce::Justification::centred);
    scanSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(scanSpeedLabel);
    
    // Setup area size slider  
    areaSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    areaSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    areaSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xff2196F3));
    addAndMakeVisible(areaSizeSlider);
    
    areaSizeLabel.setText("Area Size", juce::dontSendNotification);
    areaSizeLabel.setJustificationType(juce::Justification::centred);
    areaSizeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(areaSizeLabel);
    
    // Setup RGB Pan controls
    redPanSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    redPanSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    redPanSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff666666));
    redPanSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xffFF4444));
    addAndMakeVisible(redPanSlider);
    
    redPanLabel.setText("Red Pan", juce::dontSendNotification);
    redPanLabel.setJustificationType(juce::Justification::centredLeft);
    redPanLabel.setColour(juce::Label::textColourId, juce::Colour(0xffFF4444));
    addAndMakeVisible(redPanLabel);
    
    greenPanSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    greenPanSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    greenPanSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff666666));
    greenPanSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff44FF44));
    addAndMakeVisible(greenPanSlider);
    
    greenPanLabel.setText("Green Pan", juce::dontSendNotification);
    greenPanLabel.setJustificationType(juce::Justification::centredLeft);
    greenPanLabel.setColour(juce::Label::textColourId, juce::Colour(0xff44FF44));
    addAndMakeVisible(greenPanLabel);
    
    bluePanSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bluePanSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    bluePanSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff666666));
    bluePanSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff4444FF));
    addAndMakeVisible(bluePanSlider);
    
    bluePanLabel.setText("Blue Pan", juce::dontSendNotification);
    bluePanLabel.setJustificationType(juce::Justification::centredLeft);
    bluePanLabel.setColour(juce::Label::textColourId, juce::Colour(0xff4444FF));
    addAndMakeVisible(bluePanLabel);
    
    // Create parameter attachments for thread-safe updates
    scanSpeedAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameterTreeState(), "scanSpeed", scanSpeedSlider);
    areaSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameterTreeState(), "areaSize", areaSizeSlider);
    
    // Create RGB Pan attachments
    redPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameterTreeState(), "redPan", redPanSlider);
    greenPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameterTreeState(), "greenPan", greenPanSlider);
    bluePanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameterTreeState(), "bluePan", bluePanSlider);
}

void NeedlesAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(10);
    
    // Title at top
    titleLabel.setBounds(area.removeFromTop(40));
    
    area.removeFromTop(10); // Spacing
    
    // Controls at top
    auto controlsArea = area.removeFromTop(40);
    loadImageButton.setBounds(controlsArea.removeFromLeft(120));
    controlsArea.removeFromLeft(10); // Spacing
    imageInfoLabel.setBounds(controlsArea);
    
    area.removeFromTop(10); // Spacing
    
    // Parameter controls area
    auto parameterArea = area.removeFromTop(80);
    
    int controlWidth = 100;
    int controlSpacing = 20;
    
    // Center the two controls horizontally
    int totalWidth = 2 * controlWidth + controlSpacing;
    int startX = (parameterArea.getWidth() - totalWidth) / 2;
    
    // Scan speed control
    auto scanSpeedArea = parameterArea.withX(startX).withWidth(controlWidth);
    scanSpeedLabel.setBounds(scanSpeedArea.removeFromTop(20));
    scanSpeedSlider.setBounds(scanSpeedArea);
    
    // Area size control
    auto areaSizeArea = parameterArea.withX(startX + controlWidth + controlSpacing).withWidth(controlWidth);
    areaSizeLabel.setBounds(areaSizeArea.removeFromTop(20));
    areaSizeSlider.setBounds(areaSizeArea);
    
    area.removeFromTop(10); // Spacing
    
    // RGB Pan controls area
    auto rgbPanArea = area.removeFromTop(100);
    int labelWidth = 80;
    int sliderWidth = 200;
    int lineHeight = 30;
    
    // Calculate centering for RGB controls
    int rgbTotalWidth = labelWidth + sliderWidth + 60; // 60 for text box
    int rgbStartX = (rgbPanArea.getWidth() - rgbTotalWidth) / 2;
    
    // Red Pan
    auto redPanLine = rgbPanArea.removeFromTop(lineHeight);
    redPanLabel.setBounds(redPanLine.withX(rgbStartX).withWidth(labelWidth));
    redPanSlider.setBounds(redPanLine.withX(rgbStartX + labelWidth).withWidth(sliderWidth + 60));
    
    rgbPanArea.removeFromTop(5); // Small spacing
    
    // Green Pan
    auto greenPanLine = rgbPanArea.removeFromTop(lineHeight);
    greenPanLabel.setBounds(greenPanLine.withX(rgbStartX).withWidth(labelWidth));
    greenPanSlider.setBounds(greenPanLine.withX(rgbStartX + labelWidth).withWidth(sliderWidth + 60));
    
    rgbPanArea.removeFromTop(5); // Small spacing
    
    // Blue Pan
    auto bluePanLine = rgbPanArea.removeFromTop(lineHeight);
    bluePanLabel.setBounds(bluePanLine.withX(rgbStartX).withWidth(labelWidth));
    bluePanSlider.setBounds(bluePanLine.withX(rgbStartX + labelWidth).withWidth(sliderWidth + 60));
    
    area.removeFromTop(10); // Spacing
    
    // Image display takes remaining space
    imageDisplay.setBounds(area);
}

//==============================================================================
void NeedlesAudioProcessorEditor::ImageDisplayComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a2a));
    
    if (image.isValid())
    {
        // Calculate scaling to fit image in component while maintaining aspect ratio
        auto componentBounds = getLocalBounds().toFloat();
        auto imageAspect = (float)image.getWidth() / (float)image.getHeight();
        auto componentAspect = componentBounds.getWidth() / componentBounds.getHeight();
        
        juce::Rectangle<float> imageBounds;
        
        if (imageAspect > componentAspect)
        {
            // Image is wider - fit to width
            auto scaledHeight = componentBounds.getWidth() / imageAspect;
            imageBounds = juce::Rectangle<float>(0, (componentBounds.getHeight() - scaledHeight) * 0.5f,
                                                componentBounds.getWidth(), scaledHeight);
        }
        else
        {
            // Image is taller - fit to height
            auto scaledWidth = componentBounds.getHeight() * imageAspect;
            imageBounds = juce::Rectangle<float>((componentBounds.getWidth() - scaledWidth) * 0.5f, 0,
                                               scaledWidth, componentBounds.getHeight());
        }
        
        g.drawImage(image, imageBounds);
        
        // Draw border around image
        g.setColour(juce::Colours::white.withAlpha(0.5f));
        g.drawRect(imageBounds, 1.0f);
    }
    else
    {
        // No image loaded - show placeholder
        g.setColour(juce::Colours::grey.withAlpha(0.3f));
        g.setFont(16.0f);
        g.drawText("Load an image to see it here", getLocalBounds(), 
                   juce::Justification::centred, true);
    }
}

//==============================================================================
void NeedlesAudioProcessorEditor::loadImageFile()
{
    auto chooser = std::make_shared<juce::FileChooser>("Select an image file...",
                                                      juce::File::getSpecialLocation(juce::File::userPicturesDirectory),
                                                      "*.jpg;*.jpeg;*.png;*.gif;*.bmp;*.tiff");
    
    auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync(folderChooserFlags, [this, chooser](const juce::FileChooser& fc)
    {
        if (fc.getResults().size() > 0)
        {
            juce::File imageFile = fc.getResult();
            
            // Load the image
            currentImage = juce::ImageFileFormat::loadFrom(imageFile);
            
            if (currentImage.isValid())
            {
                imageLoaded = true;
                
                // Update the display
                imageDisplay.setImage(currentImage);
                
                // Load image into the audio processor using file path
                bool loadSuccess = audioProcessor.loadImage(imageFile.getFullPathName());
                
                if (loadSuccess)
                {
                    // Update info label with successful load message
                    auto fileSize = imageFile.getSize();
                    juce::String sizeText;
                    if (fileSize > 1024 * 1024)
                        sizeText = juce::String(fileSize / (1024 * 1024)) + " MB";
                    else if (fileSize > 1024)
                        sizeText = juce::String(fileSize / 1024) + " KB";
                    else
                        sizeText = juce::String(fileSize) + " bytes";
                        
                    imageInfoLabel.setText("✓ " + imageFile.getFileName() + " (" + 
                                          juce::String(currentImage.getWidth()) + "x" + 
                                          juce::String(currentImage.getHeight()) + ", " + 
                                          sizeText + ")", 
                                          juce::dontSendNotification);
                    imageInfoLabel.setColour(juce::Label::textColourId, juce::Colours::lightgreen);
                }
                else
                {
                    // Image loaded in UI but failed in audio processor - show detailed error
                    juce::String errorMsg = audioProcessor.getLastError();
                    if (errorMsg.isEmpty())
                        errorMsg = "Unknown error during audio processing setup";
                    
                    imageInfoLabel.setText("⚠ " + errorMsg, juce::dontSendNotification);
                    imageInfoLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
                    
                    // Keep the image visible but audio won't start
                }
            }
            else
            {
                // Failed to load image into UI
                juce::String errorMsg = "Failed to load image file";
                
                // Try to load through processor to get detailed error message
                audioProcessor.loadImage(imageFile.getFullPathName());
                juce::String processorError = audioProcessor.getLastError();
                if (processorError.isNotEmpty())
                    errorMsg = processorError;
                
                imageInfoLabel.setText("✗ " + errorMsg, juce::dontSendNotification);
                imageInfoLabel.setColour(juce::Label::textColourId, juce::Colours::red);
            }
        }
    });
}

void NeedlesAudioProcessorEditor::updateImageDisplay()
{
    if (imageLoaded && currentImage.isValid())
    {
        imageDisplay.setImage(currentImage);
    }
}