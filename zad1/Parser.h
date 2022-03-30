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
	enum class ParsingState
	{
		LEXING,
		PARSING
	};

	User* user;
	ParsingState parsingState;

	std::string expression;
	std::vector<Token*> tokens;

	int charIter;

	const std::string operators = "+-*/^()";
	const std::vector<std::string> functions = { "sin", "cos", "abs" };

	// Functions
	int findMatrix(std::string& name) const;
	bool isIn(const char& a, const std::string& s) const;

	void nextChar();
	void createWordToken();
	void createTokens();

public:
	// Functions
	void parse();
};





