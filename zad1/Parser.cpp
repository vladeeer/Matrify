#include "Parser.h"

// Constructors/Destructors
Parser::Parser(User* user, std::string expression)
{
	this->user = user;

	this->parsingState = ParsingState::LEXING;

	this->dResult = 0;
	this->bResIsMatrix = false;

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
			if (!this->tokens.empty()
				&& this->tokens.back()->type == Token::Tokens::VAL)
				this->tokens.push_back(new Token(Token::Tokens::MUL));

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

	if (!this->tokens.empty()
		&& this->tokens.back()->type == Token::Tokens::VAL)
		this->tokens.push_back(new Token(Token::Tokens::MUL));

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
			if (this->tokens.empty() || this->tokens.back()->type == Token::Tokens::OPP)
				this->tokens.push_back(new Token(Token::Tokens::NUL));

			this->tokens.push_back(new Token(Token::Tokens::ADD));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '-')
		{
			if (this->tokens.empty() || this->tokens.back()->type == Token::Tokens::OPP)
				this->tokens.push_back(new Token(Token::Tokens::NUL));

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
			if (!this->tokens.empty()
				&& (this->tokens.back()->type == Token::Tokens::VAL
					|| this->tokens[this->tokens.size() - 1]->type == Token::Tokens::MID))
				this->tokens.push_back(new Token(Token::Tokens::MUL));

			this->tokens.push_back(new Token(Token::Tokens::OPP));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == ')')
		{
			this->tokens.push_back(new Token(Token::Tokens::CLP));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == ',')
		{
			this->tokens.push_back(new Token(Token::Tokens::COM));
			this->nextChar();
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Syntax Error: Unresolved Symbol \'" + this->expression.substr(this->charIter, 1) + "\' At Col : " + std::to_string(this->charIter);
		}
	}
	if (this->tokens.empty() && this->parsingState != ParsingState::ERROR)
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Syntax Error: Expected Expression";
	}
}

void Parser::nextToken()
{
	if (this->tokenIter < this->tokens.size() - 1)
		tokenIter++;
	else
		this->parsingState = ParsingState::EVALUATING;
}

void Parser::shuntToQueue()
{
	while (this->parsingState == ParsingState::SHUNTING)
	{
		if (this->tokens[this->tokenIter]->type == Token::Tokens::VAL
			|| this->tokens[this->tokenIter]->type == Token::Tokens::MID
			|| this->tokens[this->tokenIter]->type == Token::Tokens::NUL)
		{
			this->outQueue.push_back(this->tokens[this->tokenIter]);
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::FID)
		{
			this->opStack.push(this->tokens[this->tokenIter]);
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::COM)
		{
			while (!this->opStack.empty() && opStack.top()->type != Token::Tokens::OPP)
			{
				this->outQueue.push_back(this->opStack.top());
				this->opStack.pop();
				if (this->opStack.empty())
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Syntax Error: Missing Function Parentheses";
					return;
				}
			}
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::ADD
			|| this->tokens[this->tokenIter]->type == Token::Tokens::SUB
			|| this->tokens[this->tokenIter]->type == Token::Tokens::MUL
			|| this->tokens[this->tokenIter]->type == Token::Tokens::DIV
			|| this->tokens[this->tokenIter]->type == Token::Tokens::POW)
		{
			while (!this->opStack.empty() 
				&& (opStack.top()->precedence > this->tokens[this->tokenIter]->precedence
				|| (opStack.top()->precedence == this->tokens[this->tokenIter]->precedence
				&& !this->tokens[this->tokenIter]->rightAssociative)))
			{
				this->outQueue.push_back(this->opStack.top());
				this->opStack.pop();
			}
			this->opStack.push(this->tokens[this->tokenIter]);
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::OPP)
		{
			this->opStack.push(this->tokens[this->tokenIter]);
			this->nextToken();
		}
		else if (this->tokens[this->tokenIter]->type == Token::Tokens::CLP)
		{
			while (!this->opStack.empty() && this->opStack.top()->type != Token::Tokens::OPP)
			{
				this->outQueue.push_back(this->opStack.top());
				this->opStack.pop();
				if (this->opStack.empty())	
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Syntax Error: Mismatched Parentheses";
					return;
				}
			}
			this->opStack.pop();
			if (!this->opStack.empty() && this->opStack.top()->type == Token::Tokens::FID)
			{
				this->outQueue.push_back(this->opStack.top());
				this->opStack.pop();
			}
			this->nextToken();
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Parsing Error: Invalid Token: " + this->tokens[this->tokenIter]->string(true);
			return;
		}
	}
	if (opStack.empty())
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
		if(this->opStack.top()->type != Token::Tokens::OPP)
		{	
			this->outQueue.push_back(this->opStack.top());
			this->opStack.pop();
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Syntax Error: Mismatched Parentheses";
			return;
		}
	}
}

