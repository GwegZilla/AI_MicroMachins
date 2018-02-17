#include "stdafx.h"
#include "PhysicsManager.h"

bool aimm::PhysicsManager::CreateBodyDefinitions()
{
	// a body definition tells wether an object is static (wall), kinematic (static object with velocity) or dynamic (speedy gonzales)

	b2BodyDef* carBody = new b2BodyDef();
	carBody->type = b2BodyType::b2_dynamicBody; 
	carBody->position.Set(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f); 
	carBody->angle = 0.0f;
	carBody->linearDamping = CAR_LINEAR_DAMPING;
	carBody->angularDamping = CAR_ANGULAR_DAMPING;
	carBody->fixedRotation = false;
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
	boxShape->SetAsBox(45.0f, 20.0f);
	map_polyShape.insert(std::pair<std::string, b2PolygonShape*>(CAR_SHAPE, boxShape));
	return true;
}

bool aimm::PhysicsManager::CreateFixtures()
{
	b2FixtureDef* boxFixtureDef = new b2FixtureDef();
	boxFixtureDef->shape = GetPolygonShape(CAR_SHAPE);
	boxFixtureDef->density = 0.001f;
	boxFixtureDef->friction = 1.0f;
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
	m_fLastRegisteredUpdate = TIME_ABSOLUTE;
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

	UpdateRegisteredObjects();
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

void aimm::PhysicsManager::RegisterPhysicObject(PhysicObject *l_object)
{
	MY_ASSERT(l_object == nullptr, "You are trying to register a nullptr to the physic manager");
	vec_regisPhysicObject.push_back(l_object);
}

void aimm::PhysicsManager::UnregisterPhysicObject(PhysicObject *l_object)
{
	for (int i = 0; i < vec_regisPhysicObject.size(); ++i)
	{
		if (vec_regisPhysicObject[i] == l_object)
		{
			std::swap(vec_regisPhysicObject[i], vec_regisPhysicObject[vec_regisPhysicObject.size() - 1]);
			vec_regisPhysicObject.pop_back();
		}
	}
}

void aimm::PhysicsManager::UpdateRegisteredObjects()
{
	float l_fAbsoluteTime = TIME_ABSOLUTE;
	if (l_fAbsoluteTime - m_fLastRegisteredUpdate >= PHYSIC_REGISTERED_OBJECTS_UPDATE_FREQUENCY)
	{
		for (int i = 0; i < vec_regisPhysicObject.size(); ++i)
		{
			vec_regisPhysicObject[i]->OnRegisterUpdate();
		}
	}
}

