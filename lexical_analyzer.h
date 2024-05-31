#include <iostream>
#include <string>
#include <vector>
using namespace std;


bool is_letter(char c){
    return (c > 64 && c < 91) || (c > 96 && c < 123) || c == '_'; 
}

bool is_digit(char c){
    return c > 47 && c < 58;
}

bool is_white_space(char c){
    return c == ' ' || c == '\n' || c == '\t';
}

int is_identifier(string code, int start){
    int index = start, state = 0;
    int last_accept = 0;
    while(index < code.length()){
        switch(state){
            case 0:
                if(is_letter(code[index])) state = 1;
                else if(is_digit(code[index])) state = 2;
                else state = -1;
                break;
            case 1:
                if(is_letter(code[index])) state = 3;
                else if(is_digit(code[index])) state = 4;
                else state = -1;
                break;
            case 2:
                state = -1;
                break;
            case 3:
                if(is_letter(code[index])) state = 3;
                else if(is_digit(code[index])) state = 4;
                else state = -1;
                break;
            case 4:
                if(is_letter(code[index])) state = 3;
                else if(is_digit(code[index])) state = 4;
                else state = -1;
                break;
            case -1:
                return last_accept;
                break;
        }
        index++;
        if(state == 1 || state == 3 || state == 4){
            last_accept = index - start;
        }
    }
    return last_accept;
}

int is_number(string code, int start){
    int index = start, state = 0;
    int last_accept = 0;
    while(index < code.length()){
        switch(state){
            case 0:
                if(is_digit(code[index])) state = 1;
                else state = -1;
                break;
            case 1:
                if(is_digit(code[index])) state = 1;
                else state = -1;
                break;
            case -1:
                return last_accept;
        }
        index++;
        if(state == 1){
            last_accept = index - start;
        }
    }
    return last_accept;
}


int is_reserved_word(string code, int start){
    string reserved_words[14] = {"int", "float", "return", "if", "while", "cin", "cout", "continue", "break", "#include", "using", "namespace", "std", "main"};
    int max_length = 0;
    for(int i = 0; i < 14; i++){
        string reserved_word = reserved_words[i];
        int length = reserved_word.length();
        if(code.substr(start, length) == reserved_word){
            if(max_length < length){
                max_length = length;
            }
        }
    }
    return max_length;
}

int is_symbol(string code, int start){
    string symbols[23] = {"(", ")", "{", "}", "[", "]", ",", ";", "+", "-", "*", "/", "==", "!=", ">", ">=", "<", "<=", "=", "&&", "||", "<<", ">>"};
    int max_length = 0;
    for(int i = 0; i < 23; i++){
        string symbol = symbols[i];
        int length = symbol.length();
        if(code.substr(start, length) == symbol){
            if(max_length < length){
                max_length = length;
            }
        }
    }
    return max_length;
}

int is_string(string code, int start){
    int index = start, state = 0;
    int last_accept = 0;
    while(index < code.length()){
        switch(state){
            case 0:
                if(code[index] == '"') state = 1;
                else state = -1;
                break;
            case 1:
                if(code[index] == '"') state = 2;
                else state = 1;
                break;
            case 2:
                if(code[index] == '"') state = 2;
                else state = 1;
                break;
            case -1:
                return last_accept;
        }
        index++;
        if(state == 2){
            last_accept = index - start;
        }
    }
    return last_accept;
}

vector<pair<string, pair<string, string>>> lexical_analyze(string code){
    cout << code << endl;
    int start = 0;
    vector<pair<string, pair<string, string>>> tokens;
    while(start < code.length()){
        int max_match = 0;
        string token = "", exact_token_name = "";
        int rw_match = is_reserved_word(code, start);
        if(rw_match > max_match){
            max_match = rw_match;
            token = "reservedword";
        }
        int id_match = is_identifier(code, start);
        if(id_match > max_match){
            max_match = id_match;
            token = "identifier";
        }
        int number_match = is_number(code, start);
        if(number_match > max_match){
            max_match = number_match;
            token = "number";
        }
        int symbol_match = is_symbol(code, start);
        if(symbol_match > max_match){
            max_match = symbol_match;
            token = "symbol";
        }
        int string_match = is_string(code, start);
        if(string_match > max_match){
            max_match = string_match;
            token = "string";
        }
        if(!max_match){
            start++;
        }
        else{
            string token_value = code.substr(start, max_match);
            if(token == "reservedword" || token == "symbol"){
                exact_token_name = token_value;
            }
            else{
                exact_token_name = token;
            }
            tokens.push_back(make_pair(token, make_pair(token_value, exact_token_name)));            
            start += max_match;
        }
    }
    return tokens;
}
