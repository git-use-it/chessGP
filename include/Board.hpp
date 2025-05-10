// include/Board.hpp
#pragma once
#include "Color.hpp"
#include <vector>
#include "Piece.hpp"
#include <iostream>
#include <memory>
#include "Move.hpp"

class Board {
    std::vector<Move> moveHistory;  // Track moves
private:
    Piece* board[8][8];
    std::vector<Piece*> whiteCaptured;
    std::vector<Piece*> blackCaptured;

public:
    Board();
    ~Board();

    void setup(); // Put basic pieces (like pawns for now)
    void print() const;
    Piece* getPiece(int x, int y) const;
    void setPiece(int x, int y, Piece* piece);
    bool movePiece(int sx, int sy, int dx, int dy);
    void printMoveHistory() const;

    const std::vector<Piece*>& getWhiteCaptured() const { return whiteCaptured; }
    const std::vector<Piece*>& getBlackCaptured() const { return blackCaptured; }
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }

    // New methods to implement checkmate and stalemate checks
    bool isInCheck(Color color) const; // Marked as const since it doesn't modify the board
    bool isCheckmate(Color color) const; // Marked as const
    bool isStalemate(Color color) const; // Marked as const

    // Helper methods to find the king's position
    std::pair<int, int> findKingPosition(Color color) const; // Marked as const
    
    // Helper methods to check if a piece can move to a position
    bool isMoveLegal(int sx, int sy, int dx, int dy, Color color) const; // Marked as const
};

