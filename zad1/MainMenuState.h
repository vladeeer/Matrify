#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"MatrixManagerState.h"
#include"confirmState.h"

class MainMenuState : public State
{
public:
	// Constructors/Destructors
	MainMenuState(std::stack<State*>* states, User* user);
	~MainMenuState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

	std::string lastQuery;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

