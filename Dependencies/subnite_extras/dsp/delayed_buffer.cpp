/*
  ==============================================================================

    DelayedBuffer.cpp
    Created: 25 Jun 2023 2:55:33am
    Author:  Subnite

  ==============================================================================
*/

#include "delayed_buffer.h"

using namespace subnite;

DelayedBuffer::DelayedBuffer(const int& inChannels, const int& numSamples, const int& samplesLatencyAmount)
	: buffer(inChannels, numSamples + samplesLatencyAmount),
	overflowBuffer(inChannels, numSamples % samplesLatencyAmount),
	numChannels(inChannels), blockSize(numSamples), latencySize(samplesLatencyAmount)
{
	jassert(latencySize >= 0);
	FillBuffersWithZero();
}

void DelayedBuffer::FillBuffersWithZero()
{
	for (int channel = 0; channel < numChannels; channel++)
	{
		for (int sample = 0; sample < buffer.getNumSamples(); sample++)
		{
			if (sample < overflowBuffer.getNumSamples())
				overflowBuffer.setSample(channel, sample, 0.0f);

			buffer.setSample(channel, sample, 0.0f);
		}
	}
}


void DelayedBuffer::FillBuffer(juce::AudioBuffer<float>& inputBuffer)
{
	juce::AudioBuffer<float> tempBuffer; // needed because you can't copy from yourself :/
	tempBuffer.makeCopyOf(buffer);

	for (int channel = 0; channel < numChannels; channel++)
	{
		// shift everything down by blockSize samples
		buffer.copyFrom(channel, 0, tempBuffer, channel, blockSize, buffer.getNumSamples() - blockSize);

		// copy buffer to last part of delayed buffer
		buffer.copyFrom(channel, buffer.getNumSamples() - blockSize, inputBuffer, channel, 0, blockSize);
	}
}

const juce::AudioBuffer<float>* DelayedBuffer::GetBuffer() const
{
	return &buffer;
}

void DelayedBuffer::SetBufferToDelayedBuffer(juce::AudioBuffer<float>& outputBuffer) const
{
	for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
	{
		outputBuffer.copyFrom(channel, 0, buffer, channel, 0, blockSize);
	}
}
