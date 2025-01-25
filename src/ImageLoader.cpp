#include "ImageLoader.h"

ImageLoader::ImageLoader()
{
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load Image");
    loadButton.onClick = [this] { openFileBrowser(); };
}

ImageLoader::~ImageLoader() {}

void ImageLoader::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    if (hasValidImage())
    {
        // Draw image maintaining aspect ratio
        auto imageBounds = bounds.reduced(10);
        g.drawImage(currentImage, imageBounds.toFloat(),
                   juce::RectanglePlacement::centred | juce::RectanglePlacement::doNotResize);
    }
    else 
    {
        // Draw placeholder or error message
        g.setColour(juce::Colours::darkgrey);
        g.drawRect(bounds.reduced(10), 2);
        
        g.setColour(juce::Colours::white);
        if (errorMessage.isNotEmpty())
            g.drawText(errorMessage, bounds, juce::Justification::centred);
        else
            g.drawText("No image loaded - drag & drop or click Load Image", 
                      bounds, juce::Justification::centred);
    }
}

void ImageLoader::resized()
{
    auto bounds = getLocalBounds();
    loadButton.setBounds(bounds.removeFromTop(30).reduced(5));
}

bool ImageLoader::isInterestedInFileDrag(const juce::StringArray& files)
{
    return files.size() == 1 && isImageFile(juce::File(files[0]));
}

void ImageLoader::filesDropped(const juce::StringArray& files, int /*x*/, int /*y*/)
{
    if (files.size() == 1)
        loadImageFromFile(juce::File(files[0]));
}

void ImageLoader::loadImageFromFile(const juce::File& file)
{
    if (!isImageFile(file))
    {
        displayLoadError("Invalid file format - please use PNG, JPEG or BMP");
        return;
    }

    auto image = juce::ImageFileFormat::loadFrom(file);
    
    if (!image.isValid())
    {
        displayLoadError("Failed to load image file");
        return;
    }

    currentImage = image;
    errorMessage.clear();
    
    if (onImageChanged)
        onImageChanged();
        
    repaint();
}

bool ImageLoader::isImageFile(const juce::File& file) const
{
    auto extension = file.getFileExtension().toLowerCase();
    return extension == ".png" || extension == ".jpg" || 
           extension == ".jpeg" || extension == ".bmp";
}

void ImageLoader::openFileBrowser()
{
    auto fileChooser = std::make_unique<juce::FileChooser>(
        "Select an image file...",
        juce::File::getSpecialLocation(juce::File::userPicturesDirectory),
        "*.png;*.jpg;*.jpeg;*.bmp");
        
    fileChooser->launchAsync(juce::FileBrowserComponent::openMode | 
                            juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            if (fc.getResults().size() == 1)
                loadImageFromFile(fc.getResult());
        });
}

void ImageLoader::displayLoadError(const juce::String& message)
{
    errorMessage = message;
    currentImage = juce::Image();
    repaint();
} 