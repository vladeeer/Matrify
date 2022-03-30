#include "Parser.h"

// Parser

// Constructors/Destructors
Parser::Parser(User* user, std::string expression)
{
	this->user = user;

	this->parsingState = ParsingState::LEXING;

	this->expression = expression;
	this->charIter = 0;
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

bool Parser::isIn(const char& a, const std::string& s) const
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == a)
			return true;
	return false;
}

void Parser::nextChar()
{
	if (this->charIter < this->expression.length() - 1)
		charIter++;
	else 
		this->parsingState = ParsingState::PARSING;
}

void Parser::createWordToken()
{
	std::string s = "";

	while (!this->isIn(this->expression[this->charIter], this->operators)
		&& this->expression[this->charIter] != ' '
		&& this->parsingState == ParsingState::LEXING)
	{
		s.push_back(this->expression[this->charIter]);
		this->nextChar();
	}
	
	for (int i = 0; i < this->functions.size(); i++)
	{
		if (s == this->functions[i])
		{
			this->tokens.push_back(new Token(Tokens::FID));
			this->tokens.back()->sValue = s;
			return;
		}
	}
	
	this->tokens.push_back(new Token(Tokens::MID));
	this->tokens.back()->sValue = s;
}

void Parser::createTokens()
{
	while (this->parsingState == ParsingState::LEXING)
	{
		if (this->expression[this->charIter] == ' ')
			this->nextChar();
		else if ((95 <= this->expression[this->charIter] && this->expression[this->charIter] <= 122)
			|| (65 <= this->expression[this->charIter] && this->expression[this->charIter] <= 90))
			this->createWordToken();
		else if (this->expression[this->charIter] == '+')
		{
			this->tokens.push_back(new Token(Tokens::ADD));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '-')
		{
			this->tokens.push_back(new Token(Tokens::SUB));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '*')
		{
			this->tokens.push_back(new Token(Tokens::MUL));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '/')
		{
			this->tokens.push_back(new Token(Tokens::DIV));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '^')
		{
			this->tokens.push_back(new Token(Tokens::POW));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '(')
		{
			this->tokens.push_back(new Token(Tokens::OPP));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == ')')
		{
			this->tokens.push_back(new Token(Tokens::CLP));
			this->nextChar();
		}
			;// ERROR: UNRESOLVED SYMBOL
	}
}

// Functions
void Parser::parse()
{
	createTokens();
	for (int i = 0; i < this->tokens.size(); i++)
	{
		std::cout << this->tokens[i]->string();
	}
}
 
