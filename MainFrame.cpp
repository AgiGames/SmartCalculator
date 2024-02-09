#include "MainFrame.h"
#include <wx/wx.h>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <bits/stdc++.h>

/*
User defined class that contains everything that pertains to a typical numerical mathematical expression.
The data members are as follows:
expression -> Holds the input expression, as a std::string.
leftNumberPos -> Holds the position of the char to the left of a number, where the number itself is to the left of an operand.
rightNumberPos ->Holds the position of the char to the right of a number, where the number itself is to the right of an operand.
leftNumber -> Holds the value of a number to the left of an operand, as a std::string.
rightNumber -> Holds the value of a number to the right of an operand, as a std::string.
leftBracketPos -> Holds the position of the opening bracket of an expression inside parentheses.
rightBracketPos -> Holds the position of the closing bracket of an expression inside parentheses.
parentheticValue -> Holds the evaluated value of an expression inside parentheses, as a std::string.
*/
class MathematicalExpression {
	std::string expression;
	int leftNumberPos;
	int rightNumberPos;
	std::string leftNumber;
	std::string rightNumber;
	int leftBracketPos;
	int rightBracketPos;
	std::string parentheticValue;

public:
	//Gets input expression.
	void GetInputExpression(wxString& WXInput) {
		expression = WXInput.ToStdString();
	}

