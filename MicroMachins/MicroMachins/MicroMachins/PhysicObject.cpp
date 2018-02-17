#include "stdafx.h"
#include "PhysicObject.h"


void aimm::PhysicObject::InitializeCollidersRendering()
{
	// parse all the fixtures in order to see all of the colliders
	for (b2Fixture* l_oFixture = m_poBody->GetFixtureList(); l_oFixture; l_oFixture = l_oFixture->GetNext())
	{
		switch (l_oFixture->GetType())
		{
		case b2Shape::Type::e_chain:
		{
			// not using this
		}
		break;
		case b2Shape::Type::e_circle:
		{
			// not using this
		}
		break;
		case b2Shape::Type::e_edge:
		{
			// not using this			
		}
		break;
		case b2Shape::Type::e_polygon:
		{
			b2PolygonShape* l_oPolygonShape = (b2PolygonShape*)l_oFixture->GetShape();
			sf::ConvexShape* l_oNewConvexShape = new sf::ConvexShape();
			l_oNewConvexShape->setPointCount(l_oPolygonShape->m_count);
			for (int i = 0; i < l_oPolygonShape->m_count; ++i)
			{
				sf::Vector2f vec = sf::Vector2f(l_oPolygonShape->m_vertices[i].x, l_oPolygonShape->m_vertices[i].y);
				l_oNewConvexShape->setPoint(i, vec);
			}		
			b2Vec2 l_vec2Centroid = MATH::compute2DPolygonCentroid(l_oPolygonShape->m_vertices, l_oPolygonShape->m_count);
			l_oNewConvexShape->setOrigin(MATH::B2Vec2ToSfVec2f(l_vec2Centroid));

			DRAW_MGR->AddDrawable(l_oNewConvexShape);
			m_vecColliderShape.push_back(l_oNewConvexShape);
		}
		break;
		case b2Shape::Type::e_typeCount :
		{
			// not using this
		}
			break;
		}
	}

	for (int i = 0; i < m_vecColliderShape.size(); ++i)
	{
		m_vecColliderShape[i]->setPosition(MATH::B2Vec2ToSfVec2f(m_poBody->GetPosition()));
		m_vecColliderShape[i]->setFillColor(sf::Color::Transparent);
		m_vecColliderShape[i]->setOutlineColor(sf::Color::Red);
		m_vecColliderShape[i]->setOutlineThickness(1);

		DRAW_MGR->AddDrawable(m_vecColliderShape[i], true);
	}
}

aimm::PhysicObject::PhysicObject()
{
}


aimm::PhysicObject::~PhysicObject()
{
}

void aimm::PhysicObject::OnRegisterUpdate()
{
	for (int i = 0; i < m_vecColliderShape.size(); ++i)
	{
		m_vecColliderShape[i]->setRotation(m_poBody->GetAngle() * RADTODEG);
		m_vecColliderShape[i]->setPosition(MATH::B2Vec2ToSfVec2f(m_poBody->GetPosition()));
	}
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
	float radAngle = m_poBody->GetAngle();
	l_vec2ForceToApply.x = cos(radAngle);
	l_vec2ForceToApply.y = sin(radAngle);

	l_vec2ForceToApply *= l_fForce;

	m_poBody->ApplyLinearImpulseToCenter(l_vec2ForceToApply,  true);
	return void();
}

const void aimm::PhysicObject::ApplyClockwiseTorque(float &l_fTorque)
{
	m_poBody->ApplyTorque(l_fTorque, true);
	return void();
}

void aimm::PhysicObject::SetPosition(b2Vec2 &l_vec2NewPos)
{
	m_poBody->SetTransform(l_vec2NewPos, m_poBody->GetAngle() * RADTODEG);
}

void aimm::PhysicObject::SetAngle(float &l_fAngle)
{
	m_poBody->SetTransform(m_poBody->GetPosition(), l_fAngle);
}

const sf::Vector2f aimm::PhysicObject::GetPosition()
{
	sf::Vector2f l_position;
	l_position.x = m_poBody->GetPosition().x;
	l_position.y = m_poBody->GetPosition().y;
	return l_position;
}

const float aimm::PhysicObject::GetRotationDegree()
{
	return m_poBody->GetAngle() * RADTODEG;
}
