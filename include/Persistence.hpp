// Persistence.hpp
#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include "Board.hpp"

void saveGame(const Board& board, const std::string& filename);
bool loadGame(Board& board, const std::string& filename);

#endif

