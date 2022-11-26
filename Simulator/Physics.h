#pragma once
#include <iostream>

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
	sf::ContextSettings settings;

	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Vertex line[2];
	sf::CircleShape ball;

	float vel;
	float velX;
	float velY;
	float angle;

	const float pi = 3.14f;
	const float viscosity = 0.00001f;
	const float dragCoeff = 0.47f;

	void initWindow();
	void initVariable();


	void pollEvents();
	void linePhy();
	void spawnBall();
	void updateBall();
	void renderBall();

public:
	Physics();

	const bool running() const;

	void update();

	void render();

};