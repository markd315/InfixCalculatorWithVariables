#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory.h>
#include <cstring>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <sstream>
#include <stack>
#include <unordered_map>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}//This just makes my to_string method work because it's broken on my compiler, stolen from http://stackoverflow.com/questions/12975341/to-string-is-not-a-member-of-std-says-g-mingw
using namespace std;

struct Node{
    Node* left;
    Node* right;
    double leafData;
    char op; //operator is reserved :/
    Node(){
        left=NULL;
        right=NULL; //Defaults to null pointers.
    }
};
struct RemoveDelimiter
{
  bool operator()(char c)
  {
    return (c =='\r' || c =='\t' || c == ' ' || c == '\n');
  }
};
bool isOperator(char test){
    if(test == '(' || test == ')' || test == '+'|| test == '-'|| test == '/'|| test == '*' || test == '^')
        return true;
    return false;
}
bool isOperatorNotParen(std::string &test){
    if(test == "+" ||test == "-" ||test == "*" ||test == "/" ||test == "^" ||test == "sin" ||test == "cos" ||test == "log")
        return true;
    return false;
}
int prec(std::string &token){ //precedence
    if(token == "sin" || token == "cos" || token == "log"){
        return 4;
    }
    if(token == "^"){
        return 3;
    }
    if(token == "*" || token == "/"){
        return 2;
    }
    if(token == "+" || token == "-"){
        return 1;
    }
    else return 0;
}
bool lass(std::string &token){//left associative
    if(token == "+" || token == "-" || token == "/" || token == "*" || token == "sin" || token == "cos" || token == "log"){
        return true;
    }
    return false;
}
bool rass(std::string &token){//left associative
    if(token == "^"){
        return true;
    }
    return false;
}

int getLastIndex(int start, std::string& survey){
    int i=start;
    for(; i+1<survey.size(); i++){
        if(isOperator(survey.at(i+1))){ //Break out when the next index is an operator.
            break; //Remember that length = lastIndex - startIndex +1.
        }
    }
    return i;
}
std::vector<std::string>* shuntingYard(std::vector<std::string>* tokenizedInput){
std::vector<std::string>* output = new std::vector<std::string>();
std::stack<std::string>* stack = new std::stack<std::string>();
for(int i=0; i<tokenizedInput->size(); i++){
    std::string token = tokenizedInput->at(i);
    if(isOperatorNotParen(token)){//operators
        while(stack->size()>0 && ((lass(token) && prec(token) <= prec(stack->top())) || (rass(token) && prec(token) > prec(stack->top()))))
        {
                output->push_back(stack->top());
                stack->pop(); //Gotta remove it manually.
                break;
        }
        stack->push(token);
    }
    else if(token[0] == '('){
        stack->push(token);
    }
    else if(token[0] == ')'){
        while(stack->size()>0 && stack->top() != "("){
            output->push_back(stack->top());
            stack->pop(); //Gotta remove it manually.   
        }
        stack->pop(); //Gotta remove it manually.   
    }
    else{ //token is a number
        output->push_back(token);
    }
}//end outer loop
 
while(stack->size()>0){
    output->push_back(stack->top());
    stack->pop();
}

return output;
}//end method

std::string pemdas(std::vector<std::string>* tokenized, std::unordered_map<std::string, double>& vararr){
    std::vector<std::string> tokenizedInput = *(tokenized);
    std::vector<double>* stack = new std::vector<double>();
    for(int i=0; i<tokenizedInput.size(); i++){//outer loop
    char firstChar = tokenizedInput[i].at(0);
    if((firstChar == '-' && tokenizedInput[i].size()>1) || firstChar == '.' || firstChar == '0' || firstChar == '1' || firstChar == '2' || firstChar == '3' || firstChar == '4' || firstChar == '5' || firstChar == '6' || firstChar == '7' || firstChar == '8' || firstChar == '9'){
        const char* c = tokenizedInput[i].c_str();
        stack->push_back(std::atof(c)); //if a number
    }
    else if(firstChar == '+'){
        double f2 =  stack->back();
        stack->pop_back();
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(f1+f2);
    }
    else if(firstChar == '-' && tokenizedInput.size()==1){
        double f2 =  stack->back();
        stack->pop_back();
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(f1-f2);
    }
    else if(firstChar == '*'){
        double f2 =  stack->back();
        stack->pop_back();
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(f1*f2);
    }
    else if(firstChar == '/'){
        double f2 =  stack->back();
        stack->pop_back();
        double f1 =  stack->back();
        stack->pop_back();
        if(f2 == 0){
            return "Divide-By-Zero";
        }
        stack->push_back(f1/f2);
    }
    else if(firstChar == '^'){
        double f2 =  stack->back();
        stack->pop_back();
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(pow(f1,f2));
    }
    else if(tokenizedInput[i] == "sin"){
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(sin(f1));
    }
    else if(tokenizedInput[i] == "cos"){
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(cos(f1));
    }
    else if(tokenizedInput[i] == "log"){
        double f1 =  stack->back();
        stack->pop_back();
        stack->push_back(log(f1));
    }
    else{
        if(!(vararr.find(tokenizedInput[i]) == vararr.end())){//If we can find the variable
                stack->push_back(vararr[tokenizedInput[i]]);
        }
        else
            return "Undeclared-Variable"; //Not a valid variable name.
    }

     //return stack element
}//end outer loop
return patch::to_string(stack->back());
}//end method

