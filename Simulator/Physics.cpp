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
	angle = 45.f;
	mainVel = 100.f * (5.f / 18.f);
	diameter = 0.08f;
	densityAir = 1.29f;
	gravity = mass * 9.8f;
	this->ball.setRadius(10.f);
	this->learpVel.x = mainVel * cos(angle * (pi / 180));
	this->learpVel.y = mainVel * sin(angle * (pi / 180));
	this->vel = sf::Vector2f(this->learpVel.x, this->learpVel.y);

	triangles[0].position = sf::Vector2f(1240, 680);
	triangles[1].position = sf::Vector2f(1230, 685);
	triangles[2].position = sf::Vector2f(1230, 675);
	triangles[3].position = sf::Vector2f(40, 30);
	triangles[4].position = sf::Vector2f(45, 40);
	triangles[5].position = sf::Vector2f(35, 40);

	lines[0].position = sf::Vector2f(40, 680);
	lines[1].position = sf::Vector2f(1240, 680);
	lines[2].position = sf::Vector2f(40, 680);
	lines[3].position = sf::Vector2f(40, 30);
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

void Physics::updateState()
{
	if (this->ball.getPosition().x < 1024 &&
		this->ball.getPosition().y > 30)
	{
		frameState = 3;
	}
	else if (this->ball.getPosition().x > 1024 && this->ball.getPosition().y > 30)
	{
		frameState = 1;
	}
	else if (this->ball.getPosition().y < 30 && this->ball.getPosition().x < 1024)
	{
		if ((this->lines[0].position.y + this->vel.y) < 680)
		{
			if (this->lines[0].position.y != 680)
			{
				frameState = 4;
			}
			else
			{
				frameState = 3;
			}
		}
		else
		{
			frameState = 2;
		}
	}
	else
	{
		if ((this->lines[0].position.y + this->vel.y) < 680)
		{
			if (this->lines[0].position.y != 680)
			{
				frameState = 4;
			}
			else
			{
				frameState = 1;
			}
		}
		else
		{
			frameState = 0;
		}
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
	if (this->ball.getPosition().y + 10 <= 680)
	{
		this->updateMotion();
		this->updateState();
		std::cout << frameState << std::endl;

		this->vel.x += (this->learpVel.x - this->vel.x) / 60;
		this->vel.y += (this->learpVel.y - this->vel.y) / 60;
		
		switch (frameState)
		{
		case 3:
			this->ball.move(sf::Vector2f(this->vel.x, -this->vel.y));
			break;
		case 1:
			this->ball.move(sf::Vector2f(0.0f, -this->vel.y));
			break;
		case 2:
			this->ball.move(sf::Vector2f(this->vel.x, 0.0f));
			break;
		case 0:
			break;
		default:
			float diff;
			diff = this->lines[0].position.y - 680;
			this->ball.move(sf::Vector2f(0.0f, diff));
		}
		this->vertices.push_back(sf::Vertex(sf::Vector2f(this->ball.getPosition().x + 10, this->ball.getPosition().y + 10), sf::Color::White));
	}
}

void Physics::updateGrid()
{
	switch (frameState)
	{
	case 3:
		break;
	case 1:
		for (int i = 0; i < this->vertices.size(); i++)
		{
			this->vertices[i].position -= sf::Vector2f(this->vel.x, 0.0f);
		}

		this->lines[2].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->lines[3].position -= sf::Vector2f(this->vel.x, 0.0f);

		this->triangles[3].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->triangles[4].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->triangles[5].position -= sf::Vector2f(this->vel.x, 0.0f);
		break;
	case 2:
		for (int i = 0; i < this->vertices.size(); i++)
		{
			this->vertices[i].position -= sf::Vector2f(0.0f, -this->vel.y);
		}

		this->lines[0].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->lines[1].position -= sf::Vector2f(0.0f, -this->vel.y);

		this->triangles[0].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->triangles[1].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->triangles[2].position -= sf::Vector2f(0.0f, -this->vel.y);
		break;
	case 0:
		for (int i = 0; i < this->vertices.size(); i++)
		{
			this->vertices[i].position -= sf::Vector2f(this->vel.x, -this->vel.y);
		}

		this->lines[2].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->lines[3].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->lines[0].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->lines[1].position -= sf::Vector2f(0.0f, -this->vel.y);

		this->triangles[3].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->triangles[4].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->triangles[5].position -= sf::Vector2f(this->vel.x, 0.0f);
		this->triangles[0].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->triangles[1].position -= sf::Vector2f(0.0f, -this->vel.y);
		this->triangles[2].position -= sf::Vector2f(0.0f, -this->vel.y);
		break;
	default:
		float diff;
		diff = this->lines[0].position.y - 680;

		for (int i = 0; i < this->vertices.size(); i++)
		{
			this->vertices[i].position += sf::Vector2f(0.0f, -diff);
		}

		this->lines[0].position += sf::Vector2f(0.0f, -diff);
		this->lines[1].position += sf::Vector2f(0.0f, -diff);

		this->triangles[0].position += sf::Vector2f(0.0f, -diff);
		this->triangles[1].position += sf::Vector2f(0.0f, -diff);
		this->triangles[2].position += sf::Vector2f(0.0f, -diff);
	}	
}

void Physics::update()
{
	this->pollEvents();
	if (start)
	{
		this->updateBall();
		this->updateGrid();
	}
}

void Physics::renderBall()
{
	this->window->draw(&vertices[0], this->vertices.size(), sf::Lines);
	this->window->draw(ball);
}

void Physics::renderGrid()
{
	lines[0].color = sf::Color::White;
	lines[1].color = sf::Color::White;
	lines[2].color = sf::Color::White;
	lines[3].color = sf::Color::White;

	triangles[0].color = sf::Color::White;
	triangles[1].color = sf::Color::White;
	triangles[2].color = sf::Color::White;
	triangles[3].color = sf::Color::White;
	triangles[4].color = sf::Color::White;
	triangles[5].color = sf::Color::White;

	this->window->draw(triangles, 6, sf::Triangles);
	this->window->draw(lines, 4, sf::Lines);
}

void Physics::render()
{
	this->window->clear();
	this->renderGrid();
	this->renderBall();
	this->window->display();
}
