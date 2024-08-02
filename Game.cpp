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
    if (!this->font.loadFromFile("Fonts/ChakraPetch-Bold.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font \n";
    }

        
}

void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::Black);
    this->uiText.setString("NULL");
}

void Game::initTexture()
{
    if (!this->fruitSprite.loadFromFile("assets/fruitTextures.png"))
    {
        std::cout << "ERROR::GAME::INITTEXTURE::Failed to load texture \n";
    }
    if (!this->skyTexture.loadFromFile("assets/cloud.jpg"))
    {
        std::cout << "ERROR::GAME::INITTEXTURE::Failed to load texture \n";
    }

    this->skybox.setPosition(0.f, 0.f);
    this->skybox.setTexture(this->skyTexture);
}

void Game::initEnemies()
{
    this->enemy.setPosition(0.f,0.f);
    this->enemy.setScale(5, 5);
    //this->enemy.setSize(sf::Vector2f(120.f,120.f));
    this->enemy.setTexture(this->fruitSprite);
    this->enemy.setTextureRect(sf::IntRect(0, 0, 16, 16));//setFillColor(sf::Color::Cyan);
}

Game::Game()
{
	this->initVariables();
	this->initWindow();
    this->initFonts();
    this->initText();
    this->initTexture();
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

    int type = rand() % 38;
    this->enemy.setTextureRect(sf::IntRect(type * 16, 0, 16, 16));
    
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - 80)),
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

void Game::renderSkybox(sf::RenderTarget& target)
{
    target.draw(this->skybox);
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

    this->renderSkybox(*this->window);

    this->renderEnemies(*this->window);

    this->renderText(*this->window);

    this->window->display();
}
