#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeedlesAudioProcessor::NeedlesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      parameters(*this, nullptr, juce::Identifier("NEEDLES"), createParameterLayout())
{
    // Initialize core components
    imageLoader = createImageLoader();
    imageScanner = createImageScanner();
    audioSynthesis = createAudioSynthesis();
    
    DBG("Needles: AudioProcessor initialized with core components");
}

NeedlesAudioProcessor::~NeedlesAudioProcessor()
{
    // Destructor - cleanup will be added as components are implemented
}

//==============================================================================
const juce::String NeedlesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NeedlesAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool NeedlesAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool NeedlesAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double NeedlesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NeedlesAudioProcessor::getNumPrograms()
{
    return 1; // Some hosts require at least one program
}

int NeedlesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NeedlesAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String NeedlesAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return "Default";
}

void NeedlesAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void NeedlesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Store audio format information
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    
    // Audio processing initialization will be expanded in Phase 2
    isProcessingActive = true;
    
    DBG("Needles: Prepared to play at " << sampleRate << "Hz, " << samplesPerBlock << " samples per block");
}

void NeedlesAudioProcessor::releaseResources()
{
    // Resources cleanup will be expanded as components are implemented
    isProcessingActive = false;
    
    DBG("Needles: Resources released");
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NeedlesAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void NeedlesAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Audio processing implementation skeleton - ready for user story implementations
    // Thread-safe check for image loader and loaded state
    juce::ScopedTryLock lock(imageMutex);
    if (!lock.isLocked())
    {
        // Image is being loaded, output silence
        buffer.clear();
        return;
    }
    
    auto* loader = imageLoader.get(); // Get raw pointer for null checking
    if (!isProcessingActive || !loader || !loader->isLoaded())
    {
        buffer.clear();
        return;
    }

    // Get current parameters
    auto* scanSpeedParam = parameters.getRawParameterValue("scanSpeed");
    auto* areaSizeParam = parameters.getRawParameterValue("areaSize");
    
    float scanSpeed = scanSpeedParam ? scanSpeedParam->load() : 1.0f;
    int areaSize = areaSizeParam ? static_cast<int>(areaSizeParam->load()) : 1;
    
    // Additional safety check for valid area size
    areaSize = juce::jlimit(1, 10, areaSize); // Limit area size to prevent performance issues
    
    // Generate audio samples
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    
    // Get image dimensions once for bounds checking
    auto dims = loader->getDimensions();
    if (dims.width <= 0 || dims.height <= 0)
    {
        buffer.clear();
        return;
    }
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Double-check loader is still valid (thread safety)
        if (!loader || !loader->isLoaded())
        {
            // Fill remaining samples with silence and return
            for (int channel = 0; channel < numChannels; ++channel)
            {
                for (int s = sample; s < numSamples; ++s)
                {
                    buffer.setSample(channel, s, 0.0f);
                }
            }
            return;
        }
        
        // Advance scan position based on speed
        Position currentPos = imageScanner->advancePosition(scanSpeed);
        
        // Bounds check the position before accessing image data
        if (currentPos.x < 0.0f || currentPos.x >= static_cast<float>(dims.width) ||
            currentPos.y < 0.0f || currentPos.y >= static_cast<float>(dims.height))
        {
            // Reset scanner position if out of bounds
            imageScanner->resetPosition();
            currentPos = imageScanner->getCurrentPosition();
        }
        
        // Get RGB data from current position with additional safety
        RGB pixelData{0, 0, 0};
        try
        {
            pixelData = loader->getAreaAverage(currentPos.x, currentPos.y, areaSize);
        }
        catch (...)
        {
            // If any exception occurs, use silence and continue
            pixelData = RGB{0, 0, 0};
        }
        
        // Convert RGB to audio sample
        float audioSample = audioSynthesis->rgbToAudio(pixelData, ConversionFormula::RGBAverage);
        
        // Clamp audio sample to prevent clipping/distortion
        audioSample = juce::jlimit(-1.0f, 1.0f, audioSample);
        
        // Write to all output channels
        for (int channel = 0; channel < numChannels; ++channel)
        {
            buffer.setSample(channel, sample, audioSample);
        }
    }
}

//==============================================================================
bool NeedlesAudioProcessor::hasEditor() const
{
    return true; // We want an editor
}

juce::AudioProcessorEditor* NeedlesAudioProcessor::createEditor()
{
    return new NeedlesAudioProcessorEditor(*this);
}

//==============================================================================
void NeedlesAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // State persistence implementation will be expanded in Phase 2
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NeedlesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // State restoration implementation will be expanded in Phase 2
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
    parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// Image loading integration
