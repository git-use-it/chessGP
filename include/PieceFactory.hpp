// PieceFactory.hpp

#pragma once
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "King.hpp"

class PieceFactory {
public :
  static Piece* createPiece(char symbol, Color color){
      switch (symbol) {
            
            case 'p': return new Pawn(color);
            case 'r': return new Rook(color);
            case 'n': return new Knight(color);
            case 'b': return new Bishop(color);
            case 'q': return new Queen(color);
            case 'k': return new King(color);

      default: return nullptr;
      }
  }
};
