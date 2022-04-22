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
Matrix<double> Parser::getMatrix(std::string name) 
{
	std::vector<MatrixContainer*> matrices = this->user->getMatrices();

	for (int i = 0; i < this->user->getnMatrices(); i++)
		if (matrices[i]->name == name)
			return *matrices[i]->matrix;
	if (name == "")
	{
		Matrix<double> res = *matrixStack.top()->matrix;
		delete matrixStack.top();
		matrixStack.pop();
		return res;
	}
	else
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Evaluation Error: No Such Matrix Found: " + name;
	}

	Matrix<double> M;
	return M;
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
				&& (this->tokens.back()->type == Token::Tokens::VAL
				|| this->tokens.back()->type == Token::Tokens::CLP))
				this->tokens.push_back(new Token(Token::Tokens::MUL));

			this->tokens.push_back(new Token(Token::Tokens::FID));
			this->tokens.back()->sValue = s;
			return;
		}
	}
	
	if (s == "pi" || s == "PI")
	{
		if (!this->tokens.empty()
			&& (this->tokens.back()->type == Token::Tokens::VAL
			|| this->tokens.back()->type == Token::Tokens::CLP))
			this->tokens.push_back(new Token(Token::Tokens::MUL));

		this->tokens.push_back(new Token(Token::Tokens::VAL));
		this->tokens.back()->dValue = 3.14159265;
		return;
	}
	else if (s == "e")
	{
		if (!this->tokens.empty()
			&& (this->tokens.back()->type == Token::Tokens::VAL
			|| this->tokens.back()->type == Token::Tokens::CLP))
			this->tokens.push_back(new Token(Token::Tokens::MUL));

		this->tokens.push_back(new Token(Token::Tokens::VAL));
		this->tokens.back()->dValue = 2.71828182;
		return;
	}

	if (!this->tokens.empty()
		&& (this->tokens.back()->type == Token::Tokens::VAL
		|| this->tokens.back()->type == Token::Tokens::CLP))
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
			if (this->tokens.empty() || this->tokens.back()->type == Token::Tokens::OPP
				|| this->tokens.back()->type == Token::Tokens::COM)
				this->tokens.push_back(new Token(Token::Tokens::NUL));

			this->tokens.push_back(new Token(Token::Tokens::ADD));
			this->nextChar();
		}
		else if (this->expression[this->charIter] == '-')
		{
			if (this->tokens.empty() || this->tokens.back()->type == Token::Tokens::OPP
				|| this->tokens.back()->type == Token::Tokens::COM)
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
				|| this->tokens.back()->type == Token::Tokens::MID
				|| this->tokens.back()->type == Token::Tokens::CLP))
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

