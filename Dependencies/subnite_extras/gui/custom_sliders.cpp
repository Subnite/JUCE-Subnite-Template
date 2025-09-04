#include "custom_sliders.h"
#include <algorithm>

template <typename T>
subnite::Slider<T>::Slider(T min, T max, T initValue)
    : normalizedRawValue(0), displayedValue(initValue),
    minValue(min), maxValue(max), defaultValue(initValue)
{
    jassert(minValue < maxValue); // Minimum value should be lower than max
    jassert(defaultValue >= minValue && defaultValue <= maxValue); // Default value should be between [min : max] inclusive

    T range = maxValue - minValue;
    T v = defaultValue - minValue;
    normalizedRawValue = v / static_cast<double>(range);
    updateDisplayedValueChecked(true); // in case getFromValueTree fails

    getFromValueTree();
}

/**
    Sets the mouse cursor type back to normal (just to make sure).
    calls updateValueTree() to save the state.
*/
template <typename T>
subnite::Slider<T>::~Slider(){
    // maybe make sure that the mouse is normal
    setMouseCursor(juce::MouseCursor::NormalCursor);
    updateValueTree();
}

/**
    updates the displayedValue from the normalizedRawValue, and clamping it between minValue and maxValue.
    It's up to the user's normalizedToDisplayed function to make sure this doesn't need to be clamped.
    @param updateTree calls onValueChanged() if set to true.
    @see normalizedToDisplayed, onValueChanged
*/
template <typename T>
void subnite::Slider<T>::updateDisplayedValueChecked(bool updateTree) {
    const T newValue = normalizedToDisplayed(normalizedRawValue);
    jassert(newValue >= minValue && newValue <= maxValue); // Value should be between [min : max] inclusive. The normalizedToDisplayed function did not do this
    displayedValue = std::clamp(newValue, minValue, maxValue);

    if (updateTree) onValueChanged(displayedValue);
}


template <typename T>
std::string subnite::Slider<T>::getValueString() const {
    return prefix + valueToString(displayedValue) + postfix;
}

template <typename T>
void subnite::Slider<T>::setValuePrefix(std::string newPrefix) {
    prefix = newPrefix;
}

template <typename T>
void subnite::Slider<T>::setValuePostfix(std::string newPostfix) {
    postfix = newPostfix;
}

template <typename T>
void subnite::Slider<T>::setValue(T newValue) {
    jassert (newValue <= maxValue && newValue >= minValue);

    newValue = std::clamp(newValue, minValue, maxValue);
    displayedValue = newValue;

    normalizedRawValue = (newValue-minValue) / static_cast<double>(maxValue - minValue);
    onValueChanged(displayedValue);
}

template <typename T>
double subnite::Slider<T>::getValueAngle(const double& minAngle, const double& maxAngle) const {
    const auto range = maxAngle - minAngle;
    return normalizedRawValue * range + minAngle;
}

template <typename T>
void subnite::Slider<T>::getFromValueTree() {
    if (vTree == nullptr) return;

    auto slider = vTree->GetRoot().getChildWithName(sliderTreeUniqueID);
    if (!slider.isValid()) return; // didn't exist or wasn't child of root

    auto raw = static_cast<double>(slider.getProperty(rawNormalizedValueID));
    auto min = static_cast<double>(slider.getProperty(minValueID));
    auto max = static_cast<double>(slider.getProperty(maxValueID));
    auto displayVal = static_cast<double>(slider.getProperty(displayValueID));

    normalizedRawValue = raw;
    minValue = static_cast<T>(min);
    maxValue = static_cast<T>(max);
    displayedValue = static_cast<T>(displayVal);

    updateDisplayedValueChecked(true); // we're saving the display value too, but this is still useful for initialization.
}

/**
 * Since the vTree variable can be nullptr, it would just return without doing anything in that case.
 *
 * @see subnite::vt::ValueTreeBase::setChild, setValueTree
 */
template <typename T>
void subnite::Slider<T>::updateValueTree() {
    if (vTree == nullptr) return;

    juce::ValueTree slider{sliderTreeUniqueID};

    slider.setProperty(rawNormalizedValueID, normalizedRawValue, nullptr);
    slider.setProperty(minValueID, minValue, nullptr);
    slider.setProperty(maxValueID, maxValue, nullptr);
    slider.setProperty(displayValueID, displayedValue, nullptr);

    vTree->SetChild(sliderTreeUniqueID, slider);
}


