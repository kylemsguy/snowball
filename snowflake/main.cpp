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

    auto dates = all_dates({d,m,y});
    for (auto d : dates) 
        cerr << d << endl;
    if (dates.empty()) cout << "Empty dates!\n";
}