#pragma once
#include<string>
#include"User.h"
#include"State.h"

class ConfirmState : public State
{
public:
	// Constructors/Destructors
	ConfirmState(User* user, std::string message,
		std::string yes, std::string no);
	~ConfirmState();

private:
	// Variables
	User* user;

	std::string message;
	std::string yes;
	std::string no;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

