#include "stdafx.h"
#include "Car.h"

void aimm::Car::SetPosition(b2Vec2& l_v2Vector)
{
	m_oPhysic.SetPosition( l_v2Vector );
}

void aimm::Car::UpdateSprite()
{

	m_oSprite->setPosition(m_oPhysic.GetPosition());

	m_oSprite->setRotation(m_oPhysic.GetRotationDegree());

}

aimm::Car::Car(const std::string l_strKey, b2Vec2& l_oPosition, float l_fAngle = 0.0f)
{
	// physics initialisation
	m_oPhysic.InitializeBody(PHYSIC_MGR->GetBodyDef(CAR_BODY));
	m_oPhysic.AddFixture(PHYSIC_MGR->GetFixtureDef(CAR_FIXTURE));
	m_oPhysic.InitializeCollidersRendering();
	m_oPhysic.SetAngle(l_fAngle);

	PHYSIC_MGR->RegisterPhysicObject(&m_oPhysic);


	// graphics initialisation.
	m_oSprite = new sf::Sprite();
	m_oSprite->setTexture(*DRAW_MGR->GetTexture(l_strKey));
	m_oSprite->getColor();

	sf::Vector2u l_vec2Size = m_oSprite->getTexture()->getSize();
	m_oSprite->setOrigin(l_vec2Size.x * 0.5f, l_vec2Size.y * 0.5f);

	m_oSprite->setScale(0.1f, 0.1f);

	SetPosition(l_oPosition);

	DRAW_MGR->AddDrawable(m_oSprite, true);
}

aimm::Car::~Car()
{
	free(m_oSprite);
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


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) == true)
	{
		m_oPhysic.InitializeCollidersRendering();
	}

}

void aimm::Car::Destroy()
{
	Entity::Destroy();
}

// TODO : use Box2D physics in this logic.

void aimm::Car::Accelerate(float l_fIntensity)
{
	m_oPhysic.ApplyForceToForward(l_fIntensity);
}

void aimm::Car::Brake(float l_fIntensity)
{
	l_fIntensity *= -1.0f;
	m_oPhysic.ApplyForceToForward(l_fIntensity);
}

void aimm::Car::TurnLeft(float l_fIntensity)
{
	l_fIntensity *= -1.0f;
	m_oPhysic.ApplyClockwiseTorque(l_fIntensity);
}

void aimm::Car::TurnRight(float l_fIntensity)
{
	m_oPhysic.ApplyClockwiseTorque(l_fIntensity);
}