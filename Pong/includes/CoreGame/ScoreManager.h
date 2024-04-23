#include <SFML/Graphics.hpp>

class ScoreManager {
public:
    ScoreManager(sf::RenderWindow& window);
    void addScoreLeft();
    void addScoreRight();
    void draw(sf::RenderWindow& window);
    void resetScores();
    void updateText();

private:
    int scoreLeft;
    int scoreRight;
    sf::Font font;
    sf::Text scoreText;
    sf::RenderWindow window;
};
