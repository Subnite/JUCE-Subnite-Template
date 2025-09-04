#include "plugin.h"

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "DSP/PluginProcessor.h"
#include <unordered_map>
#include "TParameter.h"
#include "TObserver.h"
#include "aura-params.h"
#include "juce-init.h"

using namespace source;

struct Plugin::Impl {
    std::shared_ptr<Observer<MyPluginProcessor>> dsp;
    juce::MidiBuffer midi{}; // empty
    std::unordered_map<PluginParameter, std::unique_ptr<Parameter>> params;
    Impl() {
        dsp = std::make_shared<Observer<MyPluginProcessor>>();

        // fill in params
        params[PluginParameter::GainNormalized] = std::make_unique<DefaultParameter>(0.f, 1.f, 1.f);
        params[PluginParameter::RootNote] = std::make_unique<NoteParameter>(dsp);
        params[PluginParameter::Scale] = std::make_unique<ScaleParameter>(dsp);
        params[PluginParameter::GainAdd] = std::make_unique<JuceFloatParameter>(0.f, 16.f, 11.f, ADDITION_GAIN_ID, dsp);
        params[PluginParameter::GainReduce] = std::make_unique<JuceFloatParameter>(0.f, 48.f, 25.f, REDUCTION_GAIN_ID, dsp);
        params[PluginParameter::LowerFreq] = std::make_unique<JuceFloatParameter>(20.f, 20000.f, 250.f, LOW_FREQ_ID, dsp);
        params[PluginParameter::UpperFreq] = std::make_unique<JuceFloatParameter>(20.f, 20000.f, 1500.f, HIGH_FREQ_ID, dsp);
        params[PluginParameter::Quality] = std::make_unique<JuceFloatParameter>(10.f, 180.f, 90.f, FILTER_QUALITY_ID, dsp);
        params[PluginParameter::SpectralGateLower] = std::make_unique<JuceFloatParameter>(0.f, 1.f, 0.3f, SPECTRAL_GATE_LOWER_THRESH_ID, dsp);
        params[PluginParameter::SpectralGateUpper] = std::make_unique<JuceFloatParameter>(0.f, 1.f, 0.5f, SPECTRAL_GATE_UPPER_THRESH_ID, dsp);
        params[PluginParameter::SpectralGateFalloff] = std::make_unique<JuceFloatParameter>(-24.f, 0.f, -16.f, SPECTRAL_GATE_FALLOFF_ID, dsp);
    }
    ~Impl() = default;
};

extern "C" {

Plugin::Plugin() {
    impl = std::make_shared<Impl>();
}
Plugin::~Plugin() {
    // it's recommended to message manager lock this
    if (impl->dsp->obj) {
        impl->dsp->obj->releaseResources();
        impl->dsp->obj.reset();
    }
    if (impl->dsp) impl->dsp.reset();
    if (impl) impl.reset();
}

PluginResult Plugin::SetParameter(const PluginParameter &parameterType, float newValue)
{
    impl->params[parameterType]->Set(newValue);
    // TODO: this doesn't update the PluginState state variable.

    return PluginResult::Success;
}


PluginResult Plugin::SetState(const PluginState &state) {
    this->state = state;

    // update params in beautiful way (ew)
    SetParameter(PluginParameter::GainNormalized, state.gainNormalized);
    SetParameter(PluginParameter::RootNote, state.rootNote);
    SetParameter(PluginParameter::Scale, state.scale);
    SetParameter(PluginParameter::GainAdd, state.gainAddDB);
    SetParameter(PluginParameter::GainReduce, state.gainReduceDB);
    SetParameter(PluginParameter::LowerFreq, state.lowerFreq);
    SetParameter(PluginParameter::UpperFreq, state.upperFreq);
    SetParameter(PluginParameter::Quality, state.quality);
    SetParameter(PluginParameter::SpectralGateLower, state.spectralGateLower);
    SetParameter(PluginParameter::SpectralGateUpper, state.spectralGateUpper);
    SetParameter(PluginParameter::SpectralGateFalloff, state.spectralGateFalloff);

    // WARNING: this would only work if the dsp was initialized of course. Make sure to do it before setting the state!
    // if (dsp) dsp->setStateInformation(
    //     this->state.juceStateInformation.getData(),
    //     static_cast<int>(this->state.juceStateInformation.getSize())
    // );
    return PluginResult::Success;
}

PluginResult Plugin::Process(const float** inputBuffer, float** outputBuffer, const size_t& bufferSize, const size_t& numChannels) {
    if (numChannels == 0 || numChannels > 2) return PluginResult::FailedToProcess;
    if (inputBuffer == nullptr || outputBuffer == nullptr) return PluginResult::FailedToProcess;
    for (size_t channel = 0; channel < numChannels; ++channel) {
        if (inputBuffer[channel] == nullptr || outputBuffer[channel] == nullptr) return PluginResult::FailedToProcess;
    }

    if (!wasPrepared) {
        Prepare(48000.0, bufferSize, numChannels, numChannels);
    }

    if (!impl) return PluginResult::FailedToProcess;
    impl->midi.clear();

    // copy input to output buffer
    for (size_t channel = 0; channel < numChannels; ++channel)
        std::memcpy(outputBuffer[channel], inputBuffer[channel], bufferSize * sizeof(float));

    juce::AudioBuffer<float> buf{outputBuffer, static_cast<int>(numChannels), static_cast<int>(bufferSize)};
    this->impl->dsp->obj->processBlock(buf, this->impl->midi);

    for (size_t channel = 0; channel < numChannels; ++channel) {
        for (size_t i = 0; i < bufferSize; ++i) {
            outputBuffer[channel][i] = outputBuffer[channel][i] * impl->params[PluginParameter::GainNormalized]->GetNormalized();
        }
    }

    return PluginResult::Success;
}

PluginResult Plugin::GetState(PluginState & stateToOverwrite) {
    WriteParamsToState(); // update state first
    stateToOverwrite = state;

    // if (state.juceStateInformation.getSize() > 0) state.juceStateInformation.reset();
    // dsp->getStateInformation(state.juceStateInformation);

    return PluginResult::Success;
}

PluginResult Plugin::Prepare(double sampleRate, size_t bufferSize, size_t inChannels, size_t outChannels) {
    // if (dsp) dsp.reset();
    source::MessageManagerLock lock{};
    impl->dsp->obj = std::make_unique<MyPluginProcessor>();
    impl->dsp->obj->prepareToPlayFull(sampleRate, bufferSize, inChannels, outChannels);
    wasPrepared = true;
    return PluginResult::Success;
}

void Plugin::WriteParamsToState() {
    state.rootNote = impl->params[PluginParameter::RootNote]->Get();
    state.scale = impl->params[PluginParameter::Scale]->Get();
    state.gainNormalized = impl->params[PluginParameter::GainNormalized]->Get();
    state.lowerFreq = impl->params[PluginParameter::LowerFreq]->Get();
    state.upperFreq = impl->params[PluginParameter::UpperFreq]->Get();
    state.gainAddDB = impl->params[PluginParameter::GainAdd]->Get();
    state.gainReduceDB = impl->params[PluginParameter::GainReduce]->Get();
    state.quality = impl->params[PluginParameter::Quality]->Get();
    state.spectralGateLower = impl->params[PluginParameter::SpectralGateLower]->Get();
    state.spectralGateUpper = impl->params[PluginParameter::SpectralGateUpper]->Get();
    state.spectralGateFalloff = impl->params[PluginParameter::SpectralGateFalloff]->Get();
}

}
