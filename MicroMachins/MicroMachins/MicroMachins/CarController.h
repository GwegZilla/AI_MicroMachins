#pragma once
#include "stdafx.h"

#define DEFAULT_BRAKE 1.0f
#define DEFAULT_ACCELERATION 2.0f
#define DEFAULT_TURN 25000.0f

/*
* This class manages a car object, and is meant to mutate in order to be used by either an AI or a player
*/
namespace aimm
{
	class CarController : public Entity
	{
	protected:
		Car* m_poCar;
		float m_fBrakeForce;
		float m_fAccelerationForce;
		float m_fTurnForce;

	public:
		CarController();
		~CarController();

		void Start();
		void Update();
		void Destroy();

		void Accelerate();
		void Brake();
		void TurnLeft();
		void TurnRight();

		void SetCar(Car*);
		const Car* GetCar();

	};

}