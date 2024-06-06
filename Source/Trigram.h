#pragma once

#include <vector>
#include <string>
#include <juce_core/juce_core.h>

// Define the Trigram struct
struct Trigram {
    std::string name;
    std::vector<int> triad; // MIDI note numbers for the triad
    
    juce::String toString() const {
            juce::String result;
            for (int value : triad) {
                result += (value == 0 ? "0" : "1");
            }
            return result;
        }
    // Convert trigram to a list of MIDI notes (example triads)
    std::vector<int> toMidiNotes() const {
    if (triad == std::vector<int>{1, 1, 1}) return {60, 64, 67}; // Example triad for "111" (C Major)
    if (triad == std::vector<int>{0, 0, 0}) return {62, 65, 69}; // Example triad for "000" (D Minor)
    if (triad == std::vector<int>{1, 1, 0}) return {63, 67, 70}; // Example triad for "110"
    if (triad == std::vector<int>{0, 0, 1}) return {65, 69, 72}; // Example triad for "001"
    if (triad == std::vector<int>{1, 0, 1}) return {66, 70, 73}; // Example triad for "101"
    if (triad == std::vector<int>{0, 1, 0}) return {67, 71, 74}; // Example triad for "010"
    if (triad == std::vector<int>{1, 0, 0}) return {68, 72, 75}; // Example triad for "100"
    if (triad == std::vector<int>{0, 1, 1}) return {69, 73, 76}; // Example triad for "011"
            
            return {}; // Default case if no match found
        }
};

// Declare the trigrams
extern std::vector<Trigram> trigrams;

// Function to get hexagram notes from two trigrams
std::vector<int> getHexagramNotes(const Trigram& upperTrigram, const Trigram& lowerTrigram);
