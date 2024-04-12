#include <iostream>
#include <string>

using namespace std;

bool is_letter(char c){
    return (c > 64 && c < 91) || (c > 96 && c < 123) || c == '_'; 
}

bool is_digit(char c){
    return c > 47 && c < 58;
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
                return last_accept;
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
            default:
                return last_accept;
        }
        index++;
        if(state == 1 || state == 3 || state == 4){
            last_accept = index;
        }
        
    }
    return last_accept;
}
int main(){
    string code;
    cin >> code;
    int length = is_identifier(code, 0);
    cout << code.substr(0, length);
    return 0;
}