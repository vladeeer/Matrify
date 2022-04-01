#include"Tokens.h"

// Constructors/Destructors
Token::Token(Tokens type)
{
	this->type = type;
	this->name = "";

	this->isString = false;
	this->isDouble = false;
	this->sValue = "";
	this->dValue = 0.0;

	this->precedence = 0;
	this->rightAssociative = false;

	switch (this->type)
	{
	case Tokens::ADD:
		this->name = "+";
		this->precedence = 2;
		break;
	case Tokens::SUB:
		this->name = "-";
		this->precedence = 2;
		break;
	case Tokens::MUL:
		this->name = "*";
		this->precedence = 3;
		break;
	case Tokens::DIV:
		this->name = "/";
		this->precedence = 3;
		break;
	case Tokens::POW:
		this->name = "^";
		this->precedence = 4;
		this->rightAssociative = true;
		break;
	case Tokens::OPP:
		this->name = "(";
		break;
	case Tokens::CLP:
		this->name = ")";
		break;
	case Tokens::COM:
		this->name = ",";
		break;
	case Tokens::VAL:
		this->name = "V";
		this->isDouble = true;
		break;
	case Tokens::MID:
		this->name = "M";
		this->isString = true;
		break;
	case Tokens::FID: 
		this->name = "F";
		this->isString = true;
		break;
	}
}

Token::~Token()
{

}

// Functions
std::string Token::string() const
{
	return this->name + ((isString) ? (":" + sValue) : "") + ((isDouble) ? (":" + std::to_string(dValue)) : "") + " ";
}
