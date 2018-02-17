#pragma once
#include "stdafx.h"

/*
Every physic object should have initialized his body and fixtures THEN Initialize his colliders
*/

namespace aimm
{
	class PhysicObject
	{
	private:
		ConditionalDrawable* m_poDebugDrawable; // keep ref on it's drawable, to show it on command

	protected:
		b2Body* m_poBody; // be carefull with this pointer, as it is managed by it's own object.
		std::vector< sf::Shape*> m_vecColliderShape;

	public:
		PhysicObject();
		~PhysicObject();

		void OnRegisterUpdate(); // what happens if the object is registered in the physicmanager, uasually position update

		void InitializeBody(b2BodyDef*);
		void InitializeCollidersRendering(); // creates a shape for each fixture in the body
		void AddFixture(b2FixtureDef*);

		const void ApplyForceToForward(float&);
		const void ApplyClockwiseTorque(float&);

		void SetPosition(b2Vec2&);
		void SetAngle(float&);

		const sf::Vector2f GetPosition();
		const float GetRotationDegree();
	};

}