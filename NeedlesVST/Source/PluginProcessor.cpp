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
    // Constructor implementation will be expanded in Phase 2
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
    if (!isProcessingActive)
    {
        buffer.clear();
        return;
    }

    // Phase 2 Foundation Complete - interfaces ready for implementation:
    // 1. Get current parameters from ValueTreeState (parameterManager)
    // 2. Check if image is loaded (imageLoader)
    // 3. For each audio sample:
    //    - Advance scan position (imageScanner) 
    //    - Get RGB pixel data (imageLoader)
    //    - Convert to audio sample (audioSynthesis)
    //    - Apply gain and channel weights (parameterManager)
    // 4. Fill output buffer with generated audio

    // User Story implementations will replace this placeholder
    buffer.clear();
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