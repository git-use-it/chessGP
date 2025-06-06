// src/Board.cpp
#include "AnsiColors.hpp"
#include "Board.hpp"
#include "Pawn.hpp"
#include "King.hpp"
#include "Rook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "Queen.hpp"
#include "Color.hpp"
#include "Move.hpp"
#include <iostream>
#include <cstdlib>
#include <map>

// Color constants
const std::string WHITE_COLOR      = "\033[1;37m";
const std::string BLACK_COLOR      = "\033[1;30m";
const std::string RESET_COLOR      = "\033[0m";
const std::string LIGHT_SQUARE_BG  = "\033[48;5;229m";  // light yellow
const std::string DARK_SQUARE_BG   = "\033[48;5;94m";   // dark green/brown
const std::string RESET_FG         = "\033[39m";        // reset foreground only
const std::string RESET_BG         = "\033[49m";        // reset background only
const std::string RESET_ALL        = "\033[0m";         // reset all colors

// Unicode symbols for pieces
const std::map<char, std::string> pieceSymbols = {
    {'K', "♔"}, {'k', "♚"}, {'Q', "♕"}, {'q', "♛"},
    {'R', "♖"}, {'r', "♜"}, {'B', "♗"}, {'b', "♝"},
    {'N', "♘"}, {'n', "♞"}, {'P', "♙"}, {'p', "♟"}
};

Board::Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            board[i][j] = nullptr;
}

Board::~Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete board[i][j];

    for (Piece* p : whiteCaptured) delete p;
    for (Piece* p : blackCaptured) delete p;
}

void Board::setup() {
    // Setup pawns
    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(Color::BLACK);
        board[6][i] = new Pawn(Color::WHITE);
    }

    // Setup kings
    board[0][4] = new King(Color::BLACK);
    board[7][4] = new King(Color::WHITE);

    // Setup rooks
    board[0][0] = new Rook(Color::BLACK);
    board[0][7] = new Rook(Color::BLACK);
    board[7][0] = new Rook(Color::WHITE);
    board[7][7] = new Rook(Color::WHITE);

    // Setup knights
    board[0][1] = new Knight(Color::BLACK);
    board[0][6] = new Knight(Color::BLACK);
    board[7][1] = new Knight(Color::WHITE);
    board[7][6] = new Knight(Color::WHITE);

    // Setup bishops
    board[0][2] = new Bishop(Color::BLACK);
    board[0][5] = new Bishop(Color::BLACK);
    board[7][2] = new Bishop(Color::WHITE);
    board[7][5] = new Bishop(Color::WHITE);

    // Setup queens
    board[0][3] = new Queen(Color::BLACK);
    board[7][3] = new Queen(Color::WHITE);
}

void Board::print() const {
    std::cout << "   a   b   c   d   e   f   g   h\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            bool isLightSquare = ((i + j) % 2 == 0);
            std::string bgColor = isLightSquare ? LIGHT_SQUARE_BG : DARK_SQUARE_BG;

            std::cout << "|" << bgColor;

            if (board[i][j]) {
                std::string fgColor = (board[i][j]->getColor() == Color::WHITE) ? WHITE_COLOR : BLACK_COLOR;
                // Print piece with fg color, then reset fg only (keep background)
                std::cout << " " << fgColor << pieceSymbols.at(board[i][j]->getSymbol()) << RESET_FG << " ";
            } else {
                std::cout << "   "; // 3 spaces for empty square (to align)
            }

            std::cout << RESET_BG; // reset background after the square
        }
        std::cout << "| " << 8 - i << '\n';
        std::cout << "  +---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "   a   b   c   d   e   f   g   h\n";

    // Captured pieces display
    std::cout << "\nWhite Captured: ";
    if (whiteCaptured.empty()) std::cout << "None ";
    else {
        for (auto p : whiteCaptured) {
            std::string fg = (p->getColor() == Color::WHITE) ? WHITE_COLOR : BLACK_COLOR;
            std::cout << fg << pieceSymbols.at(p->getSymbol()) << " " << RESET_COLOR;
        }
    }

    std::cout << "\nBlack Captured: ";
    if (blackCaptured.empty()) std::cout << "None ";
    else {
        for (auto p : blackCaptured) {
            std::string fg = (p->getColor() == Color::BLACK) ? BLACK_COLOR : WHITE_COLOR;
            std::cout << fg << pieceSymbols.at(p->getSymbol()) << " " << RESET_COLOR;
        }
    }

    std::cout << '\n';
}

