#include "includes/CoreGame/Menu.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Menu");
    Menu menu(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            menu.handleEvent(event);
        }

        menu.update();
        window.clear();
        menu.draw();
        window.display();
    }

    return 0;
}


//int main() {
//    Game game;
//    game.run();
//    return 0;
//}