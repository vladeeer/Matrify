#pragma once
#include<stack>
#include<string>
#include<vector>
#include"State.h"
#include"User.h"
#include"Tokens.h"
#include"ErrorState.h"

class Parser
{
public:
	// Constructors/Destructors
	Parser(std::stack<State*>* states, User* user, std::string expression);
	~Parser();

public:
	// Variables
	enum class ParsingState
	{
		ERROR,
		LEXING,
		PARSING
	};

	ParsingState parsingState;

private:
	

	std::stack<State*>* states;
	User* user;

	std::string expression;
	std::vector<Token*> tokens;

	int charIter;

	const std::string operators = "+-*/^()";
	const std::vector<std::string> functions = { "sin", "cos", "abs" };

	// Functions
	int findMatrix(std::string& name) const;
	bool isIn(const char& a, const std::string& s) const;
	void throwError(std::string error);

	void nextChar();
	void createWordToken();
	void createValueToken();
	void createTokens();

public:
	// Functions
	void parse();
};





