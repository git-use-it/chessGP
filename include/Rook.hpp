// Rook.hpp
#pragma once
#include "Piece.hpp"

class Rook : public Piece {
public:
    Rook(Color color);

    char getSymbol() const override;
      
      bool hasMoved() const { return moved; }  // Check if the Rook has moved
      void setMoved() { moved = true; }        // Set the Rook as moved

    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const override;

private:
  bool moved = false; // track if the rook has moved
};

