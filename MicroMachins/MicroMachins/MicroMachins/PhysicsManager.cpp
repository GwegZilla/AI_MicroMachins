#include "stdafx.h"
#include "PhysicsManager.h"

void aimm::PhysicsManager::CreateBodyDefinitions()
{
}

void aimm::PhysicsManager::CreatePolygonShapes()
{
}

void aimm::PhysicsManager::CreateFixtures()
{
}

void aimm::PhysicsManager::CreateWorld()
{
	b2Vec2 bvec2Gravity = b2Vec2(0, 0);
	m_poWorld = new b2World(bvec2Gravity);
}

aimm::PhysicsManager::PhysicsManager()
{
}

aimm::PhysicsManager::~PhysicsManager()
{
}