/*
  ==============================================================================

    LineComponent.h
    Created: 30 May 2024 9:42:16am
    Author:  shane wirkes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Trigram.h"

//==============================================================================
/*
*/
class LineComponent  : public juce::Component
{
public:
    LineComponent();
    ~LineComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateLines(); 
    std::pair<Trigram, Trigram> getCurrentTrigrams() const;

private:
    struct Segment
    {
        juce::Rectangle<float> rect;
        bool hasLine = false;
        bool hasX = false;
    };
    juce::Array<Segment> segments;
    juce::Array<int> middleSegmentStates;
    bool toggleState = true; 
    
    Trigram upperTrigram; // Store current trigrams
    Trigram lowerTrigram;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LineComponent)
};
