#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"MatrixCreatorState.h"
#include"SelectedMatrixState.h"

class MatrixManagerState : public State
{
public:
	// Constructors/Destructors
	MatrixManagerState(std::stack<State*>* states, User* user);
	~MatrixManagerState();

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

