#include "RenameState.h"

// Constructors/Destructors
RenameState::RenameState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;
	this->selectedMatrix = this->user->getMatrices()[this->user->getSelectedId()];

	this->setTotalOptions(3);

	this->newName = "None";
}

RenameState::~RenameState()
{
}

int RenameState::findMatrix(std::string& name) const
{
	std::vector<MatrixContainer*> matrices = this->user->getMatrices();

	for (int i = 0; i < this->user->getnMatrices(); i++)
		if (matrices[i]->name == name)
		{
			return i;
		}
	return -1;
}

void RenameState::printMenu() const
{
	system("CLS");

	std::stringstream ss;

	ss << " <========  Rename Matrix  ========>" << "\n" << "\n";

	ss << this->selectedMatrix->matrix->string() << "\n";

	std::string options[3];
	options[0] = "New Name:  " + this->newName;
	options[1] = "Confirm";
	options[2] = "Cancel";

	ss << "------------------------------------" << "\n";

	ss << "  " << this->selectedMatrix->name + " "
		+ std::to_string(this->selectedMatrix->matrix->getHeight()) + "x"
		+ std::to_string(this->selectedMatrix->matrix->getWidth()) << "\n" << "\n";

	ss << getOptionsString(options);

	std::cout << ss.str();
}

void RenameState::updateMenu()
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
			case 1:
				// Confirm
				if (this->newName.empty() || this->newName == "None")
					this->states->push(new ErrorState("Matrix Must Have a Name"));
				else if (48 <= this->newName[0] && this->newName[0] <= 57)
					this->states->push(new ErrorState("Matrix Name Cannot Begin With a Number"));
				else if (this->findMatrix(this->newName) != -1)
					this->states->push(new ErrorState("Matrix Must Have Unique Name"));
				else
				{
					this->selectedMatrix->name = newName;
					this->setQuit(true);
				}
				quitLoop = true;
				break;
			case 2:
				// Cancel
				this->setQuit(true);
				quitLoop = true;
				break;
			}
		}
		else if ((95 <= choice && choice <= 122)
			|| (65 <= choice && choice <= 90)
			|| (48 <= choice && choice <= 57))
		{
			// _'Aa1
			if (this->getSelectedOption() == 0)
			{
				// New Name
				if (this->newName == "None")
					this->newName = choice;
				else
					this->newName.push_back(choice);
				quitLoop = true;
			}
		}
		else if (choice == 8)
		{
			// Backspace
			if (this->getSelectedOption() == 0)
			{
				// New Name
				if (!this->newName.empty())
					this->newName.pop_back();
				quitLoop = true;
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

void RenameState::update()
{
	this->printMenu();
	this->updateMenu();
}