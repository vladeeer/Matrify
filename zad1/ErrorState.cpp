#include "ErrorState.h"

// Constructors/Destructors
ErrorState::ErrorState(std::string errorMessage) : State()
{
	this->errorMessage = errorMessage;
}
ErrorState::~ErrorState()
{

}

void ErrorState::printMenu() const
{
	system("CLS");

	std::stringstream ss;

	ss << " <======== ERROR ========>" << "\n" << "\n";

	ss << " " << this->errorMessage << "\n" << "\n";

	ss << " Press any key to continue... ";

	std::cout << ss.str();
}

void ErrorState::updateMenu()
{
	if (this->getKey())
		this->setQuit(true);
}

void ErrorState::update()
{

	this->printMenu();
	this->updateMenu();
}
