# Plugin Interface Contract

**Purpose**: Define the VST plugin's interface contracts for image loading, parameter management, and audio processing
**Created**: 2024-11-24
**Format**: C++ class interfaces (JUCE-based)

## ImageLoader Interface

```cpp
/**
 * Contract for loading and managing image files
 */
class IImageLoader {
public:
    virtual ~IImageLoader() = default;

    /**
     * Load image from file path
     * @param filePath Absolute path to image file
     * @return LoadResult with success/failure and error details
     */
    virtual LoadResult loadImage(const std::string& filePath) = 0;

    /**
     * Get pixel RGB values for specified coordinates
     * @param x X coordinate (sub-pixel precision supported)
     * @param y Y coordinate (sub-pixel precision supported)
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
     * Check if image is loaded and ready
     * @return true if image data is available
     */
    virtual bool isLoaded() const = 0;

    /**
     * Clear current image and free memory
     */
    virtual void clearImage() = 0;
};

/**
 * Supporting data structures
 */
struct RGB {
    uint8_t red, green, blue;
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : red(r), green(g), blue(b) {}
};

struct Dimensions {
    int width, height;
    Dimensions(int w = 0, int h = 0) : width(w), height(h) {}
};

struct LoadResult {
    bool success;
    std::string errorMessage;
    LoadResult(bool s = false, const std::string& msg = "") : success(s), errorMessage(msg) {}
};
```

## ImageScanner Interface

```cpp
/**
 * Contract for scanning paths through images
 */
class IImageScanner {
public:
    virtual ~IImageScanner() = default;

    /**
     * Initialize scanner with image dimensions
     * @param width Image width in pixels
     * @param height Image height in pixels
     */
    virtual void initialize(int width, int height) = 0;

    /**
     * Get current scan position
     * @return Position struct with x, y coordinates
     */
    virtual Position getCurrentPosition() const = 0;

    /**
     * Advance scan position by specified speed
     * @param speed Movement speed (pixels per sample)
     */
    virtual void advance(float speed) = 0;

    /**
     * Reset scanner to beginning of image
     */
    virtual void reset() = 0;

    /**
     * Set scanning pattern type
     * @param pattern Type of path to follow through image
     */
    virtual void setScanPattern(ScanPattern pattern) = 0;

    /**
     * Check if scanner has completed full image cycle
     * @return true if at end of scan path
     */
    virtual bool isAtEnd() const = 0;
};

/**
 * Supporting enums and structures
 */
enum class ScanPattern {
    HorizontalAlternating,  // Default left-right, right-left pattern
    HorizontalLeft,         // Always left to right
    VerticalAlternating,    // Top-bottom, bottom-top pattern
    Diagonal,               // Diagonal traversal
    Spiral                  // Spiral inward/outward
};

struct Position {
    float x, y;
    Position(float xPos = 0.0f, float yPos = 0.0f) : x(xPos), y(yPos) {}
};
```

## AudioSynthesis Interface

```cpp
/**
 * Contract for converting RGB values to audio samples
 */
class IAudioSynthesis {
public:
    virtual ~IAudioSynthesis() = default;

    /**
     * Convert RGB values to audio sample
     * @param rgb Input color values
     * @param formula Conversion method to use
     * @return Audio sample value in range [-1.0, 1.0]
     */
    virtual float rgbToAudio(const RGB& rgb, ConversionFormula formula) = 0;

    /**
     * Process audio buffer with current image and parameters
     * @param buffer Audio buffer to fill
     * @param numSamples Number of samples to process
     * @param scanner Position scanner for pixel coordinates
     * @param loader Image data source
     * @param params Current synthesis parameters
     */
    virtual void processAudioBuffer(
        juce::AudioBuffer<float>& buffer,
        int numSamples,
        IImageScanner& scanner,
        const IImageLoader& loader,
        const SynthesisParams& params
    ) = 0;

    /**
     * Set audio processing sample rate
     * @param sampleRate Host sample rate in Hz
     */
    virtual void setSampleRate(double sampleRate) = 0;

    /**
     * Prepare for audio processing
     * @param blockSize Expected audio buffer size
     */
    virtual void prepareToPlay(int blockSize) = 0;
};

/**
 * Supporting structures
 */
enum class ConversionFormula {
    RGBAverage,     // (R + G + B) / 3
    WeightedRGB,    // 0.299*R + 0.587*G + 0.114*B (luminance)
    RedChannel,     // Use only red channel
    GreenChannel,   // Use only green channel
    BlueChannel,    // Use only blue channel
    MaxChannel,     // Use maximum of R, G, B
    MinChannel      // Use minimum of R, G, B
};

struct SynthesisParams {
    float scanSpeed;            // Speed multiplier for scanning
    int areaSize;              // Area averaging size
    ConversionFormula formula; // RGB conversion method
    float outputGain;          // Master output level
    float leftChannelWeight;   // Left channel mix
    float rightChannelWeight;  // Right channel mix

    SynthesisParams() :
        scanSpeed(1.0f),
        areaSize(1),
        formula(ConversionFormula::RGBAverage),
        outputGain(0.5f),
        leftChannelWeight(1.0f),
        rightChannelWeight(1.0f) {}
};
```

