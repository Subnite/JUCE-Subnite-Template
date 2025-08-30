/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "GUI/PluginEditor.h"
#include <algorithm>

//==============================================================================
MyPluginProcessor::MyPluginProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
                       : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
.withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
.withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                       )
#endif
{
    gain = rs::create(0.5);
    if (!vTree.IsValid()) vTree.Create();
}

MyPluginProcessor::~MyPluginProcessor()
{
    rs::destroy(gain);
    gain = nullptr;
}

//==============================================================================

void MyPluginProcessor::busSettingsChanged(size_t sampleRate, size_t samplesPerBlock, size_t channels) {
    jassert(channels >= 1);
}

void MyPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    // assuming stereo channels
    int channels = getMainBusNumInputChannels();
    busSettingsChanged(sampleRate, samplesPerBlock, channels); // didn't actually check if they changed, so this can be used as initializer too.
}

void MyPluginProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    if (totalNumInputChannels != this->numChannels || buffer.getNumSamples() != this->samplesPerBlock || getSampleRate() != sampleRate){
        this->samplesPerBlock = buffer.getNumSamples();
        this->numChannels = totalNumInputChannels;
        this->sampleRate = getSampleRate();
        busSettingsChanged(this->sampleRate, this->samplesPerBlock, this->numChannels);
    }

    // do processing here
    rs::process(gain, buffer.getReadPointer(0), buffer.getReadPointer(1),
        buffer.getWritePointer(0), buffer.getWritePointer(1),
        buffer.getNumSamples()
    );
}

const juce::String MyPluginProcessor::getName() const
{
    return juce::String("MyPlugin");
}

bool MyPluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MyPluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MyPluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MyPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyPluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int MyPluginProcessor::getCurrentProgram()
{
    return 0;
}

void MyPluginProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String MyPluginProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void MyPluginProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================

void MyPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyPluginProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

//==============================================================================
bool MyPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *MyPluginProcessor::createEditor()
{
    return new MyPluginEditor(*this);
    // return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MyPluginProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    juce::ignoreUnused(destData);
    // You should use this method to store your parameters in the memory block.

    juce::MemoryOutputStream stream(destData, true);
    if (vTree.IsValid())
    {
        vTree.WriteToStream(stream);

#ifdef CMAKE_DEBUG
        // vTree.CreateXML("C:/Path/To/MyPlugin/getStateOutputTree.xml");
#endif
    }
}

void MyPluginProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
    // You should use this method to restore your parameters from this memory block, whose contents will have been created by the getStateInformation() call.

    vTree.CopyFrom(data, sizeInBytes);
    if (!vTree.IsValid())
        vTree.Create(); // this shouldn't happen, unless there were breaking changes in an update

#ifdef CMAKE_DEBUG
    // vTree.CreateXML("C:/Path/To/MyPlugin/setStateOutputTree.xml");
#endif
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MyPluginProcessor();
}
