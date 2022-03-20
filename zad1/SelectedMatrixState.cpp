#include "SelectedMatrixState.h"

// Constructors/Destructors
SelectedMatrixState::SelectedMatrixState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;
	this->selectedMatrix = this->user->getMatrices()[this->user->getSelectedId()];

	this->setTotalOptions(5);

	this->lastQuery = "";
}

SelectedMatrixState::~SelectedMatrixState()
{
}

void SelectedMatrixState::printMenu() const
{
	system("CLS");

	std::stringstream ss;

	ss << " <======== Selected Matrix ========>" << "\n" << "\n";

	ss << this->selectedMatrix->matrix->string() << "\n";

	std::string options[5];
	options[0] = "Edit";
	options[1] = "Rename";
	options[2] = "Move";
	options[3] = "Delete";
	options[4] = "Back";

	ss << "------------------------------------" << "\n";

	ss << "  " << this->selectedMatrix->name + " "
		+ std::to_string(this->selectedMatrix->matrix->getHeight()) + "x"
		+ std::to_string(this->selectedMatrix->matrix->getWidth()) << "\n" << "\n";

	ss << getOptionsString(options);

	std::cout << ss.str();
}

void SelectedMatrixState::updateMenu()
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
				// Edit
				this->states->push(new MatrixEditorState(this->states, this->user));
				break;
			case 1:
				// Rename
				this->states->push(new RenameState(this->states, this->user));
				break;
			case 2:
				// Move
				break;
			case 3:
				// Delete
				lastQuery = "Delete";
				this->states->push(new ConfirmState(this->user, "Matrix " 
					+ this->selectedMatrix->name + " Will Be Deleted", "Ok", "Cancel"));
				break;
			case 4:
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
			break;
		}
	}
}

void SelectedMatrixState::update()
{
	if (this->lastQuery == "Delete")
	{
		this->lastQuery = "";
		if (this->user->getConfirm())
		{
			this->user->getMatrices().erase(this->user->getMatrices().begin()
				+ this->user->getSelectedId());
			this->setQuit(true);
		}
	}
	else
	{
		this->printMenu();
		this->updateMenu();
	}
}