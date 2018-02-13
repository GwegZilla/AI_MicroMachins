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

void aimm::Car::UpdateSprite()
{

	m_oSprite->setPosition(m_oPhysic.GetPosition());

	m_oSprite->setRotation(m_oPhysic.GetRotation());
}

aimm::Car::Car(const std::string l_strKey, sf::Vector2f l_oPosition)
{
	// physics initialisation
	m_oPhysic.InitializeBody(PHYSIC_MGR->GetBodyDef(CAR_BODY));
	m_oPhysic.AddFixture(PHYSIC_MGR->GetFixtureDef(CAR_FIXTURE));
	
	// graphics initialisation.
	m_oSprite = new sf::Sprite();
	m_oSprite->setTexture(*DRAW_MGR->GetTexture(l_strKey));
	m_oSprite->getColor();

	sf::Vector2u l_vec2Size = m_oSprite->getTexture()->getSize();
	m_oSprite->setOrigin(l_vec2Size.x * 0.5f, l_vec2Size.y * 0.5f);

	m_oSprite->setScale(0.1f, 0.1f);

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

	// Box2D updates the physic, so all we have to do is to draw the image of the car on the physical object position.
	UpdateSprite();
}

void aimm::Car::Destroy()
{
	Entity::Destroy();
}

// TODO : use Box2D physics in this logic.

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