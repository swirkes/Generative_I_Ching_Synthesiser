/*
  ==============================================================================

    MainComponent.h
    Created: 29 May 2024 1:19:37pm
    Author:  shane wirkes

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LineComponent.h"
#include "Trigram.h"
#include "Synthesiser.h"
//==============================================================================
/*
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener
{
public:
    MainComponent();
    ~MainComponent() override;
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
    

private:
    LineComponent lineComponent;
    juce::TextButton updateButton;
    SineWaveSynth synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
