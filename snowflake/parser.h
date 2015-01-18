#include <vector>
#include <iostream>
#include "date.h"
#include "lexer.h"


using namespace std;


vector<Date> all_dates(Date sentdate) {    // add and subtract
	vector<Date> res;
    while (!ts.eof()) {
    	ts.get();
    	// cout << res.size() << ' ' << static_cast<int>(ts.peek()) << endl;
		Date cur{sentdate};
        switch (ts.current().kind) {
        	// today, tomorrow, directly a date
        	case Kind::DIR: {
        		cout << "DIRect date found\n";
        		cur += ts.current().number_val;
        		res.push_back(cur);

        		ts.clear();
        	}
        	// 4th, 5th, ... first absolute day in month
        	case Kind::ABS: {
        		cout << "ABSolute date found\n";
        		if (ts.prev().kind == Kind::MTH) {
        			cur.day() = ts.current().number_val;
        			res.push_back(cur);

        			ts.clear();
        		}
        		break;
        	}
            case Kind::DAY: {
        		cout << "DAY date found\n";
            	// assume next if not specified, eg. the assignment is due [this] thursday
            	if (ts.prev().kind == Kind::NUL) {
            		cur +=  ts.current().number_val - cur.dayofweek();
            		res.push_back(cur);

            		ts.clear();
            	}
        		// next wednesday, need to know what day currently is
            	else if (ts.prev().kind == Kind::REL) {
            		cur += ts.prev().number_val;
            		cur += ts.current().number_val - cur.dayofweek();
            		res.push_back(cur);

            		ts.clear();
            	}
            	break;
            } 
            case Kind::MTH: {
        		cout << "MTH date found\n";
            	// only valid if no other valid types have been matched
            	if (ts.prev().kind == Kind::NUL)
            		cur.mth() = ts.current().number_val;
            	else
            		ts.clear();
            	break;
            }
            case Kind::REL: {
        		cout << "RELation date found\n";
            	// next next monday
            	if (ts.prev().kind == Kind::REL) ts.current().number_val += ts.prev().number_val;
            }
            default: break;
        }
    }
    return res;
}