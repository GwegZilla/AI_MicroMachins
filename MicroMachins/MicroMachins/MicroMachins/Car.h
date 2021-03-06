#pragma once
#include "stdafx.h"
#include "PhysicObject.h"

#define DEFAULT_MAX_CAR_SPEED 100.0f
#define DEFAULT_MAX_CAR_ACCELERATION 100.0f

/*
* This script is a representation of car controlled by 4 inputs:
* - gain speed
* - loose speed
* - turn on the left
* - turn on the right
* Physics defines the position of this entity.
* This entity's position defines the graphics position.
* Basically this object is processsed using the following logic : physics > entity > graphics.
*/
namespace aimm
{
	class Car : public Entity
	{
	private:
		sf::Sprite* m_oSprite;// graphic representation of the car
		sf::Vector2f m_oPosition; // position of the sprite

		PhysicObject m_oPhysic; // physical body of the object, needs to be created by this entity.

		float m_fMaxSpeed = DEFAULT_MAX_CAR_SPEED;
		float m_fMaxAcceleration = DEFAULT_MAX_CAR_ACCELERATION;

		void SetPosition(b2Vec2&);

		void UpdateSprite();

	public:
		Car(const std::string, b2Vec2&, float); // string : texture ID in drawable manager
		~Car();

		// accessors & mutators
		void SetMaxSpeed(float);
		void SetMaxAcceleration(float);

		// heritage methods
		virtual void Start();
		virtual void Update();
		virtual void Destroy();

		// actions that can be performed on the car
		void Accelerate(float);
		void Brake(float);
		void TurnLeft(float);
		void TurnRight(float);
	};
}