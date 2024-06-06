/*
  ==============================================================================

    MainComponent.cpp
    Created: 29 May 2024 1:19:37pm
    Author:  shane wirkes

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : updateButton("Update Lines")
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(lineComponent);
    addAndMakeVisible(updateButton);
    updateButton.addListener(this);
    setSize(450, 450);
    
    setAudioChannels (0, 2);
    // Initial playback
    auto trigrams = lineComponent.getCurrentTrigrams();
    synth.playHexagram(trigrams.first, trigrams.second);

}

MainComponent::~MainComponent()
{
    updateButton.removeListener(this);
    shutdownAudio();
}

// Audio callbacks
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion(); // Clear the buffer before rendering audio into it
    juce::MidiBuffer midiMessages; // Create an empty MIDI buffer
    synth.renderNextBlock(*bufferToFill.buffer, midiMessages, bufferToFill.startSample, bufferToFill.numSamples);
}

void MainComponent::releaseResources() {}

void MainComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::darkgrey);   // clear the background

    g.setColour (juce::Colours::darkblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

//    g.setColour (juce::Colours::white);
//    g.setFont (juce::Font ("Times New Roman", 20.0f, juce::Font::italic));
//    g.drawText (currentSizeAsString, getLocalBounds(),
//                juce::Justification::centred, true);   // draw some placeholder text
    
    
    
        
}

void MainComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //currentSizeAsString = juce::String (getWidth()) + " x " + juce::String (getHeight());
    //auto& random = juce::Random::getSystemRandom();
    //changeColor = juce::Colour::fromRGB(random.nextInt(256), random.nextInt(256), random.nextInt(256));
    
    auto area = getLocalBounds();
    lineComponent.setBounds(area.removeFromTop(area.getHeight() - 40));
    updateButton.setBounds(area);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &updateButton)
    {
        // Update the lineComponent based on new hexagram (for visual representation)
        lineComponent.updateLines();

        // Retrieve the trigrams from the lineComponent
        auto trigrams = lineComponent.getCurrentTrigrams();

        DBG("Playing Hexagram:");
        DBG("Upper Trigram: " + trigrams.first.toString());
        DBG("Lower Trigram: " + trigrams.second.toString());
        
        // Stop all currently playing notes
        synth.allNotesOff(1, true);
        
        // Play the corresponding sounds
        synth.playHexagram(trigrams.first, trigrams.second);
    }
}
