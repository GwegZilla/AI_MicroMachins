#pragma once

#define DEFAULT_CAR_BODY "defaultcarbody"

#define DEFAULT_CAR_SHAPE "defaultcarshape"

#include "stdafx.h"
namespace aimm
{
	class PhysicsManager
	{
	private:
		static b2World* m_poWorld;

		std::map<std::string, b2BodyDef> map_bodyDef;
		std::map<std::string, b2PolygonShape> map_polyShape;
		std::map<std::string, b2FixtureDef> map_fixture;

		// to do on start
		void CreateBodyDefinitions();
		void CreatePolygonShapes();
		void CreateFixtures();
		void CreateWorld();
	public:
		PhysicsManager();
		~PhysicsManager();
	};
}