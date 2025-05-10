// include/Pawn.hpp
#pragma once
#include "Piece.hpp"
#include "Color.hpp"

class Pawn : public Piece {
public:
    Pawn(Color color);

    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const override;
    char getSymbol() const override;
};

