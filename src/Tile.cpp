#include "Tile.h"

Tile::Tile(int r, int c, const sf::Texture& placeholderTexture)
    : row(r), col(c), isMine(false), isRevealed(false), isFlagged(false), adjacentMines(0),
      hiddenSprite(placeholderTexture), revealedSprite(placeholderTexture),
      mineSprite(placeholderTexture), flagSprite(placeholderTexture),
      numberSprite(placeholderTexture) {
}

void Tile::setMine(bool mine) {
    isMine = mine;
}

void Tile::setRevealed(bool revealed) {
    isRevealed = revealed;
}

void Tile::setFlagged(bool flagged) {
    isFlagged = flagged;
}

void Tile::setAdjacentMines(int count) {
    adjacentMines = count;
}

void Tile::addNeighbor(Tile* neighbor) {
    neighbors.push_back(neighbor);
}

bool Tile::hasMine() const {
    return isMine;
}

bool Tile::revealed() const {
    return isRevealed;
}

bool Tile::flagged() const {
    return isFlagged;
}

int Tile::getRow() const {
    return row;
}

int Tile::getCol() const {
    return col;
}

int Tile::getAdjacentMines() const {
    return adjacentMines;
}

const std::vector<Tile*>& Tile::getNeighbors() const {
    return neighbors;
}

void Tile::toggleFlag() {
    if (!isRevealed) {
        isFlagged = !isFlagged;
    }
}

void Tile::reveal() {
    if (isFlagged || isRevealed) return;

    isRevealed = true;

    if (adjacentMines == 0 && !isMine) {
        for (Tile* neighbor : neighbors) {
            if (!neighbor->revealed() && !neighbor->hasMine() && !neighbor->flagged()) {
                neighbor->reveal();
            }
        }
    }
}

void Tile::setSprites(sf::Texture& hidden, sf::Texture& revealed,
                      sf::Texture& mine, sf::Texture& flag,
                      sf::Texture* numbers) {
    hiddenSprite.setTexture(hidden, true);
    revealedSprite.setTexture(revealed, true);
    mineSprite.setTexture(mine, true);
    flagSprite.setTexture(flag, true);

    if (adjacentMines > 0 && adjacentMines <= 8) {
        numberSprite.setTexture(numbers[adjacentMines - 1], true);
    }
}

void Tile::setPosition(float x, float y) {
    sf::Vector2f pos(x, y);
    hiddenSprite.setPosition(pos);
    revealedSprite.setPosition(pos);
    mineSprite.setPosition(pos);
    flagSprite.setPosition(pos);
    numberSprite.setPosition(pos);
}

void Tile::draw(sf::RenderWindow& window) {
    if (isRevealed) {
        window.draw(revealedSprite);
        if (isMine) {
            window.draw(mineSprite);
        } else if (adjacentMines > 0) {
            window.draw(numberSprite);
        }
    } else {
        window.draw(hiddenSprite);
        if (isFlagged) {
            window.draw(flagSprite);
        }
    }
}

bool Tile::contains(float x, float y) const {
    return hiddenSprite.getGlobalBounds().contains(sf::Vector2f(x, y));
}
