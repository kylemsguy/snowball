#include <iostream>
#include <sstream>
#include <string>
#include "lexer.h"
#include "parser.h"
using namespace std;

int main(int argc, char* argv[]) {
    int d, m, y;
    cin >> d >> m >> y;
  
    ts.set_input(cin);
    Eval eval({d,m,y});
    eval();
    auto& dates = eval.dates;
    for (auto& d : dates) 
        cerr << d << endl;
    if (dates.empty()) cout << "Empty dates!\n";

    cout << "Action: " << ts.action().number_val << endl;
    cout << "Course: " << ts.course() << endl;
    cout << "Location: " << ts.location() << endl;
    cerr << "(" << ts.begin() << ',' << ts.end() << ")\n";
    cout << eval.select << endl;
    cout << ts.header() << endl;
}