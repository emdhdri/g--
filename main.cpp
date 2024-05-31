#include "lexical_analyzer.h"
#include "parser.h"

using namespace std;


int main(){
    string code = "#include<iostream>\nusing namespace std;\nint main () {\n    int x, y=10;\n    int s=0;\n    while(t>=0 && x > 12){\n        cin>>x;\n    t = t-113134440;\n    s = s + x;\n    }\n    cout<<\"sum=\" << a;\n    return 0;\n}";
    vector<pair<string, pair<string, string>>> tokens = lexical_analyze(code);
    map<string, set<string>> first = compute_FIRST();
    map<string, set<string>> follow = compute_FOLLOW(first);
    map<string, map<string, vector<string>>> parse_table = create_parse_table(first, follow);
    try {
        vector<pair<string, vector<string>>> productions = parse(tokens, parse_table);
        cout << "Parsing successful" << endl;
        for(auto production: productions){
            cout << production.first << " -> ";
            for(auto body_element: production.second){
                if(body_element == ""){
                    cout << "e ";
                }
                else{
                    cout << body_element << " ";
                }
            }
            cout << endl;
        }
    } catch (const runtime_error& e) {
        cout << "Parsing error: " << e.what() << endl;
    }
    return 0;
}
