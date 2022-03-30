#include "MoveState.h"

// Constructors/Destructors
MoveState::MoveState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;

	this->setTotalOptions(3);
}

MoveState::~MoveState()
{
}

void MoveState::printMenu()
{
	system("CLS");

	std::stringstream ss;

	ss << " <========   Move Matrix   ========>" << "\n" << "\n";

	std::string options[3];
	options[0] = "Move Up";
	options[1] = "Move Down";
	options[2] = "Back";
 
	ss << "------------------------------------" << "\n";

	for (int i = 0; i < this->user->getnMatrices(); i++)
		ss << ((i == this->user->getSelectedId()) ? "  > " : "    ")
		<< std::to_string(i) << ". " << this->user->getMatrices()[i]->name << " "
		<< std::to_string(this->user->getMatrices()[i]->matrix->getHeight()) << "x"
		<< std::to_string(this->user->getMatrices()[i]->matrix->getWidth()) << "\n";
	
	ss << "------------------------------------" << "\n" << "\n";


	ss << getOptionsString(options, 3); // New, Quit

	std::cout << ss.str();
}

void MoveState::updateMenu()
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
			switch (this->getSelectedOption())
			{
			case 0:
				// Move Up
				if (this->user->getSelectedId() > 0)
				{
					MatrixContainer* tempMatrixPtr = this->user->getMatrices()[this->user->getSelectedId()];
					this->user->getMatrices()[this->user->getSelectedId()] = this->user->getMatrices()[this->user->getSelectedId() - 1LL];
					this->user->getMatrices()[this->user->getSelectedId() - 1LL] = tempMatrixPtr;
					this->user->setSelectedId(this->user->getSelectedId() - 1);
				}
				break;
			case 1:
				// Move Down
				if (this->user->getSelectedId() < this->user->getnMatrices() - 1)
				{
					MatrixContainer* tempMatrixPtr = this->user->getMatrices()[this->user->getSelectedId()];
					this->user->getMatrices()[this->user->getSelectedId()] = this->user->getMatrices()[this->user->getSelectedId() + 1LL];
					this->user->getMatrices()[this->user->getSelectedId() + 1LL] = tempMatrixPtr;
					this->user->setSelectedId(this->user->getSelectedId() + 1);
				}
				break;
			case 2:
				// Back
				this->setQuit(true);
				break;
			}

			quitLoop = true;
			break;
		}
		else if (choice == 27)
		{
			// Esc
			this->setQuit(true);
			quitLoop = true;
		}
	}
}

void MoveState::update()
{
	this->printMenu();
	this->updateMenu();
}