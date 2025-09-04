#pragma once
#include "common.h"

namespace source {

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
  virtual void ValueChangedCallback(const float& newRangedValue);
  // decides if the value has changed, and then updates it.
  virtual bool SetIfChanged(float& dest, float value);

private:
  float value;
};


}
