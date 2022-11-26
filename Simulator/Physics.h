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

	sf::CircleShape ball;

	float vel;
	float velX;
	float velY;
	float angle;


	//Gravity
	float gravity;
	float mass;

	//Drag
	float diameter;
	float densityAir;

	//Acceln
	float verticalAcceln;
	float horizontalAcceln;

	const float pi = 3.14f;
	const float viscosity = 0.00001f;
	const float dragCoeff = 0.47f;

	void initWindow();
	void initVariable();
	void initAcceln();
	
	void projectCalc();

public:
	Physics();

	const bool running() const;

	void pollEvents();

	void spawnBall();
	void updateBall();
	void update();

	void renderBall();
	void render();

};