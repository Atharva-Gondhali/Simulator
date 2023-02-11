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
	scale = 6;
	vel = 20.f;
	angle = 60.f;
	velX = vel * cos(angle * (3.14f / 180.f));
	velY = vel * sin(angle * (3.14f / 180.f));
	mass = 1.f;
	gravity = mass * 9.8f;
	diameter = 0.08f;
	densityAir = 50.f;
}



void Physics::updateMotion()
{
	float reynolds_x, reynolds_y, forceDrag_x, forceDrag_y;

	reynolds_x = (densityAir * diameter * velX) / viscosity;
	reynolds_y = (densityAir * diameter * velY) / viscosity;
	if (reynolds_x > 1)
	{
		forceDrag_x = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(velX, 2.f));
	}
	else
	{
		forceDrag_x = 6.f * pi * viscosity * (diameter / 2.f) * velX;
	}
	if (reynolds_y > 1)
	{
		forceDrag_y = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(velY, 2.f));
	}
	else
	{
		forceDrag_y = 6.f * pi * viscosity * (diameter / 2.f) * velY;
	}

	//std::cout << forceDrag_x << " " << forceDrag_y << " " << gravity << "\n";

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
	this->ball.setPosition(0.f, 720.f);
	this->ball.setFillColor(sf::Color::Blue);
	vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
}

void Physics::updateBall()
{
	//std::cout << velX << " " << velY << "\n";
    this->updateMotion();
	this->ball.move(sf::Vector2f(velX / 6 + scale, -(velY / 6 + scale)));
	vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
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
	this->window->draw(&vertices[0], vertices.size(), sf::Lines);
	this->window->draw(ball);
}

void Physics::render()
{
	this->window->clear();
	this->renderBall();
	this->window->display();
}
