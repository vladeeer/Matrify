#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"ConfirmState.h"
#include"RenameState.h"
#include"MatrixEditorState.h"

class SelectedMatrixState : public State
{
public:
	// Constructors/Destructors
	SelectedMatrixState(std::stack<State*>* states, User* user);
	~SelectedMatrixState();

private:
	// Variables
	std::stack<State*>* states;
	MatrixContainer* selectedMatrix;
	User* user;

	std::string lastQuery;

public:
	// Functions
	void printMenu() const;
	void updateMenu();
	void update();
};

