// Rook.cpp
#include "Rook.hpp"
#include "King.hpp"

Rook::Rook(Color color) : Piece(color, color == Color::WHITE ? 'R' : 'r') {}

char Rook::getSymbol() const {
    return (color == Color::WHITE) ? 'R' : 'r';
}

bool Rook::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    if (sx != dx && sy != dy) return false;  // Must move in a straight line (row or column)

    int stepX = (dx == sx) ? 0 : ((dx > sx) ? 1 : -1);
    int stepY = (dy == sy) ? 0 : ((dy > sy) ? 1 : -1);

    int x = sx + stepX;
    int y = sy + stepY;

    while (x != dx || y != dy) {
        if (board[x][y] != nullptr) return false;  // Path must be clear
        x += stepX;
        y += stepY;
    }

    // Can't capture a piece of the same color
    if (board[dx][dy] && board[dx][dy]->getColor() == color) return false;

    return true;
}

