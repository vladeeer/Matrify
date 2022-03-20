#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"ErrorState.h"

class MatrixCreatorState : public State
{
public:
	// Constructors/Destructors
	MatrixCreatorState(std::stack<State*>* states, User* user);
	~MatrixCreatorState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

	std::string newMatrixName;
	int newMatrixHeight;
	int newMatrixWidth;
	
	bool zeroMatrix;
	bool matrixOfOnes;
	std::string cloneOf;
	int cloneId;

	bool nextStage;

	int initMode;

	// Functions
	int findMatrix(std::string& name) const;
	void createMatrix();

public:
	// Functions
	void printMenu1() const;
	void updateMenu1();
	void printMenu2() const;
	void updateMenu2();
	void update();
};

