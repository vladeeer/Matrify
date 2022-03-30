#include "Parser.h"

// Parser

// Constructors/Destructors
Parser::Parser(std::stack<State*>* states, User* user, std::string expression)
{
	this->states = states;
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

void Parser::throwError(std::string error)
{
	this->states->push(new ErrorState(error));
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

void Parser::createValueToken()
{
	std::string s = "";

	while (!this->isIn(this->expression[this->charIter], this->operators)
		&& ((this->expression[this->charIter] >= 48 && this->expression[this->charIter] <= 57) || this->expression[this->charIter] == '.')
		&& this->expression[this->charIter] != ' '
		&& this->parsingState == ParsingState::LEXING)
	{
		if (this->expression[this->charIter] == '.' && this->isIn('.', s))
			break;
		s.push_back(this->expression[this->charIter]);
		this->nextChar();
	}

	if (s != ".")
	{
		this->tokens.push_back(new Token(Tokens::VAL));
		this->tokens.back()->dValue = std::stod(s);
	}
	else
	{
		this->parsingState = ParsingState::ERROR;
		this->throwError("Syntax Error: Expected Value After \'.\' At Col: " + std::to_string(this->charIter));
	}
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
		else if ((48 <= this->expression[this->charIter] && this->expression[this->charIter] <= 57)
			|| (this->expression[this->charIter] == '.'))
			this->createValueToken();
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
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->throwError("Unresolved Symbol \"" + std::to_string(this->expression[this->charIter]) + "\" At Col : " + std::to_string(this->charIter));
		}
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
 
