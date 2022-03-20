#include "MatrixEditorState.h"

// Constructors/Destructors
MatrixEditorState::MatrixEditorState(
	std::stack<State*>* states, User* user) : State()
{
	this->states = states;
	this->user = user;

	this->selectedMatrix = this->user->getMatrices()[this->user->getSelectedId()];
	this->selectedCol = 0;

	this->lastRow = 0;
	this->lastCol = 0;

	this->currentString = this->selectedMatrix->matrix->elementString(0, 0);
	this->removeTailSpaces(this->currentString);
}

MatrixEditorState::~MatrixEditorState()
{
}

// Functions
void MatrixEditorState::nextCol()
{
	if (this->selectedCol + 1 < this->selectedMatrix->matrix->getWidth())
		this->selectedCol++;
	else
		this->selectedCol = 0;
}

void MatrixEditorState::previousCol()
{
	if (this->selectedCol != 0)
		this->selectedCol--;
	else
		this->selectedCol = this->selectedMatrix->matrix->getWidth() - 1;
}

void MatrixEditorState::removeTailSpaces(std::string& s)
{
	while (s.back() == ' ')
	{
		s.pop_back();
	}
}

void MatrixEditorState::printMenu()
{
	system("CLS");

	std::stringstream ss;

	this->setTotalOptions(this->selectedMatrix->matrix->getHeight() + 2);

	ss << " " << this->currentString << "_" << "\n\n";

	ss << " <========  Matrix Editor  ========>" << "\n" << "\n";

	std::string matrix = this->selectedMatrix->matrix->string();

	int length = this->selectedMatrix->matrix->elementString(0, 0).length();
	if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
		matrix[3 + this->getSelectedOption() *
		(6 + this->selectedMatrix->matrix->getWidth() * (1 + length))
		+ this->selectedCol * (length + 1)] = '>';

	ss << matrix << "\n";

	std::string options[2];
	options[0] = "Clear";
	options[1] = "Back";

	ss << "------------------------------------" << "\n";

	ss << "  " << this->selectedMatrix->name + " "
		+ std::to_string(this->selectedMatrix->matrix->getHeight()) + "x"
		+ std::to_string(this->selectedMatrix->matrix->getWidth()) << "\n" << "\n";

	ss << getOptionsString(options, 2, this->selectedMatrix->matrix->getHeight()) << "\n"; // New, Quit

	std::cout << ss.str();
}

void MatrixEditorState::updateMenu()
{
	bool quitLoop = false;
	int choice = 0;

	while (!quitLoop)
	{
		choice = this->getKey(true);
		if (choice == 224)
		{
			// Special Keys
			int choice2 = this->getKey(true);
			this->lastRow = this->getSelectedOption();
			this->lastCol = this->selectedCol;

			switch (choice2)
			{
			case 80:
				// Down Arrow
				this->nextOption();
				break;
			case 72:
				// Up Arrow
				this->previousOption();
				break;
			case 75:
				// Left Arrow
				this->previousCol();
				break;
			case 77:
				// Right Arrow
				this->nextCol();
				break;	
			}
			if (choice2 == 72 || choice2 == 75 || choice2 == 77 || choice2 == 80)
			{
				if (this->lastRow < this->selectedMatrix->matrix->getHeight())
				{
					if (this->currentString.empty() || this->currentString == ".")
						this->selectedMatrix->matrix->set(this->lastRow, this->lastCol, 0);
					else
						this->selectedMatrix->matrix->set(this->lastRow, this->lastCol, std::stod(this->currentString));
				}
				if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
					this->currentString = this->selectedMatrix->matrix->elementString(this->getSelectedOption(), this->selectedCol);
				this->removeTailSpaces(this->currentString);
				quitLoop = true;
			}
		}
		else if (choice == 13)
		{
			// Enter
			int selectedOption = this->getSelectedOption();

			if (selectedOption == this->selectedMatrix->matrix->getHeight())
			{
				// Clear Matrix
				this->selectedMatrix->matrix->initWithZeros();
			}
			else if (selectedOption == this->selectedMatrix->matrix->getHeight() + 1)
			{
				// Back
				this->setQuit(true);
			}

			quitLoop = true;
			break;
		}
		if (48 <= choice && choice <= 57)
		{
			// 123
			if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
				this->currentString.push_back(choice);
			quitLoop = true;
		}
		else if (choice == 44 || choice == 46)
		{
			if (this->currentString.find('.') == std::string::npos)
			{
				this->currentString.push_back('.');
				quitLoop = true;
			}
		}
		else if (choice == 8)
		{
			// Backspace
			if (!this->currentString.empty())
			{
				this->currentString.pop_back();
				quitLoop = true;
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
	if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
	{
		if (this->currentString.empty() || this->currentString == ".")
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, 0);
		else
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, std::stod(this->currentString));
	}
}

void MatrixEditorState::update()
{
	this->printMenu();
	this->updateMenu();
}