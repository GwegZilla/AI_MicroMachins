#pragma once
#include "stdafx.h"
/*
Contains basic logic for each entity in the game
*/
namespace aimm
{
	class Entity
	{
	protected:
		int m_iPrivateID; // allows to find the entity easily
		bool m_bIsActive; // does this entity needs an update ?

	public:
		Entity();
		~Entity();

		virtual int GetID() { return m_iPrivateID; }
		virtual bool GetIsActive() { return m_bIsActive; }
		virtual void SetIsActive(bool l_b) { m_bIsActive = l_b; }

		virtual void Start(); // entry point in the game
		virtual void Update(); // update each frame in the game
		virtual void Destroy(); // exit point in the game
	};
}