// ============= Visuals   =================


template <typename T>
void subnite::Slider<T>::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds();
    auto maxSize = std::min(bounds.getWidth(), bounds.getHeight());
    bounds = bounds.withSizeKeepingCentre(maxSize, maxSize);

    g.setColour(juce::Colours::grey.withLightness(0.3f));
    g.fillEllipse(bounds.getCentreX()-maxSize/2.f, bounds.getCentreY()-maxSize/2.f, static_cast<float>(maxSize), static_cast<float>(maxSize));

    // line at value pos
    const double remove = -1.5;
    const double pi = 3.1415;
    const float angle = static_cast<float>(getValueAngle(pi-(remove/2), 0+(remove/2))); // radians
    auto r = maxSize*0.5f*0.8f;
    auto center = bounds.getCentre();
    float x = r*cosf(angle) + center.x;
    float y = r*-sinf(angle) + center.y; // -sin because y 0 is top in juce

    g.setColour(juce::Colours::red);
    g.fillEllipse(x-4, y, 8, 8);

    if (isHovering && displayValueOnHover) {
        auto textBounds = bounds.withSizeKeepingCentre(bounds.getWidth()/2, bounds.getHeight()/2);
        g.setColour(juce::Colours::white);
        g.drawFittedText(getValueString(), textBounds, juce::Justification::centredBottom, 20);
    }
}


// ============= behaviors =================


template <typename T>
void subnite::Slider<T>::mouseEnter(const juce::MouseEvent& e) {
    juce::ignoreUnused(e);
    isHovering = true;
    repaint();
}

template <typename T>
void subnite::Slider<T>::mouseExit(const juce::MouseEvent& e) {
    juce::ignoreUnused(e);
    isHovering = false;
    repaint();
}

template <typename T>
void subnite::Slider<T>::mouseDown(const juce::MouseEvent& e) {
    if (e.mods.isLeftButtonDown()){
        setMouseCursor(juce::MouseCursor::NoCursor);
    }
    // right click change the value from text.
}

template <typename T>
void subnite::Slider<T>::mouseUp(const juce::MouseEvent& e) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
    lastDragOffset.setXY(0, 0);
    // should reset mouse position but idk how
    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(e.getMouseDownScreenPosition().toFloat());

    updateValueTree();
    if (!updateTreeOnDrag) onValueChanged(displayedValue);
}

template <typename T>
void subnite::Slider<T>::mouseDrag(const juce::MouseEvent& e) {
    if (e.mods.isLeftButtonDown()){
        const auto offset = e.getOffsetFromDragStart();
        auto distanceFromLast = offset - lastDragOffset;

        const auto maxDist = (abs(distanceFromLast.x) > abs(distanceFromLast.y)) ? distanceFromLast.x : -distanceFromLast.y;
        normalizedRawValue += maxDist/1000.0;
        normalizedRawValue = std::clamp(normalizedRawValue, 0.0, 1.0);

        lastDragOffset = offset;
        updateDisplayedValueChecked(updateTreeOnDrag);
        repaint();

        if (offset.getDistanceSquaredFrom({0,0}) > 50) {
            juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(e.getMouseDownScreenPosition().toFloat()); // sometimes reset so it remains smooth
            lastDragOffset.setXY(0,0);
        }
    }
}

template <typename T>
void subnite::Slider<T>::mouseDoubleClick(const juce::MouseEvent& e) {
    if (e.mods.isLeftButtonDown()){
        setValue(defaultValue);
        repaint();
    }

    updateValueTree();
}

template <typename T>
void subnite::Slider<T>::setValueTree(subnite::vt::ValueTreeBase* parentTree, juce::Identifier uniqueSliderTreeID,
juce::Identifier rawNormalizedID, juce::Identifier displayID, juce::Identifier minID, juce::Identifier maxID) {
    vTree = parentTree;
    sliderTreeUniqueID = uniqueSliderTreeID; // the unique tree to look for

    // properties
    rawNormalizedValueID = rawNormalizedID;
    displayValueID = displayID;
    minValueID = minID;
    maxValueID = maxID;

    getFromValueTree();
}




template class subnite::Slider<double>;
template class subnite::Slider<float>;
template class subnite::Slider<int>;
