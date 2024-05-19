#include "../../includes/CoreGame/ScoreManager.h"

#include <iostream>

ScoreManager::ScoreManager(sf::RenderWindow& window) : scoreLeft(0), scoreRight(0) {
    if (!font.loadFromFile("../../Pong/includes/police/ArialMT.ttf")) {
        std::cout << "Error loading font\n";
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("0 - 0");
    // Positionner le texte du score au centre en haut de la fenêtre
    scoreText.setPosition(window.getSize().x/2 - scoreText.getLocalBounds().width / 2, 10);
    
}

void ScoreManager::addScoreLeft() {
    scoreLeft++;
    updateText();
}

void ScoreManager::addScoreRight() {
    scoreRight++;
    updateText();
}

void ScoreManager::resetScores() {
    scoreLeft = 0;
    scoreRight = 0;
    updateText();
}

void ScoreManager::draw(sf::RenderWindow& window) {
    window.draw(scoreText);
}

void ScoreManager::updateText() {
    // Mettre à jour le texte du score
    scoreText.setString(std::to_string(scoreLeft) + " - " + std::to_string(scoreRight));
    //scoreText.setPosition(window.getSize().x/2 - scoreText.getLocalBounds().width / 2, 10);
    std::cout << window.getSize().x << std::endl;
}

