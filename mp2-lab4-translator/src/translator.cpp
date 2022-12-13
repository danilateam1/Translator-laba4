#include "translator.h"
Translator::Translator(const string& str) {
    if (str.empty())
        throw invalid_argument("EMPTY STR");
    this->str = str;
    deleteSymbol(this->str, ' ');
    toInfix();
    toPostfix();
}

bool Translator::isDigit(char c) {
    return '0' <= c && c <= '9';
}
bool Translator::isOperation(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}
bool Translator::isMinus(char c) {
    return c == '-';
}
bool Translator::isLBracket(char c) {
    return c == '(';
}
bool Translator::isRBracket(char c) {
    return c == ')';
}
bool Translator::isPoint(char c) {
    return c == '.';
}


void Translator::toInfix() {
    State t = State::NOTHING;
    int begin = 0;
    int CountBracket = 0;
    for (int i = 0; i < str.size(); i++) {
        char c = str[i];
        switch (t) {
        case (State::NUMBER):
            if (isPoint(c) || isDigit(c))
                continue;
            infix.emplace_back(State::NUMBER, str.substr(begin, i - begin));
            if (isRBracket(c)) {
                begin = i;
                t = State::RBRACKET;
            }
            else if (isOperation(c)) {
                begin = i;
                t = State::OPERATION;
            }
            else {
                throw invalid_argument("BAD STR");
            }
            break;
        case (State::OPERATION):
            infix.emplace_back(State::OPERATION, str.substr(begin, i - begin));
            if (isLBracket(c)) {
                begin = i;
                t = State::LBRACKET;
            }
            else if (isDigit(c) || isMinus(c)) {
                begin = i;
                t = State::NUMBER;
            }
            else {
                throw invalid_argument("BAD STR");
            }
            break;
        case (State::LBRACKET):
            CountBracket++;
            infix.emplace_back(State::LBRACKET, str.substr(begin, i - begin));
            if (isLBracket(c)) {
                begin = i;
                t = State::LBRACKET;
            }
            else if (isDigit(c) || isMinus(c)) {
                begin = i;
                t = State::NUMBER;
            }
            else {
                throw invalid_argument("BAD STR");
            }
            break;
        case (State::RBRACKET):
            CountBracket--;
            infix.emplace_back(State::RBRACKET, str.substr(begin, i - begin));
            if (isRBracket(c)) {
                begin = i;
                t = State::RBRACKET;
            }
            else if (isOperation(c)) {
                begin = i;
                t = State::OPERATION;
            }
            else {
                throw invalid_argument("BAD STR");
            }
            break;
        case (State::NOTHING):
            if (isLBracket(c)) {
                begin = i;
                t = State::LBRACKET;
            }
            else if (isDigit(c) || isMinus(c)) {
                begin = i;
                t = State::NUMBER;
            }
            else {
                throw invalid_argument("BAD STR");
            }
            break;
        }
    }
    infix.emplace_back(t, str.substr(begin, str.size() - begin));
    if (t == State::RBRACKET)
        CountBracket--;
    if (t == State::LBRACKET)
        CountBracket--;

    if (CountBracket != 0)
        throw invalid_argument("ERROR BRACKETS");
    if ((t != State::RBRACKET && t != State::NUMBER)) {
        throw invalid_argument("ERROR END");
    }
}

void Translator::toPostfix()
{
    stack<pair<State, string>> st;
    pair<State, string> Elem;
    for (auto& lexem : infix) {
        switch (lexem.first) {
        case State::LBRACKET:
            st.push(lexem);
            break;
        case State::RBRACKET:
            Elem = st.top();
            st.pop();
            while (Elem.first != State::LBRACKET) {
                postfix.emplace_back(Elem);
                Elem = st.top();
                st.pop();
            }
            break;
        case State::OPERATION:
            while (!st.empty()) {
                Elem = st.top();
                st.pop();
                if (priority[lexem.second[0]] <= priority[Elem.second[0]])
                    postfix.emplace_back(Elem);
                else {
                    st.push(Elem);
                    break;
                }
            }
            st.push(lexem);
            break;
        default:
            postfix.emplace_back(lexem);
        }
    }
    while (!st.empty()) {
        Elem = st.top();
        st.pop();
        postfix.emplace_back(Elem);
    }
}

double Translator::Calculate()
{
    double left, right;
    stack<double> st;
    for (auto& lexem : postfix) {
        switch (lexem.second[0]) {
        case '+':
            right = st.top();
            st.pop();
            left = st.top();
            st.pop();
            st.push(left + right);
            break;
        case '-':
            if (lexem.first == State::NUMBER) { st.push(stod(lexem.second)); break; }
            right = st.top();
            st.pop();
            left = st.top();
            st.pop();
            st.push(left - right);
            break;
        case '*':
            right = st.top();
            st.pop();
            left = st.top();
            st.pop();
            st.push(left * right);
            break;
        case '/':
            right = st.top();
            st.pop();
            left = st.top();
            st.pop();
            st.push(left / right);
            break;
        default:
            st.push(stod(lexem.second));
        }
    }
    return st.top();
}
