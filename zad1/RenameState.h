#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"ErrorState.h"

class RenameState : public State
{
public:
	// Constructors/Destructors
	RenameState(std::stack<State*>* states, User* user);
	~RenameState();

private:
	// Variables
	std::stack<State*>* states;
	MatrixContainer* selectedMatrix;
	User* user;

	std::string newName;

	// Functions
	int findMatrix(std::string& name) const;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

