#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <bits/stdc++.h>
#include <conio.h>

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
class MathematicalExpression{
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
    void GetInputExpression(){
        std::cout << "Type in the expression: ";
        std::cin >> expression;
    }

//Evaluates expression according to PEMDAS.
    float evaluate(){

//Evaluates expressions in parentheses.
        int j = 0;
        while(j < expression.size()){         
            if(expression[j] == '('){
                GetLeftAndRightBracketPos(j);
                EvaluateParentheses(leftBracketPos, rightBracketPos); 
            }
            else
                j++;
        }

//Evaluates exponents.
        j = 0;
        while(j < expression.size()){
            if(expression[j] == '^'){
                std::string result = std::to_string(EvaluateExponent(j));
                UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
            }
            else
                j++;
        }

//evaluates multiplication and division.
        j = 0;
        while(j < expression.size()){
            if(expression[j] == '*'){
                std::string result = std::to_string(PerformMultiplication(j));
                UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
            }
            else if(expression[j] == '/'){
                std::string result = std::to_string(PerformDivision(j));
                UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
            }
            else if(expression[j] == '%'){
                std::string result = std::to_string(PerformModulus(j));
                UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
            }
            else
                j++;
        }


//Evaluates addition and subtraction.
        j = 0;
        while(j < expression.size()){
            if(expression[j] == '+'){
                std::string result = std::to_string(PerformAddition(j));
                UpdateExpression(result, leftNumberPos + 1, rightNumberPos - 1);
            }

//The below else clause will only fire, if the character to the left of the '-' is a number.
//The need for this condition arised, as sin-1 will not work if the condition is not written, sin here can be any math function.
//The 'left number' will be 'sin' causing bugs in the program.
            else if(expression[j] == '-' && int(expression[j-1]) >= 48 && int(expression[j-1]) <= 57){
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
    void GetLeftAndRightBracketPos(int &j){
        leftBracketPos = j;
        int unclosedBracketNo = 1; //Number of brackets that remain unlcosed.
        for(int i = j+1; i < expression.size(); i++){
            if(expression[i] == '(')
                unclosedBracketNo++; //If new brackets are opened, then unclosedBracketNo increases.
            else if(expression[i] == ')' && unclosedBracketNo == 1){

//If a bracket is closed, and the number of unclosed brackets is 1 then that must be the end of the expression in parentheses.
//Hence the position of the right bracket is the point at which the bracket is closed.

                rightBracketPos = i; 
                break;
            }
            else if(expression[i] == ')')
                unclosedBracketNo--; //If new brackets are closed, then unclosedBracketNo decreases.
        }

    }

    void GetRightAndLeftNumber(int &j){

//leftNumberPos is initialized to a point to the left of an operand.
//rightNumberPos is initialized to a point to the right of an operand.

        leftNumberPos = j-1, rightNumberPos = j+1;
        while((int(expression[leftNumberPos]) >= 48 && int(expression[leftNumberPos]) <= 57) || expression[leftNumberPos] == '.' || (int(expression[leftNumberPos]) >= 97 && int(expression[leftNumberPos]) <= 122) 
        || leftNumberPos == 0 || (int(expression[leftNumberPos-1]) >= 97 && int(expression[leftNumberPos-1]) <= 122)) 

//The above 'leftNumberPos == 0' condition is written to catch the '-' at the start of an expression, Ex: -23+1, -41.2+21.
//The above 'int(expression[leftNumberPos-1]) >= 97 && int(expression[leftNumberPos-1]) <= 122' is written to catch math functions before the '-',
//Ex: sin-23.4, cos-32.45.

            leftNumberPos--; //If it is not a number or a decimal point, the position is decreased.
        while((int(expression[rightNumberPos]) >= 48 && int(expression[rightNumberPos]) <= 57) || expression[rightNumberPos] == '.' || (int(expression[rightNumberPos]) >= 97 && int(expression[rightNumberPos]) <= 122))
            rightNumberPos++; //If it is not a number or a decimal point, the position is increased.

//At this point leftNumberPos and rightNumberPos point to the element to the left and to the right of the numbers respectively.
//Hence the weird arguments for substr().

        leftNumber = expression.substr(leftNumberPos+1, j-leftNumberPos-1);
        rightNumber = expression.substr(j+1, rightNumberPos-j-1);
        FindIfMathFunction(leftNumber); //Finds if left number is a mathematical function, Ex: sin, cos.
        FindIfMathFunction(rightNumber); ////Finds if right number is a mathematical function, Ex: sin, cos.
    }

//Performs multiplication using the number to the left of '*' and the number to the right of '*'.
//stof() takes a string and converts it into float.
    float PerformMultiplication(int &j){
        GetRightAndLeftNumber(j);
        float multipliedResult = stof(leftNumber) * stof(rightNumber);
        return multipliedResult;
    }

//Performs division using the number to the left of '/' and the number to the right of '/'.
//stof() takes a string and converts it into float.
    float PerformDivision(int &j){
        GetRightAndLeftNumber(j);
        float dividedResult = stof(leftNumber) / stof(rightNumber);
        return dividedResult;
    }

//Performs addition using the number to the left of '+' and the number to the right of '+'.
//stof() takes a string and converts it into float.
    float PerformAddition(int &j){
        GetRightAndLeftNumber(j);
        float addedResult = stof(leftNumber) + stof(rightNumber);
        return addedResult;
    }

//Performs subtraction using the number to the left of '-' and the number to the right of '-'.
//stof() takes a string and converts it into float.
    float PerformSubtraction(int &j){
        GetRightAndLeftNumber(j);
        float subtractedResult = stof(leftNumber) - stof(rightNumber);
        return subtractedResult;
    }

//Performs modulus using the number to the left of '%' and the number to the right of '%'.
//stof() takes a string and converts it into float.  
    float PerformModulus(int &j){
        GetRightAndLeftNumber(j);
        float modulusResult = stoi(leftNumber) % stoi(rightNumber);
        return modulusResult;
    }

//Evaluates the expression inside parentheses.
    void EvaluateParentheses(int &openBracketPos, int &closeBracketPos){

//Creates an object of the same class, to reuse the functions defined here.
//This is because the expression inside parentheses is a regular numerical expression, which follows PEMDAS.

        MathematicalExpression* parentheticExpression = new MathematicalExpression;

//The below substring is ONLY the expression INSIDE the parentheses and does not include the parentheses themselves. 

        parentheticExpression->expression = expression.substr(openBracketPos+1, (closeBracketPos-1) - (openBracketPos+1) + 1);
        parentheticValue = std::to_string(parentheticExpression->evaluate());
        parentheticValue = std::to_string(parentheticExpression->evaluate());
        UpdateExpression(parentheticValue, leftBracketPos, rightBracketPos);
        delete parentheticExpression;
    }

//Evaluates exponents using the number to the left of '^' and the number to the right of '^'.
//stof() takes a string and converts it into float.
    float EvaluateExponent(int &j){
        GetRightAndLeftNumber(j);
        float exponentResult = pow(stof(leftNumber), stof(rightNumber));
        return exponentResult;
    }

//Replaces the input expression with evaluated result for further calculation.
    void UpdateExpression(std::string &result, int start, int end){
        int noOfCharacters = 0;
        for(int i = start; i <= end; i++)
                noOfCharacters++; //Counts the number of characters to replace.
        expression.erase(start, noOfCharacters); //Erases the unevaluated part of the expression.
        expression.insert(start, result); //Inserts the evaluated part into the expression.
    }

    void FindIfMathFunction(std::string &function_or_number){

//The following if clauses check if the input string is a mathematical function or just a number.
//If it is one of the following functions, it replaces the value of the input string with the evaluated value of the mathematical function.

        if(function_or_number.substr(0, 2) == "ln"){
            float inputOfMathFunction = stof(function_or_number.substr(2, function_or_number.size() - 2));
            function_or_number = std::to_string(log(inputOfMathFunction));
        }
        if(function_or_number.substr(0, 3) == "log"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(log10(inputOfMathFunction));
        }
        if(function_or_number.substr(0, 3) == "sin"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(std::sin(inputOfMathFunction));
        }
        if(function_or_number.substr(0, 3) == "cos"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(std::cos(inputOfMathFunction));
        }
        if(function_or_number.substr(0, 3) == "tan"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(std::tan(inputOfMathFunction));
        }
        if(function_or_number.substr(0, 3) == "cot"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(1 / (std::tan(inputOfMathFunction)));
        }
        if(function_or_number.substr(0, 3) == "sec"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(1 / (std::cos(inputOfMathFunction)));
        }
        if(function_or_number.substr(0, 3) == "csc"){
            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(1 / (std::sin(inputOfMathFunction)));
        }
        if(function_or_number.substr(0, 3) == "deg"){ 
            
//Turns degree value to radians, i.e., the user can type in the angle in degrees using deg().
//Can be used inside trigonometric functions.

            float inputOfMathFunction = stof(function_or_number.substr(3, function_or_number.size() - 3));
            function_or_number = std::to_string(inputOfMathFunction / 57.2957795);
        }
    }
};

//Main driver code.
int main(){
    MathematicalExpression* inputExpression = new MathematicalExpression;

    inputExpression->GetInputExpression();

    float answer = inputExpression->evaluate();

    std::cout << "The answer is: " << answer;

    delete inputExpression;

    return 0;
}