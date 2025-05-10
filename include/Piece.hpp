// Piece.hpp
#pragma once
#include "Color.hpp"

// Abstract base class for all chess pieces
class Piece {
protected:
    Color color;     // Color of the piece (White/Black)
    char symbol;     // Symbol representing the piece (e.g., 'P' for Pawn, 'K' for King, etc.)

public:
    // Constructor
    Piece(Color color, char symbol) : color(color), symbol(symbol) {}

    // Virtual destructor
    virtual ~Piece() = default;

    // Delete copy constructor and copy assignment to avoid accidental copying
    Piece(const Piece&) = delete;
    Piece& operator=(const Piece&) = delete;

    // Getter for the piece's color
    Color getColor() const { return color; }

    // Pure virtual: derived classes must implement this
    virtual char getSymbol() const = 0;

    // Abstract method to validate a move
    virtual bool isValidMove
  (int sx, int sy, int dx, int dy, Piece* const board[8][8]) const = 0;

};

