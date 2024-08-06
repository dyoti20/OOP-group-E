#include "Game.h"
#include <iostream>

// Private functions
void Game::initializeVariables()
{
    this->window = nullptr; // initializes a variable

    // Game Logic
    this->endGame = false;
    this->points = 0;
    this->health = 10;
    this->circleSpawnTimerMax = 10.f;
    this->circleSpawnTimer = this->circleSpawnTimerMax;
    this->maxCircles = 2;
    this->keyboardHeld = false;
    this->showGameOverScreen = false;
}

void Game::initWindow()
{
    this->videoMode.height = 720;
    this->videoMode.width = 1080;

    this->window = new sf::RenderWindow(this->videoMode, "song: Baka mitai", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(120);
}

void Game::initFonts()
{
    if (!this->font.loadFromFile("Fonts/From-Cartoon-Blocks.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
    }
}

void Game::initTexts()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

void Game::initCircles()
{
    this->circleLifetime = (1.5);
}

void Game::initMusic()
{
    if (!this->audio.openFromFile("Music/audio.ogg"))
    {
        std::cout << "ERROR::GAME::INITMUSIC::Failed to load music!" << "\n";
    }
    else
    {
        this->audio.setLoop(false); // don't loop the music
        this->audio.play(); // play the music
        this->audio.setVolume(25.f);
    }
}

void Game::initSound()
{
    if (!this->hitSoundBuffer.loadFromFile("Music/check-off.wav"))
    {
        std::cout << "ERROR::GAME::INITSOUNDS::Failed to load hit sound!" << "\n";
    }
    this->hitSound.setBuffer(this->hitSoundBuffer);
    this->hitSound.setVolume(700.f);
}

void Game::initBackground()
{

    if (!this->backgroundTexture.loadFromFile("images/BakaMitai.png")) {
        std::cerr << "Error loading background image!" << std::endl;
        return;
    }
    this->backgroundSprite.setTexture(this->backgroundTexture);
    // Optionally scale or position the sprite
    this->backgroundSprite.setScale(
        static_cast<float>(this->window->getSize().x) / this->backgroundTexture.getSize().x,
        static_cast<float>(this->window->getSize().y) / this->backgroundTexture.getSize().y
    );
    sf::Color backgroundColor(255, 255, 255, 128);
    backgroundSprite.setColor(backgroundColor);
}

void Game::initTextures()
{
    if (!this->circleTexture.loadFromFile("images/hitcircle.png")) {
        std::cerr << "Error loading circle texture !" << std::endl;
    }
}

// Constructors / Deconstructors
Game::Game()
{
    this->initializeVariables();
    this->initWindow();
    this->initFonts();
    this->initTexts();
    this->initCircles();
    this->initMusic();
    this->initSound();
    this->loadBeatTimings("beats.txt");
    this->initBackground();
    this->initTexts();
    this->initTextures();
}

Game::~Game()
{
    delete this->window;
}

// Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

 

// Functions
void Game::spawnCircle()
{
    /*
        @return void
        Spawns circles and sets their colors and positions
        - sets a random position
        - adds circle to vector
    */


    // CircleWithTimer newCircle;
    //newCircle.circle.setRadius(100.f);
    //newCircle.circle.setScale(sf::Vector2f(0.5f, 0.5f));
    //newCircle.circle.setOutlineColor(sf::Color::Magenta);
    //newCircle.circle.setOutlineThickness(3.f);
    //newCircle.circle.setFillColor(sf::Color::Transparent);
    //newCircle.circle.setPosition(
    //    static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 2 * newCircle.circle.getRadius())),
    //    static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - 2 * newCircle.circle.getRadius()))
    //);
    //newCircle.remainingTime = this->circleLifetime;

    //this->circles.push_back(newCircle);
        
    
        // Create a new CircleWithTimer object
 
    CircleWithTimer newCircle;
    newCircle.circleSprite.setTexture(this->circleTexture);
    newCircle.circleSprite.setScale(sf::Vector2f(1.f, 1.f));
    newCircle.circleSprite.setPosition(
        static_cast<float>(rand() % (this->window->getSize().x - 100)),
        static_cast<float>(rand() % (this->window->getSize().y - 100))
    );
    newCircle.remainingTime = this->circleLifetime;
    this->circles.push_back(newCircle);

}

void Game::loadBeatTimings(const std::string& filename)
{
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "ERROR::GAME::LOADBEATTIMINGS::Failed to open beat timings file!" << std::endl;
        return;
    }

    float timing;
    while (infile >> timing) {
        beatTimings.push_back(timing);
    }
    infile.close();
}

void Game::pollEvents()
{
    // Event polling
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateMousePositions()
{
    /*
        @return void
        Updates the mouse positions:
        - Mouse positions relative to window (Vector2i)
    */

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
    std::stringstream ss;

    ss << "Points: " << this->points << "\n"
        << "Health: " << this->health << "\n";

    this->uiText.setString(ss.str());
}

void Game::updateCircles()
{
    /**
        @return void
        Updates the circle spawn timer and spawns circles when the total
        amount of circles is smaller than the max

        spawns the circles in random locations
        Removes the circles if it has been hit by the user
        Gives points to the user if the circle has been hit
        if the circle has not been hit on time, remove 1 health from the user
    */

    float currentTime = this->audio.getPlayingOffset().asSeconds();
    if (!beatTimings.empty() && currentTime >= beatTimings.front())
    {
        this->spawnCircle();
        beatTimings.erase(beatTimings.begin());
    }
    for (size_t i = 0; i < this->circles.size();)
    {
        this->circles[i].remainingTime -= 1.f / 60.f;
        if (this->circles[i].remainingTime <= 0.f)
        {
            this->circles.erase(this->circles.begin() + i);
            this->health -= 1;
            std::cout << "Health: " << this->health << "\n";
        }
        else
        {
            ++i;
        }
    }

    // Check if Z, X, or left mouse button is pressed to hit circles
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->keyboardHeld == false)
        {
            this->keyboardHeld = true;
            bool deleted = false;
            for (size_t i = 0; i < this->circles.size() && deleted == false; i++)
            {
                if (this->circles[i].circleSprite.getGlobalBounds().contains(this->mousePosView))
                {
                    deleted = true;
                    this->circles.erase(this->circles.begin() + i);
                    this->points += 1;
                    std::cout << "Points: " << this->points << "\n";
                    this->hitSound.play();
                }
            }
        }
    }
    else
    {
        this->keyboardHeld = false;
    }
}

