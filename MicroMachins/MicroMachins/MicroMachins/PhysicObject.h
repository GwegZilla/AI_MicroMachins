#pragma once
#include "stdafx.h"
namespace aimm
{
	class PhysicObject
	{
	private:
		ConditionalDrawable* m_poDebugDrawable; // keep ref on it's drawable, to show it on command

	protected:
		b2Body* m_poBody; // be carefull with this pointer, as it is managed by it's own object.

	public:
		PhysicObject();
		~PhysicObject();

		void InitializeBody(b2BodyDef*);
		void AddFixture(b2FixtureDef*);

		const void ApplyForceToForward(float&);
		const void ApplyClockwiseTorque(float&);

		const sf::Vector2f GetPosition();
		const float GetRotation();
	};

}