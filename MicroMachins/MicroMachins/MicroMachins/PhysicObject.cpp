#include "stdafx.h"
#include "PhysicObject.h"


aimm::PhysicObject::PhysicObject()
{
}


aimm::PhysicObject::~PhysicObject()
{
}

void aimm::PhysicObject::InitializeBody(b2BodyDef * l_definition)
{
	MY_ASSERT(l_definition == nullptr, "This body definition is null");

	m_poBody = PHYSIC_WORLD->CreateBody(l_definition);
}

void aimm::PhysicObject::AddFixture(b2FixtureDef * l_fixtureDefinition)
{
	MY_ASSERT(l_fixtureDefinition == nullptr, "This fixture definition is null");

	m_poBody->CreateFixture(l_fixtureDefinition);
}

const void aimm::PhysicObject::ApplyForceToForward(float & l_fForce)
{
	b2Vec2 l_vec2ForceToApply = m_poBody->GetLinearVelocity();
	// now we normalize the vector
	float angle = m_poBody->GetAngle();
	l_vec2ForceToApply.x = cos(angle * DEGTORAD ) ;
	l_vec2ForceToApply.y = sin(angle * DEGTORAD);

	l_vec2ForceToApply *= l_fForce;

	m_poBody->ApplyLinearImpulseToCenter(l_vec2ForceToApply,  true);
	return void();
}

const void aimm::PhysicObject::ApplyClockwiseTorque(float &l_fTorque)
{
	m_poBody->ApplyTorque(l_fTorque, true);
	return void();
}

const sf::Vector2f aimm::PhysicObject::GetPosition()
{
	sf::Vector2f l_position;
	l_position.x = m_poBody->GetPosition().x;
	l_position.y = m_poBody->GetPosition().y;
	return l_position;
}

const float aimm::PhysicObject::GetRotation()
{
	return m_poBody->GetAngle();
}
