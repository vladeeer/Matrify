#include "MatrixManagerState.h"

// Constructors/Destructors
MatrixManagerState::MatrixManagerState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;
}

MatrixManagerState::~MatrixManagerState()
{
}

void MatrixManagerState::printMenu() 
{
	system("CLS");

	std::stringstream ss;
	
	this->setTotalOptions(this->user->getnMatrices() + 2);

	ss << " <======== Matrix Manager ========>" << "\n" << "\n";

	std::vector<std::string> options(this->user->getMaxMatrices()+2LL);
	options[this->user->getMaxMatrices()] = "Create New Matrix"; 
	options[this->user->getMaxMatrices()+1LL] = "Back To Main Menu";
	
	if (this->user->getnMatrices() > 0)
	{
		for (int i = 0; i < this->user->getnMatrices(); i++)
		{
			options[i] = std::to_string(i) + ". "
				+ this->user->getMatrices()[i]->name + " "
				+ std::to_string(this->user->getMatrices()[i]->matrix->getHeight()) + "x"
				+ std::to_string(this->user->getMatrices()[i]->matrix->getWidth());
		}

		ss << " " << std::to_string(this->user->getnMatrices())
			<< "/" << std::to_string(this->user->getMaxMatrices())
			<< " Matrices Stored" << "\n" << "\n";

		ss << "-----------------------------------" << "\n";

		ss << getOptionsString(options.data(), this->user->getnMatrices());

		ss << "-----------------------------------" << "\n" << "\n";
	}
	else
	{
		ss << " No Matrices Stored" << " \n" << "\n";
	}
	ss << getOptionsString(options.data() + this->user->getMaxMatrices(), 2, this->user->getnMatrices()); // New, Quit

	std::cout << ss.str();
}

void MatrixManagerState::updateMenu()
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
			int selectedOption = this->getSelectedOption();

			if (selectedOption < this->user->getnMatrices())
			{
				this->user->setSelectedId(selectedOption);
				this->states->push(new SelectedMatrixState(this->states, this->user));
			}
			else if (selectedOption == this->user->getnMatrices())
			{
				// Open Matrix Creator
				if (this->user->getnMatrices() < this->user->getMaxMatrices())
					this->states->push(new MatrixCreatorState(this->states, this->user));
				else
					this->states->push(new ErrorState("Maximun Amount Of Stored Matrices Reached"));
			}
			else
			{
				// Quit
				this->setQuit(true);
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
}

void MatrixManagerState::update()
{
	this->printMenu();
	this->updateMenu();
}