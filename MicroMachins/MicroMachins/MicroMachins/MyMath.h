#pragma once
#include "stdafx.h"

#define MATH aimm::MyMath
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

namespace aimm
{
	class MyMath
	{
	public:
		MyMath();
		~MyMath();

		static const sf::Vector2f m_v2ReferenceVector;

		static float Min(float, float);
		static float Max(float, float);

		static float Vector2fMagnitude(sf::Vector2f);
		static float Vector2fAngle(sf::Vector2f);
		static float DotProductVector2f(sf::Vector2f, sf::Vector2f);

		static sf::Vector2f B2Vec2ToSfVec2f(const b2Vec2&);
		static b2Vec2 SfVec2fToB2Vec2(const sf::Vector2f&);

		static sf::Vector2f NormalizeVector2f(sf::Vector2f);
		static sf::Vector2f ClampVector2f(sf::Vector2f, float);
		static sf::Vector2f GetVector2fNormal(sf::Vector2f);

		inline static float RadToDegree(float l_fValue) { return l_fValue*180.0f / b2_pi; }
		inline static float DegreeToRad(float l_fValue) { return l_fValue*b2_pi / 180.0f; }

		static b2Vec2 compute2DPolygonCentroid(const b2Vec2* vertices, int vertexCount); // thanks : https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon

	};
}