#include "MatrixCreatorState.h"

// Constructors/Destructors
MatrixCreatorState::MatrixCreatorState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->setTotalOptions(5);

	this->user = user;

	this->newMatrixName = "None";
	this->newMatrixHeight = 3;
	this->newMatrixWidth = 3;

	this->zeroMatrix = true;
	this->matrixOfOnes = false;
	this->cloneOf = "None";

	this->nextStage = false;

	this->initMode = 0;
}
MatrixCreatorState::~MatrixCreatorState()
{
}

int MatrixCreatorState::findMatrix(std::string& name) const
{
	std::vector<MatrixContainer*> matrices = this->user->getMatrices();

	for (int i=0; i<this->user->getnMatrices(); i++)
		if (matrices[i]->name == name)
		{
			return i;
		}
	return -1;
}

void MatrixCreatorState::createMatrix()
{
	this->user->getMatrices().push_back(new MatrixContainer(this->newMatrixHeight,
		this->newMatrixWidth, this->newMatrixName));

	switch (this->initMode)
	{
	case 0:
		this->user->getMatrices().back()->matrix->initWithZeros();
		break;
	case 1:
		this->user->getMatrices().back()->matrix->initWithOnes();
		break;
	case 2:
		this->user->getMatrices().back()->matrix->initWithMatrix(*this->user->getMatrices()[this->cloneId]->matrix);
		break;
	}

	this->setQuit(true);
}

void MatrixCreatorState::printMenu1() const
{
	system("CLS");

	std::stringstream ss;
	std::string options[5];

	options[0] = "Name:     " + this->newMatrixName;
	options[1] = "Height    " + std::to_string(this->newMatrixHeight);
	options[2] = "Width     " + std::to_string(this->newMatrixWidth);
	options[3] = "Continue";
	options[4] = "Cancel";

	ss << " <======== Matrix Creator ========>" << "\n" << "\n";
	
	ss << this->getOptionsString(options, 3) << "\n"
		<< this->getOptionsString(options + 3, 2, 3);

	std::cout << ss.str();
}

void MatrixCreatorState::updateMenu1()
{
	bool quitLoop = false;
	int choice = 0;

	while (!quitLoop)
	{
		choice = this->getKey(true);
		if (choice == 224)
		{
			// Special Keys
			switch (this->getKey(true))
			{
			case 80:
				// Down Arrow
				this->nextOption();
				quitLoop = true;
				break;
			case 72:
				// Up Arrow
				this->previousOption();
				quitLoop = true;
				break;
			}
		}
		else if (choice == 13)
		{
			// Enter
			switch (this->getSelectedOption())
			{
			case 3:
				// Next
				if (this->newMatrixName.empty() || this->newMatrixName == "None")
					this->states->push(new ErrorState("Matrix Must Have a Name"));
				else if (48 <= this->newMatrixName[0] && this->newMatrixName[0] <= 57)
					this->states->push(new ErrorState("Matrix Name Cannot Begin With a Number"));
				else if (this->findMatrix(this->newMatrixName) != -1)
					this->states->push(new ErrorState("Matrix Must Have Unique Name"));
				else if (this->newMatrixHeight * this->newMatrixWidth == 0)
					this->states->push(new ErrorState("Matrix Height And Width Must Be Greater Than Zero"));
				else
					this->nextStage = true;

				quitLoop = true;
				break;
			case 4:
				// Quit
				this->setQuit(true);
				quitLoop = true;
				break;
			}
			break;
		}
		else if ((95 <= choice && choice <= 122)
			|| (65 <= choice && choice <= 90)
			|| (48 <= choice && choice <= 57))
		{
			// _'Aa1
			switch (this->getSelectedOption())
			{
			case 0:
				// Name
				if (this->newMatrixName == "None")
					this->newMatrixName = choice;
				else
					this->newMatrixName.push_back(choice);
				quitLoop = true;
				break;
			case 1:
				// Height
				if ((48 <= choice && choice <= 57)
					&& ((this->newMatrixHeight * 10 + choice - 48) <= 50))
					this->newMatrixHeight = this->newMatrixHeight * 10 + choice - 48;
				quitLoop = true;
				break;
			case 2:
				// Width
				if ((48 <= choice && choice <= 57)
					&& ((this->newMatrixWidth * 10 + choice - 48) <= 50))
					this->newMatrixWidth = this->newMatrixWidth * 10 + choice - 48;
				quitLoop = true;
				break;
			}

		}
		else if (choice == 8)
		{
			// Backspace
			switch (this->getSelectedOption())
			{
			case 0:
				// Name
				if (!this->newMatrixName.empty())
					this->newMatrixName.pop_back();
				quitLoop = true;
				break;
			case 1:
				// Height
				this->newMatrixHeight /= 10;
				quitLoop = true;
				break;
			case 2:
				// Width
				this->newMatrixWidth /= 10;
				quitLoop = true;
				break;
			}
		}
		else if (choice == 27)
		{
		// Esc
		this->setQuit(true);
		quitLoop = true;
		break;
		}
	}
}

