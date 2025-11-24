#!/bin/bash

# Catch2 Testing Framework Setup for Needles VST
# Run from Tests/ directory

echo "Setting up Catch2 testing framework..."

# Create main test runner
cat << 'EOF' > catch_main.cpp
#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"

// Custom main function for JUCE initialization if needed
int main(int argc, char* argv[]) {
    // Initialize JUCE if needed for integration tests
    juce::initialiseJuce_GUI();
    
    int result = Catch::Session().run(argc, argv);
    
    // Cleanup JUCE
    juce::shutdownJuce_GUI();
    
    return result;
}
EOF

# Create test helper utilities
cat << 'EOF' > TestHelpers.h
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
EOF

echo "Catch2 setup complete!"
echo "To build and run tests:"
echo "1. Install Catch2: brew install catch2 (macOS) or vcpkg install catch2 (Windows)"
echo "2. Add Catch2 to your build system"
echo "3. Include TestHelpers.h in your test files"