#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>
#include <stack>

using namespace std;

// GRAMMAR definition
/*
map<string, vector<vector<string>>> GRAMMAR = {
    {"program", {{"include_stmt", "namespace_stmt", "main_func"}}},
    {"include_stmt", {{"#include", "<", "identifier", ">"}}},
    {"namespace_stmt", {{"using", "namespace", "std", ";"}}},
    {"main_func", {{"int", "main", "(", ")", "{", "stmt_list", "}"}}},
    {"type", {{"int"}, {"float"}, {"void"}}},
    {"stmt_list", {{"stmt", "stmt_list"}, {""}}},
    {"stmt", {{"var_decl", ";"}, {"assignment", ";"}, {"if_stmt"}, {"while_stmt"}, {"io_stmt", ";"}, {"return", "expr", ";"}, {"break", ";"}, {"continue", ";"}}},
    {"var_decl", {{"type", "identifier", "var_decl_tail"}}},
    {"var_decl_tail", {{"=", "expr"}, {""}}},
    {"assignment", {{"identifier", "=", "expr"}}},
    {"if_stmt", {{"if", "(", "bool_expr", ")", "{", "stmt_list", "}"}}},
    {"while_stmt", {{"while", "(", "bool_expr", ")", "{", "stmt_list", "}"}}},
    {"io_stmt", {{"cin", ">>", "identifier"}, {"cout", "<<", "expr"}}},
    {"bool_expr", {{"bool_term", "bool_expr_tail"}}},
    {"bool_expr_tail", {{"||", "bool_term", "bool_expr_tail"}, {""}}},
    {"bool_term", {{"bool_factor", "bool_term_tail"}}},
    {"bool_term_tail", {{"&&", "bool_factor", "bool_term_tail"}, {""}}},
    {"bool_factor", {{"!", "bool_factor"}, {"(", "bool_expr", ")"}, {"expr", "relop", "expr"}}},
    {"relop", {{"=="}, {"!="}, {"<"}, {"<="}, {">"}, {">="}}},
    {"expr", {{"term", "expr_tail"}}},
    {"expr_tail", {{"+", "term", "expr_tail"}, {"-", "term", "expr_tail"}, {""}}},
    {"term", {{"factor", "term_tail"}}},
    {"term_tail", {{"*", "factor", "term_tail"}, {"/", "factor", "term_tail"}, {""}}},
    {"factor", {{"number"}, {"identifier"}, {"string"}, {"(", "expr", ")"}}}
};
*/
map<string, vector<vector<string>>> GRAMMAR = {
    {"program", {{"include_stmt", "namespace_stmt", "main_func"}}},
    {"include_stmt", {{"#include", "<", "identifier", ">"}}},
    {"namespace_stmt", {{"using", "namespace", "std", ";"}}},
    {"main_func", {{"int", "main", "(", ")", "{", "stmt_list", "}"}}},
    {"type", {{"int"}, {"float"}, {"void"}}},
    {"stmt_list", {{"stmt", "stmt_list"}, {""}}},
    {"stmt", {{"var_decl", ";"}, {"assignment", ";"}, {"if_stmt"}, {"while_stmt"}, {"io_stmt", ";"}, {"return", "expr", ";"}, {"break", ";"}, {"continue", ";"}}},
    {"var_decl", {{"type", "identifier", "var_decl_tail", "var_decl_list_tail"}}},
    {"var_decl_tail", {{"=", "expr"}, {""}}},
    {"var_decl_list_tail", {{",", "identifier", "var_decl_tail", "var_decl_list_tail"}, {""}}},
    {"assignment", {{"identifier", "=", "expr"}}},
    {"if_stmt", {{"if", "(", "bool_expr", ")", "{", "stmt_list", "}"}}},
    {"while_stmt", {{"while", "(", "bool_expr", ")", "{", "stmt_list", "}"}}},
    {"io_stmt", {{"cin", "io_input_tail"}, {"cout", "io_output_tail"}}},
    {"io_input_tail", {{">>", "identifier", "io_input_tail"}, {""}}},
    {"io_output_tail", {{"<<", "expr", "io_output_tail"}, {""}}},
    {"bool_expr", {{"bool_term", "bool_expr_tail"}}},
    {"bool_expr_tail", {{"||", "bool_term", "bool_expr_tail"}, {""}}},
    {"bool_term", {{"bool_factor", "bool_term_tail"}}},
    {"bool_term_tail", {{"&&", "bool_factor", "bool_term_tail"}, {""}}},
    {"bool_factor", {{"!", "bool_factor"}, {"(", "bool_expr", ")"}, {"expr", "relop", "expr"}}},
    {"relop", {{"=="}, {"!="}, {"<"}, {"<="}, {">"}, {">="}}},
    {"expr", {{"term", "expr_tail"}}},
    {"expr_tail", {{"+", "term", "expr_tail"}, {"-", "term", "expr_tail"}, {""}}},
    {"term", {{"factor", "term_tail"}}},
    {"term_tail", {{"*", "factor", "term_tail"}, {"/", "factor", "term_tail"}, {""}}},
    {"factor", {{"number"}, {"identifier"}, {"string"}, {"(", "expr", ")"}}}
};


