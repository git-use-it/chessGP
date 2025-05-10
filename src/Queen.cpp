// Queen.cpp
#include "Queen.hpp"
#include <cstdlib>

// Constructor definition
Queen::Queen(Color color) : Piece(color, color == Color::WHITE ? 'Q' : 'q') {}

char Queen::getSymbol() const {
    return (color == Color::WHITE) ? 'Q' : 'q';
}

bool Queen::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);

    // Check if it's a valid diagonal or straight (rook-like) movement
    if (dxAbs != 0 && dyAbs != 0 && dxAbs != dyAbs)
        return false;

    int stepX = (dx == sx) ? 0 : (dx > sx ? 1 : -1);
    int stepY = (dy == sy) ? 0 : (dy > sy ? 1 : -1);

    int x = sx + stepX;
    int y = sy + stepY;

    // Check path (all squares between start and destination)
    while (x != dx || y != dy) {
        if (x < 0 || x >= 8 || y < 0 || y >= 8) {
            return false; // Out of bounds
        }
        if (board[y][x] != nullptr) {
            return false; // Path blocked
        }
        x += stepX;
        y += stepY;
    }

    // Final destination check
    if (board[dy][dx] && board[dy][dx]->getColor() == color) {
        return false; // Destination blocked by own piece
    }

    return true; // Valid move
}

