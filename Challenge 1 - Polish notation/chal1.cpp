#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <iomanip>
using namespace std;

struct Prioritize
{
	Prioritize()
	{
		this->OpeLeft.assign(3, 0);
	}
	Prioritize(int _prioritize, int _Power, int _Multi, int _Sum) : prioritize(_prioritize)
	{
		OpeLeft.push_back(_Sum);
		OpeLeft.push_back(_Multi);
		OpeLeft.push_back(_Power);
	}
	~Prioritize() {};

	// Decrease operator has current prioritize by one
	void pop()
	{
		OpeLeft[prioritize]--;
	}
	// Increase operator pri prioritize by one
	void push(int pri)
	{
		if (pri > this->prioritize) this->prioritize = pri;
		OpeLeft[pri]++;
	}
	// Check if there is no operator has current prioritize left
	bool isEmpty()
	{
		return (OpeLeft[prioritize] == 0);
	}

	// Prioritize
	int prioritize = 0;
	// Total operators
	vector<int> OpeLeft ;
};

#pragma region Checking function
// Check if x is an Operator
bool isOperator(char x)
{
	return ((x == '+') || (x == '-') || (x == '*') || (x == '/') || (x == '^'));
}

// Check bracket
bool isBeginBracket(char x)
{
	if (x == '{' || x == '[' || x == '(' ) return true;
	else return false;
}
bool isEndBracket(char x)
{
	if (x == '}' || x == ']' || x == ')' || x== '\n') return true;
	else return false;
}

// Check if begin bracket and end bracket are in the same type
bool isSmallerBracket(char begin, char next = NULL)
{
	switch (begin)
	{
	case NULL: return true;
	case '{': return ((next == '{') || (next == '['));
	case '[': return (next == '(');
	case '(': return false;
	}
}

// Check if start bracket and end bracket are in the same type 
bool isMatchingParen(char start, char end)
{
	switch (start)
	{
	case '{': return (end == '}');
	case '[': return (end == ']');
	case '(': return (end == ')');
	case NULL: return (end == '\n');
	default: return false;
	}
}
#pragma endregion

#pragma region Get function
// Get the end bracket char that matching the begin bracket
char getEndBracket(char begin)
{
	switch (begin)
	{
	case NULL: return '\n';
	case '{': return '}';
	case '[': return ']';
	case '(': return ')';
	}
}
#pragma endregion

#pragma region Calculate
// Check if Expression is correct
bool check(stringstream& sin, char begin = NULL)
{
	int Number = 0;
	int Operator = 0;
	bool flagSmaller = (begin == '(' || begin == NULL) ? true : false;
	double dkeep;
	char ckeep;

	while (!isEndBracket(sin.peek()))
	{	
		if (sin.peek() == ' ') sin.ignore(1);
		// Begin new bracket and check if it correct
		if (isBeginBracket(sin.peek()))
			if (isSmallerBracket(begin, sin.peek()))
			{
				flagSmaller = true;

				if (!check(sin, sin.get())) return false;

				Number++;

				continue;
			}
			else return false;	

		// check if it 's not Operator or number
		if (isOperator(sin.peek()))
		{
			sin >> ckeep;
			Operator++;
		}
		else if (isalnum(sin.peek()))
		{
			sin >> dkeep;
			Number++;
		}
		else return false;
		
		// Check if Number always larger than Operator by 1
		if ((Number - Operator > 1) || (Number - Operator < 0) ) return false;
	}

	// Check i
	if (Number - Operator != 1) return false;

	if (!isMatchingParen(begin, sin.get())) return false;
	return flagSmaller;
}

// Return operator's prioritize
int priCal(char _operator)
{
	if (_operator == '^') return 2;
	if (_operator == '*' || _operator == '/') return 1;
	else return 0;
}

// Calculate the Expression
long double exCal(vector<long double> number, string& _operator, Prioritize& prioritize)
{
	while (number.size() != 1)
	{
		int i, offset;
		if (prioritize.prioritize == 2)
		{
			i = _operator.length() - 1;
			offset = -1;
		}
		else
		{
			i = 0;
			offset = 1;
		}


		for (i; !prioritize.isEmpty() ; i += offset)
		{
			if (priCal(_operator[i]) == prioritize.prioritize)
			{
				switch (_operator[i])
				{
				case '^': 
					number[i] = pow(number[i], number[i + 1]); 
					if (isnan(number[i])) return NAN;
					break;
				case '/': 
					if (number[i + 1] == 0) return NAN;
					number[i] = number[i] / number[i + 1];
					break;

				case '*': number[i] = number[i] * number[i + 1]; break;
				case '+': number[i] = number[i] + number[i + 1]; break;
				case '-': number[i] = number[i] - number[i + 1]; break;
				}
				number.erase(number.begin() + i + 1);
				_operator.erase(_operator.begin() + i);
				prioritize.pop();

				if (prioritize.prioritize != 2) i--;

				if (prioritize.isEmpty())
				{
					while (prioritize.isEmpty() && prioritize.prioritize > 0) prioritize.prioritize--;
					break;
				}
			}
		}
	}
	return number[0];
}

