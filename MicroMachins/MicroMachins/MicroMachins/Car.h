#pragma once
#include "stdafx.h"

/*
* This script is a representation of car controlled by 4 inputs:
* - gain speed
* - loose speed
* - turn on the left
* - turn on the right
* Physics and graphics are based upon this entity position
*/
namespace aimm
{
	class Car : public Entity
	{
	private:
		sf::Sprite* m_oSprite;// graphic representation of the car
		sf::Vector2f m_oPosition; // position of the sprite

		float m_fMaxSpeed = 100.0f;
		float m_fMaxAcceleration = 1000.0f;

		sf::Vector2f m_v2Position;
		sf::Vector2f m_v2Speed;
		sf::Vector2f m_v2Acceleration;

		void SetPosition(sf::Vector2f);
		void SetSpeed(sf::Vector2f);
		void SetAcceleration(sf::Vector2f);

		void UpdateAcceleration();
		void UpdateSpeed();
		void UpdatePosition();

		void UpdateSprite();

	public:
		Car(std::string, sf::Vector2f); // string : texture ID in drawable manager
		~Car();

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