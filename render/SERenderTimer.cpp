#include <windows.h>
#include "SERenderTimer.h"

namespace se
{
	namespace render
	{
		SERenderTimer::SERenderTimer(void)
			:m_SecondsPerCount(0.0),
			m_DeltaTime(-1.),
			m_BaseTime(0),
			m_PausedTime(0),
			m_PrevTime(0),
			m_CurrTime(0),
			m_Stopped(false)
		{
			__int64 countsPerSecond;
			QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSecond));
			m_SecondsPerCount = 1.0 / static_cast<double>(countsPerSecond);
		}


		SERenderTimer::~SERenderTimer(void)
		{
		}

		float SERenderTimer::TotalTime() const
		{
			if (m_Stopped)
			{
				return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
			}
			else
			{
				return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
			}
		}

		float SERenderTimer::DeltaTime() const
		{
			return (float)m_DeltaTime;
		}

		void SERenderTimer::Reset()
		{
			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			m_BaseTime = currTime;
			m_PrevTime = currTime;
			m_StopTime = 0;
			m_Stopped  = false;
		}

		void SERenderTimer::Start()
		{
			__int64 startTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

			if (m_Stopped)
			{
				m_PausedTime += (startTime - m_StopTime);
				m_PrevTime    = startTime;
				m_StopTime    = 0;
				m_Stopped     = false;
			}
		}

		void SERenderTimer::Stop()
		{
			if (!m_Stopped)
			{
				__int64 currTime;
				QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

				m_StopTime = currTime;
				m_Stopped  = true;
			}
		}

		void SERenderTimer::Tick()
		{
			if (m_Stopped)
			{
				m_DeltaTime = 0.0;
				return;
			}

			__int64 currTime;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			m_CurrTime = currTime;
			m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;
			m_PrevTime = m_CurrTime;

			if (m_DeltaTime < 0.0)
			{
				m_DeltaTime = 0;
			}
		}
	}
}







