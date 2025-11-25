#include "AudioSynthesis.h"
#include <algorithm>
#include <memory>

//==============================================================================
/**
 * Concrete implementation of IAudioSynthesis for RGB-to-audio conversion
 */
class AudioSynthesis : public IAudioSynthesis
{
private:
    float outputGain;

public:
    AudioSynthesis() : outputGain(1.0f) {}
    
    //==============================================================================
    float rgbToAudio(const RGB& rgb, ConversionFormula formula) override
    {
        float audioSample = 0.0f;
        
        switch (formula)
        {
            case ConversionFormula::RGBAverage:
                audioSample = rgb.toAudioSimple();
                break;
                
            case ConversionFormula::WeightedRGB:
                audioSample = rgb.toAudioWeighted();
                break;
                
            case ConversionFormula::RedChannel:
                audioSample = rgb.toAudioChannel(0);
                break;
                
            case ConversionFormula::GreenChannel:
                audioSample = rgb.toAudioChannel(1);
                break;
                
            case ConversionFormula::BlueChannel:
                audioSample = rgb.toAudioChannel(2);
                break;
                
            case ConversionFormula::MaxChannel:
                audioSample = convertMaxChannel(rgb);
                break;
                
            case ConversionFormula::MinChannel:
                audioSample = convertMinChannel(rgb);
                break;
                
            default:
                audioSample = rgb.toAudioSimple();
                break;
        }
        
        // Apply output gain and ensure we stay in valid range
        audioSample *= outputGain;
        return std::clamp(audioSample, -1.0f, 1.0f);
    }
    
    //==============================================================================
    void setGain(float gain) override
    {
        // Clamp gain to reasonable range (0.0 to 2.0 as per data model)
        outputGain = std::clamp(gain, 0.0f, 2.0f);
    }
    
    //==============================================================================
    float getGain() const override
    {
        return outputGain;
    }
    
private:
    //==============================================================================
    float convertMaxChannel(const RGB& rgb) const
    {
        uint8_t maxValue = std::max({rgb.red, rgb.green, rgb.blue});
        return (static_cast<float>(maxValue) / 255.0f) * 2.0f - 1.0f;
    }
    
    //==============================================================================
    float convertMinChannel(const RGB& rgb) const
    {
        uint8_t minValue = std::min({rgb.red, rgb.green, rgb.blue});
        return (static_cast<float>(minValue) / 255.0f) * 2.0f - 1.0f;
    }
};

//==============================================================================
// Factory function to create AudioSynthesis instance
std::unique_ptr<IAudioSynthesis> createAudioSynthesis()
{
    return std::make_unique<AudioSynthesis>();
}