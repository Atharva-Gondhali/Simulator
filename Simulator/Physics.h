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
	sf::CircleShape ball;

	bool start = false;

	float vel;
	float velX;
	float velY;
	float angle;
	float scale;


	//Gravity
	float gravity;
	float mass;

	//Drag
	float diameter;
	float densityAir;

	//Acceln
	float acc_x;
	float acc_y;

	const float pi = 3.14f;
	const float viscosity = 0.00001f;
	const float dragCoeff = 0.47f;

	void initWindow();
	void initVariable();
	void updateMotion();
	void renderBall();
	void spawnBall();
	void updateBall();
	void pollEvents();

public:
	Physics();

	const bool running() const;

	void update();

	void render();

};