#include "parameter_t.h"

using namespace subnite::dynlib;

void Parameter::Set(float rangedValue) {
    if (rangedValue > this->max) rangedValue = this->max;
    else if (rangedValue < this->min) rangedValue = this->min;

    bool wasChanged = SetIfChanged(this->value, rangedValue);
    if (wasChanged) ValueChangedCallback(rangedValue);
}

void Parameter::SetNormalized(float normalizedValue) {
    // clamp normalized
    if (normalizedValue < 0.f) normalizedValue = 0.f;
    else if (normalizedValue > 1.f) normalizedValue = 1.f;

    float val = NormalizedToRanged(normalizedValue);
    bool wasChanged = SetIfChanged(this->value, val);
    if (wasChanged) ValueChangedCallback(val);
}

float Parameter::Get() const {
    return this->value;
}

float Parameter::GetNormalized() const {
    return RangedToNormalized(this->value);
}

Parameter::Parameter(float min, float max, float defaultValue)
: min(min), max(max), value(defaultValue)
{}

float Parameter::NormalizedToRanged(float normalized) const {
    normalized *= (max-min);
    normalized += min;
    return normalized;
}

float Parameter::RangedToNormalized(float ranged) const {
    ranged -= min;
    ranged /= (max-min);
    return ranged;
}

void Parameter::ValueChangedCallback(const float& newRangedValue)
{}

bool Parameter::SetIfChanged(float& dest, float value) {
    bool differs = dest != value;
    if (differs) dest = value;
    return differs;
}
