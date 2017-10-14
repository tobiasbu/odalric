
#include "Log.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <assert.h>
#include <time.h>
#include <errno.h>

#include <iostream>
#include <conio.h>

// file system
#include "filesystem/path.h"
#include "filesystem/resolver.h"

#include "Commons.h"

namespace Odalric {

	std::string Log::fileLogPath = "bwapi-data/write";
	std::mutex Log::mutex;

	bool Log::start()
	{
		// check path validity
		fileLogPath = "bwapi-data/write";

		filesystem::path thePath(fileLogPath);

		if (!thePath.exists())
		{
			filesystem::create_directory(thePath);
		}

		fileLogPath += '/';
		fileLogPath += ODARICH_LOG_FILE;

		remove(fileLogPath.c_str());

		FILE * file;
		errno_t err;
		if ((err = fopen_s(&file, fileLogPath.c_str(), "a")) != 0) {

			fprintf(stderr, "TAI::Error: could not open LOG_FILE %s file for appending\n", ODARICH_LOG_FILE);
			return false;
		}



		time_t now = time(NULL);
		char date[64];
		if ((err = ctime_s(date, 64, &now)) != 0)
			date[0] = '/0';

		fprintf(file, "Odalric  v%d.%d - Log\n%s\n:", ODARICH_VERSION_MAJOR, ODARICH_VERSION_MINOR, date);

		fprintf(file, "------------------------------\n");

		fclose(file);
		return true;
	}

	bool Log::log(const char * message, const LogType & type, va_list & argptr) //  char *&
	{



		FILE * stdstream = stdout;
		FILE * file = NULL;

		if (type == LogType::Error)
			stdstream = stderr;


		//if (activeFileLog) {
		
			errno_t err;
			if ((err = fopen_s(&file, fileLogPath.c_str(), "a")) != 0) {

				fprintf(stderr, "TAI::Error: could not open LOG_FILE %s file for appending\n",
					fileLogPath.c_str());
				//return false;
				file = NULL;
			}
		//}

		const char * prefix;

		if (type == LogType::Error)
			prefix = "TAI::Error:\t";
		else //if (type == MessageType::Info)
			prefix = "TAI::Log:\t";


		fprintf(stdstream, prefix);

		if (file != NULL)
			fprintf(file, prefix);



		//va_start(argptr, message);
		if (file != NULL)
			vfprintf(file, message, argptr);
		vfprintf(stdstream, message, argptr);


		fprintf(stdstream, "\n");
		fprintf(file, "\n");

		if (file != NULL)
			fclose(file);

		return true;

	}

	bool Log::log(const char * message, const LogType & type)
	{
		FILE * stdstream = stdout;
		FILE * file = NULL;

		if (type == LogType::Error)
			stdstream = stderr;


		//if (activeFileLog) {

		errno_t err;
		if ((err = fopen_s(&file, fileLogPath.c_str(), "a")) != 0) {

			fprintf(stderr, "TAI::Error: could not open LOG_FILE %s file for appending\n",
				fileLogPath.c_str());
			//return false;
			file = NULL;
		}
		//}

		const char * prefix;

		if (type == LogType::Error)
			prefix = "TAI::Error:\t";
		else //if (type == MessageType::Info)
			prefix = "TAI::Log:\t";


		fprintf(stdstream, prefix);

		if (file != NULL)
			fprintf(file, prefix);



		//va_start(argptr, message);
		if (file != NULL)
			fprintf(file, message);
		fprintf(stdstream, message);


		fprintf(stdstream, "\n");
		fprintf(file, "\n");

		if (file != NULL)
			fclose(file);

		return true;
	}

	void Log::info(const std::string & message)
	{
		mutex.lock();
		log(message.c_str(), Info);
		mutex.unlock();
	}

	void Log::error(const std::string & message)
	{
		mutex.lock();
		log(message.c_str(), Error);
		mutex.unlock();
	}

	void Log::info(const char * message, ...)
	{
		mutex.lock();
		va_list argptr;
		va_start(argptr, message);
		log(message, Info, argptr);
		mutex.unlock();
		va_end(argptr);
	}

	void Log::error(const char * message, ...)
	{
		va_list argptr;
		mutex.lock();
		va_start(argptr, message);
		log(message, Error, argptr);
		mutex.unlock();
		va_end(argptr);
	}

}