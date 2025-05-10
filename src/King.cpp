// src/King.cpp
#include "King.hpp"
#include <cmath>
#include "Color.hpp"
#include "Rook.hpp"

King::King(Color color) : Piece(color, color == Color::WHITE ? 'K' : 'k') {}

bool King::isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const {
    int dx_abs = std::abs(dx - sx);
    int dy_abs = std::abs(dy - sy);

    // If the King is moving one square in any direction
    if (dx_abs <= 1 && dy_abs <= 1) {
        Piece* target = board[dy][dx];
        // Target must be either empty or occupied by an opponent's piece
        if (target == nullptr || target->getColor() != color) {
            return true;
        }
    }

    // Castling logic: King moves two squares (either left or right)
    if (!hasMoved() && sy == dy && (dx == sx + 2 || dx == sx - 2) && dy == sy) {
        int stepX = (dx > sx) ? 1 : -1;
        int x = sx + stepX;

        // Check if there are no pieces between the King and Rook
        while (x != dx) {
            if (board[sy][x] != nullptr) return false;  // Path is blocked
            x += stepX;
        }

        // Check if the Rook is in the correct position and has not moved
        int rookX = (dx > sx) ? 7 : 0;  // Rook should be at the edge of the board
        Piece* rook = board[sy][rookX];
       Rook* rookPiece = dynamic_cast<Rook*>(rook);
            if (rookPiece && rookPiece->getColor() == color && !rookPiece->hasMoved()) {

            return true;  // Castling is possible
            }
    }

    return false;  // Invalid move if none of the conditions match
}

char King::getSymbol() const {
    return symbol;
}

