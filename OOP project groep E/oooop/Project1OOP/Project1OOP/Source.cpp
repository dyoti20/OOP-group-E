#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Game.h"

void showMenu(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("Fonts/From-Cartoon-Blocks.ttf")) {
        std::cout << "Error loading font\n";
        return;
    }
    bool startGame = false;

    sf::Text title("Welcome to Rhythm Game!", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(50, 100);

    sf::Text play("Press 1 to Play", font, 30);
    play.setFillColor(sf::Color::White);
    play.setPosition(50, 300);

    sf::Text exit("Press 2 to Exit", font, 30);
    exit.setFillColor(sf::Color::White);
    exit.setPosition(50, 350);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    window.close();
                    startGame = true; // Start the game
                   
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(title);
        window.draw(play);
        window.draw(exit);
        window.display();

        //show the game if 
        if (startGame)
        {
            Game game;
            game.run();
            startGame = false;
        }

  
    }
}

int main() {

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Loading Screen");

    // Set up the loading bar
    sf::Font font;
    if (!font.loadFromFile("Fonts/From-Cartoon-Blocks.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1; // Exit if the font loading fails
    }

    sf::Text welcomeText("WELCOME", font, 50);
    welcomeText.setFillColor(sf::Color::Yellow);
    welcomeText.setPosition(200, 200);

    sf::RectangleShape barBackground(sf::Vector2f(500, 30));
    barBackground.setFillColor(sf::Color::White);
    barBackground.setPosition(150, 300);

    sf::RectangleShape barProgress(sf::Vector2f(0, 30));
    barProgress.setFillColor(sf::Color::Yellow);
    barProgress.setPosition(150, 300);

    // Loading bar animation
    for (int i = 0; i <= 500; i += 20) {
        barProgress.setSize(sf::Vector2f(i, 30));

        window.clear();
        window.draw(welcomeText);
        window.draw(barBackground);
        window.draw(barProgress);
        window.display();
        Sleep(150);
        
    }

    Sleep(1000); // Pause for a moment before showing the menu

    // Show the menu
  
    showMenu(window);

   

    return 0;
}
