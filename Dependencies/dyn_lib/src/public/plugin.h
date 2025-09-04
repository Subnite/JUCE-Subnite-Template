#pragma once
#include "common.h"
#include <memory>

namespace source {
enum class MY_API PluginResult {
    Success,
    FailedToSetState,
    ParameterDoesntExist,
    ParameterTypeDoesntMatch,
    FailedToProcess,
    FailedToReturnState
};

enum MY_API PluginParameter {
    GainNormalized,
    RootNote,
    Scale,
    GainAdd,
    GainReduce,
    LowerFreq,
    UpperFreq,
    Quality,
    SpectralGateLower,
    SpectralGateUpper,
    SpectralGateFalloff,
    Count
};

struct MY_API PluginState {
    float gainNormalized = 1.0f;
    float rootNote = 0; // 12 keys available [A, A#, B, C, C#, D, D#, E, F, F#, G, G#]
    float scale = 0; // 15 available?
    float lowerFreq = 250.f; // in Hz
    float upperFreq = 1500.f; // in Hz
    float gainAddDB = 11.f;
    float gainReduceDB = 25.f;
    float quality = 90.f;
    float spectralGateLower = .3f;
    float spectralGateUpper = .5f;
    float spectralGateFalloff = -16.f;
    // juce::MemoryBlock juceStateInformation;
};

class MY_API Plugin {
public:
    Plugin();
    ~Plugin();
    PluginResult SetParameter(const PluginParameter& parameterType, float newValue); // only working with floats because templates are a mess in libraries.
    PluginResult SetState(const PluginState& state);
    PluginResult Process(const float** inputBuffer, float** outputBuffer, const size_t& bufferSize, const size_t& numChannels);
    PluginResult GetState(PluginState& stateToOverwrite);
    PluginResult Prepare(double sampleRate, size_t bufferSize, size_t inChannels, size_t outChannels);
private:
    void WriteParamsToState();
    PluginState state{};
    bool wasPrepared = false;

    struct Impl;
    std::shared_ptr<Impl> impl; // hiding the implementation. Shared pointers don't need a complete type at declaration or destruction.
};


} // namespace