## ParameterManager Interface

```cpp
/**
 * Contract for managing plugin parameters with thread-safe access
 */
class IParameterManager {
public:
    virtual ~IParameterManager() = default;

    /**
     * Initialize parameters with JUCE AudioProcessorValueTreeState
     * @param processor Parent audio processor
     */
    virtual void initialize(juce::AudioProcessor& processor) = 0;

    /**
     * Get current synthesis parameters (thread-safe)
     * @return Current parameter values
     */
    virtual SynthesisParams getCurrentParams() const = 0;

    /**
     * Register parameter value change callback
     * @param callback Function to call when parameters change
     */
    virtual void setParameterChangeCallback(std::function<void()> callback) = 0;

    /**
     * Get parameter value by identifier
     * @param parameterID String identifier for parameter
     * @return Current parameter value
     */
    virtual float getParameterValue(const std::string& parameterID) const = 0;

    /**
     * Set parameter value (from UI thread)
     * @param parameterID String identifier for parameter
     * @param newValue New parameter value
     */
    virtual void setParameterValue(const std::string& parameterID, float newValue) = 0;

    /**
     * Get parameter as smoothed value object for audio thread
     * @param parameterID String identifier for parameter
     * @return Smoothed value object for artifact-free changes
     */
    virtual juce::LinearSmoothedValue<float>& getSmoothedParameter(const std::string& parameterID) = 0;
};
```

## PluginState Interface

```cpp
/**
 * Contract for plugin state management and persistence
 */
class IPluginState {
public:
    virtual ~IPluginState() = default;

    /**
     * Save current plugin state to memory block
     * @param destData Output memory block for state data
     */
    virtual void saveState(juce::MemoryBlock& destData) = 0;

    /**
     * Restore plugin state from memory block
     * @param data Input memory block containing state data
     * @return true if state loaded successfully
     */
    virtual bool loadState(const void* data, int sizeInBytes) = 0;

    /**
     * Get current image file path
     * @return Path to currently loaded image, empty if none
     */
    virtual std::string getCurrentImagePath() const = 0;

    /**
     * Set current image file path
     * @param imagePath New image file path
     */
    virtual void setCurrentImagePath(const std::string& imagePath) = 0;

    /**
     * Get scanner position state
     * @return Current position for state persistence
     */
    virtual Position getScannerPosition() const = 0;

    /**
     * Set scanner position state
     * @param position Position to restore
     */
    virtual void setScannerPosition(const Position& position) = 0;

    /**
     * Reset all state to defaults
     */
    virtual void resetToDefaults() = 0;
};
```

## Integration Contract

```cpp
/**
 * Main plugin processor contract integrating all components
 */
class NeedlesProcessor : public juce::AudioProcessor {
public:
    NeedlesProcessor();
    ~NeedlesProcessor() override;

    // Core AudioProcessor overrides
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override;

    // State management
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Plugin metadata
    const juce::String getName() const override { return "Needles"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }

    // Editor creation
    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override;

    // Image loading API for editor
    LoadResult loadImageFromFile(const std::string& filePath);
    bool hasImageLoaded() const;
    Dimensions getImageDimensions() const;

    // Parameter access for editor
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

private:
    // Component implementations
    std::unique_ptr<IImageLoader> imageLoader;
    std::unique_ptr<IImageScanner> imageScanner;
    std::unique_ptr<IAudioSynthesis> audioSynthesis;
    std::unique_ptr<IParameterManager> parameterManager;
    std::unique_ptr<IPluginState> pluginState;

    // JUCE parameter state
    juce::AudioProcessorValueTreeState parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeedlesProcessor)
};
```
