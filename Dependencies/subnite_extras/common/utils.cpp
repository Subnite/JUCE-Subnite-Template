/*
  ==============================================================================

    Utils.cpp
    Created: 3 Jun 2023 10:43:19pm
    Author:  Subnite

  ==============================================================================
*/

#include "utils.h"

// =============================================================================

template <typename T>
void subnite::utils::FitImageToBounds(const juce::Rectangle<T>& bounds, juce::Image& image)
{
    if (image.getHeight() > image.getWidth()) {
        image = image.rescaled(static_cast<int>((float(image.getWidth()) / image.getHeight()) * bounds.getHeight()), bounds.getHeight());
    }
    else {
        image = image.rescaled(bounds.getWidth(), static_cast<int>((float(image.getHeight()) / image.getWidth()) * bounds.getWidth()));
    }
}

template <typename T>
void subnite::utils::ClampToBounds(juce::Point<T>& point, const juce::Rectangle<T>& bounds) {
    if (point.x > bounds.getRight()) point.x = bounds.getRight();
    else if (point.x < 0) point.x = 0;

    if (point.y > bounds.getHeight()) point.y = bounds.getHeight();
    else if (point.y < 0) point.y = 0;
}

template <typename T>
juce::AffineTransform subnite::utils::CenterImageAndRotate(const juce::Image& image,
    const juce::Rectangle<T>& bounds,
    const float& angRadians)
{
    juce::AffineTransform trans;

    trans = trans.translated(-image.getWidth() * 0.5f, -image.getHeight() * 0.5f);
    trans = trans.rotated(angRadians);
    trans = trans.translated(bounds.getWidth() * 0.5f, bounds.getHeight() * 0.5f);

    return trans;
}


// =============================================================================
// Bounds

template <typename T>
void subnite::utils::RemoveFromBoundsCentered(juce::Rectangle<T>& bounds, const double& widthMultiplier, const double& heightMultiplier)
{
    jassert(widthMultiplier >= 0 && widthMultiplier < 0.5);

    bounds.removeFromTop(static_cast<int>(bounds.getHeight() * heightMultiplier));
    bounds.removeFromBottom(static_cast<int>(bounds.getHeight() * (1 / (1 - heightMultiplier)) * heightMultiplier));

    bounds.removeFromLeft(static_cast<int>(bounds.getWidth() * widthMultiplier));
    bounds.removeFromRight(static_cast<int>(bounds.getWidth() * (1 / (1 - widthMultiplier)) * widthMultiplier));

}

template <typename T>
T subnite::utils::GetPosFromFreq(const double& frequency, const juce::Rectangle<T>& bounds)
{
    using namespace juce;
    auto doubleBounds = bounds.toDouble();

    double mapped = mapFromLog10(frequency, 20.0, 20000.0);
    double pos = jmap(mapped, 0.0, 1.0, doubleBounds.getX(), doubleBounds.getX() + doubleBounds.getWidth());
    return static_cast<int>(pos);
}

template <typename T>
double subnite::utils::GetFreqFromPos(const T& pos, const juce::Rectangle<T>& bounds)
{
    using namespace juce;

    double normalizedPos = jmap(double(pos), 0.0, double(bounds.getWidth()), 0.0, 1.0);
    double freq = mapToLog10(normalizedPos, 20.0, 20000.0);
    return freq;
}

