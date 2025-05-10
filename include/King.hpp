// King.hpp
#pragma once
#include "Piece.hpp"
#include "Color.hpp"

class King : public Piece {
public:
    King(Color color);

    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) const override;
      bool hasMoved() const { return moved; }  // Check if the King has moved
      void setMoved() { moved = true; }        // Set the King as moved
    
    char getSymbol() const override;
private:
  bool moved = false; // track if the king has moved
};

