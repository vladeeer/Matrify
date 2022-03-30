#include "User.h"

// Constructors/Destructors
User::User()
{
	this->nMaxMatrices = 10;
	this->selectedId = -1;

	this->bConfirm = false;
	//this->matrices.push_back(new MatrixContainer(3, 3, "A"));
	//this->matrices.push_back(new MatrixContainer(1, 1, "Hi"));
	this->matrices.push_back(new MatrixContainer(6, 3, "Jacobian"));
}

User::~User()
{
	while (!this->matrices.empty())
	{
		delete this->matrices.back();
		this->matrices.pop_back();
	}
}

// Accessors
std::vector<MatrixContainer*>& User::getMatrices()
{
	return this->matrices;
}

int User::getMaxMatrices() const
{
	return this->nMaxMatrices;
}

int User::getnMatrices() const
{
	return (int)this->matrices.size();
}

int User::getSelectedId() const
{
	return this->selectedId;
}

bool User::getConfirm() const
{
	return this->bConfirm;
}

// Modifiers
void User::setSelectedId(int id)
{
	this->selectedId = id;
}

void User::confirm()
{
	this->bConfirm = true;
}

void User::cancel()
{
	this->bConfirm = false;
}

// Functions
