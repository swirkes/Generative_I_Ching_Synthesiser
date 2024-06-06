#include "Synthesiser.h"

// SineWaveVoice implementation
SineWaveVoice::SineWaveVoice() {}

bool SineWaveVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SineWaveVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/) {
    currentAngle = 0.0;
    level = velocity * 0.15;
    tailOff = 0.0;

    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();

    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;
}

void SineWaveVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    if (allowTailOff) {
        if (tailOff == 0.0)
            tailOff = 1.0;
    } else {
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

void SineWaveVoice::pitchWheelMoved(int /*newValue*/) {}
void SineWaveVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/) {}

void SineWaveVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (angleDelta != 0.0) {
        if (tailOff > 0.0) {
            while (--numSamples >= 0) {
                auto currentSample = (float)(sin(currentAngle) * level * tailOff);
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;

                tailOff *= 0.99;
                if (tailOff <= 0.005) {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }
        } else {
            while (--numSamples >= 0) {
                auto currentSample = (float)(sin(currentAngle) * level);
                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample(i, startSample, currentSample);

                currentAngle += angleDelta;
                ++startSample;
            }
        }
    }
}

// SineWaveSynth implementation
SineWaveSynth::SineWaveSynth() {
    for (int i = 0; i < 8; ++i)
        addVoice(new SineWaveVoice());
    addSound(new SineWaveSound());
}

void SineWaveSynth::playHexagram(const Trigram& upperTrigram, const Trigram& lowerTrigram) {
    auto upperNotes = upperTrigram.toMidiNotes();
    auto lowerNotes = lowerTrigram.toMidiNotes();
    auto allNotes = upperNotes;
    allNotes.insert(allNotes.end(), lowerNotes.begin(), lowerNotes.end());

    for (auto note : allNotes) {
        noteOn(1, note, 0.8f);
    }
}

juce::Array<int> SineWaveSynth::getHexagramNotes(const Trigram& upperTrigram, const Trigram& lowerTrigram) const {
    juce::Array<int> notes;

    // Simple mapping for demonstration, you can customize this
    auto trigramToNotes = [](const Trigram& trigram) {
        juce::Array<int> trigramNotes;
        for (int line : trigram.triad) {
            trigramNotes.add(60 + line * 12); // C4 and C5
        }
        return trigramNotes;
    };

    auto upperNotes = trigramToNotes(upperTrigram);
    auto lowerNotes = trigramToNotes(lowerTrigram);

    notes.addArray(upperNotes);
    notes.addArray(lowerNotes);

    return notes;
}
