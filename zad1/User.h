#pragma once
#include<vector>
#include"MatrixContainer.h"

class User
{
public:
	// Constructors/Destructors
	User();
	~User();

private:
	std::vector<MatrixContainer*> matrices;
	int nMaxMatrices;
	int selectedId;

	bool bConfirm;

public:
	// Accessors
	std::vector<MatrixContainer*>& getMatrices();
	int getMaxMatrices();
	int getnMatrices();
	int getSelectedId();
	bool getConfirm();

	// Modifiers
	void setSelectedId(int id);
	void confirm();
	void cancel();
 
	// Functions
};

// Seetings, Stored Matrices, 
// Calculations Log 
