/*
  ==============================================================================

    Utils.h
    Created: 3 Jun 2023 10:43:19pm
    Author:  Subnite

  ==============================================================================
*/

#pragma once
#include <juce_graphics/juce_graphics.h>

namespace subnite::utils
{
    // fits image to fully fit inside bounds
    template <typename T>
    void FitImageToBounds(const juce::Rectangle<T>& bounds, juce::Image& image);

    template <typename T>
    void ClampToBounds(juce::Point<T>& point, const juce::Rectangle<T>& bounds);

    // returns a transform that rotates an image around its center
    template <typename T>
    juce::AffineTransform CenterImageAndRotate(
        const juce::Image& image,
        const juce::Rectangle<T>& bounds,
        const float& angRadians
    );

    // removes an amount from the edges of the bounds and keeps centered.
    template <typename T>
    void RemoveFromBoundsCentered(juce::Rectangle<T>& bounds,
        const double& widthMultiplier, const double& heightMultiplier);

    // returns the position of a frequency in the bounds width
    template <typename T>
    T GetPosFromFreq(const double& frequency, const juce::Rectangle<T>& bounds);

    // returns the frequency at a given position from bounds width.
    template <typename T>
    double GetFreqFromPos(const T& pos, const juce::Rectangle<T>& bounds);

} // namespace utils