// Read expression function
long double calBracket(stringstream& sin, char Brack = NULL)
{
	vector<long double> number;
	string _operator;
	Prioritize prioritize;

	// Checking number and Operator
	while (!isMatchingParen(Brack, sin.peek()))
	{
		if (sin.peek() == ' ') sin.ignore(1);

		if (number.size() == _operator.size())
		{		
			if (isBeginBracket(sin.peek()))
			{
				number.push_back(calBracket(sin, sin.get()));
				if (isnan(number.back())) return NAN;
			}
			else
			{
				long double num;
				sin >> num;
				number.push_back(num);
			}
		}
		else
		{
			_operator.push_back(sin.get());
			prioritize.push(priCal(_operator.back()));
		}
	}

	// Calculating at end
	sin.ignore(1); // ignore end bracket
	return exCal(number, _operator, prioritize);
}

// Read from file and write to file
void calculating(string in, string out)
{
	ifstream fin(in, ios::in);
	ofstream fout(out, ios::out);

	string Expression;
	while (getline(fin,Expression))
	{
		long double result;

		Expression.push_back('\n');
		stringstream checking(Expression, ios::in);
		if (check(checking))
		{
			stringstream sin(Expression, ios::in);

			result = calBracket(sin);
		}
		else result = NAN;

		if (isnan(result)) fout << "E";
		else fout  << setprecision(int(log10(abs(result)) + 1) + 2) << result;

		if (!fin.eof()) fout << endl;
		Expression.clear();
	}

	fin.close();
	fout.close();
}
#pragma endregion

#pragma region Convert
bool IsOperand(char c) {
	return ('0' <= c && c <= '9');
}

//NOTE: +,-,x,/ -> left to right (ex: A*B*C = (AB*) * C = AB*C*)
//		^ -> right to left (ex: 2 ^ 3 ^ 2 = 2^(3 2 ^) = 2 3 2 ^ ^)
//								  |   | 		
//								 2nd 1st

//Default: IsHigherpriCaledence(c1,c2) = true -> c1 has higher precedence than c2
bool IsHigherPrecedence(char c1, char c2) {
	if (priCal(c1) > priCal(c2))
		return true;

	//if c1, c2 have the same precedence, check for special case (^: right to left)
	else if (priCal(c1) == priCal(c2)) {
		if (c1 != '^')
			return true;
		return false;
	}

	return false; //else: Prec(c1) < Prec(c2)
}

string InfixToPostfix(string s) {

	string postfix = "";
	//from here s is a validated infix expression

	stack<char> st;

	for (int i = 0; i < s.length(); i++) {
		//if the scanned character is an operand or a SIGN (which decide operand <0 or >0)
		while (IsOperand(s[i]) || s[i] == ' ' || s[i] == '.') {
			//Dealing with space:
			//if the current last elem of postfix is a space & so does the inserting elem -> no need to insert another space
			if (!postfix.empty() && postfix.back() == ' ' && s[i] == ' ') {
				i++;
				continue;
			}

			postfix += s[i];
			i++;
		}

		//If the scanned character is an operator (s[i-1] & s[i+1] to make sure that this is OPERATOR, not SIGN (ex: "-2" != " - 2"))
		if (isOperator(s[i]) && s[i - 1] == ' ' && s[i + 1] == ' ') {
			//if the scanned character has lower precedence than st.top -> append st.top to postfix
			while (!st.empty() && !isBeginBracket(st.top()) && IsHigherPrecedence(st.top(), s[i])) {
				postfix = postfix + (postfix.back() == ' ' ? "" : " ") + st.top();
				st.pop();
			}

			//now st.top has lower prec than scanned operator -> push the current operator into stack
			st.push(s[i]);
		}

		//if the scanned character is an opening bracket, just push to stack ('cause in postfix, we dont use brackets)
		else if (isBeginBracket(s[i]))
			st.push(s[i]);

		//if the scanned character is a closing bracket, append all the operators left in stack b/w open-close brackets into postfix
		else if (isEndBracket(s[i])) {
			while (!st.empty() && !isBeginBracket(st.top())) {
				postfix = postfix + (postfix.back() == ' ' ? "" : " ") + st.top();
				st.pop();
			}
			st.pop(); //truly remove opening bracket in stack
		}
	}

	//if we get to the end of array but stack is not empty -> append all the operators left to postfix
	while (!st.empty()) {
		postfix = postfix + (postfix.back() == ' ' ? "" : " ") + st.top();
		st.pop();
	}

	return postfix;
}

void convert(string in, string out)
{
	ifstream fin(in, ios::in);
	ofstream fout(out, ios::out);


	string Expression;
	while (getline(fin, Expression))
	{
		long double result;

		// Check valid expression
		Expression.push_back('\n');
		stringstream checking(Expression, ios::in);
		if (check(checking))
		{
			Expression.pop_back();
			fout << InfixToPostfix(Expression);
		}
		else fout << "E";

		if (!fin.eof()) fout << endl;

		Expression.clear();
	}

	fin.close();
	fout.close();
}
#pragma endregion

// Main and Command line
int main(int argc, char* argv[])
{	
	// Get infomation from cmd
	string in = argv[1];
	int totalEx = atoi(argv[2]);
	string action = argv[3];
	string out = argv[4];

	// Handle info
	switch (action[1])
	{
	case 'c':
		calculating(in, out);
		break;
	case 't':
		convert(in, out);
		break;
	default:
		break;
	}
}