#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Physics
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	sf::CircleShape ball;

	void initWindow();
	void initVariable();

public:
	Physics();

	const bool running() const;

	void pollEvents();

	void spawnBall();
	void update();

	void renderBall();
	void render();

};