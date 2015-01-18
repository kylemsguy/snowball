#include <iostream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
using namespace std;

int main(int argc, char* argv[]) {
    switch (argc) {
        case 1:
        	cout << "No string argument\n";
            return 0;
        case 2:
            ts.set_input(new istringstream{argv[1]});
            break;
        default: 
        	cout << "too many arguments: " << argc << '\n';
        	return 0;
    }

    auto dates = all_dates({17, 1, 2015});
    for (auto d : dates) 
    	cout << d << endl;
    if (dates.empty()) cout << "Empty dates!\n";
}