#pragma once

#include "TParameter.h"
#include "TObserver.h"
#include "DSP/PluginProcessor.h"
#include <string>
#include <memory>

namespace source {

class NoteParameter : public Parameter {
public:
    NoteParameter(std::shared_ptr<Observer<MyPluginProcessor>>dsp);
    inline static const std::string notes[12] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};

protected:
    float NormalizedToRanged(float normalized) const override;
    float RangedToNormalized(float ranged) const override;
    void ValueChangedCallback(const float &newValue) override;

private:
    std::shared_ptr<Observer<MyPluginProcessor>> dsp;
};


class JuceFloatParameter : public Parameter {
public:
    JuceFloatParameter(float min, float max, float defaultValue, std::string APVTSParamID, std::shared_ptr<Observer<MyPluginProcessor>> dsp);

protected:
    void ValueChangedCallback(const float& newValue) override;

private:
    std::string id;
    std::shared_ptr<Observer<MyPluginProcessor>> dsp;
};



class DefaultParameter : public Parameter {
public:
    DefaultParameter(float min, float max, float defaultValue)
    : Parameter(min, max, defaultValue)
    {}
};



class ScaleParameter : public Parameter {
public:
    ScaleParameter(std::shared_ptr<Observer<MyPluginProcessor>> dsp);
protected:
    float NormalizedToRanged(float normalized) const override;
    float RangedToNormalized(float ranged) const override;
    void ValueChangedCallback(const float &newValue) override;

private:
    std::shared_ptr<Observer<MyPluginProcessor>> dsp;
};

} // namespace
