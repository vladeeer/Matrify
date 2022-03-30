#include "Parser.h"

// Parser

// Constructors/Destructors
Parser::Parser(User* user, std::string expression)
{
	this->user = user;

	this->expression = expression.c_str();
	this->currentChar = this->expression;
}

Parser::~Parser()
{
}

// Functions
int Parser::findMatrix(std::string& name) const
{
	std::vector<MatrixContainer*> matrices = this->user->getMatrices();

	for (int i = 0; i < this->user->getnMatrices(); i++)
		if (matrices[i]->name == name)
		{
			return i;
		}
	return -1;
}

void Parser::nextChar()
{
	currentChar++;
}

void Parser::createTokens()
{
	while ((*this->currentChar) != 0)
	{
		if ((*this->currentChar) >= 48 && (*this->currentChar) <= 57)
			std::cout << (*this->currentChar);
		this->nextChar();
	}
}

// Functions
void Parser::parse()
{
	createTokens();
}
 
