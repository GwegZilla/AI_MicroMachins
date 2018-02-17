#include "stdafx.h"
#include "PlayerCarController.h"


aimm::PlayerCarController::PlayerCarController(Car* l_poCar)
{
	SetCar(l_poCar);
}


aimm::PlayerCarController::~PlayerCarController()
{
}

void aimm::PlayerCarController::Start()
{
	CarController::Start();
}

void aimm::PlayerCarController::Update()
{
	CarController::Update();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) == true)
	{
		CarController::TurnLeft();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) == true)
	{
		CarController::TurnRight();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) == true)
	{
		CarController::Accelerate();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) == true)
	{
		CarController::Brake();
	}
}

void aimm::PlayerCarController::Destroy()
{
	CarController::Destroy();
}
