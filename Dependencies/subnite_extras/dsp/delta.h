/*
  ==============================================================================

    Delta.h
    Created: 8 Jun 2023 9:35:37pm
    Author:  Subnite

  ==============================================================================
*/

#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

namespace subnite
{
	template <typename bufferType>
	class Delta
	{
	private:
		juce::AudioBuffer<bufferType> inputBuffer;

		void ProcessSample(const int& channelIndex, const int& sampleIndex, juce::AudioBuffer<bufferType>& buffer) const
		{
			bufferType newVal = buffer.getSample(channelIndex, sampleIndex) - inputBuffer.getSample(channelIndex, sampleIndex);
			buffer.setSample(channelIndex, sampleIndex, newVal);
		}
	public:
		// this will take in what will be considered the input buffer data
		void CopyInputBuffer(const juce::AudioBuffer<bufferType>& buffer)
		{
			inputBuffer.makeCopyOf(buffer);
		}

		// this will change the buffer to the difference with the input buffer
		void Process(juce::AudioBuffer<bufferType>& buffer) const
		{
			for (int channel{}; channel < buffer.getNumChannels(); channel++)
			{
				for (int sample{}; sample < buffer.getNumSamples(); sample++)
				{
					ProcessSample(channel, sample, buffer);
				}
			}
		}

		template<typename FloatType, typename CountType>
		static juce::AudioBuffer<FloatType> ProcessDelta(const juce::AudioBuffer<FloatType>& buffer1, const juce::AudioBuffer<FloatType>& buffer2) {
			jassert(buffer1.numSamples() == buffer2.numSamples());
			CountType numSamples = static_cast<CountType>(buffer1.numSamples());

			juce::AudioBuffer<FloatType> newBuffer{};

			for (int i = 0; i < buffer1.numChannels(); i++){
				juce::FloatVectorOperationsBase<FloatType, CountType>::subtract(newBuffer.getWritePointer(i), buffer1.getReadPointer(), buffer2.getReadPointer(), numSamples);
			}
		}
	};
}
