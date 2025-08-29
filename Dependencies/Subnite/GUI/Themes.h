/*
  ==============================================================================

    Themes.h
    Created: 17 Jun 2023 9:02:51pm
    Author:  Subnite

  ==============================================================================
*/

#pragma once
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace subnite::themes
{
    using namespace juce;

    struct CustomColours
    {
        Colour text;

        Colour comboBackground;
        Colour comboButton;
        Colour comboFocusedOutline;
        Colour comboArrow;
        Colour comboOutline;

        Colour rotarySliderBg;
        Colour rotarySliderOutline;
        Colour rotarySliderThumb;
        Colour sliderTrack;

        Colour tick;
        Colour tickDisabled;
    };

    class CustomTheme : public LookAndFeel_V4
    {
        CustomColours colors;

    public:
        CustomTheme() {};

        void SetColours(const CustomColours& c, LookAndFeel* LnF);

        CustomColours GetColours() const { return colors; };

        void SetThemeColoursOnLnF(LookAndFeel* LnF)
    	{
            SetColours(colors, LnF);
        }

        void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
            float sliderPosProportional, float rotaryStartAngle,
            float rotaryEndAngle, Slider&) override;

        void drawToggleButton(Graphics& g, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    };
}

