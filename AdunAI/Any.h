
#ifndef _OAI_ANY_H_
#define _OAI_ANY_H_


/*
class Any
{
	enum Type {Char,Short,Int, Float, String};


public:

	template<typename T>
	Any(const T & value)
	{

	}
};*/

struct any{
	enum type { Int, Float, String };
	any(int   e) { m_data.INT = e; m_type = Int; }
	any(float e) { m_data.FLOAT = e; m_type = Float; }
	any(char* e) { m_data.STRING = e; m_type = String; }
	type get_type() const { return m_type; }
	int get_int() const { return m_data.INT; }
	float get_float() const { return m_data.FLOAT; }
	char* get_string() const { return m_data.STRING; }
private:
	type m_type;
	union {
		int   INT;
		float FLOAT;
		char *STRING;
	} m_data;
};

#endif
