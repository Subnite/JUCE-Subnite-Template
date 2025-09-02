#pragma once
#include "Subnite/Commons/ValueTreeManager.h"
#include <juce_core/juce_core.h>


namespace myplugin::vt {
enum class Property {
    // trees
    T_ROOT,

    // properties
    P_POWER,

    COUNT
};

class ValueTree : public subnite::vt::ValueTreeBase, public subnite::vt::IDMap<Property> {
public:
    ValueTree()
    : subnite::vt::ValueTreeBase(), subnite::vt::IDMap<Property>()
    {
        SetupMap();
    };
    ~ValueTree(){};

    template <typename PropertyEnum>
    juce::Identifier GetIDUnwrapped(PropertyEnum id) const {
        return GetIDFromType(id).value_or(juce::Identifier{"undefined"});
    }

    void SetupMap() override {
        using p = Property;
        using id = juce::Identifier;

        #pragma region trees
        map[p::T_ROOT] = id{"MyPluginRoot"};

        #pragma endregion trees

        #pragma region properties
        map[p::P_POWER] = id{"power"};

        #pragma endregion properties
    }

    // makes the default tree
    void Create() override {
        using prop = Property;
        // using id = juce::Identifier;

        // create new trees
        vtRoot = juce::ValueTree{GetIDUnwrapped(prop::T_ROOT)};

        // add properties
        vtRoot.setProperty(GetIDUnwrapped(prop::P_POWER), {0.69}, &undoManager);
    }
};



} // namespace
