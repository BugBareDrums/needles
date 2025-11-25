#include "ImageLoader.h"
#include <juce_graphics/juce_graphics.h>

//==============================================================================
/**
 * Concrete implementation of IImageLoader using JUCE::Image
 */
class ImageLoader : public IImageLoader
{
private:
    juce::Image image;
    std::string currentFilePath;
    Dimensions dimensions;
    bool imageLoaded;

public:
    ImageLoader() : dimensions({0, 0}), imageLoaded(false) {}
    
    //==============================================================================
    LoadResult loadImage(const std::string& filePath) override
    {
        try
        {
            // Clear any existing image
            clearImage();
            
            // Validate file path
            juce::File imageFile(filePath);
            if (!imageFile.existsAsFile())
            {
                return LoadResult(false, "File does not exist: " + filePath);
            }
            
            // Check file size (limit to 50MB as per data model requirements)
            auto fileSize = imageFile.getSize();
            if (fileSize > 50 * 1024 * 1024) // 50MB
            {
                return LoadResult(false, "File too large (>50MB): " + filePath);
            }
            
            // Additional validation - check file extension
            auto extension = imageFile.getFileExtension().toLowerCase();
            if (extension != ".jpg" && extension != ".jpeg" && extension != ".png" && 
                extension != ".gif" && extension != ".bmp")
            {
                return LoadResult(false, "Unsupported file format: " + extension.toStdString());
            }
            
            // Attempt to load image using JUCE with error handling
            try
            {
                image = juce::ImageFileFormat::loadFrom(imageFile);
            }
            catch (const std::exception& e)
            {
                return LoadResult(false, "Exception during image loading: " + std::string(e.what()));
            }
            catch (...)
            {
                return LoadResult(false, "Unknown exception during image loading");
            }
            
            if (!image.isValid())
            {
                return LoadResult(false, "Failed to load image or unsupported format: " + filePath);
            }
            
            // Validate image dimensions (must be > 0 and < 4096 per data model)
            int width = image.getWidth();
            int height = image.getHeight();
            
            if (width <= 0 || height <= 0)
            {
                clearImage();
                return LoadResult(false, "Invalid image dimensions: " + std::to_string(width) + "x" + std::to_string(height));
            }
            
            if (width >= 4096 || height >= 4096)
            {
                clearImage();
                return LoadResult(false, "Image dimensions too large (>=4096): " + std::to_string(width) + "x" + std::to_string(height));
            }
            
            // Update state
            currentFilePath = filePath;
            dimensions = {width, height};
            imageLoaded = true;
            
            return LoadResult(true, "Image loaded successfully");
        }
        catch (const std::exception& e)
        {
            clearImage();
            return LoadResult(false, "Exception during image processing: " + std::string(e.what()));
        }
        catch (...)
        {
            clearImage();
            return LoadResult(false, "Unknown exception during image processing");
        }
    }
    
    //==============================================================================
    RGB getPixel(float x, float y) const override
    {
        if (!imageLoaded || !isValidPosition(x, y))
        {
            return RGB{0, 0, 0}; // Return black for invalid positions
        }
        
        // Handle sub-pixel precision using bilinear interpolation
        int x1 = static_cast<int>(std::floor(x));
        int y1 = static_cast<int>(std::floor(y));
        int x2 = std::min(x1 + 1, dimensions.width - 1);
        int y2 = std::min(y1 + 1, dimensions.height - 1);
        
        // Calculate interpolation weights
        float wx = x - x1;
        float wy = y - y1;
        
        // Get four corner pixels
        auto pixel11 = image.getPixelAt(x1, y1);
        auto pixel12 = image.getPixelAt(x1, y2);
        auto pixel21 = image.getPixelAt(x2, y1);
        auto pixel22 = image.getPixelAt(x2, y2);
        
        // Bilinear interpolation
        auto interpolateComponent = [wx, wy](uint8_t c11, uint8_t c12, uint8_t c21, uint8_t c22) -> uint8_t
        {
            float top = c11 * (1.0f - wx) + c21 * wx;
            float bottom = c12 * (1.0f - wx) + c22 * wx;
            return static_cast<uint8_t>(top * (1.0f - wy) + bottom * wy);
        };
        
        return RGB{
            interpolateComponent(pixel11.getRed(), pixel12.getRed(), pixel21.getRed(), pixel22.getRed()),
            interpolateComponent(pixel11.getGreen(), pixel12.getGreen(), pixel21.getGreen(), pixel22.getGreen()),
            interpolateComponent(pixel11.getBlue(), pixel12.getBlue(), pixel21.getBlue(), pixel22.getBlue())
        };
    }
    
    //==============================================================================
    RGB getAreaAverage(float x, float y, int areaSize) const override
    {
        if (!imageLoaded || areaSize < 1)
        {
            return RGB{0, 0, 0};
        }
        
        // Ensure area size is odd for centered sampling (per data model requirement)
        if (areaSize % 2 == 0)
        {
            areaSize++;
        }
        
        int halfSize = areaSize / 2;
        int totalPixels = 0;
        long totalR = 0, totalG = 0, totalB = 0;
        
        // Sample area around center point
        for (int dy = -halfSize; dy <= halfSize; dy++)
        {
            for (int dx = -halfSize; dx <= halfSize; dx++)
            {
                float sampleX = x + dx;
                float sampleY = y + dy;
                
                if (isValidPosition(sampleX, sampleY))
                {
                    RGB pixel = getPixel(sampleX, sampleY);
                    totalR += pixel.red;
                    totalG += pixel.green;
                    totalB += pixel.blue;
                    totalPixels++;
                }
            }
        }
        
        if (totalPixels == 0)
        {
            return RGB{0, 0, 0};
        }
        
        return RGB{
            static_cast<uint8_t>(totalR / totalPixels),
            static_cast<uint8_t>(totalG / totalPixels),
            static_cast<uint8_t>(totalB / totalPixels)
        };
    }
    
    //==============================================================================
    Dimensions getDimensions() const override
    {
        return dimensions;
    }
    
    //==============================================================================
    bool isLoaded() const override
    {
        return imageLoaded && image.isValid();
    }
    
    //==============================================================================
    void clearImage() override
    {
        image = juce::Image();
        currentFilePath.clear();
        dimensions = {0, 0};
        imageLoaded = false;
    }
    
    //==============================================================================
    std::string getFilePath() const override
    {
        return currentFilePath;
    }
    
    //==============================================================================
    bool isValidPosition(float x, float y) const override
    {
        if (!imageLoaded)
        {
            return false;
        }
        
        return x >= 0.0f && x < static_cast<float>(dimensions.width) &&
               y >= 0.0f && y < static_cast<float>(dimensions.height);
    }
};

//==============================================================================
// Factory function to create ImageLoader instance
std::unique_ptr<IImageLoader> createImageLoader()
{
    return std::make_unique<ImageLoader>();
}