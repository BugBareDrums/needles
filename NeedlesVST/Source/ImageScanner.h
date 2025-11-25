#pragma once

#include <cmath>
#include <memory>
#include "AudioSynthesis.h"

//==============================================================================
/**
 * Position structure for 2D coordinates with sub-pixel precision
 */
struct Position
{
    float x, y;
    
    Position(float xPos = 0.0f, float yPos = 0.0f) : x(xPos), y(yPos) {}
    
    bool operator==(const Position& other) const
    {
        return std::abs(x - other.x) < 0.001f && std::abs(y - other.y) < 0.001f;
    }
};

//==============================================================================
/**
 * Dimensions structure for width and height
 */
struct Dimensions
{
    int width, height;
    
    Dimensions(int w = 0, int h = 0) : width(w), height(h) {}
    
    bool isValid() const { return width > 0 && height > 0; }
    int getArea() const { return width * height; }
};

//==============================================================================
/**
 * Scan pattern enumeration for different image traversal algorithms
 */
enum class ScanPattern
{
    Horizontal = 0,     // Left-to-right, alternating scanlines
    Vertical,           // Top-to-bottom, alternating columns
    Diagonal,           // Diagonal sweeps across image
    Spiral              // Spiral from center outward
};

//==============================================================================
/**
 * Image scanner interface for path tracing through images
 */
class IImageScanner
{
public:
    virtual ~IImageScanner() = default;
    
    /**
     * Initialize scanner with image dimensions
     * @param width Image width in pixels
     * @param height Image height in pixels
     */
    virtual void initialize(int width, int height) = 0;
    
    /**
     * Get current scan position with sub-pixel precision
     * @return Current position coordinates
     */
    virtual Position getCurrentPosition() const = 0;
    
    /**
     * Advance scan position by specified speed
     * @param speed Speed multiplier (1.0 = normal speed)
     * @return New position after advancement
     */
    virtual Position advancePosition(float speed) = 0;
    
    /**
     * Set scanning pattern algorithm
     * @param pattern Pattern type to use for traversal
     */
    virtual void setScanPattern(ScanPattern pattern) = 0;
    
    /**
     * Get current scanning pattern
     * @return Current pattern type
     */
    virtual ScanPattern getScanPattern() const = 0;
    
    /**
     * Check if scanning should loop infinitely
     * @return true if looping is enabled
     */
    virtual bool isLooping() const = 0;
    
    /**
     * Enable or disable infinite looping
     * @param shouldLoop true to enable looping
     */
    virtual void setLooping(bool shouldLoop) = 0;
    
    /**
     * Reset scan position to beginning
     */
    virtual void resetPosition() = 0;
    
    /**
     * Check if scan has completed (only relevant when looping is disabled)
     * @return true if scan is complete
     */
    virtual bool isComplete() const = 0;
};

//==============================================================================
/**
 * Factory function to create ImageScanner instance
 * @return Unique pointer to IImageScanner implementation
 */
std::unique_ptr<IImageScanner> createImageScanner();