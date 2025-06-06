// main.cpp

#include <iostream>
#include <string>
#include "Board.hpp"
#include "AnsiColors.hpp" 
#include <thread>
#include <chrono>

#define CLEAR_SCREEN "\033[2J\033[1;1H"  // Clear screen and move cursor to top-left

// Function to clear the screen and provide a better UI experience
void clearScreen() {
    std::cout << CLEAR_SCREEN << std::flush;
}

// Function to display whether a player is in check or checkmate
void printCheckStatus(const Board& board, Color currentTurn) {
    if (board.isInCheck(currentTurn)) {
        std::cout << Ansi::red(((currentTurn == Color::WHITE) ? "White" : "Black")) << " is in check!\n";
    }

    if (board.isCheckmate(currentTurn)) {
        std::cout << Ansi::green(((currentTurn == Color::WHITE) ? "Black" : "White")) 
                  << " wins by checkmate!\n";
    } else if (board.isStalemate(currentTurn)) {
        std::cout << Ansi::yellow("It's a draw by stalemate.\n");
    }
}

// Function to get user input in a consistent manner
bool getMoveInput(std::string& from, std::string& to, Color currentTurn) {
    std::cout << "\n" << ((currentTurn == Color::WHITE) ? "White" : "Black") 
              << "'s move (e.g. e2 e4): ";
    std::cin >> from >> to;

    // Validate input format
    if (from.length() != 2 || to.length() != 2 ||
        from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8' ||
        to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8') {
        std::cout << Ansi::red("Invalid input format. Try again.\n");
        return false;
    }

    return true;
}

// Function to check for end game conditions (Checkmate/Stalemate)
bool checkEndGame(Board& board) {
    if (board.isCheckmate(Color::WHITE)) {
        std::cout << Ansi::green("Checkmate! Black wins.\n");
        return true;
    } else if (board.isCheckmate(Color::BLACK)) {
        std::cout << Ansi::green("Checkmate! White wins.\n");
        return true;
    } else if (board.isStalemate(Color::WHITE)) {
        std::cout << Ansi::yellow("Stalemate! It's a draw.\n");
        return true;
    } else if (board.isStalemate(Color::BLACK)) {
        std::cout << Ansi::yellow("Stalemate! It's a draw.\n");
        return true;
    }
    return false;
}

int main() {
    Board board;
    board.setup();  // Initialize the board with pieces

    Color currentTurn = Color::WHITE;  // Start with White

    while (true) {
        clearScreen();  // Clear the screen before printing the updated board
        board.print();  // Redraw updated board

        // Print check status
        printCheckStatus(board, currentTurn);

        std::string from, to;

        // Get user input for the move
        if (!getMoveInput(from, to, currentTurn)) {
            continue;  // If input is invalid, prompt again
        }

        // Convert input to board coordinates
        int sx = 8 - (from[1] - '0');  // Convert from file rank notation to array index
        int sy = from[0] - 'a';        // Convert from file notation (a-h) to array index
        int dx = 8 - (to[1] - '0');    // Same as above for destination
        int dy = to[0] - 'a';          // Same as above for destination

        Piece* src = board.getPiece(sx, sy);
        if (!src) {
            std::cout << Ansi::red("No piece at source. Try again.\n");
            continue;
        }

        // Ensure the piece belongs to the current player
        if (src->getColor() != currentTurn) {
            std::cout << Ansi::red("You must move your own piece.\n");
            continue;
        }

        // Attempt to make the move
        if (board.movePiece(sx, sy, dx, dy)) {
            // Switch turns if the move is valid
            currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
        } else {
            std::cout << Ansi::red("Invalid move. Try again.\n");
            continue;
        }

        // Check for game termination conditions (Checkmate, Stalemate)
        if (checkEndGame(board)) {
            break;  // End the game if there's a checkmate or stalemate
        }
    }

    return 0;
}

