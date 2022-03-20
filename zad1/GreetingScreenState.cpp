#include "GreetingScreenState.h"

// Constructors/Destructors
GreetingScreenState::GreetingScreenState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->viewed = false;

	this->user = user;
}	

GreetingScreenState::~GreetingScreenState()
{
}

void GreetingScreenState::printMenu() const
{
	system("CLS");
	std::cout << " <======== Matrify 0.3 ========>" << "\n" << "\n"
		<< "-------------------------------------------------------------" << "\n"
		<< " Matrify is a math toolkit developed by Vladeeer to automate" << "\n"
		<< " basic matrix calculations" << "\n"
		<< "-------------------------------------------------------------" << "\n"
		<< " What can it(or will WIP) do:" << "\n"
		<< " - Evaluate basic math expressions with scalars and matrices" << "\n"
		<< " - Find roots of a system of linear equations" << "\n"
		<< "   ~ Using the Gauss-Jordan method" << "\n"
		<< "   ~ Using the Cramer's rule" << "\n"
		<< " - Calculate the Inverse matrix" << "\n"
		<< " - Calculate the determinant of a matrix" << "\n"
		<< "-------------------------------------------------------------" << "\n" << "\n"
		<< " Press any key to continue... ";
}

void GreetingScreenState::updateMenu()
{
	if (this->getKey())
		this->states->push(new MainMenuState(this->states, this->user));
}

void GreetingScreenState::update()
{
	// Make sure splash is only printed once
	if (!this->viewed)
	{
		this->printMenu();
		this->updateMenu();
		this->viewed = true;
	}
	else
		this->setQuit(true);
}
