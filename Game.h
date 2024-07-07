#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

/*
	Main game class
*/



class Game
{
private:

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//resources
	sf::Font font;

	sf::Text uiText;

	//game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemy;
	bool mouseHeld;

	//game objects
	std::vector < sf::RectangleShape> enemies;
	sf::RectangleShape enemy;


	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();

public:
	//const / dist
	Game();
	virtual ~Game();

	const bool running() const;
	const bool getEndGame() const;

	//function
	void spawnEnemy();

	void pollEvents();
	void updateMousePos();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};

