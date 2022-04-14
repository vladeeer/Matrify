#include "EvalState.h"

// Constructors/Destructors
EvalState::EvalState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;

	this->expression = "3+4*2/(1-5)^2^3";
	this->result = "";
	this->cursorPos = (int)this->expression.length();
}

EvalState::~EvalState()
{
}

bool EvalState::isIn(const char& a, const std::string& s) const
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] == a)
			return true;
	return false;
}

void EvalState::printMenu() const
{
	system("CLS");

	std::stringstream ss;

	ss <<  " <======== Evaluate  ========>" << "\n" << "\n";

	ss << " : " << this->expression << "\n";

	std::string cursorLine;
	if (!this->expression.empty())
	{
		if (this->expression == " ")
		{
			cursorLine.resize(3 + 1, ' ');
			cursorLine[3] = '^';
		}
		else
		{
			cursorLine.resize(3 + this->expression.length() + 1, ' ');
			cursorLine[3LL + this->cursorPos] = '^';
		}
	}

	cursorLine.append("\n");
	ss << cursorLine;

	ss 
		<< this->result << "\n";

	std::cout << ss.str();
}

void EvalState::updateMenu()
{
	bool quitLoop = false;
	int choice = 0;

	while (!quitLoop)
	{
		choice = this->getKey(true);
		if (choice == 224)
		{
			// Special Keys
			switch (this->getKey(true))
			{
			case 80:
				// Down Arrow
				this->nextOption();
				quitLoop = true;
				break;
			case 72:
				// Up Arrow
				this->previousOption();
				quitLoop = true;
				break;
			case 75:
				// Left Arrow
				if (!this->expression.empty() && (this->cursorPos > 0))
					this->cursorPos--;
				quitLoop = true;
				break;
			case 77:
				// Right Arrow
				if (!this->expression.empty() && this->cursorPos < this->expression.length())
					this->cursorPos++;
				quitLoop = true;
				break;
			}
		}
		else if (choice == 13)
		{
			// Enter
			Parser parser(this->user, this->expression);
			parser.parse();
			if (!parser.error())
			{
				if (parser.resIsMatrix())
					this->result = parser.getMatrixResult().string();
				else
					this->result = std::to_string(parser.getDoubleResult());
			}
			else
			{
				this->result = "";
				this->states->push(new ErrorState(parser.getError()));
			}
			quitLoop = true;
		}
		else if ((95 <= choice && choice <= 122)
			|| (65 <= choice && choice <= 90)
			|| (48 <= choice && choice <= 57)
			|| (choice == 44 || choice == 46)
			|| choice == 32
			|| this->isIn(choice, "+-*/^()"))
		{
			// _Aa1
			this->expression.insert(this->cursorPos, 1, choice);
			this->cursorPos++;
			quitLoop = true;
		}
		else if (choice == 8)
		{
			// Backspace
			if (!this->expression.empty())
			{
				if (this->cursorPos > 0)
				{
					this->expression.erase(this->cursorPos - 1LL, 1);
					this->cursorPos--;
				}
			}
			quitLoop = true;
		}
		else if (choice == 27)
		{
			// Esc
			this->setQuit(true);
			quitLoop = true;
		}
	}
	if (this->expression.empty())
		this->expression = " ";
}

void EvalState::update()
{
	this->printMenu();
	this->updateMenu();
}
