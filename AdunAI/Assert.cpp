
#include "Assert.h"
#include "Exception.h"
#include "Log.h"

#include <stdarg.h>
#include <sstream>

namespace Odarich
{
	char messageBuffer[4096] = "";

	void Assert::Failure(const char * expression, const char * file, int line, const char * msg, ...)
	{
		std::stringstream ss;

		//char * msg;
		//char messageBuffer[4096] = "";

		if (msg || msg != NULL || msg != nullptr)
		{
			va_list argptr;
			va_start(argptr, msg);
			vsprintf_s(messageBuffer, msg, argptr);
			va_end(argptr);

			//msg = &messageBuffer[0];

		}
		
		

		ss << "\nAssertion thrown!" << '\n';
		ss << "Expression: " << expression << '\n';
		ss << "File: " << file << '\n';
		ss << "Line: " << line << '\n';
		ss << "Message: " << messageBuffer << '\n';

		//Log::error("---------------------\nAssertion thrown!\n---------------------");


		Log::error(ss.str());
		throw Exception(ss.str().c_str());

	}

	void Assert::Failure(const char * expression, const char * file, int line)
	{
		Assert::Failure(expression, file, line, NULL);
	}
	
}