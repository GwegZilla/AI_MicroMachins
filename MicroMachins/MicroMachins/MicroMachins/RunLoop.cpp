#include "stdafx.h"
#include "RunLoop.h"
#include "imgui.h"
#include "imgui-SFML.h"

aimm::RunLoop::RunLoop()
{
	// only mem allocation here. Game logic should be in start
	m_opWindow = new sf::RenderWindow(
		sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
		APPLICATION_NAME);
	DRAW_MGR->SetRenderWindow(m_opWindow);
	DRAW_MGR->LoadGraphics();

	// Init ImGUI
	ImGui::SFML::Init(*m_opWindow);

	// Init menuDebug
	sft::MenuDebug::Create();
	sft::MenuDebug::SetEnable(true);

	// sample menuDebug
	/*
	cocoRico = true;
	if (sft::MenuDebug::OpenDirectory("MyFirstDirectory"))
	{
		sft::MenuDebug::AddBool("Test", &cocoRico);
		sft::MenuDebug::CloseDirectory();
	}
	*/
	m_opEntityMgr = new EntityManager();
}

aimm::RunLoop::~RunLoop()
{
}

void aimm::RunLoop::Populate()
{
	// add gameplay in managers elements here
	Car* whiteCar = new Car("whitecar", sf::Vector2f(250.0f, 250.0f));
	m_opEntityMgr->AddEntity(whiteCar);
}

void aimm::RunLoop::Start()
{
	//initialize time
	TIME_MGR->Start();

	// Init Physics
	PHYSIC_MGR->Initialize();

	// starts the game run
	m_opEntityMgr->Start();

	// initialize window
	m_opWindow->setFramerateLimit(FRAMERATE_LIMIT);
}

void aimm::RunLoop::Update()
{
	// Update the physic
	PHYSIC_MGR->Update();

	// Update the game
	m_opEntityMgr->Update();

	// Update the time
	TIME_MGR->Update();

	// Update menuDebug via IMGUI-SFML
	ImGui::SFML::Update(TIME_DELTA);
}

void aimm::RunLoop::Draw()
{
	m_opWindow->clear(WINDOW_COLOR);
	// draw the game----
	DRAW_MGR->DrawAll();
	//------------------

	PA_TODO("Post Loop");
	sft::ImGuiRenderMenuDebug();
	ImGui::Render();

	m_opWindow->display();
}

void aimm::RunLoop::Stop()
{
	// free memory here
	TIME_MGR->Destroy();
}