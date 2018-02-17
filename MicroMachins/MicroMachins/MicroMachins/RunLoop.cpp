#include "stdafx.h"
#include "RunLoop.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include "PlayerCarController.h"


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
	// add gameplay elements in managers  here
	// default's car

	Car* redCar = new Car("redcar", b2Vec2(550.0f, 250.0f), 0.0f);
	m_opEntityMgr->AddEntity(redCar);

	Car* whiteCar = new Car("whitecar", b2Vec2(450.0f, 250.0f), 90.0f);
	m_opEntityMgr->AddEntity(whiteCar);

	Car* yellowcar = new Car("yellowcar", b2Vec2(550.0f, 450.0f), -45.0f);
	m_opEntityMgr->AddEntity(yellowcar);

	Car* greencar = new Car("greencar", b2Vec2(800.0f, 300.0f), 45.0f);
	m_opEntityMgr->AddEntity(greencar);

	Car* blackcar = new Car("blackcar", b2Vec2(750.0f, 500.0f), 90.0f);
	m_opEntityMgr->AddEntity(blackcar);


	// play controller, linked to the default car
	PlayerCarController* playerController = new PlayerCarController(redCar);
	m_opEntityMgr->AddEntity(playerController);

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