void delSpaces(char* source)
{
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}
char* delSpaces(const char* s){
    int numSpaces = 0;
    for(int i=0; i<strlen(s); i++){
        if(s[i] == ' '){
            numSpaces++;
        }
    }
    char* ret = new char[strlen(s)-numSpaces];
    int i=0;
    int j=0;
    while(i < strlen(ret)){
        if(s[j] == ' '){
            ret[i] = s[j];
            i++;
        }
        j++;
    }
    return ret;
    //s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );
    
    //s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
}

void lowerCase(std::string& ans){ // remember to pass by reference &
    std::transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
}
std::vector<std::string>* tokenize(std::string pre){
    std::vector<std::string>* tokens = new std::vector<std::string>();
    bool expectingConstant = true;
    for(int i=0; i<pre.size(); i++){
        //2 classes we can expect.
        if(expectingConstant){
            if(pre.at(i) == '('){ //This is an open paren.
                expectingConstant = true;
                tokens->push_back("(");
            }
            else if(i+3 <= pre.size()-1 && pre.substr(i,4) == "sin("){ //This is a function.
                tokens->push_back("sin");
                tokens->push_back("(");
                expectingConstant = true;
                i+=3;
            }
            else if(i+3 <= pre.size()-1 && pre.substr(i,4) == "cos("){ //This is a function.
                tokens->push_back("cos");
                tokens->push_back("(");
                expectingConstant = true;
                i+=3;
            }
            else if(i+3 <= pre.size()-1 && pre.substr(i,4) == "log("){ //This is a function.
                tokens->push_back("log");
                tokens->push_back("(");
                expectingConstant = true;
                i+=3;
            }
            else{ //This is either a constant or a variable, followed by a operator.
                bool negateConstant = false;
                if(pre.at(i) == '-'){
                    negateConstant = true;
                    i++; //Gotta skip over the negative.
                }
                expectingConstant = false;
                int lastIndex = getLastIndex(i, pre);
                if(negateConstant){
                    tokens->push_back("-"+pre.substr(i,lastIndex-i+1));
                }
                else{
                    tokens->push_back(pre.substr(i,lastIndex-i+1));
                }
                i=lastIndex;
            }
        }
        else if(!expectingConstant){
            if(pre.at(i) == ')'){
                expectingConstant = false;
                tokens->push_back(")");
            }
            else{ //+-*/^
                expectingConstant = true;
                std::stringstream ss;
                std::string s;
                ss << pre.at(i);
                ss >> s;
                tokens->push_back(s);
            }
        }
    }
    return tokens;
}
char* convert(const std::string& str) {
    char* result = new char[str.length()+1];
    strcpy(result,str.c_str());
    return result;
}
int main(){
    std::string lineInput;
    std::unordered_map<std::string, double> map;
    map["e"] = 2.718;
    map["pi"] = 3.14169;
    do{
        std::getline(std::cin, lineInput);
        lowerCase(lineInput);
        lineInput.erase( std::remove_if( lineInput.begin(), lineInput.end(), RemoveDelimiter()), lineInput.end());
        if(lineInput.substr(0,3) == "let"){ //"let[var]="
            int equalsIndex = lineInput.find("=");
            std::string varName = lineInput.substr(3,equalsIndex-3); //trust but verify
            lineInput = lineInput.substr(equalsIndex+1);
            std::vector<std::string>* tokenizedInput = tokenize(lineInput);
            std::string result = pemdas(shuntingYard(tokenizedInput), map);
            if(result == "Divide-By-Zero" || result == "Undeclared-Variable"){
                cout<<result<<endl;
            }
            else{
            double numResult = std::atof(result.c_str());
            map[varName] = numResult;
        }
        }
        else{
            std::vector<std::string>* tokenizedInput = tokenize(lineInput);
            std::string result = pemdas(shuntingYard(tokenizedInput), map);
            cout<<result<<endl;
        }
    }
    while(lineInput!="quit");
}
//std::vector<std::string>* tokens = tokenize("6+sin(180/2)*3"); //=9
//