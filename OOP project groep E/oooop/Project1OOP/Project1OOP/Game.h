#pragma once

#include <iostream>
#include <Vector>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream> //allows you to create a long string with mix input types like int, float, double etc.

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/window.hpp>
#include <SFML/audio.hpp>
#include <SFML/Network.hpp>

/*
Class that acts as the game engine.
Wrapper class.
*/

struct CircleWithTimer
{
	sf::Sprite circleSprite;
	float remainingTime;
};


class Game
{
private:
	//Variables
	//window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;



	//Recources
	sf::Font font;
	//Text (creating a variable to hold text
	sf::Text uiText;
	//Circle texture
	sf::Texture circleTexture; // Texture for circles
	//Background
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	//Music
	sf::Music audio;
	//plays sound effect when circle is hit
	sf::SoundBuffer hitSoundBuffer;
	sf::Sound hitSound;


	//Game Logic
	bool endGame;
	int points;		//unsigned means positive values only
	int health;
	float circleSpawnTimer;
	float circleSpawnTimerMax;
	unsigned maxCircles;
	bool keyboardHeld;
	bool showGameOverScreen;


    //circle timer
	float circleLifetime;
	//Game objects
	std::vector<CircleWithTimer> circles;
	//Beat timing
	std::vector<float> beatTimings; //adding a member variable to store beat timings
	//Mouse positions
	sf::Vector2i mousePosWindow; //holds 2 positions(x,y)
	sf::Vector2f mousePosView;
	

	//Private functions
	void initializeVariables();
	void initWindow();
	void initFonts();
	void initTexts();
	void initCircles(); //too much (not needed because of struct CircleWithTimer)
	void initMusic();
	void initSound();
	void initBackground();
	void initTextures();
	void loadBeatTimings(const std::string& filename);

public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnCircle();
	struct circleWithTimer;

	

	void pollEvents();
	void updateMousePositions();
	void updateText();
	//void updateMusic();
	void updateCircles();
	void update();

	void renderText(sf::RenderTarget& target); // this makes it able to render directly without calling to render in the window//
	void renderCircles(sf::RenderTarget& target);
	void render();
	
	//run the rythm game
	void run();
};