	//Evaluates expression according to PEMDAS.
	float evaluate() {

		//Evaluates expressions in parentheses.
		int j = 0;
		while (j < expression.size()) {
			if (expression[j] == '(') {
				GetLeftAndRightBracketPos(j);
				EvaluateParentheses(leftBracketPos, rightBracketPos);
			}
			else
				j++;
		}

		//Evaluates exponents.
		j = 0;
		while (j < expression.size()) {
			if (expression[j] == '^') {
				std::string result = std::to_string(EvaluateExponent(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}
			else
				j++;
		}

		//evaluates multiplication and division.
		j = 0;
		while (j < expression.size()) {
			if (expression[j] == '*') {
				std::string result = std::to_string(PerformMultiplication(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}
			else if (expression[j] == '/') {
				std::string result = std::to_string(PerformDivision(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}
			else if (expression[j] == '%') {
				std::string result = std::to_string(PerformModulus(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}
			else
				j++;
		}


		//Evaluates addition and subtraction.
		j = 0;
		while (j < expression.size()) {
			if (expression[j] == '+') {
				std::string result = std::to_string(PerformAddition(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}

			//The below else clause will only fire, if the character to the left of the '-' is a number.
			//The need for this condition arised, as sin-1 will not work if the condition is not written, sin here can be any math function.
			//The 'left number' will be 'sin' causing bugs in the program.
			else if (j != 0 && (expression[j] == '-' && int(expression[j - 1]) >= 48 && int(expression[j - 1]) <= 57)) {
				std::string result = std::to_string(PerformSubtraction(j));
				UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
			}
			else
				j++;
		}

		//Checks if the input expression itself  is a mathematical function.
		FindIfMathFunction(expression);

		return stof(expression);
	}

	//Gets the positon of the open bracket and close bracket of an expression in parentheses.
	void GetLeftAndRightBracketPos(int& j) {
		leftBracketPos = j;
		int unclosedBracketNo = 1; //Number of brackets that remain unlcosed.
		for (int i = j + 1; i < expression.size(); i++) {
			if (expression[i] == '(')
				unclosedBracketNo++; //If new brackets are opened, then unclosedBracketNo increases.
			else if (expression[i] == ')' && unclosedBracketNo == 1) {

				//If a bracket is closed, and the number of unclosed brackets is 1 then that must be the end of the expression in parentheses.
				//Hence the position of the right bracket is the point at which the bracket is closed.

				rightBracketPos = i;
				break;
			}
			else if (expression[i] == ')')
				unclosedBracketNo--; //If new brackets are closed, then unclosedBracketNo decreases.
		}

	}

	void GetRightAndLeftNumber(int& j) {

		//leftNumberPos is initialized to a point to the left of an operand.
		//rightNumberPos is initialized to a point to the right of an operand.
			rightNumberPos = j + 1;
			leftNumberPos = j - 1;
			while ((int(expression[leftNumberPos]) >= 48 && int(expression[leftNumberPos]) <= 57) || expression[leftNumberPos] == '.' || (int(expression[leftNumberPos]) >= 97 && int(expression[leftNumberPos]) <= 122)
				|| leftNumberPos == 0 || (int(expression[leftNumberPos - 1]) >= 97 && int(expression[leftNumberPos - 1]) <= 122)) {

				//The above 'leftNumberPos == 0' condition is written to catch the '-' at the start of an expression, Ex: -23+1, -41.2+21.
				/*
				The above 'int(expression[leftNumberPos-1]) >= 97 && int(expression[leftNumberPos-1]) <= 122' is written to catch math functions before the '-',
				Ex: sin-23.4, cos-32.45.
				*/

				leftNumberPos--; //If it is not a number or a decimal point, the position is decreased.
				if (leftNumberPos == -1)
					break;
			}

		while ((int(expression[rightNumberPos]) >= 48 && int(expression[rightNumberPos]) <= 57) || expression[rightNumberPos] == '.' || (int(expression[rightNumberPos]) >= 97 && int(expression[rightNumberPos]) <= 122)
			|| ((int(expression[rightNumberPos + 1]) >= 48 && int(expression[rightNumberPos + 1]) <= 57) && expression[rightNumberPos - 1] == '^') && rightNumberPos <= expression.size()) {

			/*
			The above (int(expression[rightNumberPos+1]) >= 48 && int(expression[rightNumberPos+1]) <= 57) && expression[rightNumberPos-1] == '^'
			is written to catch the negative symbol in negative exponents.
			Ex: 2^-2, 1.6*10^-19.
			*/
			rightNumberPos++; //If it is not a number or a decimal point, the position is increased.
			if (rightNumberPos == expression.size())
				break;
		}

		//At this point leftNumberPos and rightNumberPos point to the element to the left and to the right of the numbers respectively.
		//Hence the weird arguments for substr().

		leftNumber = expression.substr(leftNumberPos + 1, j - leftNumberPos - 1);
		rightNumber = expression.substr(j + 1, rightNumberPos - j - 1);
		FindIfMathFunction(leftNumber); //Finds if left number is a mathematical function, Ex: sin, cos.
		FindIfMathFunction(rightNumber); //Finds if right number is a mathematical function, Ex: sin, cos.
	}

	//Performs multiplication using the number to the left of '*' and the number to the right of '*'.
	//stof() takes a string and converts it into float.
	float PerformMultiplication(int& j) {
		GetRightAndLeftNumber(j);
		float multipliedResult = stof(leftNumber) * stof(rightNumber);
		return multipliedResult;
	}

	//Performs division using the number to the left of '/' and the number to the right of '/'.
	//stof() takes a string and converts it into float.
	float PerformDivision(int& j) {
		GetRightAndLeftNumber(j);
		float dividedResult = stof(leftNumber) / stof(rightNumber);
		return dividedResult;
	}

	//Performs addition using the number to the left of '+' and the number to the right of '+'.
	//stof() takes a string and converts it into float.
	float PerformAddition(int& j) {
		GetRightAndLeftNumber(j);
		float addedResult = stof(leftNumber) + stof(rightNumber);
		return addedResult;
	}

	//Performs subtraction using the number to the left of '-' and the number to the right of '-'.
	//stof() takes a string and converts it into float.
	float PerformSubtraction(int& j) {
		GetRightAndLeftNumber(j);
		float subtractedResult = stof(leftNumber) - stof(rightNumber);
		return subtractedResult;
	}

	//Performs modulus using the number to the left of '%' and the number to the right of '%'.
	//stof() takes a string and converts it into float.  
	float PerformModulus(int& j) {
		GetRightAndLeftNumber(j);
		float modulusResult = stoi(leftNumber) % stoi(rightNumber);
		return modulusResult;
	}

	//Evaluates the expression inside parentheses.
	void EvaluateParentheses(int& openBracketPos, int& closeBracketPos) {

		//Creates an object of the same class, to reuse the functions defined here.
		//This is because the expression inside parentheses is a regular numerical expression, which follows PEMDAS.

		MathematicalExpression* parentheticExpression = new MathematicalExpression;

		//The below substring is ONLY the expression INSIDE the parentheses and does not include the parentheses themselves. 

		parentheticExpression->expression = expression.substr(openBracketPos + 1, (closeBracketPos - 1) - (openBracketPos + 1) + 1);
		parentheticValue = std::to_string(parentheticExpression->evaluate());
		parentheticValue = std::to_string(parentheticExpression->evaluate());
		UpdateExpression(parentheticValue, leftBracketPos, rightBracketPos);
		delete parentheticExpression;
	}

	//Evaluates exponents using the number to the left of '^' and the number to the right of '^'.
	//stof() takes a string and converts it into float.
	float EvaluateExponent(int& j) {
		GetRightAndLeftNumber(j);
		float exponentResult = pow(stof(leftNumber), stof(rightNumber));
		return exponentResult;
	}

	//Replaces the input expression with evaluated result for further calculation.
	void UpdateExpression(std::string& result, int start, int end) {
		int noOfCharacters = 0;
		for (int i = start; i <= end; i++)
			noOfCharacters++; //Counts the number of characters to replace.
		expression.erase(start, noOfCharacters); //Erases the unevaluated part of the expression.
		expression.insert(start, result); //Inserts the evaluated part into the expression.
	}

	void FindIfMathFunction(std::string& function_or_number) {

		//The following if clauses check if the input string is a mathematical function or just a number.
		//If it is one of the following functions, it replaces the value of the input string with the evaluated value of the mathematical function.

		if (function_or_number.substr(0, 2) == "ln") {
			float inputOfMathFunction = stof(function_or_number.substr(2, function_or_number.size() - 2));
			function_or_number = std::to_string(log(inputOfMathFunction));
		}
		if (function_or_number.substr(0, 3) == "log") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(log10(inputOfMathFunction));
		}
		if (function_or_number.substr(0, 3) == "sin") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(std::sin(inputOfMathFunction));
		}
		if (function_or_number.substr(0, 3) == "cos") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(std::cos(inputOfMathFunction));
		}
		if (function_or_number.substr(0, 3) == "tan") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(std::tan(inputOfMathFunction));
		}
		if (function_or_number.substr(0, 3) == "cot") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(1 / (std::tan(inputOfMathFunction)));
		}
		if (function_or_number.substr(0, 3) == "sec") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(1 / (std::cos(inputOfMathFunction)));
		}
		if (function_or_number.substr(0, 3) == "csc") {
			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(1 / (std::sin(inputOfMathFunction)));
		}
		if (function_or_number.substr(0, 3) == "deg") {

			//Turns degree value to radians, i.e., the user can type in the angle in degrees using deg().
			//Can be used inside trigonometric functions.

			float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
			function_or_number = std::to_string(inputOfMathFunction / 57.2957795);
		}
	}

	//Converts the given input expression to one that the code can work with.
	void FormatInputExpression() {

		//Adds '(' and ')' to the input expression if unclosed brackets are deteccted.
		int openBracketNo = 0, closeBracketNo = 0;
		int i = 0;
		while (i < expression.size()) {
			if (expression[i] == '(')
				openBracketNo++;
			else if (expression[i] == ')')
				closeBracketNo++;
			else if (expression[i] == ' ') {
				expression.erase(i, 1); //Removes spaces from input expression.
				i--;
			}
			i++;
		}
		if (openBracketNo > closeBracketNo)
			for (int i = 0; i < openBracketNo - closeBracketNo; i++)
				expression.append(")");
		else if (closeBracketNo > openBracketNo)
			for (int i = 0; i < closeBracketNo - openBracketNo; i++)
				expression.insert(0, "(");

		/*
		Adds the '*' character when a number and a bracket are next to eachother.
		Ex: 2(2+3) will be converted to 2*(2+3).
		*/
		for (int i = 0; i < expression.size(); i++) {
			if (expression[i] == '(' && int(expression[i - 1]) >= 48 && int(expression[i - 1]) <= 57)
				expression.insert(i, "*");
			else if (expression[i] == ')' && int(expression[i + 1]) >= 48 && int(expression[i + 1]) <= 57)
				expression.insert(i + 1, "*");
		}
	}
};

enum IDs {
	InputOutputTextCtrlId = 2,
	Button0Id = 3,
	Button1Id = 4,
	Button2Id = 5,
	Button3Id = 6,
	Button4Id = 7,
	Button5Id = 8,
	Button6Id = 9,
	Button7Id = 10,
	Button8Id = 11,
	Button9Id = 12,
	OpenBracketButtonId = 13,
	CloseBracketButtonId = 14,
	MultiplyButtonId = 15,
	DivideButtonId = 16,
	AddButtonId = 17,
	SubtractButtonId = 18,
	ClearButtonId = 19,
	EqualButtonId = 20,
	TrigChoiceId = 21,
	LogChoiceId = 22,
	SinButtonId = 23,
	CosButtonId = 24,
	TanButtonId = 25,
	CotButtonId = 26,
	SecButtonId = 27,
	CscButtonId = 28,
	LogButtonId = 29,
	LnButtonId = 30,
	ExponentButtonId = 31,
	InputTextId = 32,
	DegButtonId = 33,
	DecimalButtonId = 34
};

wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Digital7");

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	AddControls();
	BindControls();
}

void MainFrame::AddControls() {
	InputOutputPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 100));
	InputOutputPanel->SetBackgroundColour(wxColour(50, 50, 50));

	ControlsPanel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel1->SetBackgroundColour(wxColour(40, 40, 40));

	ControlsPanel2 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel2->SetBackgroundColour(wxColour(40, 40, 40));

	ControlsPanel3 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel3->SetBackgroundColour(wxColour(40, 40, 40));

	ControlsPanel4 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel4->SetBackgroundColour(wxColour(40, 40, 40));

	ControlsPanel5 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel5->SetBackgroundColour(wxColour(40, 40, 40));

	ControlsPanel6 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	ControlsPanel6->SetBackgroundColour(wxColour(40, 40, 40));

	InputOutputSizer = new wxBoxSizer(wxVERTICAL);

	ControlsSizer1 = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizer2 = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizer3 = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizer4 = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizer5 = new wxBoxSizer(wxHORIZONTAL);
	ControlsSizer6 = new wxBoxSizer(wxHORIZONTAL);

	MainSizer = new wxBoxSizer(wxVERTICAL);

	MainSizer->Add(InputOutputPanel, 0, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel1, 1, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel2, 1, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel3, 1, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel4, 1, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel5, 1, wxEXPAND | wxALL, 5);
	MainSizer->Add(ControlsPanel6, 1, wxEXPAND | wxALL, 5);

	Button0 = new wxButton(ControlsPanel3, Button0Id, "0", wxDefaultPosition, wxDefaultSize);
	Button0->SetBackgroundColour(wxColour(64, 64, 64));
	Button0->SetForegroundColour(wxColour(102, 178, 255));
	Button0->SetFont(font);

	Button1 = new wxButton(ControlsPanel3, Button1Id, "1", wxDefaultPosition, wxDefaultSize);
	Button1->SetBackgroundColour(wxColour(64, 64, 64));
	Button1->SetForegroundColour(wxColour(102, 178, 255));
	Button1->SetFont(font);

	Button2 = new wxButton(ControlsPanel3, Button2Id, "2", wxDefaultPosition, wxDefaultSize);
	Button2->SetBackgroundColour(wxColour(64, 64, 64));
	Button2->SetForegroundColour(wxColour(102, 178, 255));
	Button2->SetFont(font);

	Button3 = new wxButton(ControlsPanel3, Button3Id, "3", wxDefaultPosition, wxDefaultSize);
	Button3->SetBackgroundColour(wxColour(64, 64, 64));
	Button3->SetForegroundColour(wxColour(102, 178, 255));
	Button3->SetFont(font);

	Button4 = new wxButton(ControlsPanel2, Button4Id, "4", wxDefaultPosition, wxDefaultSize);
	Button4->SetBackgroundColour(wxColour(64, 64, 64));
	Button4->SetForegroundColour(wxColour(102, 178, 255));
	Button4->SetFont(font);

	Button5 = new wxButton(ControlsPanel2, Button5Id, "5", wxDefaultPosition, wxDefaultSize);
	Button5->SetBackgroundColour(wxColour(64, 64, 64));
	Button5->SetForegroundColour(wxColour(102, 178, 255));
	Button5->SetFont(font);

	Button6 = new wxButton(ControlsPanel2, Button6Id, "6", wxDefaultPosition, wxDefaultSize);
	Button6->SetBackgroundColour(wxColour(64, 64, 64));
	Button6->SetForegroundColour(wxColour(102, 178, 255));
	Button6->SetFont(font);

	Button7 = new wxButton(ControlsPanel1, Button7Id, "7", wxDefaultPosition, wxDefaultSize);
	Button7->SetBackgroundColour(wxColour(64, 64, 64));
	Button7->SetForegroundColour(wxColour(102, 178, 255));
	Button7->SetFont(font);

	Button8 = new wxButton(ControlsPanel1, Button8Id, "8", wxDefaultPosition, wxDefaultSize);
	Button8->SetBackgroundColour(wxColour(64, 64, 64));
	Button8->SetForegroundColour(wxColour(102, 178, 255));
	Button8->SetFont(font);

	Button9 = new wxButton(ControlsPanel1, Button9Id, "9", wxDefaultPosition, wxDefaultSize);
	Button9->SetBackgroundColour(wxColour(64, 64, 64));
	Button9->SetForegroundColour(wxColour(102, 178, 255));
	Button9->SetFont(font);

	OpenBracketButton = new wxButton(ControlsPanel1, OpenBracketButtonId, "(", wxDefaultPosition, wxDefaultSize);
	OpenBracketButton->SetBackgroundColour(wxColour(64, 64, 64));
	OpenBracketButton->SetForegroundColour(wxColour(102, 178, 255));
	OpenBracketButton->SetFont(font);

	CloseBracketButton = new wxButton(ControlsPanel2, CloseBracketButtonId, ")", wxDefaultPosition, wxDefaultSize);
	CloseBracketButton->SetBackgroundColour(wxColour(64, 64, 64));
	CloseBracketButton->SetForegroundColour(wxColour(102, 178, 255));
	CloseBracketButton->SetFont(font);

	MultiplyButton = new wxButton(ControlsPanel4, MultiplyButtonId, "*", wxDefaultPosition, wxDefaultSize);
	MultiplyButton->SetBackgroundColour(wxColour(64, 64, 64));
	MultiplyButton->SetForegroundColour(wxColour(102, 178, 255));
	MultiplyButton->SetFont(font);

	DivideButton = new wxButton(ControlsPanel4, DivideButtonId, "/", wxDefaultPosition, wxDefaultSize);
	DivideButton->SetBackgroundColour(wxColour(64, 64, 64));
	DivideButton->SetForegroundColour(wxColour(102, 178, 255));
	DivideButton->SetFont(font);

	AddButton = new wxButton(ControlsPanel4, AddButtonId, "+", wxDefaultPosition, wxDefaultSize);
	AddButton->SetBackgroundColour(wxColour(64, 64, 64));
	AddButton->SetForegroundColour(wxColour(102, 178, 255));
	AddButton->SetFont(font);

	SubtractButton = new wxButton(ControlsPanel4, SubtractButtonId, "-", wxDefaultPosition, wxDefaultSize);
	SubtractButton->SetBackgroundColour(wxColour(64, 64, 64));
	SubtractButton->SetForegroundColour(wxColour(102, 178, 255));
	SubtractButton->SetFont(font);

	ClearButton = new wxButton(ControlsPanel5, ClearButtonId, "CLEAR", wxDefaultPosition, wxDefaultSize);
	ClearButton->SetBackgroundColour(wxColour(64, 64, 64));
	ClearButton->SetForegroundColour(wxColour(102, 178, 255));
	ClearButton->SetFont(font);

	EqualButton = new wxButton(ControlsPanel5, EqualButtonId, "=", wxDefaultPosition, wxDefaultSize);
	EqualButton->SetBackgroundColour(wxColour(64, 64, 64));
	EqualButton->SetForegroundColour(wxColour(102, 178, 255));
	EqualButton->SetFont(font);

	InputOutputTextCtrl = new wxTextCtrl(InputOutputPanel, InputOutputTextCtrlId, "", wxDefaultPosition, wxSize(400, 100));
	InputOutputTextCtrl->SetBackgroundColour(wxColour(70, 70, 70));
	InputOutputTextCtrl->SetForegroundColour(wxColour(102, 178, 255));
	InputOutputTextCtrl->SetFont(font);

	InputTextShow = new wxStaticText(InputOutputPanel, InputTextId, "", wxDefaultPosition, wxDefaultSize);
	InputTextShow->SetForegroundColour(wxColour(102, 178, 255));
	InputTextShow->SetFont(font);

	SinButton = new wxButton(ControlsPanel6, SinButtonId, "SIN", wxDefaultPosition, wxDefaultSize);
	SinButton->SetBackgroundColour(wxColour(64, 64, 64));
	SinButton->SetForegroundColour(wxColour(102, 178, 255));
	SinButton->SetFont(font);

	CosButton = new wxButton(ControlsPanel6, CosButtonId, "COS", wxDefaultPosition, wxDefaultSize);
	CosButton->SetBackgroundColour(wxColour(64, 64, 64));
	CosButton->SetForegroundColour(wxColour(102, 178, 255));
	CosButton->SetFont(font);

	TanButton = new wxButton(ControlsPanel6, TanButtonId, "TAN", wxDefaultPosition, wxDefaultSize);
	TanButton->SetBackgroundColour(wxColour(64, 64, 64));
	TanButton->SetForegroundColour(wxColour(102, 178, 255));
	TanButton->SetFont(font);

	CotButton = new wxButton(ControlsPanel6, CotButtonId, "COT", wxDefaultPosition, wxDefaultSize);
	CotButton->SetBackgroundColour(wxColour(64, 64, 64));
	CotButton->SetForegroundColour(wxColour(102, 178, 255));
	CotButton->SetFont(font);

	SecButton = new wxButton(ControlsPanel6, SecButtonId, "SEC", wxDefaultPosition, wxDefaultSize);
	SecButton->SetBackgroundColour(wxColour(64, 64, 64));
	SecButton->SetForegroundColour(wxColour(102, 178, 255));
	SecButton->SetFont(font);

	CscButton = new wxButton(ControlsPanel6, CscButtonId, "CSC", wxDefaultPosition, wxDefaultSize);
	CscButton->SetBackgroundColour(wxColour(64, 64, 64));
	CscButton->SetForegroundColour(wxColour(102, 178, 255));
	CscButton->SetFont(font);

	LogButton = new wxButton(ControlsPanel6, LogButtonId, "LOG", wxDefaultPosition, wxDefaultSize);
	LogButton->SetBackgroundColour(wxColour(64, 64, 64));
	LogButton->SetForegroundColour(wxColour(102, 178, 255));
	LogButton->SetFont(font);

	LnButton = new wxButton(ControlsPanel6, LnButtonId, "Ln", wxDefaultPosition, wxDefaultSize);
	LnButton->SetBackgroundColour(wxColour(64, 64, 64));
	LnButton->SetForegroundColour(wxColour(102, 178, 255));
	LnButton->SetFont(font);

	ExponentButton = new wxButton(ControlsPanel5, ExponentButtonId, "^", wxDefaultPosition, wxDefaultSize);
	ExponentButton->SetBackgroundColour(wxColour(64, 64, 64));
	ExponentButton->SetForegroundColour(wxColour(102, 178, 255));
	ExponentButton->SetFont(font);

	DegButton = new wxButton(ControlsPanel6, DegButtonId, "DEG", wxDefaultPosition, wxDefaultSize);
	DegButton->SetBackgroundColour(wxColour(64, 64, 64));
	DegButton->SetForegroundColour(wxColour(102, 178, 255));
	DegButton->SetFont(font);

	DecimalButton = new wxButton(ControlsPanel5, DecimalButtonId, ".", wxDefaultPosition, wxDefaultSize);
	DecimalButton->SetBackgroundColour(wxColour(64, 64, 64));
	DecimalButton->SetForegroundColour(wxColour(102, 178, 255));
	DecimalButton->SetFont(font);

	ControlsSizer6->Add(SinButton, 1, wxEXPAND);
	ControlsSizer6->Add(CosButton, 1, wxEXPAND);
	ControlsSizer6->Add(TanButton, 1, wxEXPAND);
	ControlsSizer6->Add(CotButton, 1, wxEXPAND);
	ControlsSizer6->Add(SecButton, 1, wxEXPAND);
	ControlsSizer6->Add(CscButton, 1, wxEXPAND);
	ControlsSizer6->Add(DegButton, 1, wxEXPAND);
	ControlsSizer6->Add(LogButton, 1, wxEXPAND);
	ControlsSizer6->Add(LnButton, 1, wxEXPAND);
	ControlsSizer5->Add(EqualButton, 1, wxEXPAND);
	ControlsSizer5->Add(ExponentButton, 1, wxEXPAND);
	ControlsSizer5->Add(DecimalButton, 1, wxEXPAND);
	ControlsSizer5->Add(ClearButton, 1, wxEXPAND);
	ControlsSizer4->Add(MultiplyButton, 1, wxEXPAND);
	ControlsSizer4->Add(DivideButton, 1, wxEXPAND);
	ControlsSizer4->Add(AddButton, 1, wxEXPAND);
	ControlsSizer4->Add(SubtractButton, 1, wxEXPAND);
	ControlsSizer3->Add(Button1, 1, wxEXPAND);
	ControlsSizer3->Add(Button2, 1, wxEXPAND);
	ControlsSizer3->Add(Button3, 1, wxEXPAND);
	ControlsSizer3->Add(Button0, 1, wxEXPAND);
	ControlsSizer2->Add(Button4, 1, wxEXPAND);
	ControlsSizer2->Add(Button5, 1, wxEXPAND);
	ControlsSizer2->Add(Button6, 1, wxEXPAND);
	ControlsSizer2->Add(CloseBracketButton, 1, wxEXPAND);
	ControlsSizer1->Add(Button7, 1, wxEXPAND);
	ControlsSizer1->Add(Button8, 1, wxEXPAND);
	ControlsSizer1->Add(Button9, 1, wxEXPAND);
	ControlsSizer1->Add(OpenBracketButton, 1, wxEXPAND);

	ControlsPanel1->SetSizerAndFit(ControlsSizer1);
	ControlsPanel2->SetSizerAndFit(ControlsSizer2);
	ControlsPanel3->SetSizerAndFit(ControlsSizer3);
	ControlsPanel4->SetSizerAndFit(ControlsSizer4);
	ControlsPanel5->SetSizerAndFit(ControlsSizer5);
	ControlsPanel6->SetSizerAndFit(ControlsSizer6);

	InputOutputSizer->Add(InputTextShow, 1, wxEXPAND);
	InputOutputSizer->Add(InputOutputTextCtrl, 1, wxEXPAND);
	InputOutputPanel->SetSizerAndFit(InputOutputSizer);

	this->SetSizerAndFit(MainSizer);
}

void MainFrame::BindControls() {
	Button0->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button1->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button3->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button4->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button5->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button6->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button7->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button8->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	Button9->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	OpenBracketButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	CloseBracketButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	MultiplyButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	DivideButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	AddButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	SubtractButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	ExponentButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	ClearButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	SinButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	CosButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	TanButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	CotButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	SecButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	CscButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	LogButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	LnButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	EqualButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	InputOutputTextCtrl->Bind(wxEVT_TEXT, &MainFrame::OnType, this);
	DegButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
	DecimalButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
}

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
	int Id = event.GetId();
	switch (Id) {
	case Button0Id:
		InputOutputTextCtrl->AppendText("0");
		break;
	case Button1Id:
		InputOutputTextCtrl->AppendText("1");
		break;
	case Button2Id:
		InputOutputTextCtrl->AppendText("2");
		break;
	case Button3Id:
		InputOutputTextCtrl->AppendText("3");
		break;
	case Button4Id:
		InputOutputTextCtrl->AppendText("4");
		break;
	case Button5Id:
		InputOutputTextCtrl->AppendText("5");
		break;
	case Button6Id:
		InputOutputTextCtrl->AppendText("6");
		break;
	case Button7Id:
		InputOutputTextCtrl->AppendText("7");
		break;
	case Button8Id:
		InputOutputTextCtrl->AppendText("8");
		break;
	case Button9Id:
		InputOutputTextCtrl->AppendText("9");
		break;
	case OpenBracketButtonId:
		InputOutputTextCtrl->AppendText("(");
		break;
	case CloseBracketButtonId:
		InputOutputTextCtrl->AppendText(")");
		break;
	case MultiplyButtonId:
		InputOutputTextCtrl->AppendText("*");
		break;
	case DivideButtonId:
		InputOutputTextCtrl->AppendText("/");
		break;
	case AddButtonId:
		InputOutputTextCtrl->AppendText("+");
		break;
	case SubtractButtonId:
		InputOutputTextCtrl->AppendText("-");
		break;
	case ExponentButtonId:
		InputOutputTextCtrl->AppendText("^");
		break;
	case ClearButtonId:
		InputOutputTextCtrl->Clear();
		break;
	case SinButtonId:
		InputOutputTextCtrl->AppendText("sin(");
		break;
	case CosButtonId:
		InputOutputTextCtrl->AppendText("cos(");
		break;
	case TanButtonId:
		InputOutputTextCtrl->AppendText("tan(");
		break;
	case CotButtonId:
		InputOutputTextCtrl->AppendText("cot(");
		break;
	case SecButtonId:
		InputOutputTextCtrl->AppendText("sec(");
		break;
	case CscButtonId:
		InputOutputTextCtrl->AppendText("csc(");
		break;
	case LogButtonId:
		InputOutputTextCtrl->AppendText("log(");
		break;
	case LnButtonId:
		InputOutputTextCtrl->AppendText("ln(");
		break;
	case EqualButtonId:
		EqualPressed = true;
		CalculateAndShowOutput();
		break;
	case DegButtonId:
		InputOutputTextCtrl->AppendText("deg(");
		break;
	case DecimalButtonId:
		InputOutputTextCtrl->AppendText(".");
		break;
	}
}

void MainFrame::OnType(wxCommandEvent& event) {
	wxTextCtrl* textCtrl = wxDynamicCast(event.GetEventObject(), wxTextCtrl);
	if (textCtrl && !(EqualPressed))
		InputTextShow->SetLabel(InputOutputTextCtrl->GetValue());
}

void MainFrame::CalculateAndShowOutput() {
	wxString WXInput = InputOutputTextCtrl->GetValue();
	MathematicalExpression InputExpression;
	InputExpression.GetInputExpression(WXInput);
	InputExpression.FormatInputExpression();
	float answer;
	try {
		answer = InputExpression.evaluate();
		int IntCast = static_cast<int>(std::round(answer));
		if (IntCast / answer == 1) {
			wxString WXOutput(std::to_string(IntCast));
			InputOutputTextCtrl->SetValue(WXOutput);
		}
		else {
			wxString WXOutput(std::to_string(answer));
			InputOutputTextCtrl->SetValue(WXOutput);
		}
	}
	catch (std::invalid_argument& e) {
		InputOutputTextCtrl->SetValue("INVALID SYNTAX");
	}
	EqualPressed = false;
}