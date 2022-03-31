#include "Parser.h"

// Parser

// Constructors/Destructors
Parser::Parser(User* user, std::string expression)
{
	this->user = user;

	this->parsingState = ParsingState::LEXING;

	this->expression = expression;
	this->charIter = 0;

	this->tokenIter = 0;
}

Parser::~Parser()
{
	while (!this->tokens.empty())
	{
		delete this->tokens.back();
		this->tokens.pop_back();
	}
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
		this->parsingState = ParsingState::SHUNTING;
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
			this->tokens.push_back(new Token(Token::Tokens::FID));
			this->tokens.back()->sValue = s;
			return;
		}
	}
	
	if (s == "pi" || s == "PI")
	{
		this->tokens.push_back(new Token(Token::Tokens::VAL));
		this->tokens.back()->dValue = 3.14159265;
		return;
	}
	else if (s == "e")
	{
		this->tokens.push_back(new Token(Token::Tokens::VAL));
		this->tokens.back()->dValue = 2.71828182;
		return;
	}

	this->tokens.push_back(new Token(Token::Tokens::MID));
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
		this->tokens.push_back(new Token(Token::Tokens::VAL));
		this->tokens.back()->dValue = std::stod(s);
	}
	else
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Syntax Error: Expected Value After \'.\' At Col: " + std::to_string(this->charIter);
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
			this->tokens.push_back(new Token(Token::Tokens::ADD));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '-')
		{
			this->tokens.push_back(new Token(Token::Tokens::SUB));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '*')
		{
			this->tokens.push_back(new Token(Token::Tokens::MUL));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '/')
		{
			this->tokens.push_back(new Token(Token::Tokens::DIV));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '^')
		{
			this->tokens.push_back(new Token(Token::Tokens::POW));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '(')
		{
			this->tokens.push_back(new Token(Token::Tokens::OPP));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == ')')
		{
			this->tokens.push_back(new Token(Token::Tokens::CLP));
			this->nextChar();
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Syntax Error: Unresolved Symbol \"" + std::to_string(this->expression[this->charIter]) + "\" At Col : " + std::to_string(this->charIter);
		}
	}
}

void Parser::nextToken()
{
	if (this->tokenIter < this->tokens.size() - 1)
		tokenIter++;
	else
		this->parsingState = ParsingState::PARSING;
}

void Parser::shuntToQueue()
{
	while (this->parsingState == ParsingState::SHUNTING)
	{
		if (this->tokens[this->tokenIter]->type == Token::Tokens::VAL
			|| this->tokens[this->tokenIter]->type == Token::Tokens::MID)
		{
			this->outQueue.push_back(this->tokens[this->tokenIter]);
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::FID)
		{
			this->opStack.push(this->tokens[this->tokenIter]);
			this->nextToken();
		}
	}
	if (this->error())
		return;
	else if (opStack.empty())
		return;
	else if (opStack.top()->type == Token::Tokens::OPP
		|| opStack.top()->type == Token::Tokens::CLP)
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Syntax Error: Mismatched Parentheses";
		return;
	}
	while (!this->opStack.empty())
	{
		this->outQueue.push_back(this->opStack.top());
		this->opStack.pop();
	}
}

// Functions
void Parser::parse()
{
	createTokens();
	shuntToQueue();

	std::stringstream ss;
	ss << "\n";
	for (int i = 0; i < this->tokens.size(); i++)
	{
		ss << this->tokens[i]->string();
	}
	ss << "\n";
	ss << "Out: ";
	for (int i = 0; i < this->outQueue.size(); i++)
	{
		ss << this->outQueue[i]->string();
	}
	ss << "\n";
	std::cout << ss.str();
}

bool Parser::error()
{
	return this->parsingState == ParsingState::ERROR;
}

std::string& Parser::getError()
{
	return this->errorString;
}
 
