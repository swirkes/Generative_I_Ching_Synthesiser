#include "Trigram.h"

// Define the trigrams and their triads
std::vector<Trigram> trigrams = {
    {"Heaven", {60, 64, 67}},   // C Major
    {"Lake", {67, 71, 74}},     // G Major
    {"Fire", {69, 72, 76}},     // A Minor
    {"Thunder", {62, 65, 69}},  // D Minor
    {"Wind", {64, 67, 71}},     // E Minor
    {"Water", {65, 69, 72}},    // F Major
    {"Mountain", {71, 74, 77}}, // B Diminished
    {"Earth", {69, 73, 76}}     // A Major
};

std::vector<int> getHexagramNotes(const Trigram& upperTrigram, const Trigram& lowerTrigram) {
    std::vector<int> hexagramNotes;
    hexagramNotes.insert(hexagramNotes.end(), upperTrigram.triad.begin(), upperTrigram.triad.end());
    hexagramNotes.insert(hexagramNotes.end(), lowerTrigram.triad.begin(), lowerTrigram.triad.end());
    return hexagramNotes;
}
