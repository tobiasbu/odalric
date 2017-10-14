
#ifndef _OAI_LOG_H_
#define _OAI_LOG_H_

#include <string>
#include <mutex>

namespace Odalric 
{
	
#define ODARICH_LOG_FILE "Odalric.log" // ../bwapi-data/write/

	class Log
	{
	private:

		 enum LogType : unsigned char
		{
			Info,
			Warning,
			Error
		};

		 static std::mutex mutex;
		static std::string fileLogPath;
		static bool log(const char * message, const LogType & type, char *& argptr);
		static bool log(const char * message, const LogType & type);

	public:

		static bool start();
		static void info(const char * message, ...);
		static void error(const char * message, ...);
		static void info(const std::string & message);
		static void error(const std::string & message);
		static void warn(const char * message, ...);
	};

}

#endif