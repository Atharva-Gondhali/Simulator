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
	vel = 13.8f;
	angle = 45.f;
	velX = vel * cos(angle * (3.14f / 180.f));
	velY = vel * sin(angle * (3.14f / 180.f));
	mass = 1.f;
	gravity = mass * 9.8f;
	diameter = 0.08f;
	densityAir = 1.2f;
}



void Physics::updateMotion()
{
	float reynolds, forceDrag_x, forceDrag_y;

	reynolds = (densityAir * diameter * velX) / viscosity;
	if (reynolds > 1)
	{
		forceDrag_x = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(velX, 2.f));
		forceDrag_y = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(velY, 2.f));
	}
	else
	{
		forceDrag_x = 6.f * pi * viscosity * (diameter / 2.f) * velX;
		forceDrag_y = 6.f * pi * viscosity * (diameter / 2.f) * velY;
	}

	std::cout << forceDrag_x << " " << forceDrag_y << " " << gravity << "\n";

	acc_x = -(forceDrag_x / mass);
	if (velY > 0)
	{
		acc_y = -((gravity + forceDrag_y) / mass);
	}
	else
	{
		acc_y = ((gravity - forceDrag_y) / mass);
	}

	// v sqrt(2a + u2)
	float t_velX, t_velY;
	if (velX < 0)
	{
		t_velX = -(powf(velX, 2.f) + (2.f * acc_x));
	}
	else
	{
		t_velX = powf(velX, 2.f) + (2.f * acc_x);
	}
	if (velY < 0)
	{
		t_velY = -(powf(velY, 2.f) + (2.f * acc_y));
	}
	else
	{
		t_velY = powf(velY, 2.f) + (2.f * acc_y) ;
	}

	if (t_velX > 0)
	{
		velX = sqrt(t_velX);
	}
	else
	{
		velX = -sqrt(-t_velX);
	}
	if (t_velY > 0)
	{
		velY = sqrt(t_velY);
	}
	else
	{
		velY = -sqrt(-t_velY);
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
			start = true;
			break;
		}
	}
}

void Physics::spawnBall()
{
	this->ball.setPosition(640.f, 360.f);
	this->ball.setFillColor(sf::Color::Blue);
}

void Physics::updateBall()
{
	std::cout << velX << " " << velY << "\n";
    updateMotion();
	this->ball.move(sf::Vector2f(velX / 6 + 5, -(velY / 6 + 5)));
}

void Physics::update()
{
	this->pollEvents();
	this->updateBall();
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
