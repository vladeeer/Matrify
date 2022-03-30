#pragma once
#include<string>
#include<vector>
#include"User.h"
#include"Tokens.h"

class Parser
{
public:
	// Constructors/Destructors
	Parser(User* user, std::string expression);
	~Parser();

private:
	// Variables
	User* user;

	const char* expression;
	std::vector<Token*> tokens;

	const char* currentChar;

	// Functions
	int findMatrix(std::string& name) const;
	

	void nextChar();
	void createTokens();

public:
	// Functions
	void parse();
};





