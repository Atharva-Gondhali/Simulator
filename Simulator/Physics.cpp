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
	vel = 41.6f;
	angle = 20.f;
	velX = vel * cos(angle * (3.14f / 180.f));
	velY = -(vel * sin(angle * (3.14f / 180.f)));
	gravity = 9.8f;
	mass = 10.f;
	diameter = 0.08f;
	densityAir = 1.2f;
}

void Physics::initAcceln()
{
	float reynolds, forceGravity, forceDrag;

	forceGravity = mass * gravity;

	reynolds  = (densityAir * diameter * velX) / viscosity;
	if (reynolds > 1)
	{
		forceDrag = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(velX, 2.f));
	}
	else
	{
		forceDrag = 6.f * pi * viscosity * (diameter / 2.f) * velX;
	}
	verticalAcceln = -((forceGravity / mass) / 10);
	horizontalAcceln = -((forceDrag / mass) / 10);
}

void Physics::projectCalc()
{
	// v sqrt(2a + u2)
	float t_velX, t_velY;
	if (velX < 0)
	{
		t_velX = -powf(velX, 2.f) + (2.f * horizontalAcceln);
	}
	else
	{
		t_velX = powf(velX, 2.f) + (2.f * horizontalAcceln);
	}
	if (velY < 0)
	{
		t_velY = -powf(velY, 2.f) - (2.f * verticalAcceln) ;
	}
	else
	{
		t_velY = powf(velY, 2.f) - (2.f * verticalAcceln) ;
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
	this->initAcceln();
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

void Physics::spawnBall()
{
	this->ball.setPosition(0.f, 720.f);
	this->ball.setFillColor(sf::Color::Blue);
}

void Physics::updateBall()
{
	std::cout << velX << " " << velY << "\n";
	projectCalc();
	this->ball.move(0.167f * velX, 0.167f * velY);
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
