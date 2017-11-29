#pragma once
#include "stdafx.h"

#define MATH aimm::MyMath
#define NUMBER_PI 3.141592

namespace aimm
{
	class MyMath
	{
	public:
		MyMath();
		~MyMath();

		static const sf::Vector2f m_v2ReferenceVector;

		static float Vector2fMagnitude(sf::Vector2f);
		static float Vector2fAngle(sf::Vector2f);
		static float DotProductVector2f(sf::Vector2f, sf::Vector2f);

		static sf::Vector2f NormalizeVector2f(sf::Vector2f);
		static sf::Vector2f ClampVector2f(sf::Vector2f, float);
		static sf::Vector2f GetVector2fNormal(sf::Vector2f);

		inline static float RadToDegree(float l_fValue) { return l_fValue*180.0f / NUMBER_PI; }
		inline static float DegreeToRad(float l_fValue) { return l_fValue*NUMBER_PI / 180.0f; }
	};
}