cmake_minimum_required(VERSION 3.15)
project(VST_Plugin VERSION 1.0.0)

# Add JUCE as a subdirectory
add_subdirectory(JUCE)

# Enable JUCE's VST3 format
juce_add_plugin(VST_Plugin
    COMPANY_NAME "Funny Onion Ltd"
    PLUGIN_MANUFACTURER_CODE "Ymfg"
    PLUGIN_CODE "Vst1"
    FORMATS VST3
    NAME "needles")

# Add source files
target_sources(VST_Plugin
    PRIVATE
        src/audio/PluginProcessor.cpp
        src/audio/PluginProcessor.h
        src/gui/PluginEditor.cpp
        src/gui/PluginEditor.h
)

# Link with JUCE modules
target_link_libraries(VST_Plugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_core
        juce::juce_data_structures
        juce::juce_graphics
        juce::juce_gui_basics
) 