bool NeedlesAudioProcessor::loadImage(const juce::String& filePath)
{
    // Thread-safe image loading - prevent audio thread access during loading
    juce::ScopedLock lock(imageMutex);
    
    if (!imageLoader)
    {
        lastErrorMessage = "Image loader not initialized";
        DBG("Needles: Error - " << lastErrorMessage);
        return false;
    }
    
    // Validate file path
    if (filePath.isEmpty())
    {
        lastErrorMessage = "Empty file path provided";
        DBG("Needles: Error - " << lastErrorMessage);
        return false;
    }
    
    // Check if file exists
    juce::File imageFile(filePath);
    if (!imageFile.existsAsFile())
    {
        lastErrorMessage = "File does not exist: " + filePath;
        DBG("Needles: Error - " << lastErrorMessage);
        return false;
    }
    
    // Check file size (prevent loading extremely large files)
    auto fileSize = imageFile.getSize();
    const int64_t maxFileSize = 100 * 1024 * 1024; // 100MB limit
    if (fileSize > maxFileSize)
    {
        lastErrorMessage = "File too large: " + juce::String(fileSize / (1024 * 1024)) + "MB (max 100MB)";
        DBG("Needles: Error - " << lastErrorMessage);
        return false;
    }
    
    // Check file extension for supported formats
    auto extension = imageFile.getFileExtension().toLowerCase();
    juce::StringArray supportedFormats = { ".jpg", ".jpeg", ".png", ".gif", ".bmp" };
    if (!supportedFormats.contains(extension))
    {
        lastErrorMessage = "Unsupported file format: " + extension + 
                          " (supported: " + supportedFormats.joinIntoString(", ") + ")";
        DBG("Needles: Error - " << lastErrorMessage);
        return false;
    }
    
    // Attempt to load the image
    LoadResult result = imageLoader->loadImage(filePath.toStdString());
    
    if (result.success)
    {
        // Verify image dimensions are valid
        auto dimensions = imageLoader->getDimensions();
        if (dimensions.width <= 0 || dimensions.height <= 0)
        {
            lastErrorMessage = "Invalid image dimensions: " + 
                              juce::String(dimensions.width) + "x" + juce::String(dimensions.height);
            DBG("Needles: Error - " << lastErrorMessage);
            isProcessingActive = false;
            return false;
        }
        
        // Check minimum image size
        if (dimensions.width < 2 || dimensions.height < 2)
        {
            lastErrorMessage = "Image too small for audio synthesis (minimum 2x2 pixels): " + 
                              juce::String(dimensions.width) + "x" + juce::String(dimensions.height);
            DBG("Needles: Error - " << lastErrorMessage);
            isProcessingActive = false;
            return false;
        }
        
        // Initialize scanner with image dimensions
        imageScanner->initialize(dimensions.width, dimensions.height);
        imageScanner->setLooping(true); // Enable infinite looping for US1
        
        // Clear any previous errors
        lastErrorMessage.clear();
        
        // Audio will start automatically on next processBlock
        isProcessingActive = true;
        
        DBG("Needles: Image loaded successfully - " << filePath << " (" << dimensions.width << "x" << dimensions.height << ")");
        return true;
    }
    else
    {
        // Handle various error types from ImageLoader
        lastErrorMessage = "Failed to load image";
        if (!result.errorMessage.empty())
        {
            lastErrorMessage += ": " + juce::String(result.errorMessage.c_str());
        }
        
        // Add user-friendly error interpretation
        if (result.errorMessage.find("format") != std::string::npos || 
            result.errorMessage.find("decode") != std::string::npos ||
            result.errorMessage.find("invalid") != std::string::npos)
        {
            lastErrorMessage += " (The file may be corrupted or in an unsupported format)";
        }
        else if (result.errorMessage.find("memory") != std::string::npos)
        {
            lastErrorMessage += " (Insufficient memory to load image)";
        }
        else if (result.errorMessage.find("access") != std::string::npos ||
                 result.errorMessage.find("permission") != std::string::npos)
        {
            lastErrorMessage += " (File access denied)";
        }
        
        DBG("Needles: " << lastErrorMessage);
        isProcessingActive = false;
        return false;
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NeedlesAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Core scanning parameters (User Story 1 & 2)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "scanSpeed",
        "Scan Speed",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f),
        1.0f,
        "x"));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        "areaSize",
        "Area Size",
        1, 50,
        5,
        " pixels"));

    // Audio output parameters (User Story 2)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "outputGain",
        "Output Gain", 
        juce::NormalisableRange<float>(0.0f, 2.0f, 0.01f),
        1.0f,
        " dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "leftWeight",
        "Left Channel Weight",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "rightWeight", 
        "Right Channel Weight",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f));

    // Advanced synthesis parameters (User Story 3)
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "conversionFormula",
        "Conversion Formula",
        juce::StringArray{"RGB Average", "Weighted RGB", "Red Channel", "Green Channel", "Blue Channel", "Max Channel", "Min Channel"},
        0));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "scanPattern",
        "Scan Pattern", 
        juce::StringArray{"Horizontal", "Vertical", "Diagonal", "Spiral"},
        0));

    return {params.begin(), params.end()};
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeedlesAudioProcessor();
}