// AnsiColors.hpp

#pragma once
#include <string>

namespace Ansi {

    const std::string RESET = "\033[0m";  // Reset color

    const std::string FG_WHITE = "\033[97m";
    const std::string FG_BLACK = "\033[30m";
    const std::string FG_RED = "\033[91m";
    const std::string FG_GREEN = "\033[92m";
    const std::string FG_YELLOW = "\033[93m";
    const std::string FG_BLUE = "\033[94m";
    const std::string FG_MAGENTA = "\033[95m";
    const std::string FG_CYAN = "\033[96m";

    // Optionally, add functions to simplify usage
    inline std::string red(const std::string& text) {
        return FG_RED + text + RESET;
    }

    inline std::string green(const std::string& text) {
        return FG_GREEN + text + RESET;
    }

    inline std::string yellow(const std::string& text) {
        return FG_YELLOW + text + RESET;
    }

    inline std::string blue(const std::string& text) {
        return FG_BLUE + text + RESET;
    }

    const std::string BG_WHITE = "\033[47m";
    const std::string BG_BLACK = "\033[40m";

    const std::string BG_BRIGHT_WHITE = "\033[107m";
    const std::string BG_BRIGHT_BLACK = "\033[100m";


    // Add more functions for other colors if needed
}

