
#ifndef _OAI_ASSERT_H_
#define _OAI_ASSERT_H_



namespace Odarich
{
	namespace Assert
	{
		void Failure(const char * expression, const char * file, int line, const char * msg, ...);
		void Failure(const char * expression, const char * file, int line);
	};
}

//#define _OAI_ASSERT( e ) if ( !(e) Odarich::Assert::Failure(#e, __FILE__, __LINE__) )

#define OAI_ASSERT( e ) ( e ) ? (void)0 : Odarich::Assert::Failure(#e, __FILE__, __LINE__)
#define OAI_ASSERT_MSG( e, msg, ... ) if ( !(e) ) Odarich::Assert::Failure(#e, __FILE__, __LINE__, (msg), ##__VA_ARGS__)

#endif