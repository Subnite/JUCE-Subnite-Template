/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "Commons/PluginValueTree.h"

//==============================================================================

struct BusSettings {
    size_t sampleRate = 48000;
    size_t bufferSize = 512;
    size_t channels = 2;
};

class MyPluginProcessor  : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
, public juce::AudioProcessorARAExtension
#endif
{
public:
    //==============================================================================
    MyPluginProcessor();
    ~MyPluginProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    using juce::AudioProcessor::processBlock; // there is a "double" version of processblock. This let's the bass class handle it.
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    // void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) = delete;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================

    // contains all info that is stored and restored from the plugin data block
    myplugin::vt::ValueTree vTree{};
private:

  BusSettings busSettings;
  void busSettingsChanged(BusSettings newSettings);
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPluginProcessor)
};
