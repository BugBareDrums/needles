#include "ImageScanner.h"
#include <cmath>
#include <memory>

//==============================================================================
/**
 * Concrete implementation of IImageScanner with alternating scanline pattern
 */
class ImageScanner : public IImageScanner
{
private:
    Dimensions imageDimensions;
    Position currentPosition;
    ScanPattern currentPattern;
    bool loopingEnabled;
    bool isInitialized;
    bool scanComplete;
    
    // Scanline-specific state
    int currentLine;
    bool scanningRightToLeft;  // Alternates each scanline
    
public:
    ImageScanner() 
        : imageDimensions(0, 0)
        , currentPosition(0.0f, 0.0f)
        , currentPattern(ScanPattern::Horizontal)
        , loopingEnabled(true)
        , isInitialized(false)
        , scanComplete(false)
        , currentLine(0)
        , scanningRightToLeft(false)
    {
    }
    
    //==============================================================================
    void initialize(int width, int height) override
    {
        imageDimensions = Dimensions(width, height);
        isInitialized = imageDimensions.isValid();
        
        if (isInitialized)
        {
            resetPosition();
        }
    }
    
    //==============================================================================
    Position getCurrentPosition() const override
    {
        return currentPosition;
    }
    
    //==============================================================================
    Position advancePosition(float speed) override
    {
        if (!isInitialized || scanComplete)
        {
            return currentPosition;
        }
        
        // Apply speed multiplier to position advancement
        switch (currentPattern)
        {
            case ScanPattern::Horizontal:
                advanceHorizontalScan(speed);
                break;
                
            case ScanPattern::Vertical:
                advanceVerticalScan(speed);
                break;
                
            case ScanPattern::Diagonal:
                advanceDiagonalScan(speed);
                break;
                
            case ScanPattern::Spiral:
                advanceSpiralScan(speed);
                break;
        }
        
        return currentPosition;
    }
    
    //==============================================================================
    void setScanPattern(ScanPattern pattern) override
    {
        if (currentPattern != pattern)
        {
            currentPattern = pattern;
            if (isInitialized)
            {
                resetPosition();
            }
        }
    }
    
    //==============================================================================
    ScanPattern getScanPattern() const override
    {
        return currentPattern;
    }
    
    //==============================================================================
    bool isLooping() const override
    {
        return loopingEnabled;
    }
    
    //==============================================================================
    void setLooping(bool shouldLoop) override
    {
        loopingEnabled = shouldLoop;
        if (shouldLoop)
        {
            scanComplete = false;
        }
    }
    
    //==============================================================================
    void resetPosition() override
    {
        if (!isInitialized)
        {
            return;
        }
        
        currentPosition = Position(0.0f, 0.0f);
        currentLine = 0;
        scanningRightToLeft = false;
        scanComplete = false;
    }
    
    //==============================================================================
    bool isComplete() const override
    {
        return scanComplete;
    }
    
private:
    //==============================================================================
    void advanceHorizontalScan(float speed)
    {
        if (scanningRightToLeft)
        {
            // Scanning from right to left
            currentPosition.x -= speed;
            
            if (currentPosition.x <= 0.0f)
            {
                // Reached left edge, move to next line
                currentPosition.x = 0.0f;
                moveToNextScanLine();
            }
        }
        else
        {
            // Scanning from left to right
            currentPosition.x += speed;
            
            if (currentPosition.x >= static_cast<float>(imageDimensions.width - 1))
            {
                // Reached right edge, move to next line
                currentPosition.x = static_cast<float>(imageDimensions.width - 1);
                moveToNextScanLine();
            }
        }
    }
    
    //==============================================================================
    void moveToNextScanLine()
    {
        currentLine++;
        
        if (currentLine >= imageDimensions.height)
        {
            // Completed full image scan
            if (loopingEnabled)
            {
                // Reset to beginning for infinite loop
                currentLine = 0;
                currentPosition.y = 0.0f;
                scanningRightToLeft = false;
                currentPosition.x = 0.0f;
            }
            else
            {
                // Mark scan as complete
                scanComplete = true;
                currentLine = imageDimensions.height - 1;
                currentPosition.y = static_cast<float>(currentLine);
            }
        }
        else
        {
            // Move to next scanline
            currentPosition.y = static_cast<float>(currentLine);
            
            // Alternate scan direction for alternating scanline pattern
            scanningRightToLeft = !scanningRightToLeft;
            
            if (scanningRightToLeft)
            {
                currentPosition.x = static_cast<float>(imageDimensions.width - 1);
            }
            else
            {
                currentPosition.x = 0.0f;
            }
        }
    }
    
    //==============================================================================
    void advanceVerticalScan(float speed)
    {
        // For now, just implement basic vertical scanning
        // Full implementation would be similar to horizontal but moving vertically
        currentPosition.y += speed;
        
        if (currentPosition.y >= static_cast<float>(imageDimensions.height - 1))
        {
            if (loopingEnabled)
            {
                currentPosition.y = 0.0f;
                currentPosition.x += 1.0f;
                
                if (currentPosition.x >= static_cast<float>(imageDimensions.width))
                {
                    currentPosition.x = 0.0f;
                }
            }
            else
            {
                scanComplete = true;
                currentPosition.y = static_cast<float>(imageDimensions.height - 1);
            }
        }
    }
    
    //==============================================================================
    void advanceDiagonalScan(float speed)
    {
        // Placeholder for diagonal scanning
        // For now, default to horizontal scanning
        advanceHorizontalScan(speed);
    }
    
    //==============================================================================
    void advanceSpiralScan(float speed)
    {
        // Placeholder for spiral scanning
        // For now, default to horizontal scanning
        advanceHorizontalScan(speed);
    }
};

//==============================================================================
// Factory function to create ImageScanner instance
std::unique_ptr<IImageScanner> createImageScanner()
{
    return std::make_unique<ImageScanner>();
}