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
