/*
  ==============================================================================

    Themes.cpp
    Created: 18 Jun 2023 1:15:38am
    Author:  Subnite

  ==============================================================================
*/

#include "Themes.h"

using namespace subnite::themes;

void CustomTheme::SetColours(const CustomColours& c, LookAndFeel* LnF)
{
    colors = c;

    LnF->setColour(ComboBox::backgroundColourId, c.comboBackground);
    LnF->setColour(ComboBox::buttonColourId, c.comboButton); // no clue what this does
    LnF->setColour(ComboBox::focusedOutlineColourId, c.comboFocusedOutline); // no clue what this does
    LnF->setColour(ComboBox::arrowColourId, c.comboArrow);
    LnF->setColour(ComboBox::outlineColourId, c.comboOutline);
    LnF->setColour(ComboBox::textColourId, c.text);

    LnF->setColour(PopupMenu::backgroundColourId, c.comboBackground);
    LnF->setColour(PopupMenu::textColourId, c.text);
    LnF->setColour(PopupMenu::highlightedBackgroundColourId, c.comboArrow);
    LnF->setColour(PopupMenu::highlightedTextColourId,
        c.comboArrow.contrasting(c.comboArrow, c.comboArrow));

    LnF->setColour(Slider::backgroundColourId, c.rotarySliderBg);
    LnF->setColour(Slider::rotarySliderOutlineColourId, c.rotarySliderOutline);
    LnF->setColour(Slider::thumbColourId, c.rotarySliderThumb);
    LnF->setColour(Slider::trackColourId, c.sliderTrack);
    LnF->setColour(Slider::textBoxTextColourId, c.text);
    LnF->setColour(Slider::textBoxOutlineColourId, c.text.withAlpha(0.f));

    LnF->setColour(ToggleButton::textColourId, c.text);
    LnF->setColour(ToggleButton::tickColourId, c.tick);
    LnF->setColour(ToggleButton::tickDisabledColourId, c.tickDisabled);

    LnF->setColour(Label::textColourId, c.text);
    LnF->setColour(Label::backgroundWhenEditingColourId, c.comboBackground);

    LnF->setColour(TooltipWindow::backgroundColourId, c.rotarySliderBg);
    LnF->setColour(TooltipWindow::textColourId, c.text);
    LnF->setColour(TooltipWindow::outlineColourId, c.comboFocusedOutline);
}