bool is_terminal(const string& symbol) {
    return !GRAMMAR.count(symbol);
}

// Compute FIRST sets
set<string> compute_first(map<string, set<string>>& first, const string &symbol) {
    if (is_terminal(symbol)) {
        return {symbol};
    }
    if (!first[symbol].empty()) {
        return first[symbol];
    }
    
    set<string> first_set;
    for (const auto &production : GRAMMAR[symbol]) {
        bool nullable = true;
        for (const string &prod_symbol : production) {
            set<string> first_of_prod_symbol = compute_first(first, prod_symbol);
            first_set.insert(first_of_prod_symbol.begin(), first_of_prod_symbol.end());
            if (first_of_prod_symbol.find("") == first_of_prod_symbol.end()) {
                nullable = false;
                break;
            }
            first_set.erase("");
        }
        if (nullable) {
            first_set.insert("");
        }
    }
    first[symbol] = first_set;
    return first_set;
}

map<string, set<string>> compute_FIRST() {
    map<string, set<string>> FIRST;
    for (const auto &rule : GRAMMAR) {
        compute_first(FIRST, rule.first);
    }
    return FIRST;
}

map<string, set<string>> compute_FOLLOW(map<string, set<string>>& first) {
    map<string, set<string>> FOLLOW;
    FOLLOW["program"].insert("$");
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto &rule : GRAMMAR) {
            const string &non_terminal = rule.first;
            for (const auto &production : rule.second) {
                for (size_t i = 0; i < production.size(); ++i) {
                    if (!is_terminal(production[i])) {
                        size_t old_size = FOLLOW[production[i]].size();
                        bool nullable = true;
                        for (size_t j = i + 1; j < production.size(); ++j) {
                            const string &next_symbol = production[j];
                            if (is_terminal(next_symbol)) {
                                FOLLOW[production[i]].insert(next_symbol);
                                nullable = false;
                                break;
                            } else {
                                FOLLOW[production[i]].insert(first[next_symbol].begin(), first[next_symbol].end());
                                FOLLOW[production[i]].erase("");
                                if (first[next_symbol].find("") == first[next_symbol].end()) {
                                    nullable = false;
                                    break;
                                }
                            }
                        }
                        if (nullable) {
                            FOLLOW[production[i]].insert(FOLLOW[non_terminal].begin(), FOLLOW[non_terminal].end());
                        }
                        if (FOLLOW[production[i]].size() > old_size) {
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    return FOLLOW;
}

map<string, map<string, vector<string>>> create_parse_table(map<string, set<string>>& first, map<string, set<string>>& follow) {
    map<string, map<string, vector<string>>> parse_table;
    for (auto& rule : GRAMMAR) {
        string non_terminal = rule.first;
        for (auto& production : rule.second) {
            set<string> first_set;
            if (production[0] == "") {
                first_set.insert("");
            } else {
                for (auto& symbol : production) {
                    if (is_terminal(symbol)) {
                        first_set.insert(symbol);
                        break;
                    } else {
                        first_set.insert(first[symbol].begin(), first[symbol].end());
                        if (first[symbol].find("") == first[symbol].end()) {
                            break;
                        }
                    }
                }
            }
            for (auto& terminal : first_set) {
                if (terminal != "") {
                    parse_table[non_terminal][terminal] = production;
                }
            }
            if (first_set.find("") != first_set.end()) {
                for (auto& follow_terminal : follow[non_terminal]) {
                    parse_table[non_terminal][follow_terminal] = production;
                }
            }
        }
    }
    return parse_table;
}


// Parsing function using the parse table
vector<pair<string, vector<string>>> parse(vector<pair<string, pair<string, string>>>& tokens, map<string, map<string, vector<string>>>& parse_table) {
    stack<string> parse_stack;
    parse_stack.push("program");
    size_t index = 0;
    vector<pair<string, vector<string>>> productions;
    while (!parse_stack.empty()) {
        string top = parse_stack.top();
        parse_stack.pop();
        string current_token = index < tokens.size() ? tokens[index].second.second : "$";
        if (is_terminal(top)) {
            if (top == current_token) {
                index++;
            } else {
                throw runtime_error("Syntax error: unexpected token " + current_token);
            }
        } else {
            if (parse_table[top].count(current_token)) {
                vector<string> production = parse_table[top][current_token];
                productions.push_back(make_pair(top, production));
                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    if (*it != "") {
                        parse_stack.push(*it);
                    }
                }
            } else {
                throw runtime_error("Syntax error: unexpected token " + current_token);
            }
        }
    }
    return productions;
}