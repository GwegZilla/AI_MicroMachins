#pragma once
#include"stdafx.h"

#define APPLICATION_NAME "Micro Manchins"

#define WINDOW_COLOR sf::Color(50,50,150)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define FRAMERATE_LIMIT 0

/*
	Manages the game's loop, instantiation & destruction
*/
namespace aimm
{
	class RunLoop
	{
	private:
		EntityManager* m_opEntityMgr; // all entities in the game
		DrawableManager* m_opDrawableManager; // all drawables in the game
		sf::RenderWindow* m_opWindow;

		bool cocoRico;

	public:

		bool m_bIsRunning;

		RunLoop();
		~RunLoop();

		void Populate(); // alloc memory, assign entities
		void Start(); // initialize entities
		void Update(); // Run
		void Draw(); // draw the scene
		void Stop(); // free memory, say goodbye
	};
}