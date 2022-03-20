#include "Matrify.h"

// Constructors/Destructors
Matrify::Matrify()
{
	this->quit = false;

	// Create User
	this->user = new User();

	// Init first active State
	this->states.push(new GreetingScreenState(&this->states, this->user));
}

Matrify::~Matrify()
{
	// Delete all States
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}

	delete this->user;
}

// Accessors
const bool& Matrify::getQuit() const
{
	return this->quit;
}

// Functions
void Matrify::update()
{
	// Update active State
	this->states.top()->update();

	// Clear last active States if non-active
	while ((!this->states.empty()) && this->states.top()->getQuit())
	{
		delete this->states.top();
		this->states.pop();
	}

	// Quit when no active States left
	if (this->states.empty())
		this->quit = true;
}


