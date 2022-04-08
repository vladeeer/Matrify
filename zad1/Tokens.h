#pragma once
#include<string>

class Token
{
public:
	enum class Tokens
	{
		ADD,
		SUB,
		MUL,
		DIV,
		POW,
		OPP,
		CLP,
		COM,
		VAL,
		MID,
		NUL,
		FID  // WIP
	};

	// Constructors/Destructors
	Token(Tokens type);
	~Token();

	// Variables
	Tokens type;
	std::string name;

	bool isString;
	bool isDouble;
	std::string sValue;
	double dValue;

	int precedence;
	bool rightAssociative;

	// Functions
	std::string string(const bool fullName = false) const;
};

