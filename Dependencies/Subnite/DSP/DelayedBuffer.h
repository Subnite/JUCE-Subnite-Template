/*
  ==============================================================================

    DelayedBuffer.h
    Created: 25 Jun 2023 2:55:33am
    Author:  Subnite

  ==============================================================================
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

namespace subnite
{
	class DelayedBuffer
	{
	private:
		juce::AudioBuffer<float> buffer;
		juce::AudioBuffer<float> overflowBuffer;

		int numChannels;
		int blockSize;
		int latencySize;
		int samplesAdded = 0;

		void FillBuffersWithZero();
	public:
		DelayedBuffer(const int& numChannels, const int& blockSize, const int& samplesLatencyAmount);

		void FillBuffer(juce::AudioBuffer<float>& inputBuffer);

		const juce::AudioBuffer<float>* GetBuffer() const;
		void SetBufferToDelayedBuffer(juce::AudioBuffer<float>& outputBuffer) const;
	};
}
