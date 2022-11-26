#include "Physics.h"

void Physics::initWindow()
{
	this->videoMode.height = 720;
	this->videoMode.width = 1280;
	this->settings.antialiasingLevel = 8;
	this->window = new sf::RenderWindow(this->videoMode, "Simulator", sf::Style::Titlebar | sf::Style::Close, settings);
	this->window->setFramerateLimit(60);
}

void Physics::initVariable()
{
	this->ball.setRadius(10.f);

	vel = 27.7f;
	angle = 45.f;
	velX = vel * cos(angle * (3.14f / 180.f));
	velY = -(vel * sin(angle * (3.14f / 180.f)));
}

Physics::Physics()
{
	this->initVariable();
	this->initWindow();

	this->spawnBall();
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

void Physics::linePhy()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->line[0].position = this->mousePosView;
		this->line[1].position = sf::Vector2f(this->ball.getPosition().x + 10.f, this->ball.getPosition().y + 10.f);
		this->line[0].color = ball.getFillColor();
		this->line[1].color = sf::Color::White;
	}
	else
	{
		this->line[0].position = sf::Vector2f(0.f, 0.f);
		this->line[1].position = sf::Vector2f(0.f, 0.f);
		this->line->color = sf::Color::Black;
	}
}

void Physics::spawnBall()
{
	this->ball.setPosition(100.f, 100.f);
	this->ball.setFillColor(sf::Color::Red);
}

void Physics::updateBall()
{
}

void Physics::update()
{
	this->pollEvents();
	this->linePhy();
	this->updateBall();
}

void Physics::renderBall()
{
	this->window->draw(ball);
}

void Physics::render()
{
	this->window->clear();
	this->window->draw(line, 2, sf::Lines);
	this->renderBall();
	this->window->display();
}
