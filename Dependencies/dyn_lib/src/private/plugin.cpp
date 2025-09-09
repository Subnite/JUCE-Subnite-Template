#include "plugin.h"

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"
#include "DSP/PluginProcessor.h"
#include <unordered_map>
#include "parameter_t.h"
#include "observer_t.h"
#include "juce_init.h"

using namespace subnite::dynlib;

struct Plugin::Impl {
    std::shared_ptr<Observer<MyPluginProcessor>> dsp;
    juce::MidiBuffer midi{}; // empty
    std::unordered_map<PluginParameter, std::unique_ptr<Parameter>> params;
    Impl() {
        dsp = std::make_shared<Observer<MyPluginProcessor>>();

        // fill in params
        params[PluginParameter::Power] = std::make_unique<Parameter>(0.f, 1.f, 0.f);
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
    SetParameter(PluginParameter::Power, state.power);

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
    subnite::dynlib::MessageManagerLock lock{};
    impl->dsp->obj = std::make_unique<MyPluginProcessor>();
    impl->dsp->obj->prepareToPlayFull(sampleRate, bufferSize, inChannels, outChannels);
    wasPrepared = true;
    return PluginResult::Success;
}

void Plugin::WriteParamsToState() {
    state.power = impl->params[PluginParameter::Power]->Get();
}

}
