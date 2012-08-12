#ifndef ASYNC_HPP_INCLUDED
#define ASYNC_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
using namespace boost;

#include <sys/time.h>

class Async
{
public:
	Async();
	~Async();

	static Async* GetInstance();

	/**
	@brief Delay a function execution
	@param fDelay Time in seconds to wait before executing the function (precision : 0.000001)
	@param func The function bind to execute. ex: boost::bind(MyFunction, param1, param2)	boost::bind(&MyClass::MyFunction, &MyObject, param1, param2)
	**/
	static void DelayCommand(double fDelay, function<void()> func);


	//==========================================> EXECUTION
	/**
	@brief Executes commands that has come over the execution date (set with the delay of DelayCommand)
	@note Useful when you want to handle manually the watch frequence
	@warning Do nothing if the system has been halted with Async::Pause(true)
	**/
	void ExecuteOnce();


	//==========================================> THREAD
    /**
    @brief Launch a thread to watch and execute delayed commands. (It will call ExecuteOnce();)
    @param fLoopTimeS The time between two thread execution
    @note You can have only 1 thread for a Async instance.
    **/
	void LaunchThread(double fLoopTimeS=0.01);

	/**
	@brief stops and kill the current thread
	**/
	void StopThread();

	/**
	@brief Gets if the thread has been launched or not
	@return true if the thread is running
	**/
	bool GetIsThreadRunning();

	/**
	@brief Change the time between two executions of the thread
	@param fLoopTimeS The time in seconds (precision: 0.000001)
	**/
	void SetThreadLoopTime(double fLoopTimeS);

	/**
	@brief Gets the time between two executions of the thread
	@return the time in seconds
	@note If the thread is not running, will return the last used value
	**/
	double GetThreadLoopTime();

	/**
	@brief Delete all registered commands
	**/
	void Clear();


	/**
	@brief Pause/Unpause the execution.
	@param bPause true tu pause, false to unpause
	@note When unpaused, the date of the functions are modified.
	**/
	void Pause(bool bPause);

	/**
	@brief Get if the system has been halted or not
	@return true if the system is halted
	**/
	bool GetIsPaused();

private:
	//Type declarations
	struct delayableFunction
	{
		bool exec;
		struct timeval date;
		function<void()> func;
	};
	//


	//===================
	int GetFirstSlot();

	void ThreadFunction();



	//Vars
	vector<delayableFunction> m_Functions;

	bool m_bPaused;
	struct timeval m_PauseDate;

	thread* m_Thread;
	bool m_bThread;
	int m_nThreadLoopTimeUS;

	static Async m_instance;



};



#endif // ASYNC_HPP_INCLUDED

