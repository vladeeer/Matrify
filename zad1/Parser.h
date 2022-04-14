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

	// Lexing
	std::string expression;
	int charIter;
	const std::string operators = "+-*/^(),";
	const std::vector<std::string> functions = { "sin", "cos", "tan", "tg", "ctg", 
		"asin", "arcsin", "acos", "arccos", "atan", "arctg", "arcctg",
		"sqrt", "cbrt", "abs", 
		"exp", "ln", "lg",
		"pow", "log", "max", "min",
		"inverse", "det"};
	std::vector<Token*> tokens;

	// Shunting
	std::stack<Token*> opStack;
	std::deque<Token*> outQueue;
	int tokenIter;

	// Evaluating
	std::stack<Token*> evalStack;
	std::stack<MatrixContainer*> matrixStack;
	double dResult;
	Matrix<double> mResult;
	bool bResIsMatrix;

	// Functions
	Matrix<double> getMatrix(std::string name = "");
	bool isIn(const char& a, const std::string& s) const;

	void nextChar();
	void createWordToken();
	void createValueToken();
	void createTokens();

	void nextToken();
	void shuntToQueue();

	void moveQueue();
	void evalFunc();
	void evaluate();

public:
	// Functions
	void parse();
	const bool error() const;
	const bool complete() const;
	const bool resIsMatrix() const;
	double getDoubleResult();
	Matrix<double> getMatrixResult();
	std::string& getError();
};





