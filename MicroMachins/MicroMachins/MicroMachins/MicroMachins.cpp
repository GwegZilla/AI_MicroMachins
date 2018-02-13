#include "stdafx.h"

int main()
{
	aimm::RunLoop l_oRun;

	l_oRun.Start();

	l_oRun.Populate();

	l_oRun.m_bIsRunning = true;

	while (l_oRun.m_bIsRunning)
	{
		l_oRun.Update();
		l_oRun.Draw();
	}

	l_oRun.Stop();

	return 0;
}

// alternative main to test
/*
int main()
{
	b2Vec2 gravity(0, -9.8); //normal earth gravity, 9.8 m/s/s straight down!

	b2World* myWorld = new b2World(gravity);

	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	myBodyDef.position.Set(0, 20); //set the starting position
	myBodyDef.angle = 0; //set the starting angle

	b2Body* dynamicBody = myWorld->CreateBody(&myBodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(1, 1);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1;
	dynamicBody->CreateFixture(&boxFixtureDef);

	float32 timeStep = 1.0 / 60.0;      //the length of time passed to simulate (seconds)
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;   //how strongly to correct position
	while (1)
		myWorld->Step(timeStep, velocityIterations, positionIterations);
}
*/