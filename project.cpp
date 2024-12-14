#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

class node {
public:
    node* left;
    node* right;
    double value;
    bool io;
    char op;

    node(double a, bool IO, char c) {
        left = nullptr;
        right = nullptr;
        value = a;
        io = IO;
        op = c;
    }

};

class tree {
private:
    stack<char> op;
    stack<node*> operant;

public:

    void printBothStacks() {
        cout << endl << endl;
        displayOPALL();
        displayOPTALL();
    }

    void displayOPALL() {
        while (!op.empty()) {
            cout << op.top() << " ";
            op.pop();
        }
        cout << endl;
    }

    void displayOPTALL() {
        while (!operant.empty()) {
            cout << operant.top()->value << " ";
            operant.pop();
        }
        cout << endl;
    }

    void newNodePushOpT(double& x) {
        node* temp;
        temp = new node(x, false, '#');
        operant.push(temp);
    }

    node* newNodePushOp(char& x) {
        node* temp;
        temp = new node(0, true, x);
        return temp;
    }

    void opFound(char x) {

        //IF EMPTY
        if (op.empty()) {
            op.push(x);
        }
        //IF (
        else if (x == '(') {
            //PUSH ALL
            if (op.top() == '-' || op.top() == '+' || op.top() == '/' || op.top() == '*' || op.top() == '(') {
                op.push(x);
            }
        }
        //IF ) 
        else if (x == ')') {
            //special case
            popTilPar(x);
            //^^^THIS IS EXIT() IF ITS BAD
        }
        //IF +
        else if (x == '+') {
            //CORRECT LEVELS? PUSH!
            if (op.top() == '-' || op.top() == '+' || op.top() == '(') {
                op.push(x);
            }
            //INCORRECT LEVELS? POP AND TREE SOME STUFF
            else if (op.top() == '/' || op.top() == '*') {
                // the levels are right so we do some tree non-sense
                badLevel(x);
                //THIS WILL EXIT IF BAD
            }
        }
        //IF -
        else if (x == '-') {
            //CORRECT LEVELS? PUSH!
            if (op.top() == '-' || op.top() == '+' || op.top() == '(') {
                op.push(x);
            }
            //NCORRECT LEVELS? POP AND TREE SOME STUFF
            else if (op.top() == '/' || op.top() == '*') {
                // the levels are right so we do some tree non-sense
                badLevel(x);
                //THIS WILL EXIT IF BAD
            }
        }
        else if (x == '*') {
            //CORRECT LEVELS? PUSH! (ACTUALLY THEY ARE ALL CORRECT LEVELS)
            if (op.top() == '-' || op.top() == '+' || op.top() == '(' || op.top() == '/' || op.top() == '*') {
                op.push(x);
            }

        }
        else if (x == '/') {
            //CORRECT LEVELS? PUSH! (ACTUALLY THEY ARE ALL CORRECT LEVELS)
            if (op.top() == '-' || op.top() == '+' || op.top() == '(' || op.top() == '/' || op.top() == '*') {
                op.push(x);
            }
        }
    }

    void badLevel(char& c) {
        if (op.empty()) {
            cout << "Insufficient operators. \n";
            exit(0);
        }
        else {
            char x = op.top();
            op.pop();
            if (operant.empty()) {
                cout << "Error: Insufficient operands for operator. 3. \n";
                exit(0);
            }
            node* temp = newNodePushOp(x);
            temp->right = operant.top();
            operant.pop();
            if (operant.empty()) {
                cout << "Error: Insufficient operands for operator. 4. \n";
                exit(0);
            }
            temp->left = operant.top();
            operant.pop();
            operant.push(temp);
            op.push(c);
        }
    }

    void popTilPar(char& x) {
        if (op.empty()) {
            cout << "There has been an error. A ')' cannot exist without its matching '('.\n";
            exit(0);
        }
        else {
            while (op.top() != '(') {
                x = op.top();
                op.pop();
                if (operant.empty()) {
                    cout << "Error: Insufficient operands for operator. 1. \n";
                    exit(0);
                }
                node* temp = newNodePushOp(x);
                temp->right = operant.top();
                operant.pop();
                if (operant.empty()) {
                    cout << "Error: Insufficient operands for operator. 2. \n";
                    exit(0);
                }
                temp->left = operant.top();
                operant.pop();
                operant.push(temp);
            }

            if (!op.empty()) // Check if op stack is not empty before popping
                op.pop(); // Remove the matching '(' from the operator stack
            else {
                cout << "Error: Missing opening parenthesis '('.\n";
                exit(0);
            }
        }
    }

    void checkEnd() {

        if (!op.empty() && op.top() == '(') {
            cout << "Error: There is an extra '(. \n";
            exit(0);
        }
        else if (!op.empty() && (op.top() == '+' || op.top() == '-' || op.top() == '*' || op.top() == '/')) {
            while (!op.empty() && operant.size() != 1) {
                char x = op.top();
                op.pop();
                node* temp = newNodePushOp(x);
                temp->right = operant.top();
                operant.pop();
                temp->left = operant.top();
                operant.pop();
                operant.push(temp);
            }
        }
        else if (!op.empty() && operant.size() == 1) {
            cout << "Tree was unsuccessful. Due to extra operators. \n";
            exit(0);
        }

        if (op.empty() && operant.size() == 1) {
            cout << "Tree was successful. \n\n";
        }
        else {
            cout << "Tree was unsuccessful. Due to unknown error. \n";
        }

    }

