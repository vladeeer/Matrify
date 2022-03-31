#pragma once
#include<iostream>
#include<stack>
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
	User* user;

	// Variables
	enum class ParsingState
	{
		ERROR,
		LEXING,
		SHUNTING,
		PARSING
	};

	ParsingState parsingState;

	std::string errorString;

	// Lexing
	std::string expression;
	int charIter;
	const std::string operators = "+-*/^()";
	const std::vector<std::string> functions = { "sin", "cos", "abs" };
	std::vector<Token*> tokens;

	// Shunting
	std::stack<Token*> opStack;
	std::deque<Token*> outQueue;
	int tokenIter;

	// Functions
	int findMatrix(std::string& name) const;
	bool isIn(const char& a, const std::string& s) const;

	void nextChar();
	void createWordToken();
	void createValueToken();
	void createTokens();

	void nextToken();
	void shuntToQueue();

public:
	// Functions
	void parse();
	bool error();
	std::string& getError();
};





