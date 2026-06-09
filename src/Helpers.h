#include <SFML/Graphics.hpp>
#include <string>

// helper function to center text
void setText(sf::Text& text, float x, float y);

// read config file
void readConfig(int& cols, int& rows, int& mines);

// format name (first letter uppercase, rest lowercase)
std::string formatName(const std::string& name);

