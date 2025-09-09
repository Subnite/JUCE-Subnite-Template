#pragma once
#include "common.h"
#include <memory>

namespace subnite::dynlib {

enum class MY_API PluginResult {
    Success,
    FailedToSetState,
    ParameterDoesntExist,
    ParameterTypeDoesntMatch,
    FailedToProcess,
    FailedToReturnState
};

enum MY_API PluginParameter {
    Power,
    Count
};

struct MY_API PluginState {
    float power = 1.0f;
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
