#pragma once
#include<string>
#include"State.h"

class ErrorState : public State
{
public:
	// Constructors/Destructors
	ErrorState(std::string errorMessage);
	~ErrorState();

private:
	// Variables
	std::string errorMessage;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

