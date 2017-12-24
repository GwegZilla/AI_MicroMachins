#include "stdafx.h"
#include "MyMath.h"

const sf::Vector2f aimm::MyMath::m_v2ReferenceVector = sf::Vector2f{ 1.0f,0.0f };

aimm::MyMath::MyMath()
{
}

aimm::MyMath::~MyMath()
{
}

float aimm::MyMath::Min(float a, float b)
{
	if (a <= b) // equal placed here so there is more chances to quit this func early
		return a;
	else
		return b;
}

float aimm::MyMath::Max(float a, float b)
{
	if (a >= b)
		return a;
	else
		return b;
}

float aimm::MyMath::Vector2fMagnitude(sf::Vector2f l_v2Vector)
{
	// ||U|| = sqrt( (Ux * Ux) + (Uy * Uy) )
	return std::sqrt((l_v2Vector.x*l_v2Vector.x) + (l_v2Vector.y*l_v2Vector.y));
}

float aimm::MyMath::Vector2fAngle(sf::Vector2f l_v2Vector)
{
	float l_fMagnitude = (Vector2fMagnitude(l_v2Vector)*Vector2fMagnitude(m_v2ReferenceVector));
	float l_fDotProduct = DotProductVector2f(l_v2Vector, m_v2ReferenceVector);
	MY_ASSERT(l_fMagnitude == 0.0f, "Division by zero.");
	if (l_fMagnitude == 0.0f)
		return 0.0f;
	return std::acos(l_fDotProduct / l_fMagnitude);
}

float aimm::MyMath::DotProductVector2f(sf::Vector2f l_v2VectorA, sf::Vector2f l_v2VectorB)
{
	return ((l_v2VectorA.x*l_v2VectorB.x) + (l_v2VectorA.y*l_v2VectorB.y));
}

sf::Vector2f aimm::MyMath::NormalizeVector2f(sf::Vector2f l_v2Vector)
{
	// nU = U / ||U||
	float l_fMagnitude = Vector2fMagnitude(l_v2Vector);
	MY_ASSERT(l_fMagnitude == 0.0f, "Division by zero");
	if (l_fMagnitude == 0.0f)
		return sf::Vector2f(0.0f, 0.0f);
	return (l_v2Vector / l_fMagnitude);
}

sf::Vector2f aimm::MyMath::ClampVector2f(sf::Vector2f l_v2Vector, float l_fMaxMagnitude)
{
	if (Vector2fMagnitude(l_v2Vector) > l_fMaxMagnitude)
		return (NormalizeVector2f(l_v2Vector) * l_fMaxMagnitude);
	return l_v2Vector;
}

sf::Vector2f aimm::MyMath::GetVector2fNormal(sf::Vector2f l_v2Vector)
{
	// U { a , b }    nU { -b , a  }
	return sf::Vector2f(-l_v2Vector.y, l_v2Vector.x);
}