#include "Helpers.h"
#include <fstream>
#include <cctype>

void setText(sf::Text& text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f{x, y});
}

void readConfig(int& cols, int& rows, int& mines) {
    std::ifstream file("files/config.cfg");
    if (file.is_open()) {
        file >> cols >> rows >> mines;
        file.close();
    } else {
        cols = 25;
        rows = 16;
        mines = 50;
    }
}

std::string formatName(const std::string& name) {
    if (name.empty()) return "";
    std::string formatted = name;
    formatted[0] = std::toupper(static_cast<unsigned char>(formatted[0]));
    for (size_t i = 1; i < formatted.length(); ++i) {
        formatted[i] = std::tolower(static_cast<unsigned char>(formatted[i]));
    }
    return formatted;
}