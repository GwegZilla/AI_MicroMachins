#pragma once
#include "stdafx.h"
#define DRAWABLE_ARRAY_SIZE 100
#define DRAW_MGR aimm::DrawableManager::instance()
/*
This class contains each drawable object in the
*/
namespace aimm
{
	class DrawableManager : public Singleton<DrawableManager>
	{
	private:
		sf::Drawable* m_arrpDrawables[DRAWABLE_ARRAY_SIZE];
		sf::RenderWindow* m_opWindow;
		std::map<std::string, sf::Texture*> m_mpTextures;

	public:
		DrawableManager();
		DrawableManager(sf::RenderWindow*);
		~DrawableManager();

		void LoadGraphics();
		void SetRenderWindow(sf::RenderWindow*);

		bool AddDrawable(sf::Drawable*);
		bool RemoveDrawable(int);

		// cache a texture, and create a sprite indexed with given name
		void LoadTexture(std::string, std::string);
		sf::Texture* GetTexture(std::string);

		void DrawAll(); // draw each of his drawables
		void DrawElement(int); // draw particular drawable by given id
	};
}