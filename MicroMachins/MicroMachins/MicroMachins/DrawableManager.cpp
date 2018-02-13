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

bool aimm::DrawableManager::AddDrawable(ConditionalDrawable * l_poConditionalDrawable)
{
	for (int i = 0; i < ENTITY_ARRAY_SIZE; ++i)
	{
		if (m_arrpConditionalDrawables[i] == nullptr)
		{
			m_arrpConditionalDrawables[i] = l_poConditionalDrawable;
		}
		return true;
	}

	return false;
}

bool aimm::DrawableManager::AddDrawable(sf::Drawable *l_poDrawable, bool l_bIsDraw)
{
	ConditionalDrawable* oConditionalDrawable = new ConditionalDrawable();
	oConditionalDrawable->m_poDrawable = l_poDrawable;
	oConditionalDrawable->m_bIsDraw = l_bIsDraw;
	return AddDrawable(oConditionalDrawable);
}

bool aimm::DrawableManager::AddDrawable(sf::Drawable *l_poDrawable)
{
	return AddDrawable(l_poDrawable, true);
}

bool aimm::DrawableManager::RemoveDrawable(int l_iIndex)
{
	if (m_arrpConditionalDrawables[l_iIndex])
	{
		free(m_arrpConditionalDrawables[l_iIndex]);
		m_arrpConditionalDrawables[l_iIndex] = nullptr;
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

		if (m_arrpConditionalDrawables[i] != nullptr)
		{
			// draw object if his drawing is active and if it's not null
			if (m_arrpConditionalDrawables[i]->m_poDrawable != nullptr
				&& m_arrpConditionalDrawables[i]->m_bIsDraw == true)
			{
				m_opWindow->draw(*m_arrpConditionalDrawables[i]->m_poDrawable);
			}
		}
	}
}

void aimm::DrawableManager::DrawElement(int l_iId)
{
	if (m_opWindow == nullptr)
		return;

	if (m_arrpConditionalDrawables[l_iId] != nullptr)
	{
		m_opWindow->draw(*m_arrpConditionalDrawables[l_iId]->m_poDrawable);
	}
}