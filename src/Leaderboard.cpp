#include "Leaderboard.h"
#include "Helpers.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

void showLeaderboard(int width, int height, const std::string& newPlayer, int newTime) {
    sf::RenderWindow window(
        sf::VideoMode(static_cast<unsigned int>(width),
                     static_cast<unsigned int>(height)),
        "Leaderboard",
        sf::Style::Close
    );

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        return;
    }

    // read leaderboard
    std::vector<std::pair<int, std::string>> scores;
    std::ifstream file("files/leaderboard.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t comma = line.find(',');
            if (comma != std::string::npos) {
                std::string timeStr = line.substr(0, comma);
                std::string name = line.substr(comma + 1);
                int minutes = std::stoi(timeStr.substr(0, 2));
                int seconds = std::stoi(timeStr.substr(3, 2));
                int totalSeconds = minutes * 60 + seconds;
                scores.emplace_back(totalSeconds, name);
            }
        }
        file.close();
    }

    // add new score if provided (if there's a victory)
    if (newTime >= 0) {
        scores.emplace_back(newTime, newPlayer + "*");
        std::sort(scores.begin(), scores.end());
        if (scores.size() > 5) scores.resize(5);

        // save updated leaderboard
        std::ofstream outFile("files/leaderboard.txt");
        for (auto& score : scores) {
            int mins = score.first / 60;
            int secs = score.first % 60;
            std::string name = score.second;
            if (!name.empty() && name.back() == '*') name.pop_back();
            outFile << std::setfill('0') << std::setw(2) << mins << ":"
                    << std::setw(2) << secs << "," << name << "\n";
        }
        outFile.close();
    }

    // display leaderboard
    sf::Text title("LEADERBOARD", font, 20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(title, width / 2.0f, height / 2.0f - 120);

    std::string leaderboardText;
    for (size_t i = 0; i < scores.size(); i++) {
        int mins = scores[i].first / 60;
        int secs = scores[i].first % 60;
        std::ostringstream oss;
        oss << (i + 1) << ".\t" << std::setfill('0') << std::setw(2) << mins << ":"
            << std::setw(2) << secs << "\t" << scores[i].second << "\n\n";
        leaderboardText += oss.str();
    }

    sf::Text content(leaderboardText, font, 18);
    content.setFillColor(sf::Color::White);
    content.setStyle(sf::Text::Bold);
    setText(content, width / 2.0f, height / 2.0f + 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(content);
        window.display();
    }
}