#pragma once

#include "stdafx.h"
#include "PhysicObject.h"

#define PHYSIC_MGR aimm::PhysicsManager::instance()
#define PHYSIC_WORLD aimm::PhysicsManager::instance()->GetWorld()

#define CAR_BODY "carbody"
#define CAR_SHAPE "carshape"
#define CAR_FIXTURE "carfixture"

#define WALL_BODY "wallbody"

#define CAR_LINEAR_DAMPING 2.5f
#define CAR_ANGULAR_DAMPING 2.5f

// world config
#define PHYSIC_VELOCITY_ITERATIONS 60
#define PHYSIC_POSITION_ITERATIONS 60

#define PHYSIC_REGISTERED_OBJECTS_UPDATE_FREQUENCY 1.0f/60.0f

/*
* Contains all Box2D physics definitions for bodies, shapes and fixtures in order to create easily new bodies.
*
* Each definitions are created in the function "CreateX", run at the start of the app
*
* Some PhysicObjects (bodies with drawables) may need to update their position during the game,
* if you need this update, you can register them with "RegisterPhysicObject". 
* These registered objects have an update fixed by PHYSIC_REGISTERED_OBJECTS_UPDATE_FREQUENCY
* NB : non-static bodies often (always?) need to be registered
*/ 

namespace aimm
{
	class PhysicsManager : public Singleton<PhysicsManager>
	{
	private:
		b2World* m_poWorld;

		std::map<std::string, b2BodyDef*> map_bodyDef;
		std::map<std::string, b2PolygonShape*> map_polyShape;
		std::map<std::string, b2FixtureDef*> map_fixture;

		std::vector<PhysicObject*> vec_regisPhysicObject;

		float m_fLastRegisteredUpdate;

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
		
		void RegisterPhysicObject(PhysicObject*);
		void UnregisterPhysicObject(PhysicObject*);
		void UpdateRegisteredObjects();
	};

}