#pragma once
#include<stack>
#include"GreetingScreenState.h"
#include"User.h"

class Matrify
{
public:
	// Constructors/Destructors
	Matrify();
	~Matrify();

private:
	// Variables
	bool quit;
	std::stack<State*> states;
	User* user;

public: 
	// Accessors
	const bool& getQuit() const;

	// Functions
	void update();
};

