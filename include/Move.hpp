// Move.hpp
#pragma once
#include <string>
#include "Color.hpp"

struct Move {
    std::string from;      // e.g., "e2"
    std::string to;        // e.g., "e4"
    Color playerColor;     // WHITE or BLACK
    char pieceSymbol;      // e.g., 'P', 'Q', etc.
    bool isCapture;        // true if a capture occurred

    Move(const std::string& from, const std::string& to, Color color, char symbol, bool isCapture = false)
        : from(from), to(to), playerColor(color), pieceSymbol(symbol), isCapture(isCapture) {}
};

