#include "stdafx.h"
#include "PhysicsManager.h"

bool aimm::PhysicsManager::CreateBodyDefinitions()
{
	// a body definition tells wether an object is static (wall), kinematic (static object with velocity) or dynamic (speedy gonzales)

	b2BodyDef* carBody = new b2BodyDef();
	carBody->type = b2BodyType::b2_dynamicBody; 
	carBody->position.Set(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f); 
	carBody->angle = 0.0f;
	if (carBody == nullptr)
		return false;
	map_bodyDef.insert(std::pair<std::string, b2BodyDef*>(CAR_BODY, carBody));

	b2BodyDef* wallBody = new b2BodyDef();
	wallBody->type = b2BodyType::b2_staticBody;
	wallBody->position.Set(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);
	wallBody->angle = 0.0f;
	if (wallBody == nullptr)
		return false;
	map_bodyDef.insert(std::pair<std::string, b2BodyDef*>(WALL_BODY, wallBody));
	return true;
}

bool aimm::PhysicsManager::CreatePolygonShapes()
{
	b2PolygonShape *boxShape = new b2PolygonShape();
	boxShape->SetAsBox(1, 1);
	map_polyShape.insert(std::pair<std::string, b2PolygonShape*>(CAR_SHAPE, boxShape));
	return true;
}

bool aimm::PhysicsManager::CreateFixtures()
{
	b2FixtureDef* boxFixtureDef = new b2FixtureDef();
	boxFixtureDef->shape = GetPolygonShape(CAR_SHAPE);
	boxFixtureDef->density = 1;
	map_fixture.insert(std::pair<std::string, b2FixtureDef*>(CAR_FIXTURE, boxFixtureDef));

	return true;
}

bool aimm::PhysicsManager::CreateWorld()
{
	b2Vec2 bvec2Gravity = b2Vec2(0.0f, 0.0f);
	aimm::PhysicsManager::m_poWorld = new b2World(bvec2Gravity);
	return true;
}

aimm::PhysicsManager::PhysicsManager()
{
}

aimm::PhysicsManager::~PhysicsManager()
{
}

void aimm::PhysicsManager::Initialize()
{
	bool handle = false;

	handle = CreateBodyDefinitions();
	MY_ASSERT(handle == false, "Body Definitions not loaded");
	handle = CreatePolygonShapes();
	MY_ASSERT(handle == false, "Polygon shapes not loaded");
	handle = CreateFixtures();
	MY_ASSERT(handle == false, "Fixtures not loaded");
	handle = CreateWorld();
	MY_ASSERT(handle == false, "World not loaded");
}

void aimm::PhysicsManager::Update()
{
	m_poWorld->Step(TIME_DELTA, PHYSIC_VELOCITY_ITERATIONS, PHYSIC_POSITION_ITERATIONS);
	m_poWorld->ClearForces();
}

void aimm::PhysicsManager::Exit()
{
	// TODO : free all the memory from the maps
}

b2World * aimm::PhysicsManager::GetWorld()
{
	if (m_poWorld != nullptr)
		return m_poWorld;
	throw "Physic's World is not defined. Please define a physics world";
	return nullptr;
}

b2BodyDef * aimm::PhysicsManager::GetBodyDef(std::string l_strKey)
{
	if (map_bodyDef[l_strKey] == nullptr)
		throw "Body definition not found";
	return map_bodyDef[l_strKey];
}

b2PolygonShape * aimm::PhysicsManager::GetPolygonShape(std::string l_strKey)
{
	if (map_polyShape[l_strKey] == nullptr)
		throw "Poly shape not found";
	return map_polyShape[l_strKey];
}

b2FixtureDef * aimm::PhysicsManager::GetFixtureDef(std::string l_strKey)
{
	if (map_fixture[l_strKey] == nullptr)
		throw "Fixture definition not found";
	return map_fixture[l_strKey];
}
