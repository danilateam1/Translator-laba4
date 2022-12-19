#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
using namespace std;
class Translator {
    map<char, int> priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
    enum class State {
        NUMBER,
        OPERATION,
        LBRACKET,
        RBRACKET,
        NOTHING,
    };
    string str;
    vector<pair<State, string>> infix;
    vector<pair<State, string>> postfix;
    bool isDigit(char c);
    bool isOperation(char c);
    bool isMinus(char c);
    bool isPoint(char c);
    bool isLBracket(char c);
    bool isRBracket(char c);
    void deleteSymbol(string& str, char symbol) {
        int count = 0;
        for (int i = 0; i < str.size(); i++) {
            str[i - count] = str[i];
            if (str[i] == symbol)
                count++;
        }
        str.erase(str.size() - count, count);
    }
    void toInfix();
    void toPostfix();
public:
    Translator(const string& str);
    string getInfix() const { return str; }
    string getPostfix() const {
        string postfixStr;
        for (auto& p : postfix) {
            postfixStr += p.second;
        }
        return postfixStr;
    }
    double Calculate();
};