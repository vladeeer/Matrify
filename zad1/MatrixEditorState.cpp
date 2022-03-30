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

	this->setTotalOptions(this->selectedMatrix->matrix->getHeight() + 2);

	this->lastQuery = "";
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

	ss << " <========  Matrix Editor  ========>" << "\n" << "\n";

	std::string matrix = this->selectedMatrix->matrix->string();

	int length = (int)this->selectedMatrix->matrix->elementString(0, 0).length();
	if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
		matrix[3LL + this->getSelectedOption() *
		(6 + this->selectedMatrix->matrix->getWidth() * (1LL + length))
		+ this->selectedCol * (length + 1LL)] = '>';

	ss << matrix << "\n";

	std::string options[2];
	options[0] = "Clear";
	options[1] = "Back";

	ss << "------------------------------------" << "\n";

	ss << "  " << this->selectedMatrix->name + " "
		+ std::to_string(this->selectedMatrix->matrix->getHeight()) + "x"
		+ std::to_string(this->selectedMatrix->matrix->getWidth()) << "\n" << "\n";

	ss << getOptionsString(options, 2, this->selectedMatrix->matrix->getHeight()); // New, Quit

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
				if (this->getSelectedOption() > this->selectedMatrix->matrix->getHeight() - 1)
					this->selectedCol = 0;
				break;
			case 72:
				// Up Arrow
				this->previousOption();
				if (this->getSelectedOption() > this->selectedMatrix->matrix->getHeight() - 1)
					this->selectedCol = 0;
				break;
			case 75:
				// Left Arrow
				if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
					this->previousCol();
				break;
			case 77:
				// Right Arrow
				if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
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
				this->lastQuery = "Clear";
				this->states->push(new ConfirmState(this->user, "Matrix "
					+ this->selectedMatrix->name + " Will Be Cleared", "Ok", "Cancel"));
				quitLoop = true;
			}
			else if (selectedOption == this->selectedMatrix->matrix->getHeight() + 1)
			{
				// Back
				this->setQuit(true);
				quitLoop = true;
			}
		}
		else if (48 <= choice && choice <= 57)
		{
			// 123
			if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
				this->currentString.push_back(choice);
			quitLoop = true;
		}
		else if (choice == 44 || choice == 46)
		{
			// '.'
			if (this->currentString.find('.') == std::string::npos)
			{
				this->currentString.push_back('.');
				quitLoop = true;
			}
		}
		else if (choice == 45)
		{
			// '-'
			if (this->currentString[0] == '-')
				this->currentString.erase(0, 1);
			else
				this->currentString.insert(0, "-");
			quitLoop = true;
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
		}
	}
	if (this->getSelectedOption() < this->selectedMatrix->matrix->getHeight())
	{
		if (this->currentString == "-.")
		{
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, 0);
			this->currentString = "-0.";
		}
		else if (this->currentString.empty() || this->currentString == "-")
		{
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, 0);
			this->currentString = "0";
		}
		else if (this->currentString == ".")
		{
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, 0);
			this->currentString = "0.";
		}
		else
			this->selectedMatrix->matrix->set(this->getSelectedOption(), this->selectedCol, std::stod(this->currentString));
	}
}

void MatrixEditorState::update()
{
	if (this->lastQuery == "Clear")
	{
		this->lastQuery = "";
		if (this->user->getConfirm())
			this->selectedMatrix->matrix->initWithZeros();
	}
	else
	{
		this->printMenu();
		this->updateMenu();
	}
}