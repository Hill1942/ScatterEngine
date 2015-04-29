#ifndef SE_RENDER_TIMER_H
#define SE_RENDER_TIMER_H

namespace se
{
	namespace render
	{
		class SERenderTimer
		{
		public:
			SERenderTimer(void);
			~SERenderTimer(void);

			float TotalTime() const;
			float DeltaTime() const;

			void Reset();
			void Start();
			void Stop();
			void Tick();

		private:
			double  m_SecondsPerCount;
			double  m_DeltaTime;

			__int64 m_BaseTime;
			__int64 m_PausedTime;
			__int64 m_StopTime;
			__int64 m_PrevTime;
			__int64 m_CurrTime;

			bool    m_Stopped;
		};
	}
}

#endif


