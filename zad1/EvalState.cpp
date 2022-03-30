#include "EvalState.h"

// Constructors/Destructors
EvalState::EvalState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;

	this->expression = "None";
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

	ss << " <======== Matrix Creator ========>" << "\n" << "\n";

	ss << " : " << this->expression << "\n";

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
			}
		}
		else if (choice == 13)
		{
			// Enter
			Parser parser(this->user, this->expression);
			parser.parse();
			system("PAUSE");
			quitLoop = true;
		}
		else if ((95 <= choice && choice <= 122)
			|| (65 <= choice && choice <= 90)
			|| (48 <= choice && choice <= 57)
			|| choice == 32
			|| this->isIn(choice, "+-*/^()"))
		{
			// _'Aa1
			switch (this->getSelectedOption())
			{
			case 0:
				// Name
				if (this->expression == "None")
					this->expression = choice;
				else
					this->expression.push_back(choice);
				quitLoop = true;
				break;
			}
		}
		else if (choice == 8)
		{
			// Backspace
			switch (this->getSelectedOption())
			{
			case 0:
				// Name
				if (!this->expression.empty())
					this->expression.pop_back();
				quitLoop = true;
				break;
			}
		}
		else if (choice == 27)
		{
			// Esc
			this->setQuit(true);
			quitLoop = true;
		}
	}
}

void EvalState::update()
{
	this->printMenu();
	this->updateMenu();
}
