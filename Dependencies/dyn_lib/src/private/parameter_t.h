#pragma once
#include "common.h"
#include <string>
#include <memory>
#include "observer_t.h"

namespace subnite::dynlib
{

class Parameter {
public:
    Parameter() = delete;
    Parameter(float min, float max, float defaultValue);
    void Set(float rangedValue);
    void SetNormalized(float normalizedValue);
    float Get() const;
    float GetNormalized() const;

protected:
    const float min;
    const float max;

    // defaults to a linear remap
    virtual float NormalizedToRanged(float normalized) const;
    // defaults to a linear remap
    virtual float RangedToNormalized(float ranged) const;
    // called when the value has succesfully changed
    virtual void ValueChangedCallback(const float &newRangedValue);
    // decides if the value has changed, and then updates it.
    virtual bool SetIfChanged(float &dest, float value);

private:
    float value;
};

} // namespace