void Game::update()
{
    this->pollEvents();

    if (!this->endGame)
    {
        this->updateMousePositions();
        this->updateText();
        this->updateCircles();
    }
    // Check if music has ended
    if (this->audio.getStatus() == sf::SoundSource::Stopped)
    {
        this->showGameOverScreen = true;
    }

    // End game condition
    if (this->health <= 0)
        this->showGameOverScreen = true;
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderCircles(sf::RenderTarget& target)
{
    // Rendering all the circles
    for (auto& circle: this->circles)
    {
        target.draw(circle.circleSprite);
    }
}

void Game::render()
{
    {
        /*
            @return void

            - Clear old frame
            - Render objects
            - Display frame in window
            - Display end game screen where it shows total points
            - End game screen pops up when the song is finished playing or health reaches 0

            Renders the game objects
        */

        this->window->clear();

        if (!this->showGameOverScreen)
        {
            // Draw game objects
            this->window->draw(this->backgroundSprite);
            this->renderCircles(*this->window);
            this->renderText(*this->window);
            
        }
        else
        {
            // Stop music from playing in case health depleted before music is done playing
            this->audio.stop();

            // Displaying endgame screen with points
            sf::Text endText;
            endText.setFont(this->font);
            endText.setCharacterSize(45);
            endText.setFillColor(sf::Color::Red);
            endText.setString("Game over\n Total Points: " + std::to_string(this->points));

            // Centering the text on the screen
            sf::FloatRect textRect = endText.getLocalBounds();
            endText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            endText.setPosition(sf::Vector2f(this->window->getSize().x / 2.0f, this->window->getSize().y / 2.0f));

            this->window->draw(endText);
        }

        this->window->display();
    }
}


//this makes it able to run the game after clicking play on the main menu

void Game::run()
{
    while (this->window->isOpen()) {
        this->update();
        this->render();
    }
}

