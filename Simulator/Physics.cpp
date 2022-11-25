#include "Physics.h"

void Physics::initWindow()
{
	this->videoMode.height = 720;
	this->videoMode.width = 1280;
	this->window = new sf::RenderWindow(this->videoMode, "Simulator", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void Physics::initVariable()
{
	this->ball.setRadius(10.f);
}

Physics::Physics()
{
	this->initVariable();	
	this->initWindow();
}

const bool Physics::running() const
{
	return this->window->isOpen();
}

void Physics::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		}
	}
}

void Physics::spawnBall()
{
	this->ball.setPosition(10.f, 10.f);
	this->ball.setFillColor(sf::Color::Blue);
}

void Physics::update()
{
	this->pollEvents();
	this->spawnBall();
}

void Physics::renderBall()
{
	this->window->draw(ball);
}

void Physics::render()
{
	this->window->clear();
	this->renderBall();
	this->window->display();
}
