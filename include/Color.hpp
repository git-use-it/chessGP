// Color.hpp
#pragma once
#include <string>

enum class Color {
    WHITE,
    BLACK
};

inline std::string toString(Color color) {
    switch (color) {
        case Color::WHITE:
            return "White";
        case Color::BLACK:
            return "Black";
        default:
            return "Unknown";
    }
}

