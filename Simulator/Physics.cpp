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
		case sf::Event::MouseButtonReleased:
			this->strike();
			break;
 		}
	}
}

void Physics::checkBounds()
{
	if (!(this->ball.getPosition().x > -1.f) || !(this->ball.getPosition().x < 1260.f)) 
	{
		velX *= -1;
	}
	if (!(this->ball.getPosition().y > -1.f) || !(this->ball.getPosition().y < 700.f))
	{
		velY *=-1;
	}
}

void Physics::linePhy()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->mousePosWindow = sf::Mouse::getPosition(*this->window);
		this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

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

void Physics::strike()
{
	float distX, distY;
	distX = this->ball.getPosition().x - this->mousePosView.x;
	distY = this->ball.getPosition().y - this->mousePosView.y;

	angle = atanf(distY / distX);
	
	vel = sqrtf(powf(distX, 2.f) + powf(distY, 2.f));

	velX = vel * cos(angle);
	velY = vel * sin(angle);

	if ((distX < 0 && distY < 0) || (distX < 0 && distY > 0))
	{
		velX *= -1;
		velY *= -1;
	}
}

void Physics::spawnBall()
{
	this->ball.setPosition(100.f, 100.f);
	this->ball.setFillColor(sf::Color::Red);
}

void Physics::updateBall()
{
	this->ball.move(0.05f * velX, 0.05f * velY);
}

void Physics::update()
{
	this->pollEvents();
	this->linePhy();
	this->updateBall();
	this->checkBounds();
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
