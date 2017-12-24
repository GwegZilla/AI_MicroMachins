#include "stdafx.h"
#include "Car.h"

void aimm::Car::SetPosition(sf::Vector2f l_v2Vector)
{
	m_v2Position = l_v2Vector;
}

void aimm::Car::SetSpeed(sf::Vector2f)
{
}

void aimm::Car::SetAcceleration(sf::Vector2f)
{
}

void aimm::Car::UpdateAcceleration()
{
	m_v2Acceleration = MATH::ClampVector2f(m_v2Acceleration, m_fMaxAcceleration);
}

void aimm::Car::UpdateSpeed()
{
	m_v2Speed += m_v2Acceleration * TIME_DELTA;
	m_v2Speed = MATH::ClampVector2f(m_v2Speed, m_fMaxSpeed);
}

void aimm::Car::UpdatePosition()
{
	m_v2Position += m_v2Speed * TIME_DELTA;
}

void aimm::Car::UpdateSprite()
{
	m_oSprite->setPosition(m_v2Position);

	float angle = MATH::RadToDegree(MATH::Vector2fAngle(m_v2Speed));
	if ((MATH::DotProductVector2f(m_v2Speed, MATH::GetVector2fNormal(MATH::m_v2ReferenceVector))) < 0.0f)
		angle = 360.0f - angle;

	m_oSprite->setRotation(angle);
}

aimm::Car::Car(const std::string l_strKey, sf::Vector2f l_oPosition)
{
	m_oSprite = new sf::Sprite();
	m_oSprite->getColor();
	m_oSprite->setOrigin(0.5f, 0.5f);
	m_oSprite->setScale(0.1f, 0.1f);
	m_oSprite->setTexture(*DRAW_MGR->GetTexture(l_strKey));

	SetPosition(l_oPosition);

	DRAW_MGR->AddDrawable(m_oSprite);
}

aimm::Car::~Car()
{
}

void aimm::Car::SetMaxSpeed(float)
{
}

void aimm::Car::SetMaxAcceleration(float)
{
}

void aimm::Car::Start()
{
	Entity::Start();
}

void aimm::Car::Update()
{
	Entity::Update();
	Accelerate(1.0f);
	TurnLeft(1.0f);

	UpdateAcceleration();
	UpdateSpeed();
	UpdatePosition();

	UpdateSprite();
}

void aimm::Car::Destroy()
{
	Entity::Destroy();
}

void aimm::Car::Accelerate(float l_fIntensity)
{
	if (m_v2Acceleration == sf::Vector2f(0.0f, 0.0f))
		m_v2Acceleration = sf::Vector2f(1.0f, 0.0f);
	float l_fCurrentMagnitude = MATH::Vector2fMagnitude(m_v2Acceleration);
	l_fCurrentMagnitude += l_fIntensity * TIME_DELTA;
	m_v2Acceleration = MATH::NormalizeVector2f(m_v2Acceleration) * l_fCurrentMagnitude;
}

void aimm::Car::Brake(float l_fIntensity)
{
	Accelerate(-l_fIntensity*TIME_DELTA);
}

void aimm::Car::TurnLeft(float l_fIntensity)
{
	m_v2Acceleration -= l_fIntensity * TIME_DELTA * MATH::GetVector2fNormal(m_v2Acceleration);
}

void aimm::Car::TurnRight(float l_fIntensity)
{
	m_v2Acceleration += l_fIntensity * TIME_DELTA * MATH::GetVector2fNormal(m_v2Acceleration);
}