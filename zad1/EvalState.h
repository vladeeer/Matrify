#pragma once
#include<stack>
#include<string>
#include"User.h"
#include"State.h"
#include"ErrorState.h"
#include"Parser.h"

class EvalState : public State
{
public:
	// Constructors/Destructors
	EvalState(std::stack<State*>* states, User* user);
	~EvalState();

private:
	// Variables
	std::stack<State*>* states;
	User* user;

	std::string expression;
	std::string result;
	int cursorPos;

public:
	// Functions
	bool isIn(const char& a, const std::string& s) const;

	void printMenu() const;
	void updateMenu();
	void update();
};
