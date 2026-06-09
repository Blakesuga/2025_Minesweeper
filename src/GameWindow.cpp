#include "GameWindow.h"
#include "Tile.h"
#include "Leaderboard.h"
#include "Timer.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

void showGameWindow(const std::string& playerName, int cols, int rows, int mineCount) {
    int width = cols * 32;
    int height = rows * 32 + 100;
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    // load textures
    sf::Texture tileHidden, tileRevealed, mine, flag, faceHappy, faceLose, faceWin, digits;
    sf::Texture debug, play, pause, leaderboard;
    sf::Texture numbers[8];

    if (!tileHidden.loadFromFile("files/images/tile_hidden.png")) return;
    if (!tileRevealed.loadFromFile("files/images/tile_revealed.png")) return;
    if (!mine.loadFromFile("files/images/mine.png")) return;
    if (!flag.loadFromFile("files/images/flag.png")) return;
    if (!faceHappy.loadFromFile("files/images/face_happy.png")) return;
    if (!faceLose.loadFromFile("files/images/face_lose.png")) return;
    if (!faceWin.loadFromFile("files/images/face_win.png")) return;
    if (!digits.loadFromFile("files/images/digits.png")) return;
    if (!debug.loadFromFile("files/images/debug.png")) return;
    if (!play.loadFromFile("files/images/play.png")) return;
    if (!pause.loadFromFile("files/images/pause.png")) return;
    if (!leaderboard.loadFromFile("files/images/leaderboard.png")) return;

    for (int i = 0; i < 8; i++) {
        if (!numbers[i].loadFromFile("files/images/number_" + std::to_string(i+1) + ".png")) return;
    }

    // create tiles
    std::vector<std::vector<Tile*>> tiles(rows, std::vector<Tile*>(cols));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            tiles[r][c] = new Tile(r, c, tileHidden);
            tiles[r][c]->setSprites(tileHidden, tileRevealed, mine, flag, numbers);
            tiles[r][c]->setPosition(c * 32.0f, r * 32.0f);
        }
    }

    // setup neighbors
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        tiles[r][c]->addNeighbor(tiles[nr][nc]);
                    }
                }
            }
        }
    }

    // place mines function
    auto placeMines = [&]() {
        std::vector<Tile*> allTiles;
        for (auto& row : tiles) {
            for (auto tile : row) {
                tile->setMine(false);
                tile->setRevealed(false);
                tile->setFlagged(false);
                allTiles.push_back(tile);
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(allTiles.begin(), allTiles.end(), gen);

        for (int i = 0; i < mineCount; i++) {
            allTiles[i]->setMine(true);
        }

        for (auto& row : tiles) {
            for (auto tile : row) {
                if (!tile->hasMine()) {
                    int count = 0;
                    for (auto n : tile->getNeighbors()) {
                        if (n->hasMine()) count++;
                    }
                    tile->setAdjacentMines(count);
                    tile->setSprites(tileHidden, tileRevealed, mine, flag, numbers);
                }
            }
        }
    };

    placeMines();

    // UI sprites
    sf::Sprite faceSprite(faceHappy);
    faceSprite.setPosition(sf::Vector2f((cols/2.0f) * 32 - 32, 32 * (rows + 0.5f)));

    sf::Sprite debugSprite(debug);
    debugSprite.setPosition(sf::Vector2f((cols * 32) - 304, 32 * (rows + 0.5f)));

    sf::Sprite pausePlaySprite(pause);
    pausePlaySprite.setPosition(sf::Vector2f((cols * 32) - 240, 32 * (rows + 0.5f)));

    sf::Sprite leaderboardSprite(leaderboard);
    leaderboardSprite.setPosition(sf::Vector2f((cols * 32) - 176, 32 * (rows + 0.5f)));

    // counter and timer sprites
    std::vector<sf::Sprite> counterDigits;
    for (int i = 0; i < 3; i++) {
        counterDigits.push_back(sf::Sprite(digits));
    }

    std::vector<sf::Sprite> timerMinutes;
    std::vector<sf::Sprite> timerSeconds;
    for (int i = 0; i < 2; i++) {
        timerMinutes.push_back(sf::Sprite(digits));
        timerSeconds.push_back(sf::Sprite(digits));
    }

    // game state
    bool gameOver = false;
    bool gameWon = false;
    bool isPaused = false;
    bool debugMode = false;
    int flagCount = 0;

    std::vector<std::vector<bool>> pausedRevealedStates;
    std::vector<std::vector<bool>> pausedFlaggedStates;

    Timer gameTimer;
    gameTimer.start();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // face button -> reset game
                if (event.mouseButton.button == sf::Mouse::Left &&
                    faceSprite.getGlobalBounds().contains(mousePos)) {
                    placeMines();
                    gameOver = gameWon = isPaused = debugMode = false;
                    flagCount = 0;
                    faceSprite.setTexture(faceHappy, true);
                    pausePlaySprite.setTexture(pause, true);
                    gameTimer.reset();
                    gameTimer.start();
                }

                // debug button
                else if (event.mouseButton.button == sf::Mouse::Left &&
                         debugSprite.getGlobalBounds().contains(mousePos) &&
                         !gameOver && !gameWon) {
                    debugMode = !debugMode;
                }

                // pause/play button
                else if (event.mouseButton.button == sf::Mouse::Left &&
                         pausePlaySprite.getGlobalBounds().contains(mousePos) &&
                         !gameOver && !gameWon) {
                    if (isPaused) {
                        isPaused = false;
                        pausePlaySprite.setTexture(pause, true);
                        gameTimer.resume();

                        for (int r = 0; r < rows; r++) {
                            for (int c = 0; c < cols; c++) {
                                tiles[r][c]->setRevealed(pausedRevealedStates[r][c]);
                                tiles[r][c]->setFlagged(pausedFlaggedStates[r][c]);
                            }
                        }
                    } else {
                        isPaused = true;
                        pausePlaySprite.setTexture(play, true);
                        gameTimer.pause();

                        pausedRevealedStates.clear();
                        pausedFlaggedStates.clear();
                        pausedRevealedStates.resize(rows, std::vector<bool>(cols));
                        pausedFlaggedStates.resize(rows, std::vector<bool>(cols));

                        for (int r = 0; r < rows; r++) {
                            for (int c = 0; c < cols; c++) {
                                pausedRevealedStates[r][c] = tiles[r][c]->revealed();
                                pausedFlaggedStates[r][c] = tiles[r][c]->flagged();
                                tiles[r][c]->setRevealed(true);
                                tiles[r][c]->setFlagged(false);
                            }
                        }
                    }
                }

                // leaderboard button
                else if (event.mouseButton.button == sf::Mouse::Left &&
                         leaderboardSprite.getGlobalBounds().contains(mousePos)) {
                    std::vector<std::vector<bool>> savedRevealedStates(rows, std::vector<bool>(cols));
                    std::vector<std::vector<bool>> savedFlaggedStates(rows, std::vector<bool>(cols));

                    for (int r = 0; r < rows; r++) {
                        for (int c = 0; c < cols; c++) {
                            savedRevealedStates[r][c] = tiles[r][c]->revealed();
                            savedFlaggedStates[r][c] = tiles[r][c]->flagged();
                            tiles[r][c]->setRevealed(true);
                            tiles[r][c]->setFlagged(false);
                        }
                    }

                    bool wasRunning = !gameOver && !gameWon && !isPaused;
                    if (wasRunning) {
                        gameTimer.pause();
                    }

                    showLeaderboard(cols * 16, rows * 16 + 50);

                    if (wasRunning) {
                        gameTimer.resume();
                    }

                    for (int r = 0; r < rows; r++) {
                        for (int c = 0; c < cols; c++) {
                            tiles[r][c]->setRevealed(savedRevealedStates[r][c]);
                            tiles[r][c]->setFlagged(savedFlaggedStates[r][c]);
                        }
                    }
                }

                // left click on tiles
                else if (event.mouseButton.button == sf::Mouse::Left &&
                         mousePos.y < rows * 32 && !gameOver && !gameWon && !isPaused) {
                    for (auto& row : tiles) {
                        for (auto tile : row) {
                            if (tile->contains(mousePos.x, mousePos.y) && !tile->flagged()) {
                                tile->reveal();

                                if (tile->hasMine()) {
                                    gameOver = true;
                                    faceSprite.setTexture(faceLose, true);
                                    gameTimer.stop();
                                    for (auto& r : tiles) {
                                        for (auto t : r) {
                                            if (t->hasMine()) {
                                                t->setRevealed(true);
                                            }
                                        }
                                    }
                                } else {
                                    int revealed = 0;
                                    for (auto& r : tiles) {
                                        for (auto t : r) {
                                            if (t->revealed() && !t->hasMine()) {
                                                revealed++;
                                            }
                                        }
                                    }

                                    if (revealed == rows * cols - mineCount) {
                                        gameWon = true;
                                        faceSprite.setTexture(faceWin, true);
                                        gameTimer.stop();

                                        for (auto& r : tiles) {
                                            for (auto t : r) {
                                                if (t->hasMine() && !t->flagged()) {
                                                    t->toggleFlag();
                                                    flagCount++;
                                                }
                                            }
                                        }

                                        showLeaderboard(cols * 16, rows * 16 + 50, playerName, gameTimer.getElapsedSeconds());
                                    }
                                }
                                break;
                            }
                        }
                    }
                }

                // right click -> flag
                else if (event.mouseButton.button == sf::Mouse::Right &&
                         mousePos.y < rows * 32 && !gameOver && !gameWon && !isPaused) {
                    for (auto& row : tiles) {
                        for (auto tile : row) {
                            if (tile->contains(mousePos.x, mousePos.y) && !tile->revealed()) {
                                tile->toggleFlag();
                                flagCount += tile->flagged() ? 1 : -1;
                                break;
                            }
                        }
                    }
                }
            }
        }

        // update counter display
        int displayCount = gameWon ? 0 : (mineCount - flagCount);
        displayCount = std::abs(displayCount);
        if (displayCount > 999) displayCount = 999;

        for (int i = 0; i < 3; i++) {
            int digit = (displayCount / (int)pow(10, 2-i)) % 10;
            counterDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
            counterDigits[i].setPosition(sf::Vector2f(33 + i * 21, 32 * (rows + 0.5f) + 16));
        }

        // update timer display
        int minutes = gameTimer.getMinutes();
        int seconds = gameTimer.getSeconds();

        timerMinutes[0].setTextureRect(sf::IntRect((minutes / 10) * 21, 0, 21, 32));
        timerMinutes[0].setPosition(sf::Vector2f(cols * 32 - 97, 32 * (rows + 0.5f) + 16));
        timerMinutes[1].setTextureRect(sf::IntRect((minutes % 10) * 21, 0, 21, 32));
        timerMinutes[1].setPosition(sf::Vector2f(cols * 32 - 76, 32 * (rows + 0.5f) + 16));

        timerSeconds[0].setTextureRect(sf::IntRect((seconds / 10) * 21, 0, 21, 32));
        timerSeconds[0].setPosition(sf::Vector2f(cols * 32 - 54, 32 * (rows + 0.5f) + 16));
        timerSeconds[1].setTextureRect(sf::IntRect((seconds % 10) * 21, 0, 21, 32));
        timerSeconds[1].setPosition(sf::Vector2f(cols * 32 - 33, 32 * (rows + 0.5f) + 16));

        // draw everything
        window.clear(sf::Color::White);

        for (auto& row : tiles) {
            for (auto tile : row) {
                tile->draw(window);
            }
        }

        // debug mode
        if (debugMode && !gameOver && !gameWon) {
            for (auto& row : tiles) {
                for (auto tile : row) {
                    if (tile->hasMine() && !tile->revealed()) {
                        sf::Sprite debugMine(mine);
                        debugMine.setPosition(sf::Vector2f(tile->getCol() * 32.0f, tile->getRow() * 32.0f));
                        window.draw(debugMine);
                    }
                }
            }
        }

        window.draw(faceSprite);
        window.draw(debugSprite);
        window.draw(pausePlaySprite);
        window.draw(leaderboardSprite);
        for (auto& d : counterDigits) window.draw(d);
        for (auto& d : timerMinutes) window.draw(d);
        for (auto& d : timerSeconds) window.draw(d);

        window.display();
    }

    // cleanup of board
    for (auto& row : tiles) {
        for (auto tile : row) {
            delete tile;
        }
    }
}