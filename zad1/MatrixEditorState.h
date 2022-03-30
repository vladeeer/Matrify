#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"MatrixContainer.h"
#include"ConfirmState.h"

class MatrixEditorState : public State
{
public:
	// Constructors/Destructors
	MatrixEditorState(std::stack<State*>* states, User* user);
	~MatrixEditorState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

	MatrixContainer* selectedMatrix;
	int selectedCol;

	int lastRow;
	int lastCol;

	std::string currentString;

	std::string lastQuery;

public:
	// Functions
	void nextCol();
	void previousCol();
	void removeTailSpaces(std::string& s);

	void printMenu();
	void updateMenu();
	void update();
};

