// Bishop.cpp 
#include "Bishop.hpp"
#include <cstdlib>

Bishop::Bishop(Color color) : Piece(color, 'B') {}

char Bishop::getSymbol() const {
  return (color == Color::WHITE) ? 'B' : 'b';
}

bool Bishop::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    // Must move diagonally
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    if (dxAbs != dyAbs) return false;

    int stepX = (dx > sx) ? 1 : -1;
    int stepY = (dy > sy) ? 1 : -1;

    int x = sx + stepX;
    int y = sy + stepY;

    while (x != dx && y != dy) {
        if (board[y][x] != nullptr) return false;
        x += stepX;
        y += stepY;
    }

    // Check if final destination is not occupied by same color
    if (board[dy][dx] && board[dy][dx]->getColor() == color)
        return false;

    return true;
}

