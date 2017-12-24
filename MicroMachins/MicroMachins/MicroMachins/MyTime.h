#pragma once
#include "stdafx.h"

#define TIME_DELTA aimm::MyTime::instance()->GetTimeDelta()
#define TIME_ABSOLUTE aimm::MyTime::instance()->GetTimeAbsolute();
#define TIME_FPS 1.0f/TIME_DELTA
#define TIME_MGR aimm::MyTime::instance()

namespace aimm
{
	class MyTime : public Singleton<MyTime>, public Entity
	{
	private:

		sf::Clock m_oClockDeltaTime;

		float m_fAbsoluteTime;
		float m_fTimeRatio;

	public:

		const float GetTimeDelta();
		const float GetTimeAbsolute();

		void SetTimeRatio(float);
		const void UpdateActions();

		// heritage methods
		virtual void Start();
		virtual void Update();// nb: this update should be the last one, as it will restart a delta-time timer
		virtual void Destroy();
	};
}
