#include "ConfirmState.h"

// Constructors/Destructors
ConfirmState::ConfirmState(User* user, std::string message,
	std::string yes, std::string no) : State()
{
	this->user = user;

	this->message = message;
	this->yes = yes;
	this->no = no;

	this->setTotalOptions(2);
}
ConfirmState::~ConfirmState()
{

}

void ConfirmState::printMenu() const
{
	system("CLS");

	std::stringstream ss;
	
	std::string options[2];
	options[0] = this->yes;
	options[1] = this->no;

	ss << " <======== Choice ========>" << "\n" << "\n";

	ss << " " << this->message << "\n" << "\n";

	ss << this->getOptionsString(options);

	std::cout << ss.str();
}

void ConfirmState::updateMenu()
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
				// Yes
				this->user->confirm();
				break;
			case 1:
				// No
				this->user->cancel();
				break;
			}
			this->setQuit(true);
			quitLoop = true;
			break;
		}
		else if (choice == 27)
		{
			// Esc
			this->user->cancel();
			this->setQuit(true);
			quitLoop = true;
			break;
		}
	}
}

void ConfirmState::update()
{

	this->printMenu();
	this->updateMenu();
}
