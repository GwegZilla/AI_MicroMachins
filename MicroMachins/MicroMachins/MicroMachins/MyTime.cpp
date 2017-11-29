#include "stdafx.h"
#include "MyTime.h"

float aimm::MyTime::GetTimeDelta()
{
	return m_oClockDeltaTime.getElapsedTime().asSeconds();
}

float aimm::MyTime::GetTimeAbsolute()
{
	return m_oClockAbsoluteTime.getElapsedTime().asSeconds();
}

void aimm::MyTime::Start()
{
	Entity::Start();
	m_oClockDeltaTime.restart();
	m_oClockAbsoluteTime.restart();
}

void aimm::MyTime::Update()
{
	Entity::Update();
	m_oClockDeltaTime.restart();
}

void aimm::MyTime::Destroy()
{
	Entity::Destroy();
}