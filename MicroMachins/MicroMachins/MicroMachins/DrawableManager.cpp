#include "stdafx.h"
#include "DrawableManager.h"

aimm::DrawableManager::DrawableManager()
{
}

aimm::DrawableManager::DrawableManager(sf::RenderWindow * l_opWindow)
{
	m_opWindow = l_opWindow;
}

aimm::DrawableManager::~DrawableManager()
{
}

void aimm::DrawableManager::LoadGraphics()
{
	LoadTexture("Assets/Sprites/white_car.png", "whitecar");
}

void aimm::DrawableManager::SetRenderWindow(sf::RenderWindow * l_poWindow)
{
	m_opWindow = l_poWindow;
}

bool aimm::DrawableManager::AddDrawable(sf::Drawable * l_poDrawable)
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpDrawables[i] == nullptr)
		{
			m_arrpDrawables[i] = l_poDrawable;
		}
		return true;
	}

	return false;
}

bool aimm::DrawableManager::RemoveDrawable(int l_iIndex)
{
	if (m_arrpDrawables[l_iIndex])
	{
		free(m_arrpDrawables[l_iIndex]);
		m_arrpDrawables[l_iIndex] = nullptr;
	}
	return false;
}

void aimm::DrawableManager::LoadTexture(std::string l_strFilename, std::string l_strSpriteKey)
{
	sf::Texture* l_pTexture = new sf::Texture();
	l_pTexture->loadFromFile(l_strFilename);
	m_mpTextures.insert(std::pair<std::string, sf::Texture*>(l_strSpriteKey, l_pTexture));
}

sf::Texture* aimm::DrawableManager::GetTexture(std::string l_strKey)
{
	if (m_mpTextures[l_strKey] == nullptr)
		throw "Texture not found";
	return m_mpTextures[l_strKey];
}

void aimm::DrawableManager::DrawAll()
{
	for (int i = 0; i < DRAWABLE_ARRAY_SIZE; ++i)
	{
		if (m_opWindow == nullptr)
			return;

		if (m_arrpDrawables[i] != nullptr)
		{
			m_opWindow->draw(*m_arrpDrawables[i]);
		}
	}
}

void aimm::DrawableManager::DrawElement(int l_iId)
{
	if (m_opWindow == nullptr)
		return;

	if (m_arrpDrawables[l_iId] != nullptr)
	{
		m_opWindow->draw(*m_arrpDrawables[l_iId]);
	}
}