void CustomTheme::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider&)
{
     using namespace juce;

    Rectangle<int> bounds{ x, y, width, height };
    auto center = bounds.getCentre();


    Colour defaultC = findColour(Slider::backgroundColourId);
    Colour defaultContrasting = findColour(Slider::rotarySliderOutlineColourId);
    Colour thumbColour = findColour(Slider::thumbColourId);
    Colour thumb{ thumbColour.withSaturation(0.3f)};

    //thumbColour = Colours::blue;

    // ============== gradient behind knob
    Path circle;
    float circleSizeMult = 0.23f;
    circle.addCentredArc(static_cast<float>(center.getX()), static_cast<float>(center.getY()), bounds.getWidth() * circleSizeMult, bounds.getHeight() * circleSizeMult, 0.f, 0.f, MathConstants<float>::twoPi, true);

    auto gradientTest = ColourGradient(thumbColour, static_cast<float>(center.getX()), static_cast<float>(center.getY()),
        thumb, center.getX() + bounds.getWidth() * circleSizeMult, center.getY() + bounds.getHeight() * circleSizeMult, true);

    g.setGradientFill(gradientTest);
    g.fillPath(circle);

    // ============== background knob
    Path roundEllipse;

    auto floatCenter = center.toFloat();
    roundEllipse.addCentredArc(floatCenter.getX(), floatCenter.getY(),
        bounds.getWidth() * 0.5f, bounds.getHeight() * 0.5f,
        0.f, rotaryStartAngle, rotaryEndAngle, true);

    roundEllipse.addCentredArc(floatCenter.getX(), floatCenter.getY(), bounds.getWidth() * 0.3f, bounds.getWidth() * 0.3f, 0.f, rotaryEndAngle, rotaryStartAngle);
    roundEllipse.closeSubPath();

    g.setColour(defaultC);
    g.fillPath(roundEllipse);

    g.setColour(defaultContrasting);
    g.strokePath(roundEllipse, { 1.5f, PathStrokeType::JointStyle::mitered });


    // ============== triangle arrow thing
    Path p;

    float thickness = bounds.getWidth() * 0.05f;
    p.addTriangle(floatCenter.getX(), floatCenter.getY(),
    bounds.getCentreX() - thickness, bounds.getY()*1.f, bounds.getCentreX() + thickness, bounds.getY() * 1.f);

    jassert(rotaryStartAngle < rotaryEndAngle);

    auto sliderAngRadians = jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);
    AffineTransform rotateTransform = AffineTransform().rotated(sliderAngRadians, floatCenter.getX(), floatCenter.getY());

    p.applyTransform(rotateTransform);
    g.setColour(defaultContrasting);
    g.fillPath(p);

    // ============== progress line
    Path followerLine{};

    followerLine.addCentredArc(floatCenter.getX(), floatCenter.getY(),
        bounds.getWidth() * 0.5f, bounds.getHeight() * 0.5f,
        0.f, rotaryStartAngle, sliderAngRadians, true);

    float thicknessArrow = bounds.getWidth() * 0.01f;

    PathStrokeType dotted(2.f);
    dotted.createStrokeWithArrowheads(followerLine, followerLine, 2.f, thicknessArrow * 10, thicknessArrow * 1.5f, thicknessArrow * 4, {});
    dotted.setJointStyle(PathStrokeType::curved);
    dotted.setEndStyle(PathStrokeType::rounded);

    g.setColour(thumbColour);
    g.strokePath(followerLine, dotted);

    // ============== thumb
    Path dotPath;

    float ellipseWidth = bounds.getWidth() * 0.1f;
    dotPath.addEllipse(center.getX() - ellipseWidth / 2, bounds.getY() - ellipseWidth / 2, ellipseWidth, ellipseWidth);
    dotPath.applyTransform(rotateTransform);
    g.setColour(thumb);
    g.strokePath(dotPath, PathStrokeType{ ellipseWidth * 0.13f });

    Path thumbPath;
    ellipseWidth *= 0.5;
    thumbPath.addEllipse(center.getX() - ellipseWidth / 2, bounds.getY() - ellipseWidth / 2, ellipseWidth, ellipseWidth);
    thumbPath.applyTransform(rotateTransform);

    g.setColour(thumbColour.withMultipliedSaturation(5).brighter(0.7f));
    g.fillPath(thumbPath);

    // ============== Center Dot

    Path centerDot;
    float centerDotWidth = bounds.getWidth() * 0.2f;
    centerDot.addRoundedRectangle(center.getX() - centerDotWidth * 0.5f, center.getY() - centerDotWidth * 0.5f,
    centerDotWidth, centerDotWidth, centerDotWidth * 0.3f);

    g.setColour(defaultC);
    g.fillPath(centerDot);

    g.setColour(defaultContrasting);
    g.strokePath(centerDot, PathStrokeType{ 1.5f });
}

void CustomTheme::drawToggleButton(Graphics& g, ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted);
    juce::ignoreUnused(shouldDrawButtonAsDown);
    auto buttonBounds = button.getLocalBounds();

    float alpha = 1.f;
    if (! button.getToggleState()) alpha = 0.5f;

    g.setColour(findColour(ComboBox::ColourIds::backgroundColourId).withAlpha(0.7f));
    g.fillRoundedRectangle(buttonBounds.toFloat(), 10.f);

    g.setColour(findColour(ComboBox::textColourId).withAlpha(alpha));
    g.drawFittedText(button.getButtonText(), buttonBounds, juce::Justification::centred, 3);
}
