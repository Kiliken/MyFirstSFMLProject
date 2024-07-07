#include "Game.h"

void Game::initVariables() 
{
	this->window = nullptr;

    //game logic
    this->endGame = false;
    this->points = 0;
    this->health = 10;
    this->enemySpawnTimerMax = 50.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemy = 5;
    this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800; 
	this->window = new sf::RenderWindow(this->videoMode, "First Game", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
    if (this->font.loadFromFile("Fonts/ChakraPetch-Regular.ttf"))
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font \n";
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NULL");
}

void Game::initEnemies()
{
    this->enemy.setPosition(0.f,0.f);
    this->enemy.setSize(sf::Vector2f(120.f,120.f));
    this->enemy.setFillColor(sf::Color::Cyan);
}

Game::Game()
{
	this->initVariables();
	this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
    return this->endGame;
}

void Game::spawnEnemy()
{

    int type = rand() % 5;

    switch (type)
    {
        case 0:
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            this->enemy.setFillColor(sf::Color::Magenta);
            break;
        case 1:
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;
        case 2:
            this->enemy.setSize(sf::Vector2f(70.f, 70.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;
        case 3:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;
        case 4:
            this->enemy.setSize(sf::Vector2f(120.f, 120.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;
    }

    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
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

void Game::updateMousePos()
{
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

void Game::updateEnemies()
{   
    if (this->enemies.size() < this->maxEnemy)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
            
        else
            this->enemySpawnTimer += 1.f;
    }
    
    for (int i=0; i < this->enemies.size(); i++) 
    {

        this->enemies[i].move(0.f, 5.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
        }
            
    }

    

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false) 
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);

                    this->points += 1;
                }
            }
        }
        
    }
    else
    {
        this->mouseHeld = false;
    }
}

void Game::update()
{
    this->pollEvents();

    if (!this->endGame)
    {
       this->updateMousePos();

       this->updateText();

       this->updateEnemies();
    }

    if (this->health <= 0)
        this->endGame = true;
    
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}

void Game::render()
{
    this->window->clear();

    this->renderEnemies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}