void MatrixCreatorState::printMenu2() const
{
	system("CLS");

	std::stringstream ss;
	std::string options[5];

	options[0] = "Zero Matrix:     [ ]";
	options[1] = "Matrix Of Ones   [ ]";
	options[2] = "Clone Of         " + this->cloneOf;
	options[3] = "Confirm";
	options[4] = "Back";

	if (this->zeroMatrix)
		options[0][18] = 'X';
	if (this->matrixOfOnes)
		options[1][18] = 'X';

	ss << " <======== Matrix Creator ========>" << "\n" << "\n";

	ss << this->getOptionsString(options, 3) << "\n"
		<< this->getOptionsString(options + 3, 2, 3);

	std::cout << ss.str();
}

void MatrixCreatorState::updateMenu2()
{
	bool quitLoop = false;
	int choice = 0;

	while (!quitLoop)
	{
		choice = this->getKey(true);
		if (choice == 224)
		{
			// Special Keys
			switch (this->getKey(true))
			{
			case 80:
				// Down Arrow
				this->nextOption();
				quitLoop = true;
				break;
			case 72:
				// Up Arrow
				this->previousOption();
				quitLoop = true;
				break;
			}
		}
		else if (choice == 13)
		{
			// Enter
			switch (this->getSelectedOption())
			{
			case 0:
				// Zero Matrix
				this->initMode = 0;
				this->zeroMatrix = true;
				this->matrixOfOnes = false;
				this->cloneOf = "None";
				quitLoop = true;
				break;
			case 1:
				// Matrix Of Ones
				this->initMode = 1;
				this->zeroMatrix = false;
				this->matrixOfOnes = true;
				this->cloneOf = "None";
				quitLoop = true;
				break;
			case 3:
				// Confirm
				if (this->initMode == 1)
				{
					if (this->newMatrixHeight != this->newMatrixWidth)
					{
						this->states->push(new ErrorState("Matrix Of Ones Must Be A Square Matrix"));
						break;
					}
				}
				else if (this->initMode == 2)
				{
					this->cloneId = this->findMatrix(this->cloneOf);
					if (this->cloneId == -1)
					{
						this->states->push(new ErrorState("No Matrix " + this->cloneOf + " Found"));
						break;
					}
					else if (this->user->getMatrices()[this->cloneId]->matrix->getWidth() != this->newMatrixWidth
						|| this->user->getMatrices()[this->cloneId]->matrix->getHeight() != this->newMatrixHeight)
					{
						this->states->push(new ErrorState("Clone Matrix Must Be Of The Same Size As Original"));
						break;
					}
				} 
				
				this->createMatrix();
				quitLoop = true;
				break;
			case 4:
				// Back;
				this->nextStage = false;
				quitLoop = true;
				break;
			}
			break;
		}
		else if ((95 <= choice && choice <= 122)
			|| (65 <= choice && choice <= 90)
			|| (48 <= choice && choice <= 57))
		{
			// _'Aa1
			switch (this->getSelectedOption())
			{
			case 2:
				// Clone Of
				this->initMode = 2;
				this->zeroMatrix = false;
				this->matrixOfOnes = false;
				if (this->cloneOf == "None")
					this->cloneOf = choice;
				else
					this->cloneOf.push_back(choice);
				quitLoop = true;
				break;
			}
		}
		else if (choice == 8)
		{
			// Backspace
			switch (this->getSelectedOption())
			{
			case 2:
				// Clone Of
				this->initMode = 2;
				this->zeroMatrix = false;
				this->matrixOfOnes = false;
				if (!this->cloneOf.empty())
					this->cloneOf.pop_back();
				quitLoop = true;
				break;
			}
		}
		else if (choice == 27)
		{
		// Esc
		this->nextStage = false;
		quitLoop = true;
		break;
		}
	}
}

void MatrixCreatorState::update()
{
	if (!this->nextStage)
	{
		// Name, Size
		this->printMenu1();
		this->updateMenu1();
	}
	else
	{
		// Initialization Type
		this->printMenu2();
		this->updateMenu2();
	}
}
