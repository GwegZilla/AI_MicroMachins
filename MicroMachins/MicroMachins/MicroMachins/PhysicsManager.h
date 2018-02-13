#pragma once
#define PHYSIC_MGR aimm::PhysicsManager::instance()
#define PHYSIC_WORLD aimm::PhysicsManager::instance()->GetWorld()

#define CAR_BODY "carbody"
#define WALL_BODY "wallbody"

#define CAR_SHAPE "carshape"

#define CAR_FIXTURE "carfixture"

// world config
#define PHYSIC_VELOCITY_ITERATIONS 10
#define PHYSIC_POSITION_ITERATIONS 10
//#define PHYSIC_TIME_STEP 1.0f/60.0f  // if this define is commented, then the macro DELTA_TIME may be used

#include "stdafx.h"
namespace aimm
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		b2World* m_poWorld;

		std::map<std::string, b2BodyDef*> map_bodyDef;
		std::map<std::string, b2PolygonShape*> map_polyShape;
		std::map<std::string, b2FixtureDef*> map_fixture;

		// to do on start
		bool CreateBodyDefinitions();
		bool CreatePolygonShapes();
		bool CreateFixtures();
		bool CreateWorld();

	public:
		PhysicsManager();
		~PhysicsManager();

		void Initialize();
		void Update();
		void Exit();

		b2World* GetWorld();

		b2BodyDef* GetBodyDef(std::string);
		b2PolygonShape* GetPolygonShape(std::string);
		b2FixtureDef* GetFixtureDef(std::string);
	};

}