void Parser::evalFunc()
{
	std::string FID = this->evalStack.top()->sValue;
	this->evalStack.pop();

	int nArgs = 0;
	if (FID == "sin" || FID == "cos" || FID == "tan" || FID == "tg" || FID == "ctg" 
		|| FID == "asin" || FID == "arcsin" || FID == "acos" || FID == "arccos" 
		|| FID == "atan" || FID == "arctg" || FID == "arcctg"
		|| FID == "sqrt" || FID == "cbrt" || FID == "abs" 
		|| FID == "exp" || FID == "ln" || FID == "lg" 
		|| FID == "trans" || FID == "adj" || FID == "inv" || FID == "det")
		nArgs = 1;
	else if (FID == "pow" || FID == "log" || FID == "max" || FID == "min")
		nArgs = 2;
	else
	{
		this->parsingState = ParsingState::ERROR;
		this->errorString = "Evaluation Error: Invalid Function: " + FID + "()";
		return;
	}
	if (FID == "trans" || FID == "adj" || FID == "inv" || FID == "det")
	{
		if (this->evalStack.size() >= nArgs) //nArgs=1
		{
			Matrix<double> arg;

			if (this->evalStack.top()->type == Token::Tokens::MID)
			{
				arg = getMatrix(this->evalStack.top()->sValue);

				if (this->parsingState == ParsingState::ERROR)
					return;
			}
			else
			{
				this->parsingState = ParsingState::ERROR;
				this->errorString = "Evaluation Error: Invalid Arguments For Function: " + FID + "()";
				return;
			}
			this->evalStack.pop();

			if (FID == "det")
			{
				if (arg.getHeight() != arg.getWidth())
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Find Determinant Of A Non-Square Matrix: det("
						+ std::to_string(arg.getHeight()) + "x" + std::to_string(arg.getWidth()) + ")";
					return;
				}

				double res;
				res = 0;
				res = arg.det();
				this->tokens.push_back(new Token(Token::Tokens::VAL));
				this->tokens.back()->dValue = res;
				this->evalStack.push(this->tokens.back());
			}
			else if (FID == "trans")
			{
				Matrix<double> res;
				res = arg.trans();
				this->tokens.push_back(new Token(Token::Tokens::MID));
				this->tokens.back()->sValue = "";
				this->evalStack.push(this->tokens.back());
				matrixStack.push(new MatrixContainer(res, ""));
			}
			else if (FID == "adj")
			{
				if (arg.getHeight() != arg.getWidth())
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Find Adjugate Of A Non-Square Matrix: adj("
						+ std::to_string(arg.getHeight()) + "x" + std::to_string(arg.getWidth()) + ")";
					return;
				}
				if (arg.det() == 0)
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Find Adjugate Of A Matrix With Zero Determinant: adj("
						+ std::to_string(arg.getHeight()) + "x" + std::to_string(arg.getWidth()) + ")";
					return;
				}

				Matrix<double> res;
				res = arg.adj();
				this->tokens.push_back(new Token(Token::Tokens::MID));
				this->tokens.back()->sValue = "";
				this->evalStack.push(this->tokens.back());
				matrixStack.push(new MatrixContainer(res, ""));
			}
			else if (FID == "inv")
			{
				if (arg.getHeight() != arg.getWidth())
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Find Inverse Of A Non-Square Matrix: det("
						+ std::to_string(arg.getHeight()) + "x" + std::to_string(arg.getWidth()) + ")";
					return;
				}
				if (arg.det() == 0)
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Find Inverse Of A Matrix With Zero Determinant: det("
						+ std::to_string(arg.getHeight()) + "x" + std::to_string(arg.getWidth()) + ")";
					return;
				}

				Matrix<double> res;
				res = arg.inv();
				this->tokens.push_back(new Token(Token::Tokens::MID));
				this->tokens.back()->sValue = "";
				this->evalStack.push(this->tokens.back());
				matrixStack.push(new MatrixContainer(res, ""));
			}
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Evaluation Error: Missing Function Arguments For: " + FID + "()";
		}
	}
	else
	{
		if (this->evalStack.size() >= nArgs)
		{
			double args[2];
			for (int i = 0; i < nArgs; i++)
			{
				if (this->evalStack.top()->type == Token::Tokens::VAL)
					args[nArgs - 1 - i] = this->evalStack.top()->dValue;
				else
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Invalid Arguments For Function: " + FID + "()";
					return;
				}
				this->evalStack.pop();
			}

			double res = 0;
			if (FID == "sin")
				res = sin(args[0]);
			else if (FID == "cos")
				res = cos(args[0]);
			else if (FID == "tan" || FID == "tg")
				res = tan(args[0]);
			else if (FID == "ctg")
				res = 1 / tan(args[0]);
			else if (FID == "asin" || FID == "arcsin")
				res = asin(args[0]);
			else if (FID == "acos" || FID == "arccos")
				res = acos(args[0]);
			else if (FID == "ctg")
				res = 1 / tan(args[0]);
			else if (FID == "atan" || FID == "arctg")
				res = atan(args[0]);
			else if (FID == "arcctg")
				res = atan(1 / args[0]);
			else if (FID == "sqrt")
				res = sqrt(args[0]);
			else if (FID == "cbrt")
				res = cbrt(args[0]);
			else if (FID == "abs")
				res = abs(args[0]);
			else if (FID == "exp")
				res = exp(args[0]);
			else if (FID == "ln")
				res = log(args[0]);
			else if (FID == "lg")
				res = log10(args[0]);
			else if (FID == "pow")
				res = pow(args[0], args[1]);
			else if (FID == "log")
				res = log(args[1]) / log(args[0]);
			else if (FID == "max")
				res = (args[0] > args[1]) ? args[0] : args[1];
			else if (FID == "min")
				res = (args[0] < args[1]) ? args[0] : args[1];


			this->tokens.push_back(new Token(Token::Tokens::VAL));
			this->tokens.back()->dValue = res;
			this->evalStack.push(this->tokens.back());
		}
		else
		{
			this->parsingState = ParsingState::ERROR;
			this->errorString = "Evaluation Error: Missing Function Arguments For: " + FID + "()";
		}
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
		else if (this->evalStack.top()->type == Token::Tokens::FID)
		{
			evalFunc();
			if (this->parsingState == ParsingState::ERROR)
				return;

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
					&& (op1->type == Token::Tokens::VAL
					|| op1->type == Token::Tokens::NUL))
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
					Matrix<double> M2 = getMatrix(op2->sValue);
					if (this->parsingState == ParsingState::ERROR)
						return;
						

					if (op1->type == Token::Tokens::NUL)
					{
						op1->type = Token::Tokens::MID;
						op1->isString = true;
						op1->sValue = "";
						this->matrixStack.push(new MatrixContainer(M2.getHeight(), M2.getWidth(), ""));
					}

					Matrix<double> M1 = getMatrix(op1->sValue);
					if (this->parsingState == ParsingState::ERROR)
						return;

					if (M1.getHeight() == M2.getHeight() && M1.getWidth() == M2.getWidth())
					{
						Matrix<double> M3;
						if (op3->type == Token::Tokens::ADD)
							M3 = M1 + M2;
						else if (op3->type == Token::Tokens::SUB)
							M3 = M1 - M2;

						this->tokens.push_back(new Token(Token::Tokens::MID));
						this->tokens.back()->sValue = "";
						this->evalStack.push(this->tokens.back());
						matrixStack.push(new MatrixContainer(M3, ""));
					}
					else
					{
						this->parsingState = ParsingState::ERROR;
						this->errorString = "Evaluation Error: Cannot Add/Subtract Matrices Of Diffirent Sizes: "
							+ op1->sValue + " " + std::to_string(M1.getHeight()) + "x" + std::to_string(M1.getWidth())
							+ " & " + op2->sValue + " " + std::to_string(M2.getHeight()) + "x" + std::to_string(M2.getWidth());
						return;
					}
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
					// Matrix Scalar Mul
					Matrix<double> M1;
					double db;
					if (op2->type == Token::Tokens::MID)
					{
						M1 = getMatrix(op2->sValue);
						db = op1->dValue;
					}
					else
					{
						M1 = getMatrix(op1->sValue);
						db = op2->dValue;
					}

					if (this->parsingState == ParsingState::ERROR)
						return;

					Matrix<double> M2;
					M2 = M1 * db;

					this->tokens.push_back(new Token(Token::Tokens::MID));
					this->tokens.back()->sValue = "";
					this->evalStack.push(this->tokens.back());
					matrixStack.push(new MatrixContainer(M2, ""));
				}
				else if (op2->type == Token::Tokens::MID
					&& op1->type == Token::Tokens::MID)
				{
					// Dot Product
					Matrix<double> M2 = getMatrix(op2->sValue);
					if (this->parsingState == ParsingState::ERROR)
						return;

					Matrix<double> M1 = getMatrix(op1->sValue);
					if (this->parsingState == ParsingState::ERROR)
						return;

					if (M1.getWidth() == M2.getHeight())
					{
						Matrix<double> M3;
						M3 = M1 * M2;
	
						this->tokens.push_back(new Token(Token::Tokens::MID));
						this->tokens.back()->sValue = "";
						this->evalStack.push(this->tokens.back());
						matrixStack.push(new MatrixContainer(M3, ""));
					}
					else
					{
						this->parsingState = ParsingState::ERROR;
						this->errorString = "Evaluation Error: Cannot Multiply Matrices Of These Sizes: "
							+ op1->sValue + " " + std::to_string(M1.getHeight()) + "x" + std::to_string(M1.getWidth())
							+ " & " + op2->sValue + " " + std::to_string(M2.getHeight()) + "x" + std::to_string(M2.getWidth());
						return;
					}
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
			else if (op1->type == Token::Tokens::MID
				&& op2->type == Token::Tokens::VAL)
			{
				// Matrix Scalar Div
				Matrix<double> M1;
				double db;
				M1 = getMatrix(op1->sValue);
				db = op2->dValue;

				if (this->parsingState == ParsingState::ERROR)
					return;

				Matrix<double> M2;
				M2 = M1 / db;

				this->tokens.push_back(new Token(Token::Tokens::MID));
				this->tokens.back()->sValue = "";
				this->evalStack.push(this->tokens.back());
				matrixStack.push(new MatrixContainer(M2, ""));
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
				// Matrix To Power
				Matrix<double> M1 = getMatrix(op1->sValue);
				if (this->parsingState == ParsingState::ERROR)
					return;

				int power = (int)op2->dValue;
				if (power <= 0)
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Raise Matrix To A Power Less Than 1: "
						+ op1->sValue + " " + std::to_string(M1.getHeight()) + "x" + std::to_string(M1.getWidth())
						+ " ^ " + std::to_string(power);
					return;
				}

				if (M1.getHeight() == M1.getWidth())
				{
					Matrix<double> M2;
					M2 = M1;
					for (int i = 1; i < power; i++)
						M2 = M2 * M1;

					this->tokens.push_back(new Token(Token::Tokens::MID));
					this->tokens.back()->sValue = "";
					this->evalStack.push(this->tokens.back());
					matrixStack.push(new MatrixContainer(M2, ""));
				}
				else
				{
					this->parsingState = ParsingState::ERROR;
					this->errorString = "Evaluation Error: Cannot Raise Non-Sqaure Matrix To A Power: "
						+ op1->sValue + " " + std::to_string(M1.getHeight()) + "x" + std::to_string(M1.getWidth())
						+ " ^ " + std::to_string(power);
					return;
				}
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
		this->mResult = getMatrix(this->evalStack.top()->sValue);
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
	s1 << "Original: ";
	for (int i = 0; i < this->tokens.size(); i++)
	{
		s1 << this->tokens[i]->string();
	}
	s1 << "\n";
	std::cout << s1.str();

	shuntToQueue();

	std::stringstream s2;
	s2 << "Polish:   ";
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

double Parser::getDoubleResult()
{
	return this->dResult;
}

Matrix<double> Parser::getMatrixResult()
{
	return this->mResult;
}

std::string& Parser::getError()
{
	return this->errorString;
}

