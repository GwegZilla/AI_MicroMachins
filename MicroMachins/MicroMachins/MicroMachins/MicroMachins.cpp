#include "stdafx.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Micro Machins");
	aimm::RunLoop l_oRun;
	l_oRun.Populate();

	l_oRun.Start();

	l_oRun.m_bIsRunning = true;

	while (l_oRun.m_bIsRunning)
	{
		l_oRun.Update();
		l_oRun.Draw();
	}

	l_oRun.Stop();

	return 0;
}