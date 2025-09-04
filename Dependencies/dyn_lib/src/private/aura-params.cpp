#include "aura-params.h"

using namespace source;

NoteParameter::NoteParameter(std::shared_ptr<Observer<MyPluginProcessor>> dsp)
: Parameter(0, 11, 0), dsp(dsp)
{}

float NoteParameter::NormalizedToRanged(float normalized) const {
    size_t noteIdx = static_cast<size_t>(normalized*max + .5f);
    if (noteIdx > 11) noteIdx = 11;
    return static_cast<float>(noteIdx);
}

float NoteParameter::RangedToNormalized(float ranged) const {
    size_t noteIdx = static_cast<size_t>(ranged + .5f);
    if (noteIdx > 11) noteIdx = 11;
    return ranged/noteIdx;
}

void NoteParameter::ValueChangedCallback(const float& newValue) {
    if (dsp->obj.get() == nullptr) return;
    size_t noteIdx = static_cast<size_t>(newValue + .5f);
    auto note = notes[noteIdx];
    dsp->obj->UpdateMode(note);
}

// =======================================================

JuceFloatParameter::JuceFloatParameter(float min, float max, float defaultValue, std::string APVTSParamID,
std::shared_ptr<Observer<MyPluginProcessor>> dsp
)
: Parameter(min, max, defaultValue),
id(APVTSParamID),
dsp(dsp)
{ }

void JuceFloatParameter::ValueChangedCallback(const float& newValue) {
    if (dsp->obj.get() == nullptr) return;
    auto param = dsp->obj->apvts.getParameter(id);

    float normalized = RangedToNormalized(newValue);
    param->setValueNotifyingHost(normalized);
}

// =======================================================

ScaleParameter::ScaleParameter(std::shared_ptr<Observer<MyPluginProcessor>> dsp)
: Parameter(0.f, static_cast<float>((size_t)subnite::Scale::LAST_ENUM - 1), static_cast<float>(subnite::Scale::PentatonicMinor)),
dsp(dsp)
{}

float ScaleParameter::NormalizedToRanged(float normalized) const {
    size_t scaleIdx = static_cast<size_t>(normalized*max + .5f);
    if (scaleIdx > (size_t)subnite::Scale::LAST_ENUM - 1) scaleIdx = (size_t)subnite::Scale::LAST_ENUM - 1;
    return static_cast<float>(scaleIdx);
}

float ScaleParameter::RangedToNormalized(float ranged) const {
    size_t scaleIdx = static_cast<size_t>(ranged + .5f);
    if (scaleIdx > (size_t)subnite::Scale::LAST_ENUM - 1) scaleIdx = (size_t)subnite::Scale::LAST_ENUM - 1;
    return ranged/scaleIdx;
}

void ScaleParameter::ValueChangedCallback(const float& newValue) {
    if (dsp->obj.get() == nullptr) return;
    size_t scaleIdx = static_cast<size_t>(newValue + .5f);
    dsp->obj->UpdateMode(static_cast<subnite::Scale>(scaleIdx));
}
