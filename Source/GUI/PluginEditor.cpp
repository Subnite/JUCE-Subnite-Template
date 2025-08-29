/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

//==============================================================================
MyPluginEditor::MyPluginEditor(MyPluginProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 500);
    setResizable(true, true);
    setResizeLimits(400, 250, 1500, 1000);

    // make your components visible as well.
}

MyPluginEditor::~MyPluginEditor()
{
}

//==============================================================================
void MyPluginEditor::paintOverChildren(juce::Graphics &g)
{
}

void MyPluginEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::purple.withSaturation(0.5f).withBrightness(0.10f));
}

void MyPluginEditor::resized()
{
    auto bounds = getLocalBounds();
}
