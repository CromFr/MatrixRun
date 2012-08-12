#include "Async.hpp"



//===================
	bool operator>=(const struct timeval& t1, const struct timeval& t2)
	{
		if(t1.tv_sec>t2.tv_sec)
			return true;
		else if(t1.tv_sec<t2.tv_sec)
			return false;
		else
		{
			if(t1.tv_usec>=t2.tv_usec)
				return true;
			else
				return false;
		}
	}
	//===================
	struct timeval operator+(const struct timeval& t1, const struct timeval& t2)
	{
		struct timeval ret;
		int nMod = 0;

		ret.tv_usec = t1.tv_usec + t2.tv_usec;
		if(ret.tv_usec>=1000000)
		{
			nMod = 1;
			ret.tv_usec-=1000000;
		}
		ret.tv_sec = t1.tv_sec + t2.tv_sec + nMod;

		return ret;
	}
	//===================
	struct timeval operator-(const struct timeval& t1, const struct timeval& t2)
	{
		struct timeval ret;
		int nMod = 0;

		ret.tv_usec = t1.tv_usec - t2.tv_usec;
		if(ret.tv_usec<0)
		{
			nMod = 1;
			ret.tv_usec=1000000-ret.tv_usec;
		}
		ret.tv_sec = t1.tv_sec - t2.tv_sec - nMod;

		return ret;
	}
//===================


Async Async::m_instance;
//=======================================================================================
Async* Async::GetInstance()
{
	return &m_instance;
}
//=======================================================================================
Async::Async()
{
	m_bPaused=false; m_Thread=0;
}
//=======================================================================================
Async::~Async()
{
	StopThread();
}
//=======================================================================================
void Async::DelayCommand(double fDelay, function<void()> func)
{
	if(fDelay<0.0)fDelay=0.0;

	struct timeval Delay;
	Delay.tv_sec = (int)fDelay;
	Delay.tv_usec = (fDelay-Delay.tv_sec)*1000000;


	int nSlot = m_instance.GetFirstSlot();
	if(nSlot == -1)
	{
		struct delayableFunction toAdd;
		toAdd.exec = false;
		struct timeval now;
		gettimeofday(&now, NULL);
		toAdd.date = now + Delay;

		toAdd.func = func;
		m_instance.m_Functions.push_back(toAdd);

		#ifdef PRINT_DEBUG
		cout<<"Registered at\tdate="<<toAdd.date.tv_sec<<":"<<toAdd.date.tv_usec<<endl;
		#endif
	}
	else
	{
		m_instance.m_Functions[nSlot].exec = false;
		struct timeval now;
		gettimeofday(&now, NULL);
		m_instance.m_Functions[nSlot].date = now + Delay;

		m_instance.m_Functions[nSlot].func = func;

		#ifdef PRINT_DEBUG
		cout<<"Registered at\tdate="<<m_Functions[nSlot].date.tv_sec<<":"<<m_Functions[nSlot].date.tv_usec<<endl;
		#endif
	}
}
//=======================================================================================
void Async::ExecuteOnce()
{
	#ifdef PRINT_DEBUG
	struct timeval now2;
	gettimeofday(&now2, NULL);
	cout<<"Execute\t\tdate="<<now2.tv_sec<<":"<<now2.tv_usec<<endl;
	#endif

	if(m_bPaused)return;

	unsigned int nSize = m_Functions.size();
	for(unsigned int i=0 ; i<nSize ; i++)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		if( !m_Functions[i].exec && now>=m_Functions[i].date)
		{
			(m_Functions[i].func)();
			m_Functions[i].exec = true;
		}
	}
}
//=======================================================================================
void Async::LaunchThread(double fLoopTimeS)
{
	if(m_Thread==0)
	{
		m_bThread = true;
		m_nThreadLoopTimeUS = fLoopTimeS*1000000;
		m_Thread = new thread(bind(&Async::ThreadFunction, this));
	}
}
//=======================================================================================
void Async::StopThread()
{
	if(m_Thread!=0)
	{
		m_bThread=false;

		delete m_Thread;
		m_Thread = 0;
	}
}
//=======================================================================================
bool Async::GetIsThreadRunning()
{
	return m_Thread && m_bThread;
}
//=======================================================================================
void Async::SetThreadLoopTime(double fLoopTimeS)
{
	m_nThreadLoopTimeUS = fLoopTimeS*1000000;
}
//=======================================================================================
double Async::GetThreadLoopTime()
{
	return m_nThreadLoopTimeUS/1000000;
}
//=======================================================================================
void Async::Clear()
{
	m_Functions.clear();
}
//=======================================================================================
void Async::Pause(bool bPause)
{
	m_bPaused = bPause;
	if(bPause)
	{
		gettimeofday(&m_PauseDate, NULL);
	}
	else
	{
		struct timeval delta, now;
		gettimeofday(&now, NULL);
		delta = now-m_PauseDate;

		unsigned int nSize = m_Functions.size();
		for(unsigned int i=0 ; i<nSize ; i++)
		{
			m_Functions[i].date= m_Functions[i].date+delta;
		}
	}
}
//=======================================================================================
bool Async::GetIsPaused()
{
	return m_bPaused;
}
//=======================================================================================
//=======================================================================================
//=======================================================================================
int Async::GetFirstSlot()
{
	unsigned int nSize = m_Functions.size();
	for(unsigned int i=0 ; i<nSize ; i++)
	{
		if(m_Functions[i].exec)
			return i;
	}
	return -1;
}
//=======================================================================================
void Async::ThreadFunction()
{
	#ifdef PRINT_DEBUG
	cout<<"Launching Thread"<<endl;
	#endif
	struct timeval start, end, delta;
	while(m_bThread)
	{
		gettimeofday(&start, NULL);
		ExecuteOnce();


		//Waiting
		gettimeofday(&end, NULL);
		delta = end-start;

		this_thread::sleep_for(chrono::microseconds(m_nThreadLoopTimeUS-(delta.tv_sec*1000000 + delta.tv_usec)));
	}
}
