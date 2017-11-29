#pragma once
#include "stdafx.h"
#define ENTITY_ARRAY_SIZE 100

/*
	Basic object able to manage multiple entities by calling their
	different routines in the same place for readability's sake
*/
namespace aimm
{
	class EntityManager
	{
	private:

		Entity* m_arrpEntities[ENTITY_ARRAY_SIZE];

	public:
		EntityManager();
		~EntityManager();

		// basic calls, similar to Entity ones
		void Start();
		void Update();
		void Stop();

		// array management
		bool AddEntity(Entity*);
		bool RemoveEntityById(int);
		bool RemoveEntityByPosition(int);
	};
}