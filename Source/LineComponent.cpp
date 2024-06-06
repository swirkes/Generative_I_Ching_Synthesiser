/*
  ==============================================================================

    LineComponent.cpp
    Created: 30 May 2024 9:42:16am
    Author:  shane wirkes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LineComponent.h"


//==============================================================================
LineComponent::LineComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

LineComponent::~LineComponent()
{
}

void LineComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    
    for (auto& segment : segments)
        {
            if (segment.hasLine)
            {
                g.fillRect(segment.rect); // Draw the line
            }

            if (segment.hasX)
            {
                // Draw an 'X' inside the segment rectangle
                auto topLeft = segment.rect.getTopLeft();
                auto bottomRight = segment.rect.getBottomRight();
                auto topRight = segment.rect.getTopRight();
                auto bottomLeft = segment.rect.getBottomLeft();
                g.setColour(juce::Colours::darkslateblue);
                g.drawLine(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 20.0f);
                g.drawLine(topRight.x, topRight.y, bottomLeft.x, bottomLeft.y, 20.0f);
                g.setColour(juce::Colours::black);
            }
    }
}

void LineComponent::resized()
{

}

void LineComponent::updateLines()
{
    auto bounds = getLocalBounds().reduced(27);

    int numLines = 6;

    // Calculate the dynamic height of each line
    float totalHeight = bounds.getHeight();
    float lineHeight = (totalHeight - (numLines - 1) * 10.0f) / numLines; // 10.0f is the gap between lines

    // Calculate the total width available
    int totalWidth = bounds.getWidth();

    // Define the lengths of the segments
    float sideSegmentLength = totalWidth * 0.4f;  // Example: 40% of the total width for side segments
    float middleSegmentLength = totalWidth * 0.2f;  // Example: 20% of the total width for middle segment

    segments.clear();
    
    middleSegmentStates.clear();
    juce::Random random;
    for (int i = 0; i < numLines; ++i)
    {
        middleSegmentStates.add(random.nextInt(4)); // 0 for space, 1 for line, 2 for line with 'X', 3 for 'X'
    }

    // Debug: Log the generated hexagram states
    DBG("Generated Hexagram States:");
    for (int state : middleSegmentStates)
    {
        DBG(juce::String(state));
    }
    
    for (int i = 0; i < numLines; ++i)
    {
        float y = bounds.getY() + i * (lineHeight + 10.0f); // 10.0f is the gap between lines
        float startX = bounds.getX();
        
        // Add the left segment
        segments.add({ juce::Rectangle<float>(startX, y, sideSegmentLength, lineHeight), true, false });

        // Determine the middle segment
        int middleType = middleSegmentStates[i]; // 0 for space, 1 for line, 2 for line with 'X', 3 for 'X'
        bool hasLine = (middleType == 1 || middleType == 2);
        bool hasX = (middleType == 2 || middleType == 3);
        segments.add({ juce::Rectangle<float>(startX + sideSegmentLength, y, middleSegmentLength, lineHeight), hasLine, hasX });

        // Add the right segment
        segments.add({ juce::Rectangle<float>(startX + sideSegmentLength + middleSegmentLength, y, sideSegmentLength, lineHeight), true, false });
        
        // Debug: Log the middle segment type before toggling
        DBG("Before Toggle - Line " + juce::String(i) + ": " + juce::String(middleType));
        
        // Toggle the middle segment state based on its current state
        if (toggleState)
        {
            if (middleType == 2) // Line with 'X' to space
            {
                middleSegmentStates.set(i, 0);
            }
            else if (middleType == 3) // 'X' to solid line
            {
                middleSegmentStates.set(i, 1);
            }
        }
        else
        {
            if (middleType != 2 && middleType != 3) // If it is currently a line or space without 'X', randomize it
            {
                middleSegmentStates.set(i, random.nextInt(4)); // 0 for space, 1 for line, 2 for line with 'X', 3 for 'X'
            }
        }
        
        // Debug: Log the middle segment type after toggling
        DBG("After Toggle - Line " + juce::String(i) + ": " + juce::String(middleSegmentStates[i]));
    }
    
    toggleState = !toggleState;
    repaint();
}

std::pair<Trigram, Trigram> LineComponent::getCurrentTrigrams() const
{
    // Log the middle segment states before processing
    DBG("Processing Middle Segment States:");
    for (int i = 0; i < middleSegmentStates.size(); ++i)
    {
        DBG("Line " + juce::String(i) + ": " + juce::String(middleSegmentStates[i]));
    }

    // Get the top 3 and bottom 3 lines
    auto getTrigram = [](const juce::Array<int>& states, int start) -> Trigram {
        Trigram trigram;
        for (int i = start; i < start + 3; ++i)
        {
            int state = states[i];
            // Convert state to a binary-like representation
            if (state == 0 || state == 3) // Space or 'X'
                trigram.triad.push_back(0); // Represent as 0 (Yin)
            else if (state == 1 || state == 2) // Line or Line with 'X'
                trigram.triad.push_back(1); // Represent as 1 (Yang)

            // Debug: Log the conversion
            DBG("State " + juce::String(state) + " converted to " + juce::String(trigram.triad.back()));
        }
        return trigram;
    };

    Trigram upperTrigram = getTrigram(middleSegmentStates, 0); // Top 3 lines
    Trigram lowerTrigram = getTrigram(middleSegmentStates, 3); // Bottom 3 lines

    // Debug: Log the derived trigrams
    DBG("Upper Trigram: " + upperTrigram.toString());
    DBG("Lower Trigram: " + lowerTrigram.toString());

    return {upperTrigram, lowerTrigram};
}

