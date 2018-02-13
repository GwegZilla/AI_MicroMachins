#pragma once
#include "stdafx.h"
#define DRAWABLE_ARRAY_SIZE 100
#define DRAW_MGR aimm::DrawableManager::instance()
/*
This class contains each drawable object in the
*/
namespace aimm
{
	// all of Drawables objects are cast as Conditional drawables
	struct ConditionalDrawable 
	{
	public :
		sf::Drawable* m_poDrawable;
		bool m_bIsDraw = true;
	};

	class DrawableManager : public Singleton<DrawableManager>
	{
	private:
		ConditionalDrawable* m_arrpConditionalDrawables[DRAWABLE_ARRAY_SIZE];
		sf::RenderWindow* m_opWindow;
		std::map<std::string, sf::Texture*> m_mpTextures;

	public:
		DrawableManager();
		DrawableManager(sf::RenderWindow*);
		~DrawableManager();

		void LoadGraphics();
		void SetRenderWindow(sf::RenderWindow*);

		bool AddDrawable(ConditionalDrawable*);
		bool AddDrawable(sf::Drawable*, bool);
		bool AddDrawable(sf::Drawable*);
		bool RemoveDrawable(int);

		// cache a texture, and create a sprite indexed with given name
		void LoadTexture(std::string, std::string);
		sf::Texture* GetTexture(std::string);

		void DrawAll(); // draw each of his drawables
		void DrawElement(int); // draw particular drawable by given id
	};
}