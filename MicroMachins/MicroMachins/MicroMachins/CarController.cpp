#include "stdafx.h"
#include "CarController.h"

aimm::CarController::CarController()
{
	m_fBrakeForce = DEFAULT_BRAKE;
	m_fAccelerationForce = DEFAULT_ACCELERATION;
	m_fTurnForce = DEFAULT_TURN;
}

aimm::CarController::~CarController()
{
	free(m_poCar);
}
void aimm::CarController::Start()
{
	Entity::Start();
}

void aimm::CarController::Update()
{
	Entity::Update();
}

void aimm::CarController::Destroy()
{
	Entity::Destroy();
}

void aimm::CarController::Accelerate()
{
	MY_ASSERT(m_poCar == nullptr, "This controller's car is null");
	m_poCar->Accelerate(m_fAccelerationForce);

}

void aimm::CarController::Brake()
{
	MY_ASSERT(m_poCar == nullptr, "This controller's car is null");
	m_poCar->Accelerate(-1.0f * m_fBrakeForce);
}

void aimm::CarController::TurnLeft()
{
	MY_ASSERT(m_poCar == nullptr, "This controller's car is null");
	m_poCar->TurnLeft(m_fTurnForce);
}

void aimm::CarController::TurnRight()
{
	MY_ASSERT(m_poCar == nullptr, "This controller's car is null");
	m_poCar->TurnRight(m_fTurnForce);
}

void aimm::CarController::SetCar(Car * l_oCar)
{
	if (l_oCar != nullptr)
		m_poCar = l_oCar;
}

const aimm::Car * aimm::CarController::GetCar()
{
	return m_poCar;
}