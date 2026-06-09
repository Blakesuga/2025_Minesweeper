#include "Helpers.h"
#include <SFML/Graphics.hpp>
#include <cctype>

std::string showWelcomeWindow(int width, int height) {
    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned int>(width),
                      static_cast<unsigned int>(height)),
        "Minesweeper",
        sf::Style::Close
    );

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        return "";
    }

    std::string playerName;

    sf::Text title("WELCOME TO MINESWEEPER!", font, 24);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    setText(title, width / 2.0f, height / 2.0f - 100);

    sf::Text prompt("Enter your name:", font, 20);
    prompt.setFillColor(sf::Color::White);
    setText(prompt, width / 2.0f, height / 2.0f - 40);

    sf::Text nameText("", font, 20);
    nameText.setFillColor(sf::Color::Yellow);
    setText(nameText, width / 2.0f, height / 2.0f + 10);

    // cursor indicator
    sf::Text cursorText("|", font, 20);
    cursorText.setFillColor(sf::Color::Yellow);

    auto updateTextAndCursor = [&]() {

        // capitalize first letter and lowercase the rest
        if (!playerName.empty()) {
            playerName[0] = static_cast<char>(std::toupper(playerName[0]));
            for (size_t i = 1; i < playerName.size(); ++i) {
                playerName[i] = static_cast<char>(std::tolower(playerName[i]));
            }
        }
        nameText.setString(playerName);
        setText(nameText, width / 2.0f, height / 2.0f + 10);

        // position cursor after name
        sf::FloatRect textBounds = nameText.getGlobalBounds();
        cursorText.setPosition(textBounds.left + textBounds.width + 5.f, textBounds.top);
    };

    updateTextAndCursor();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return "";
            }

            if (event.type == sf::Event::TextEntered) {
                auto unicode = event.text.unicode;
                if (unicode == 8 && !playerName.empty()) {

                    // backspace removes last character
                    playerName.pop_back();
                } else if (std::isalpha(static_cast<unsigned char>(unicode)) && playerName.size() < 10) {

                    // only accept alphabetical characters (max 10 chars)
                    playerName.push_back(static_cast<char>(unicode));
                }

                updateTextAndCursor();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return) {
                    if (!playerName.empty()) {
                        window.close();
                        return playerName;
                    }
                }
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(prompt);
        window.draw(nameText);
        window.draw(cursorText);
        window.display();
    }

    return "";
}
