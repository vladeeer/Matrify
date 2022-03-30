#pragma once
#include<string>

enum class Tokens
{
	ADD,
	SUB,
	MUL,
	DIV,
	POW,
	OPP,
	CLP,
	VAL,
	MID,
	FID
};

class Token
{
public:
	// Constructors/Destructors
	Token(Tokens type);
	~Token();

	// Variables
	bool isString;
	bool isDouble;
	std::string sValue;
	double dValue;

	Tokens type;
	std::string name;

	// Functions
	std::string string() const;
};

