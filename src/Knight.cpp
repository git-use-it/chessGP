// Knight.cpp

#include <cstdlib>
#include "Knight.hpp"

// Constructor
Knight::Knight(Color color) : Piece(color, 'N') {}

// getSymbol method
char Knight::getSymbol() const {
    return (color == Color::WHITE) ? 'N' : 'n';
}

// isValidMove method
bool Knight::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    // Ensure the destination is within bounds
    if (dx < 0 || dx >= 8 || dy < 0 || dy >= 8) {
        return false; // Out of bounds
    }

    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);

    // Valid knight move (L-shape)
    if ((dxAbs == 2 && dyAbs == 1) || (dxAbs == 1 && dyAbs == 2)) {
        // Can't capture same-color Piece
        if (board[dx][dy] && board[dx][dy]->getColor() == color) {
            return false;
        }
        return true;
    }
    return false;
}

