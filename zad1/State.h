#pragma once
#include<string>
#include<sstream>
#include<iostream>
#include<iomanip>
#include<conio.h>

class State
{
public:
	// Constructors/Destructors
	State();
	virtual ~State();

private:
	// Variables
	bool quit;
	int selectedOption;
	int totalOptions;

public:
	// Accessors
	const bool& getQuit() const;
	int getSelectedOption() const;
	int getTotalOptions() const;

	// Modifiers
	void setQuit(const bool quit);
	void setSelectedOption(const int option);
	void setTotalOptions(const int n);

	// Functionse
	std::string getOptionsString(std::string* options, 
		int nOptions=-1, int firstOptionId=0) const;

	void nextOption();
	void previousOption();

	int getKey(bool raw = false) const;
	int getChoice() const;

	virtual void update() = 0;
};

