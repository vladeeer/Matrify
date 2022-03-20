#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"MainMenuState.h"

class GreetingScreenState : public State
{
public:
	// Constructors/Destructors
	GreetingScreenState(std::stack<State*>* states, User* user);
	~GreetingScreenState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

	bool viewed;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

