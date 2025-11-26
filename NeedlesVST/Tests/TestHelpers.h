#pragma once

#include <catch2/catch_all.hpp>
#include <JuceHeader.h>

namespace TestHelpers {
    /**
     * Helper to create test images programmatically
     */
    juce::Image createTestImage(int width, int height, const juce::Colour& color) {
        juce::Image img(juce::Image::RGB, width, height, true);
        juce::Graphics g(img);
        g.fillAll(color);
        return img;
    }
    
    /**
     * Helper to create gradient test image
     */
    juce::Image createGradientImage(int width, int height) {
        juce::Image img(juce::Image::RGB, width, height, true);
        juce::Graphics g(img);
        juce::ColourGradient gradient(juce::Colours::black, 0, 0, 
                                      juce::Colours::white, width, height, false);
        g.setGradientFill(gradient);
        g.fillAll();
        return img;
    }
    
    /**
     * Helper to compare float values with tolerance
     */
    bool floatEqual(float a, float b, float tolerance = 0.001f) {
        return std::abs(a - b) < tolerance;
    }
    
    /**
     * Helper to create RGB struct
     */
    struct RGB {
        uint8_t red, green, blue;
        RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : red(r), green(g), blue(b) {}
        
        bool operator==(const RGB& other) const {
            return red == other.red && green == other.green && blue == other.blue;
        }
    };
}