Piece* Board::getPiece(int x, int y) const {
    return board[x][y];
}

void Board::setPiece(int x, int y, Piece* piece) {
    board[x][y] = piece;
}

bool Board::movePiece(int sx, int sy, int dx, int dy) {
    Piece* piece = board[sx][sy];
    if (!piece) {
        std::cout << "No piece at source.\n";
        return false;
    }

    if (piece->isValidMove(sx, sy, dx, dy, board)) {
        Piece* dest = board[dx][dy];

        if (dest) {
            // Capture logic
            if (dest->getColor() == Color::WHITE)
                whiteCaptured.push_back(dest);
            else
                blackCaptured.push_back(dest);
        }

        std::string from = std::string(1, 'a' + sy) + std::to_string(8 - sx);
        std::string to   = std::string(1, 'a' + dy) + std::to_string(8 - dx);

        moveHistory.emplace_back(from, to, piece->getColor(), piece->getSymbol(), dest != nullptr);

        board[dx][dy] = piece;
        board[sx][sy] = nullptr;
        return true;
    }

    std::cout << "Invalid move.\n";
    return false;
}

void Board::printMoveHistory() const {
    std::cout << "Move History:\n";
    for (size_t i = 0; i < moveHistory.size(); ++i) {
        const Move& move = moveHistory[i];
        if (i % 2 == 0) std::cout << (i / 2) + 1 << ". ";

        std::cout << move.pieceSymbol
                  << move.from
                  << (move.isCapture ? "x" : "-")
                  << move.to << " ";

        if (i % 2 == 1) std::cout << '\n';
    }
    if (moveHistory.size() % 2 != 0) std::cout << '\n';
}

bool Board::isInCheck(Color color) const {
    auto [kingX, kingY] = findKingPosition(color);
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[x][y] && board[x][y]->getColor() == opponent)
                if (board[x][y]->isValidMove(x, y, kingX, kingY, board))
                    return true;

    return false;
}

bool Board::isCheckmate(Color color) const {
    if (!isInCheck(color)) return false;

    auto [kingX, kingY] = findKingPosition(color);
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy) {
            int nx = kingX + dx, ny = kingY + dy;
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
                if (isMoveLegal(kingX, kingY, nx, ny, color))
                    return false;
        }
    return true;
}

bool Board::isStalemate(Color color) const {
    if (isInCheck(color)) return false;

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[x][y] && board[x][y]->getColor() == color)
                for (int dx = 0; dx < 8; ++dx)
                    for (int dy = 0; dy < 8; ++dy)
                        if (isMoveLegal(x, y, dx, dy, color))
                            return false;

    return true;
}

std::pair<int, int> Board::findKingPosition(Color color) const {
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[x][y] && board[x][y]->getColor() == color && board[x][y]->getSymbol() == 'K')
                return {x, y};

    return {-1, -1};
}

bool Board::isMoveLegal(int sx, int sy, int dx, int dy, Color color) const {
    Piece* piece = board[sx][sy];
    if (!piece || piece->getColor() != color) return false;
    if (!piece->isValidMove(sx, sy, dx, dy, board)) return false;

    // Copy board pointers (shallow copy) to simulate move
    Piece* tempBoard[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            tempBoard[i][j] = board[i][j];

    // Simulate move
    Piece* dest = tempBoard[dx][dy];
    tempBoard[dx][dy] = tempBoard[sx][sy];
    tempBoard[sx][sy] = nullptr;

    // Check if move leaves own king in check
    auto [kingX, kingY] = findKingPosition(color);

    // If the piece is king, update king position
    if (piece->getSymbol() == 'K')
        kingX = dx, kingY = dy;

    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (tempBoard[x][y] && tempBoard[x][y]->getColor() == opponent)
                if (tempBoard[x][y]->isValidMove(x, y, kingX, kingY, tempBoard))
                    return false;

    return true;
}

