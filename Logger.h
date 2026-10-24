#pragma once
/*!
* \file Logger.h
* \brief Game logger.
* \author Ndongmo Silatsa Fabrice
* \date 11-07-2015
* \version 1.0
*/

#pragma once
#pragma warning(disable : 4996)

#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <SDL3/SDL.h>

#ifndef logE
#define logE NS2::Logger::log() << " ERROR @ FILE ("<< __FILE__ << ", "<< __FUNCTION__ <<", "<< __LINE__ << ") @ "
#define logW NS2::Logger::log() << " WARNING @ FILE ("<< __FILE__ << ", "<< __FUNCTION__ <<", "<< __LINE__ << ") @ "
#define logD NS2::Logger::log() << " DEBUG @ FILE ("<< __FILE__ << ", "<< __FUNCTION__ <<", "<< __LINE__ << ") @ "
#define logI NS2::Logger::log() << " INFO @ FILE ("<< __FILE__ << ", "<< __FUNCTION__ <<", "<< __LINE__ << ") @ "
#endif

namespace NS2
{
	class Logger
	{
	public:
		Logger(std::ostream& os) : os(os) {}

		template<typename T>
		std::ostream& operator<<(const T& data)
		{
			auto now = std::chrono::system_clock::now();
			auto now_time_t = std::chrono::system_clock::to_time_t(now);
			auto now_tm = std::localtime(&now_time_t);

			return os << "[" << std::put_time(now_tm, "%c") << "] " << data;
		}

		/*!
		* \brief Obtain log singleton object.
		* \return logger.
		*/
		static Logger& log() {
			return m_logger;
		}

		static void exitGame() {
#ifdef _DEBUG
			system("pause");
#endif
			SDL_Quit();
			exit(1);
		}

	private:
		static Logger m_logger; /* Logger singleton object */
		std::ostream& os; /* output stream for logs */
	};
}

