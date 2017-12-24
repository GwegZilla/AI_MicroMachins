#include "stdafx.h"
#include "MyTime.h"

const float aimm::MyTime::GetTimeDelta()
{
	return m_oClockDeltaTime.getElapsedTime().asSeconds() * m_fTimeRatio;
}

const float aimm::MyTime::GetTimeAbsolute()
{
	return m_fAbsoluteTime;
}

void aimm::MyTime::SetTimeRatio(float l_fNumber)
{
	m_fTimeRatio = l_fNumber;
}

const void aimm::MyTime::UpdateActions()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) == true)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) == true)
		{
			SetTimeRatio(m_fTimeRatio + 1.0f * TIME_DELTA);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) == true)
		{
			SetTimeRatio(m_fTimeRatio - 1.0f * TIME_DELTA);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply) == true)
		{
			SetTimeRatio(1.0f);
		}
	}
}

void aimm::MyTime::Start()
{
	Entity::Start();
	m_oClockDeltaTime.restart();
	m_fAbsoluteTime = 0.0f;
	m_fTimeRatio = 1.0f;
}

void aimm::MyTime::Update()
{
	Entity::Update();
	UpdateActions();
	m_fAbsoluteTime += GetTimeDelta();
	m_oClockDeltaTime.restart();
}

void aimm::MyTime::Destroy()
{
	Entity::Destroy();
}