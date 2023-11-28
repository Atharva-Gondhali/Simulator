#pragma once
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Physics
{
private:
	sf::Event ev;
	sf::CircleShape ball;
	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	sf::ContextSettings settings;
	std::vector<sf::Vertex> vertices;


	sf::Vertex lines[4];
	sf::Vertex triangles[6];

	bool start = false;
	int frameState = 0;

	float angle;
	float mainVel;
	sf::Vector2f vel;
	sf::Vector2f diffVel;
	sf::Vector2f learpVel;


	//Gravity
	float gravity;
	float mass;

	//Drag
	float diameter;
	float densityAir;

	const float pi = 3.142857f;
	const float dragCoeff = 0.47f;
	const float viscosity = 0.00001f;

	void initWindow();
	void initVariable();

	void updateMotion();
	void updateState();

	void spawnBall();
	void updateBall();
	void updateGrid();

	void renderBall();
	void renderGrid();

	void pollEvents();

public:
	Physics();

	const bool running() const;

	void update();
	void render();

};