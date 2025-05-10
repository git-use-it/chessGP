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
const std::string WHITE_COLOR = "\033[1;37m";
const std::string BLACK_COLOR = "\033[1;30m";
const std::string RESET_COLOR = "\033[0m";

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
    // Setup the board with initial pieces
    for (int i = 0; i < 8; ++i) {
        board[1][i] = new Pawn(Color::BLACK);
        board[6][i] = new Pawn(Color::WHITE);
    }

    board[0][4] = new King(Color::BLACK);
    board[7][4] = new King(Color::WHITE);

    board[0][0] = new Rook(Color::BLACK);
    board[0][7] = new Rook(Color::BLACK);
    board[7][0] = new Rook(Color::WHITE);
    board[7][7] = new Rook(Color::WHITE);

    board[0][1] = new Knight(Color::BLACK);
    board[0][6] = new Knight(Color::BLACK);
    board[7][1] = new Knight(Color::WHITE);
    board[7][6] = new Knight(Color::WHITE);

    board[0][2] = new Bishop(Color::BLACK);
    board[0][5] = new Bishop(Color::BLACK);
    board[7][2] = new Bishop(Color::WHITE);
    board[7][5] = new Bishop(Color::WHITE);

    board[0][3] = new Queen(Color::BLACK);
    board[7][3] = new Queen(Color::WHITE);
}

void Board::print() const {
    std::cout << "   a   b   c   d   e   f   g   h\n";
    std::cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            std::cout << "| ";
            if (board[i][j]) {
                // Set color based on the piece's color
                if (board[i][j]->getColor() == Color::WHITE)
                    std::cout << WHITE_COLOR << pieceSymbols.at(board[i][j]->getSymbol()) << ' ' << RESET_COLOR;
                else
                    std::cout << BLACK_COLOR << pieceSymbols.at(board[i][j]->getSymbol()) << ' ' << RESET_COLOR;
            }
            else
                std::cout << ". ";  // Empty space
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
            if (p->getColor() == Color::WHITE)
                std::cout << WHITE_COLOR << pieceSymbols.at(p->getSymbol()) << ' ' << RESET_COLOR;
            else
                std::cout << BLACK_COLOR << pieceSymbols.at(p->getSymbol()) << ' ' << RESET_COLOR;
        }
    }

    std::cout << "\nBlack Captured: ";
    if (blackCaptured.empty()) std::cout << "None ";
    else {
        for (auto p : blackCaptured) {
            if (p->getColor() == Color::BLACK)
                std::cout << BLACK_COLOR << pieceSymbols.at(p->getSymbol()) << ' ' << RESET_COLOR;
            else
                std::cout << WHITE_COLOR << pieceSymbols.at(p->getSymbol()) << ' ' << RESET_COLOR;
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
        bool capture = false;
        Piece* dest = board[dx][dy];

        if (dest) {
            capture = true;
            if (dest->getColor() == Color::WHITE)
                whiteCaptured.push_back(dest);
            else
                blackCaptured.push_back(dest);
        }

        std::string from = std::string(1, 'a' + sy) + std::to_string(8 - sx);
        std::string to   = std::string(1, 'a' + dy) + std::to_string(8 - dx);

        moveHistory.emplace_back(from, to, piece->getColor(), piece->getSymbol(), capture);

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

    Piece* tempBoard[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            tempBoard[i][j] = board[i][j];

    tempBoard[dx][dy] = tempBoard[sx][sy];
    tempBoard[sx][sy] = nullptr;

    int kingX = -1, kingY = -1;
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (tempBoard[x][y] && tempBoard[x][y]->getColor() == color && tempBoard[x][y]->getSymbol() == 'K') {
                kingX = x;
                kingY = y;
                break;
            }

    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (tempBoard[x][y] && tempBoard[x][y]->getColor() == opponent)
                if (tempBoard[x][y]->isValidMove(x, y, kingX, kingY, tempBoard))
                    return false;

    return true;
}
