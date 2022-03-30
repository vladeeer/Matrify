#include"Tokens.h"

// Constructors/Destructors
Token::Token(Tokens type)
{
	this->type = type;
	this->name = "";
	this->isString = false;
	this->isDouble = false;

	switch (this->type)
	{
	case Tokens::ADD:
		this->name = "+";
		break;
	case Tokens::SUB:
		this->name = "-";
		break;
	case Tokens::MUL:
		this->name = "*";
		break;
	case Tokens::DIV:
		this->name = "/";
		break;
	case Tokens::POW:
		this->name = "^";
		break;
	case Tokens::OPP:
		this->name = "(";
		break;
	case Tokens::CLP:
		this->name = ")";
		break;
	case Tokens::VAL:
		this->name = "VAL";
		this->isDouble = true;
		break;
	case Tokens::MID:
		this->name = "MATRIX";
		this->isString = true;
		break;
	case Tokens::FID: 
		this->name = "FUNCTION";
		this->isString = true;
		break;
	}
	this->sValue = "";
	this->dValue = 0.0;
}

Token::~Token()
{

}

// Functions
std::string Token::string() const
{
	return this->name + ((isString) ? (":" + sValue) : "") + ((isDouble) ? (":" + std::to_string(dValue)) : "") + " ";
}
