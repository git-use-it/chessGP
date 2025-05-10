// Knight.hpp

#pragma once
#include "Piece.hpp"

class Knight : public Piece {
public:
  Knight(Color color);

  char getSymbol() const override;
  bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const override;
};
