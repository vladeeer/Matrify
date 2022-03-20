#include "State.h"

// Base State Class
// Cannot be Initialized
// Made only to be inherited

// Constructors/Destructors
State::State()
{
	this->quit = false;
	this->selectedOption = 0;
	this->totalOptions = 0;
}

State::~State()
{
}

// Accessors
const bool& State::getQuit() const
{
	return this->quit;
}

int State::getSelectedOption() const
{
	return this->selectedOption;
}

int State::getTotalOptions() const
{
	return this->totalOptions;
}

// Modifiers
void State::setQuit(const bool quit)
{
	this->quit = quit;
}

void State::setSelectedOption(const int option)
{
	this->selectedOption = option;
}

void State::setTotalOptions(const int n)
{
	this->totalOptions = n;
}

// Functions
std::string State::getOptionsString(std::string* options, 
	int nOptions, int firstOptionId) const
{
	std::stringstream ss;

	int n;
	if (nOptions == -1)
		n = this->totalOptions;
	else
		n = nOptions;

	for (int i = 0; i < n; i++)
	{
		ss << " " << ((i == this->selectedOption - firstOptionId) ? "(X)" : " - ")
			<< " " << options[i] << "\n";
	}

	return ss.str();
}

void State::nextOption()
{
	if (this->selectedOption + 1 < this->totalOptions)
		this->selectedOption++;
	else
		this->selectedOption = 0;
}

void State::previousOption()
{
	if (this->selectedOption > 0)
		this->selectedOption--;
	else
		this->selectedOption = this->totalOptions - 1;
}

int State::getKey(bool raw) const
{
	// If !raw return combined
	int key = _getch();
	if (key == 224 && !raw)
		return _getch();
	return key;
}

int State::getChoice() const
{
	// Int Input
	int choice = 0;

	std::cout << " - Choice: ";
	std::cin >> std::setw(1) >> choice;

	while (!std::cin.good())
	{
		std::cout << " ERROR: Incorrect Input" << "\n";

		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');

		std::cout << " - Choice: ";
		std::cin >> std::setw(1) >> choice;
	}

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	return choice;
}
