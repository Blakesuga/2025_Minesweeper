#include <SFML/Graphics.hpp>
#include <vector>

class Tile {

private:
    int row, col;
    bool isMine, isRevealed, isFlagged;
    int adjacentMines;

    sf::Sprite hiddenSprite;
    sf::Sprite revealedSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;
    sf::Sprite numberSprite;

    std::vector<Tile*> neighbors;

public:
    Tile(int r, int c, const sf::Texture& placeholderTexture);

    void setMine(bool mine);
    void setRevealed(bool revealed);
    void setFlagged(bool flagged);
    void setAdjacentMines(int count);
    void addNeighbor(Tile* neighbor);

    bool hasMine() const;
    bool revealed() const;
    bool flagged() const;
    int getRow() const;
    int getCol() const;
    int getAdjacentMines() const;
    const std::vector<Tile*>& getNeighbors() const;

    void toggleFlag();
    void reveal();

    void setSprites(sf::Texture& hidden, sf::Texture& revealed, sf::Texture& mine,
                    sf::Texture& flag, sf::Texture* numbers);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);

    bool contains(float x, float y) const;
};

