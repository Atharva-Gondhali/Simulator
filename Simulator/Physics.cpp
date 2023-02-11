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
	mass = 1.f;
	scale = 10;
	angle = 45.f;
	mainVel = 14.f;
	diameter = 0.08f;
	densityAir = 50.f;
	gravity = mass * 9.8f;
	this->ball.setRadius(10.f);
	this->learpVel.x = mainVel * cos(angle * (pi / 180));
	this->learpVel.y = mainVel * sin(angle * (pi / 180));
	this->vel = sf::Vector2f(this->learpVel.x, this->learpVel.y);
}



void Physics::updateMotion()
{
	sf::Vector2f reynolds, forceDrag, acc, tempVel;

	reynolds.x = (densityAir * diameter * this->learpVel.x) / viscosity;
	reynolds.y = (densityAir * diameter * this->learpVel.y) / viscosity;

	if (reynolds.x > 1)
	{
		forceDrag.x = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(this->learpVel.x, 2.f));
	}
	else
	{
		forceDrag.x = 6.f * pi * viscosity * (diameter / 2.f) * this->learpVel.x;
	}
	if (reynolds.y > 1)
	{
		forceDrag.y = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(this->learpVel.y, 2.f));
	}
	else
	{
		forceDrag.y = 6.f * pi * viscosity * (diameter / 2.f) * this->learpVel.y;
	}

	acc.x = -(forceDrag.x / mass);

	if (this->learpVel.y > 0)
	{
		acc.y = -((gravity + forceDrag.y) / mass);
	}
	else
	{
		acc.y = ((gravity - forceDrag.y) / mass);
	}

	// v sqrt(2a + u2)

	if (this->learpVel.x < 0)
	{
		tempVel.x = -(powf(this->learpVel.x, 2.f) + (2.f * acc.x));
	}
	else
	{
		tempVel.x = powf(this->learpVel.x, 2.f) + (2.f * acc.x);
	}
	if (this->learpVel.y < 0)
	{
		tempVel.y = -(powf(this->learpVel.y, 2.f) + (2.f * acc.y));
	}
	else
	{
		tempVel.y = powf(this->learpVel.y, 2.f) + (2.f * acc.y);
	}

	if (tempVel.x > 0)
	{
		this->learpVel.x = sqrt(tempVel.x);
	}
	else
	{
		this->learpVel.x = -sqrt(-tempVel.x);
	}
	if (tempVel.y > 0)
	{
		this->learpVel.y = sqrt(tempVel.y);
	}
	else
	{
		this->learpVel.y = -sqrt(-tempVel.y);
	}
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

		case sf::Event::MouseButtonPressed:
			if(start)
			{
				start = false;
			}
			else
			{
				start = true;
			}
			break;
		}
	}
}

void Physics::spawnBall()
{
	this->ball.setPosition(30.f, 670.f);
	this->ball.setFillColor(sf::Color::Blue);
	this->vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
}

void Physics::updateBall()
{
	std::cout << this->vel.x << " " << this->vel.y << "\n";


	if (counter > 10)
	{
		this->updateMotion();
		counter = 0;
	}
	else
	{
		if (counter == 0)
		{
			this->updateMotion();
		}
			
		this->vel.x += (this->learpVel.x - this->vel.x) / scale;
		this->vel.y += (this->learpVel.y - this->vel.y) / scale;

		counter++;
	}


	this->ball.move(sf::Vector2f(this->vel.x, -this->vel.y));
	this->vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
}

void Physics::update()
{
	this->pollEvents();
	if (start)
	{
		this->updateBall();
	}
}

void Physics::renderBall()
{
	this->window->draw(&vertices[0], this->vertices.size(), sf::Lines);
	this->window->draw(ball);
}

void Physics::render()
{
	this->window->clear();
	this->renderBall();
	this->window->display();
}