void Parser::moveQueue()
{
	if (!this->outQueue.empty())
	{
		this->evalStack.push(this->outQueue[0]);
		this->outQueue.pop_front();
	}
	else if (this->parsingState != ParsingState::ERROR)
	{
		this->parsingState = ParsingState::COMPLETE;
	}
}

void Parser::evaluate()
{
	this->moveQueue();

	while (this->parsingState == ParsingState::EVALUATING)
	{
		if (this->evalStack.top()->type == Token::Tokens::VAL
			|| this->evalStack.top()->type == Token::Tokens::MID
			|| this->evalStack.top()->type == Token::Tokens::NUL)
		{
			moveQueue();
		}
		else if (this->evalStack.top()->type == Token::Tokens::ADD
			|| this->evalStack.top()->type == Token::Tokens::SUB)
		{
			if (this->evalStack.size() >= 3)
			{
				Token* op3 = this->evalStack.top();
				this->evalStack.pop();
				Token* op2 = this->evalStack.top();
				this->evalStack.pop();
				Token* op1 = this->evalStack.top();
				this->evalStack.pop();

				if (op2->type == Token::Tokens::VAL
					&& (op1->type == Token::Tokens::VAL)
					|| op1->type == Token::Tokens::NUL)
				{
					if (op1->type == Token::Tokens::NUL)
					{
						op1->type = Token::Tokens::VAL;
						op1->isDouble = true;
					}

					this->tokens.push_back(new Token(Token::Tokens::VAL));
					if (op3->type == Token::Tokens::ADD)
						this->tokens.back()->dValue = op1->dValue + op2->dValue;
					else if (op3->type == Token::Tokens::SUB)
						this->tokens.back()->dValue = op1->dValue - op2->dValue;
					this->evalStack.push(this->tokens.back());
				}
				else if (op2->type == Token::Tokens::MID
					&& (op1->type == Token::Tokens::MID
					|| op1->type == Token::Tokens::NUL))
				{
					; // Add Matrices
				}
				else
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Undefined Addition/Subtraction Operation For: " 
						+ op1->string(true) + "& " + op2->string(true);
					return;
				}
			}
			else 
			{
				this->parsingState = ParsingState::ERROR;
				this->errorString = "Evaluation Error: Missing Operands For Addition/Subtraction Operation";
				return;
			}

			moveQueue();
		} 
		else if (this->evalStack.top()->type == Token::Tokens::MUL)
		{
			if (this->evalStack.size() >= 3)
			{
				this->evalStack.pop();
				Token* op2 = this->evalStack.top();
				this->evalStack.pop();
				Token* op1 = this->evalStack.top();
				this->evalStack.pop();

				if (op2->type == Token::Tokens::VAL
					&& op1->type == Token::Tokens::VAL)
				{
					this->tokens.push_back(new Token(Token::Tokens::VAL));
					this->tokens.back()->dValue = op1->dValue * op2->dValue;
					this->evalStack.push(this->tokens.back());
				}
				else if ((op2->type == Token::Tokens::MID
					&& op1->type == Token::Tokens::VAL)
					|| (op2->type == Token::Tokens::VAL
					&& op1->type == Token::Tokens::MID))
				{
					; // Matrix Scalar Mul
				}
				else if (op2->type == Token::Tokens::MID
					&& op1->type == Token::Tokens::MID)
				{
					; // Dot Product
				}
				else
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Undefined Multiplication Operation For: "
						+ op1->string(true) + "& " + op2->string(true);
					return;
				}
			}
			else
			{
				this->parsingState = ParsingState::ERROR;
				this->errorString = "Evaluation Error: Missing Operands For Multiplication Operation";
				return;
			}

			moveQueue();
		}
		else if (this->evalStack.top()->type == Token::Tokens::DIV)
		{
		if (this->evalStack.size() >= 3)
		{
			this->evalStack.pop();
			Token* op2 = this->evalStack.top();
			this->evalStack.pop();
			Token* op1 = this->evalStack.top();
			this->evalStack.pop();

			if (op2->type == Token::Tokens::VAL
				&& op1->type == Token::Tokens::VAL)
			{
				this->tokens.push_back(new Token(Token::Tokens::VAL));
				this->tokens.back()->dValue = op1->dValue / op2->dValue;
				this->evalStack.push(this->tokens.back());
			}
			else if (op2->type == Token::Tokens::MID
				&& op1->type == Token::Tokens::VAL)
			{
				; // Matrix Scalar Div
			}
			else
			{
				this->parsingState = ParsingState::ERROR;
				this->errorString = "Evaluation Error: Undefined Division Operation For: "
					+ op1->string(true) + "& " + op2->string(true);
				return;
			}
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Evaluation Error: Missing Operands For Division Operation";
			return;
		}

		moveQueue();
		}
		else if (this->evalStack.top()->type == Token::Tokens::POW)
		{
		if (this->evalStack.size() >= 3)
		{
			this->evalStack.pop();
			Token* op2 = this->evalStack.top();
			this->evalStack.pop();
			Token* op1 = this->evalStack.top();
			this->evalStack.pop();

			if (op2->type == Token::Tokens::VAL
				&& op1->type == Token::Tokens::VAL)
			{
				this->tokens.push_back(new Token(Token::Tokens::VAL));
				this->tokens.back()->dValue = pow(op1->dValue, op2->dValue);
				this->evalStack.push(this->tokens.back());
			}
			else if (op2->type == Token::Tokens::VAL
				&& op1->type == Token::Tokens::MID)
			{
				; // Matrix To Power
			}
			else
			{
				this->parsingState = ParsingState::ERROR;
				this->errorString = "Evaluation Error: Undefined Power Operation For: "
					+ op1->string(true) + "& " + op2->string(true);
				return;
			}
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Evaluation Error: Missing Operands For Power Operation";
			return;
		}

		moveQueue();
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Evaluation Error: Invalid Token: " + this->evalStack.top()->string(true);
			return;
		}
	}

	if (this->parsingState == ParsingState::COMPLETE
		&& this->evalStack.size() == 1 
		&& this->evalStack.top()->type == Token::Tokens::VAL)
	{
		this->bResIsMatrix = false;
		this->dResult = this->evalStack.top()->dValue;
	}
	else if (this->parsingState == ParsingState::COMPLETE
		&& this->evalStack.size() == 1
		&& this->evalStack.top()->type == Token::Tokens::MID)
	{
		this->bResIsMatrix = true;
		//this->mResult = this->evalStack.top()->dValue;
	}
	else if (this->parsingState != ParsingState::ERROR)
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Evaluation Error: Missing Operator";
		return;
	}
}

// Functions
void Parser::parse()
{
	createTokens();

	std::stringstream s1;
	s1 << "\n";
	for (int i = 0; i < this->tokens.size(); i++)
	{
		s1 << this->tokens[i]->string();
	}
	s1 << "\n";
	std::cout << s1.str();

	shuntToQueue();

	std::stringstream s2;
	s2 << "Out: ";
	for (int i = 0; i < this->outQueue.size(); i++)
	{
		s2 << this->outQueue[i]->string();
	}
	s2 << "\n";
	std::cout << s2.str();

	system("PAUSE");

	evaluate();

	if (this->parsingState != ParsingState::ERROR)
		std::cout << "\n" << std::to_string(this->dResult) << "\n";
}

const bool Parser::error() const
{
	return this->parsingState == ParsingState::ERROR;
}

const bool Parser::complete() const
{
	return this->parsingState == ParsingState::COMPLETE;
}

const bool Parser::resIsMatrix() const
{
	return this->bResIsMatrix;
}

std::string& Parser::getError()
{
	return this->errorString;
}

