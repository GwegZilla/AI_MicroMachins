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

sf::Vector2f aimm::MyMath::B2Vec2ToSfVec2f(const b2Vec2 &vec)
{
	return sf::Vector2f(vec.x, vec.y);
}

b2Vec2 aimm::MyMath::SfVec2fToB2Vec2(const sf::Vector2f &vec)
{
	return b2Vec2(vec.x, vec.y);
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


b2Vec2 aimm::MyMath::compute2DPolygonCentroid(const b2Vec2* vertices, int vertexCount)
{
	b2Vec2 centroid = { 0, 0 };
	double signedArea = 0.0;
	double x0 = 0.0; // Current vertex X
	double y0 = 0.0; // Current vertex Y
	double x1 = 0.0; // Next vertex X
	double y1 = 0.0; // Next vertex Y
	double a = 0.0;  // Partial signed area

					 // For all vertices except last
	int i = 0;
	for (i = 0; i<vertexCount - 1; ++i)
	{
		x0 = vertices[i].x;
		y0 = vertices[i].y;
		x1 = vertices[i + 1].x;
		y1 = vertices[i + 1].y;
		a = x0*y1 - x1*y0;
		signedArea += a;
		centroid.x += (x0 + x1)*a;
		centroid.y += (y0 + y1)*a;
	}

	// Do last vertex separately to avoid performing an expensive
	// modulus operation in each iteration.
	x0 = vertices[i].x;
	y0 = vertices[i].y;
	x1 = vertices[0].x;
	y1 = vertices[0].y;
	a = x0*y1 - x1*y0;
	signedArea += a;
	centroid.x += (x0 + x1)*a;
	centroid.y += (y0 + y1)*a;

	signedArea *= 0.5;
	centroid.x /= (6.0*signedArea);
	centroid.y /= (6.0*signedArea);

	return centroid;
}
