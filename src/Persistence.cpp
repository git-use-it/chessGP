// src/Persistence.cpp

#include "Persistence.hpp"
#include "PieceFactory.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>

// Save current board state, move history, and captured pieces
void saveGame(const Board& board, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for saving.\n";
        return;
    }

    // Save board layout
    file << "Board:\n";
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = board.getPiece(i, j);
            if (piece) {
                char symbol = piece->getSymbol();
                file << symbol << ' ';
            } else {
                file << ". ";
            }
        }
        file << '\n';
    }

    // Save captured pieces
    file << "WhiteCaptured: ";
    for (auto p : board.getWhiteCaptured())
        file << p->getSymbol() << ' ';
    file << '\n';

    file << "BlackCaptured: ";
    for (auto p : board.getBlackCaptured())
        file << p->getSymbol() << ' ';
    file << '\n';

    // Save move history
    file << "History:\n";
    const auto& history = board.getMoveHistory();
    for (size_t i = 0; i < history.size(); ++i) {
        const Move& move = history[i];
        if (i % 2 == 0) file << (i / 2 + 1) << ". ";
        file << move.pieceSymbol
             << move.from
             << (move.isCapture ? "x" : "-")
             << move.to << ' ';
        if (i % 2 == 1) file << '\n';
    }
    if (history.size() % 2 != 0) file << '\n';

    file.close();
    std::cout << "Game saved to " << filename << "\n";
}

// Load board, captured pieces, and move history
bool loadGame(Board& board, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for loading.\n";
        return false;
    }

    board = Board(); // Reset board
    std::string line;
    int row = 0;
    bool readingBoard = false;
    auto& whiteCap = const_cast<std::vector<Piece*>&>(board.getWhiteCaptured());
    auto& blackCap = const_cast<std::vector<Piece*>&>(board.getBlackCaptured());
    auto& history = const_cast<std::vector<Move>&>(board.getMoveHistory());

    while (std::getline(file, line)) {
        if (line == "Board:") {
            readingBoard = true;
            row = 0;
            continue;
        }
        if (line.rfind("WhiteCaptured:", 0) == 0) {
            std::istringstream iss(line.substr(14));
            char sym;
            while (iss >> sym) {
                Color color = isupper(sym) ? Color::WHITE : Color::BLACK;
                whiteCap.push_back(PieceFactory::createPiece(tolower(sym), color));
            }
            continue;
        }
        if (line.rfind("BlackCaptured:", 0) == 0) {
            std::istringstream iss(line.substr(14));
            char sym;
            while (iss >> sym) {
                Color color = isupper(sym) ? Color::WHITE : Color::BLACK;
                blackCap.push_back(PieceFactory::createPiece(tolower(sym), color));
            }
            continue;
        }

        if (line == "History:") {
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string token;
                while (iss >> token) {
                    if (isdigit(token[0])) continue; // skip turn number like "1."

                    // Format: e2-e4 or Nf3xg5
                    if (token.length() >= 5) {
                      char symbol = token[0];
                      std::string from = token.substr(1, 2);
                      bool isCapture = token[3] == 'x';
                      std::string to = token.substr(4, 2);

                    // Guess color based on piece symbol (uppercase = WHITE, lowercase = BLACK)
                      Color color = isupper(symbol) ? Color::WHITE : Color::BLACK;

                        history.emplace_back(from, to, color, symbol, isCapture);
                    }

                }
            }
            break;
        }

        if (readingBoard && row < 8) {
            std::istringstream iss(line);
            std::string token;
            int col = 0;
            while (iss >> token && col < 8) {
                if (token != ".") {
                    char sym = token[0];
                    Color color = isupper(sym) ? Color::WHITE : Color::BLACK;
                    board.setPiece(row, col, PieceFactory::createPiece(tolower(sym), color));
                }
                col++;
            }
            row++;
        }
    }

    std::cout << "Game loaded from " << filename << "\n";
    return true;
}