    void clearStacks() {
        while (!op.empty()) {
            op.pop();
        }
        while (!operant.empty()) {
            operant.pop();
        }
    }

    node* getRoot() {
        return operant.top();
    }

    void displayRoot(node* root) {
        //cout << "Root: Left, Right, Double Value, isOp, Op: " << root->left->right->value << ", " << root->right->right->value << ", " << root->value << ", " << root->io << ", " << root->op << endl;
    }

    void postOrder(node* root)
    {
        if (root)
        {
            postOrder(root->left);
            postOrder(root->right);
            if (root->io == true) {
                cout << root->op << " ";
            }
            else if (root->io == false) {
                cout << root->value << " ";
            }
        }
    }

    void inOrder(node* root)
    {
        if (root)
        {
            if (root->io == true) {
                cout << "(";
            }

            inOrder(root->left);

            if (root->io == true) {
                cout << root->op << " ";
            }
            else if (root->io == false) {
                cout << root->value << " ";
            }

            inOrder(root->right);

            if (root->io == true) {
                cout << ")";
            }
        }
    }

    void preOrder(node* root)
    {
        if (root)
        {
            if (root->io == true) {
                cout << root->op << " ";
            }
            else if (root->io == false) {
                cout << root->value << " ";
            }

            preOrder(root->left);
            preOrder(root->right);
        }
    }

    double evaluateExpressionTree(node* root) {
        if (root == NULL) {
            return 0; // Return 0 if the tree is empty
        }

        // If the node is a leaf (operand), return its value
        if (!root->io) {
            return root->value;
        }

        // Recursively evaluate left and right subtrees
        double leftValue = evaluateExpressionTree(root->left);
        double rightValue = evaluateExpressionTree(root->right);

        // Apply the operator and return the result
        switch (root->op) {
        case '+':
            return leftValue + rightValue;
        case '-':
            return leftValue - rightValue;
        case '*':
            return leftValue * rightValue;
        case '/':
            if (rightValue == 0) {
                cout << "Error: Division by zero!\n";
                return 0; // Return 0 in case of division by zero
            }
            return leftValue / rightValue;
        default:
            cout << "Error: Invalid operator!\n";
            return 0; // Return 0 in case of invalid operator
        }
    }

};

bool balanced(string expr) {
    // Declare a stack to hold the previous brackets.
    stack<char> temp;
    for (int i = 0; i < expr.length(); i++) {
        if (expr[i] == '(') {
            temp.push(expr[i]);
        }
        else if (expr[i] == ')') {
            if (temp.empty()) {
                return false;
            }
            else {
                temp.pop();
            }
        }
    }
    if (temp.empty()) {
        // If stack is empty return true
        return true;
    }
    return false;
}

bool isValidExpression(const string expression) {
    istringstream iss(expression);
    char prev = ' '; // Initialize prev with a space character

    while (iss) {
        char current;
        iss >> current;

        if (isspace(current)) {
            // Skip whitespace
            continue;
        }
        else if (isdigit(current)) {
            // If current is a digit or a decimal point, prev should not be an operator
            if (prev == ')') {
                return false;
            }
        }
        else if (current == '.') {
            // If current is a digit or a decimal point, prev should not be an operator
            if (prev == '+' || prev == '-' || prev == '*' || prev == '/' || prev == '(' || prev == ')') {
                return false;
            }
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            // If current is an operator, prev should be a digit, a decimal point, or closing parenthesis
            if (!(isdigit(prev) || prev == '.' || prev == ')')) {
                return false;
            }
        }
        else if (current == '(') {
            // If current is an opening parenthesis, prev should be an operator or whitespace
            if (prev == '.') {
                return false;
            }
        }
        else if (current == ')') {
            // If current is a closing parenthesis, prev should be a digit, a decimal point, or closing parenthesis
            if (prev == '.' || prev == '-' || prev == '+' || prev == '/' || prev == '*') {
                return false;
            }
        }
        else {
            // Invalid character
            return false;
        }

        prev = current;
    }

    // If the loop completes without encountering any issues, the expression is valid
    return true;
}

int main()
{
    string str;
    tree tree;
    ifstream input;
    input.open("Text.txt");

    while (!input.eof()) {
        getline(input, str);

        if (balanced(str) && isValidExpression(str)) {
            for (int i = 0; i < str.length(); i++) {
                if (str[i] == ' ') {
                }
                else if (str[i] == '(' || str[i] == '+' || str[i] == '-' || str[i] == ')' || str[i] == '*' || str[i] == '/') {
                    tree.opFound(str[i]);
                }
                else if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {

                    //LOOKS FOR ENTIRE DOUBLE
                    string numStr = "";
                    //LOOKS FOR FULL DECIMAL AS CHAR
                    while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
                        numStr += str[i];
                        i++;
                    }
                    i--;
                    //TURNS CHARS INTO DOUBLE
                    double x = stod(numStr);
                    //MAKES NODE AND PUSHES
                    tree.newNodePushOpT(x);
                }
            }

            tree.checkEnd();

        }
        else {
            cout << "Tree was unsuccessful. Due to uneven () or invalid syntax. :(\n";
            exit(0);
        }
    }

    node* root = tree.getRoot();

    cout << "Post Order: ";
    tree.postOrder(root);
    cout << endl << endl;

    cout << "In Order: ";
    tree.inOrder(root);
    cout << endl << endl;

    cout << "Pre Order: ";
    tree.preOrder(root);
    cout << endl << endl;

    cout << "Result: " << tree.evaluateExpressionTree(root) << endl << endl;

    input.close();
    return -1;
}