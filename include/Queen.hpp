#pragma once
#include "Piece.hpp"

class Queen : public Piece {
public:
    Queen(Color color);
    char getSymbol() const override;
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const override;
};

