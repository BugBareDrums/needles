#pragma once

#include <JuceHeader.h>

class ImageLoader : public juce::Component,
                   public juce::FileDragAndDropTarget
{
public:
    ImageLoader();
    ~ImageLoader() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // FileDragAndDropTarget interface
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Custom methods
    void loadImageFromFile(const juce::File& file);
    bool isImageFile(const juce::File& file) const;
    const juce::Image& getCurrentImage() const { return currentImage; }
    bool hasValidImage() const { return currentImage.isValid(); }

    // Callback for when image changes
    std::function<void()> onImageChanged;

private:
    void openFileBrowser();
    void displayLoadError(const juce::String& message);

    juce::TextButton loadButton;
    juce::Image currentImage;
    juce::String errorMessage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageLoader)
}; 