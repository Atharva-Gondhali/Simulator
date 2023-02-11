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
	mainVel = 14.f;
	angle = 1.047f;
	learpVel.x = mainVel * cos(angle);
	learpVel.y = mainVel * sin(angle);
	vel = sf::Vector2f(learpVel.x, learpVel.y);
	mass = 1.f;
	gravity = mass * 9.8f;
	diameter = 0.08f;
	densityAir = 50.f;
}



void Physics::updateMotion()
{
	
	float reynolds_x, reynolds_y, forceDrag_x, forceDrag_y;

	reynolds_x = (densityAir * diameter * learpVel.x) / viscosity;
	reynolds_y = (densityAir * diameter * learpVel.y) / viscosity;
	if (reynolds_x > 1)
	{
		forceDrag_x = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(learpVel.x, 2.f));
	}
	else
	{
		forceDrag_x = 6.f * pi * viscosity * (diameter / 2.f) * learpVel.x;
	}
	if (reynolds_y > 1)
	{
		forceDrag_y = 0.5f * densityAir * dragCoeff * (pi * powf(diameter / 2.f, 2.f)) * (powf(learpVel.y, 2.f));
	}
	else
	{
		forceDrag_y = 6.f * pi * viscosity * (diameter / 2.f) * learpVel.y;
	}

	//std::cout << forceDrag_x << " " << forceDrag_y << " " << gravity << "\n";

	acc_x = -(forceDrag_x / mass);
	if (learpVel.y > 0)
	{
		acc_y = -((gravity + forceDrag_y) / mass);
	}
	else
	{
		acc_y = ((gravity - forceDrag_y) / mass);
	}

	// v sqrt(2a + u2)
	float t_velX, t_velY;
	if (learpVel.x < 0)
	{
		t_velX = -(powf(learpVel.x, 2.f) + (2.f * acc_x));
	}
	else
	{
		t_velX = powf(learpVel.x, 2.f) + (2.f * acc_x);
	}
	if (learpVel.y < 0)
	{
		t_velY = -(powf(learpVel.y, 2.f) + (2.f * acc_y));
	}
	else
	{
		t_velY = powf(learpVel.y, 2.f) + (2.f * acc_y);
	}

	if (t_velX > 0)
	{
		learpVel.x = sqrt(t_velX);
	}
	else
	{
		learpVel.x = -sqrt(-t_velX);
	}
	if (t_velY > 0)
	{
		learpVel.y = sqrt(t_velY);
	}
	else
	{
		learpVel.y = -sqrt(-t_velY);
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
	this->ball.setPosition(0.f, 710.f);
	this->ball.setFillColor(sf::Color::Blue);
	vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
}

void Physics::updateBall()
{
	std::cout << vel.x << " " << vel.y << "\n";


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
			
		vel.x += (learpVel.x - vel.x) /10;
		vel.y += (learpVel.y - vel.y) /10;

		counter++;
	}


	this->ball.move(sf::Vector2f(vel.x, -vel.y));
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
