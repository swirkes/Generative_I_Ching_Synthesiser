#pragma once

#include <JuceHeader.h>
#include "Trigram.h"

// Define the SineWaveVoice class
class SineWaveVoice : public juce::SynthesiserVoice {
public:
    SineWaveVoice();
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
    double currentAngle = 0.0;
    double angleDelta = 0.0;
    double level = 0.0;
    double tailOff = 0.0;
};

// Define the SineWaveSynth class
class SineWaveSynth : public juce::Synthesiser {
public:
    SineWaveSynth();
    void playHexagram(const Trigram& upperTrigram, const Trigram& lowerTrigram);
    
private:
    juce::Array<int> getHexagramNotes(const Trigram& upperTrigram, const Trigram& lowerTrigram) const;
};

// Define the SineWaveSound class
class SineWaveSound : public juce::SynthesiserSound {
public:
    bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel (int /*midiChannel*/) override { return true; }
};
