#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"MatrixContainer.h"

class MoveState : public State
{
public:
	// Constructors/Destructors
	MoveState(std::stack<State*>* states, User* user);
	~MoveState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

public:
	// Functions
	void printMenu();
	void updateMenu();
	void update();
};

