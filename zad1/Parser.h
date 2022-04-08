#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<cmath>
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
		EVALUATING,
		COMPLETE
	};

	ParsingState parsingState;

	std::string errorString;

	double dResult;
	MatrixContainer mResult;
	bool bResIsMatrix;

	// Lexing
	std::string expression;
	int charIter;
	const std::string operators = "+-*/^(),";
	const std::vector<std::string> functions = { "sin", "cos", "abs", "max" };
	std::vector<Token*> tokens;

	// Shunting
	std::stack<Token*> opStack;
	std::deque<Token*> outQueue;
	int tokenIter;

	// Evaluating
	std::stack<Token*> evalStack;

	// Functions
	int findMatrix(std::string& name) const;
	bool isIn(const char& a, const std::string& s) const;

	void nextChar();
	void createWordToken();
	void createValueToken();
	void createTokens();

	void nextToken();
	void shuntToQueue();

	void moveQueue();
	void evaluate();

public:
	// Functions
	void parse();
	const bool error() const;
	const bool complete() const;
	const bool resIsMatrix() const;
	std::string& getError();
};





