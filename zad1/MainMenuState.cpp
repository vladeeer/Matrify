#include "MainMenuState.h"

// Constructors/Destructors
MainMenuState::MainMenuState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->setTotalOptions(4);

	this->user = user;

	this->lastQuery = "";
}
MainMenuState::~MainMenuState()
{
}

void MainMenuState::printMenu() const
{
	system("CLS");

	std::stringstream ss;

	ss << " <======== Main Menu ========>" << "\n" << "\n";

	std::string options[4] = { "Evaluate Expression","Manage Stored Matrices","Settings","Quit"};
	ss << getOptionsString(options); 

	std::cout << ss.str();
}

void MainMenuState::updateMenu()
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
				break;
			case 1:
				// Manage Stored Matrices
				this->states->push(new MatrixManagerState(this->states, this->user));
				break;
			case 2:
				break;
			case 3:
				// Quit
				this->states->push(new ConfirmState(this->user, "Are You Sure You Want To Leave?", "Leave", "Stay"));
				this->lastQuery = "Quit";
				break;
			}
			quitLoop = true;
			break;
		}
		else if (choice == 27)
		{
			// Esc
			this->states->push(new ConfirmState(this->user, "Are You Sure You Want To Quit?", "Leave", "Stay"));
			this->lastQuery = "Quit";
			quitLoop = true;
			break;
		}
	}
}

void MainMenuState::update()
{
	if (lastQuery == "Quit")
	{
		this->lastQuery = "";
		if (this->user->getConfirm())
			this->setQuit(true);
	}
	else
	{
		this->printMenu();
		this->updateMenu();
	}
}
