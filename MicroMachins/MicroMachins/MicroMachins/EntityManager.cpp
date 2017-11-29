#include "stdafx.h"
#include "EntityManager.h"

bool aimm::EntityManager::AddEntity(Entity* l_poEntity)
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i] == nullptr)
		{
			m_arrpEntities[i] = l_poEntity;
			return true;
		}
	}
	return false;
}

bool aimm::EntityManager::RemoveEntityById(int l_id)
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i]->GetID() == l_id)
		{
			free(m_arrpEntities[i]);
			m_arrpEntities[i] = nullptr;
			return true;
		}
	}
	return false;
}

bool aimm::EntityManager::RemoveEntityByPosition(int l_id)
{
	if (m_arrpEntities[l_id] != nullptr)
	{
		free(m_arrpEntities[l_id]);
		m_arrpEntities[l_id] = nullptr;
		return true;
	}
	return false;
}

aimm::EntityManager::EntityManager()
{
	//initialize our array to nullptr so we know wichg cell is empty
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		m_arrpEntities[i] = nullptr;
	}
}

aimm::EntityManager::~EntityManager()
{
	//free memory, give a nullptr to clean each cell
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i] != nullptr)
		{
			free(m_arrpEntities[i]);
			m_arrpEntities[i] = nullptr;
		}
	}
}

void aimm::EntityManager::Start()
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i] != nullptr)
		{
			m_arrpEntities[i]->Start();
		}
	}
}

void aimm::EntityManager::Update()
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i] != nullptr)
			m_arrpEntities[i]->Update();
	}
}

void aimm::EntityManager::Stop()
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpEntities[i] != nullptr)
		{
			m_arrpEntities[i]->Destroy();
			free(m_arrpEntities[i]);
			m_arrpEntities[i] = nullptr;
		}
	}
}