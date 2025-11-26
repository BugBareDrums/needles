cmake_minimum_required(VERSION 3.20)

project(Needles VERSION 1.0.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE framework
find_package(PkgConfig REQUIRED)

# JUCE setup - use the system installation
set(JUCE_ROOT "/Applications/JUCE")
list(APPEND CMAKE_PREFIX_PATH "${JUCE_ROOT}")
add_subdirectory("${JUCE_ROOT}" JUCE)

# Create plugin target
juce_add_plugin(Needles
    COMPANY_NAME "Needles Audio"
    COMPANY_WEBSITE "https://needles.audio"
    COMPANY_EMAIL "info@needles.audio"
    PLUGIN_MANUFACTURER_CODE "NAud"
    PLUGIN_CODE "Nedl"
    FORMATS VST3 AU AAX Standalone
    PRODUCT_NAME "Needles"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES "Fx" "Generator"
    AAX_CATEGORY "AAX_ePlugInCategory_Effect"
)

# Source files
target_sources(Needles PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/ImageLoader.cpp
    Source/ImageLoader.h
    Source/ImageScanner.cpp
    Source/ImageScanner.h
    Source/AudioSynthesis.cpp
    Source/AudioSynthesis.h
    Source/ParameterManager.cpp
    Source/ParameterManager.h
    Source/PluginState.cpp
    Source/PluginState.h
    Source/StereoProcessor.cpp
    Source/StereoProcessor.h
)

# Link JUCE modules
target_link_libraries(Needles PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Compile definitions
target_compile_definitions(Needles PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Testing (if Catch2 is available)
if(BUILD_TESTS)
    find_package(Catch2 3 QUIET)
    if(Catch2_FOUND)
        enable_testing()
        
        # Unit tests for RGB channel panning
        add_executable(NeedlesUnitTests
            Tests/Unit/ImageLoaderTest.cpp
            Tests/Unit/ImageScannerTest.cpp
            Tests/Unit/AudioSynthesisTest.cpp
            Tests/Unit/ParameterManagerTest.cpp
            # RGB Channel Panning Unit Tests
            Tests/unit/StereoProcessorTest.cpp
            Tests/unit/ParameterRangeTest.cpp
            Tests/unit/ParameterSmoothingTest.cpp
        )
        
        # Integration tests for complete workflows
        add_executable(NeedlesIntegrationTests
            Tests/Integration/NeedlesWorkflowTest.cpp
            Tests/Integration/ParameterUpdateTest.cpp
            Tests/Integration/AdvancedSynthesisTest.cpp
            # RGB Channel Panning Integration Tests  
            Tests/integration/RGBChannelTest.cpp
            Tests/integration/AutomationTest.cpp
            Tests/integration/StereoOutputTest.cpp
        )
        
        # Performance tests for CPU and memory validation
        add_executable(NeedlesPerformanceTests
            Tests/performance/PanPerformanceTest.cpp
        )
        
        # Link libraries for all test executables
        target_link_libraries(NeedlesUnitTests PRIVATE
            Catch2::Catch2WithMain
            juce::juce_audio_basics
            juce::juce_audio_formats
            juce::juce_audio_processors
            juce::juce_core
            juce::juce_graphics
        )
        
        target_link_libraries(NeedlesIntegrationTests PRIVATE
            Catch2::Catch2WithMain
            juce::juce_audio_basics
            juce::juce_audio_formats
            juce::juce_audio_processors
            juce::juce_core
            juce::juce_graphics
        )
        
        target_link_libraries(NeedlesPerformanceTests PRIVATE
            Catch2::Catch2WithMain
            juce::juce_audio_basics
            juce::juce_audio_formats
            juce::juce_audio_processors
            juce::juce_core
            juce::juce_graphics
        )
        
        # Register tests with CTest
        add_test(NAME UnitTests COMMAND NeedlesUnitTests)
        add_test(NAME IntegrationTests COMMAND NeedlesIntegrationTests)
        add_test(NAME PerformanceTests COMMAND NeedlesPerformanceTests)
    endif()
endif()