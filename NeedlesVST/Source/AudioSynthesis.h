#pragma once

#include <cstdint>

//==============================================================================
/**
 * RGB color data structure for image-to-audio conversion
 * 
 * Represents RGB values extracted from image pixels with support for
 * area averaging and different conversion formulas.
 */
struct RGB
{
    uint8_t red, green, blue;
    
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) 
        : red(r), green(g), blue(b) {}
    
    /**
     * Convert RGB to audio sample using simple averaging
     * @return Audio sample in range [-1.0, 1.0]
     */
    float toAudioSimple() const
    {
        float avg = (static_cast<float>(red) + green + blue) / (3.0f * 255.0f);
        return avg * 2.0f - 1.0f; // Convert [0,1] to [-1,1]
    }
    
    /**
     * Convert RGB to audio using weighted luminance formula
     * @return Audio sample in range [-1.0, 1.0] 
     */
    float toAudioWeighted() const
    {
        // ITU-R BT.709 luminance weights
        float luminance = (0.2126f * red + 0.7152f * green + 0.0722f * blue) / 255.0f;
        return luminance * 2.0f - 1.0f;
    }
    
    /**
     * Extract individual color channel
     * @param channel 0=red, 1=green, 2=blue
     * @return Audio sample in range [-1.0, 1.0]
     */
    float toAudioChannel(int channel) const
    {
        uint8_t value = (channel == 0) ? red : (channel == 1) ? green : blue;
        return (static_cast<float>(value) / 255.0f) * 2.0f - 1.0f;
    }
    
    bool operator==(const RGB& other) const
    {
        return red == other.red && green == other.green && blue == other.blue;
    }
};

//==============================================================================
/**
 * Conversion formula enumeration for different RGB-to-audio algorithms
 */
enum class ConversionFormula
{
    RGBAverage = 0,     // Simple average of R, G, B channels
    WeightedRGB,        // Luminance-weighted conversion (ITU-R BT.709)
    RedChannel,         // Red channel only
    GreenChannel,       // Green channel only  
    BlueChannel,        // Blue channel only
    MaxChannel,         // Maximum of R, G, B
    MinChannel          // Minimum of R, G, B
};

//==============================================================================
/**
 * Audio synthesis interface for converting image data to audio samples
 */
class IAudioSynthesis
{
public:
    virtual ~IAudioSynthesis() = default;
    
    /**
     * Convert RGB pixel data to audio sample
     * @param rgb RGB color values from image
     * @param formula Conversion algorithm to use
     * @return Audio sample in range [-1.0, 1.0]
     */
    virtual float rgbToAudio(const RGB& rgb, ConversionFormula formula = ConversionFormula::RGBAverage) = 0;
    
    /**
     * Set output gain for audio synthesis
     * @param gain Gain multiplier (0.0 to 2.0)
     */
    virtual void setGain(float gain) = 0;
    
    /**
     * Get current output gain
     * @return Current gain value
     */
    virtual float getGain() const = 0;
};