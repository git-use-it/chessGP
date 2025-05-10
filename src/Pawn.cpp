// src/Pawn.cpp
#include "Pawn.hpp"
#include <cmath>
#include "Color.hpp"

// Constructor definition
Pawn::Pawn(Color color) : Piece(color, color == Color::WHITE ? 'P' : 'p') {}

// isValidMove method definition
bool Pawn::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    int direction = (color == Color::WHITE) ? -1 : 1;
    int startRow = (color == Color::WHITE) ? 6 : 1;

    // Single step forward
    if (sx + direction == dx && sy == dy && board[dx][dy] == nullptr) {
        return true;
    }

    // Double step forward from starting position
    if (sx == startRow && sx + 2 * direction == dx && sy == dy &&
        board[sx + direction][sy] == nullptr && board[dx][dy] == nullptr) {
        return true;
    }

    // Diagonal capture
    if (sx + direction == dx && std::abs(sy - dy) == 1 && board[dx][dy] &&
        board[dx][dy]->getColor() != color) {
        return true;
    }

    return false;
}
// getSymbol implementation
char Pawn::getSymbol() const {
    return symbol; 
}
