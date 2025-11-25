#pragma once

#include <juce_graphics/juce_graphics.h>
#include "ImageScanner.h"
#include "AudioSynthesis.h"
#include <string>
#include <memory>

//==============================================================================
/**
 * Load result structure for image loading operations
 */
struct LoadResult
{
    bool success;
    std::string errorMessage;
    
    LoadResult(bool s = false, const std::string& msg = "") 
        : success(s), errorMessage(msg) {}
};

//==============================================================================
/**
 * Image loader interface for loading and managing image files
 */
class IImageLoader
{
public:
    virtual ~IImageLoader() = default;
    
    /**
     * Load image from file path
     * @param filePath Absolute path to image file
     * @return LoadResult with success/failure and error details
     */
    virtual LoadResult loadImage(const std::string& filePath) = 0;
    
    /**
     * Get pixel RGB values for specified coordinates with sub-pixel precision
     * @param x X coordinate (supports sub-pixel sampling)
     * @param y Y coordinate (supports sub-pixel sampling)
     * @return RGB struct with red, green, blue values [0-255]
     */
    virtual RGB getPixel(float x, float y) const = 0;
    
    /**
     * Get averaged RGB values for area around coordinates
     * @param x Center X coordinate
     * @param y Center Y coordinate
     * @param areaSize Size of area to average (pixel radius)
     * @return RGB struct with averaged values
     */
    virtual RGB getAreaAverage(float x, float y, int areaSize) const = 0;
    
    /**
     * Get image dimensions
     * @return Dimensions struct with width and height
     */
    virtual Dimensions getDimensions() const = 0;
    
    /**
     * Check if image is loaded and ready for processing
     * @return true if image data is available
     */
    virtual bool isLoaded() const = 0;
    
    /**
     * Clear current image and free memory
     */
    virtual void clearImage() = 0;
    
    /**
     * Get file path of currently loaded image
     * @return File path or empty string if no image loaded
     */
    virtual std::string getFilePath() const = 0;
    
    /**
     * Check if coordinates are within image bounds
     * @param x X coordinate to check
     * @param y Y coordinate to check
     * @return true if coordinates are valid
     */
    virtual bool isValidPosition(float x, float y) const = 0;
};

//==============================================================================
/**
 * Factory function to create ImageLoader instance
 * @return Unique pointer to IImageLoader implementation
 */
std::unique_ptr<IImageLoader> createImageLoader();