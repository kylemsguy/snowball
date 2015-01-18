#include <vector>
#include <iostream>
#include "date.h"
#include "lexer.h"


using namespace std;


vector<Date> all_dates(Date sentdate) {    // add and subtract
	vector<Date> res;
    Date cur{sentdate};
    while (!ts.eof()) {
    	ts.get();
    	// cerr << res.size() << ' ' << static_cast<int>(ts.peek()) << endl;
        switch (ts.current().kind) {
        	// today, tomorrow, directly a date
        	case Kind::DIR: {
        		cerr << "DIRect date found " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << ts.current().number_val << '\n' ;
        		cur += ts.current().number_val;

        		res.push_back(cur);
                cur = sentdate;
        		ts.clear();
                break;
        	}
        	// 4th, 5th, ... first absolute day in month
        	case Kind::ABS: {
        		cerr << "ABSolute date found " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << ts.current().number_val << '\n';
        		if (ts.prev().kind == Kind::MTH || ts.prev().kind == Kind::NUL) {
        			cur.day() = ts.current().number_val;

        			res.push_back(cur);
                    cur = sentdate;
        			ts.clear();
        		}
        		break;
        	}
            case Kind::DAY: {
        		cerr << "DAY date found\n";
            	// assume next if not specified, eg. the assignment is due [this] thursday
            	if (ts.prev().kind == Kind::NUL) {
                    cerr << "DAY date pushed: " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << ts.current().number_val << endl;
                    cur +=  ts.current().number_val - cur.dayofweek();

                    res.push_back(cur);
                    cur = sentdate;
                    ts.clear();
                }
                // next wednesday, need to know what day currently is
                else if (ts.prev().kind == Kind::REL) {
                    cerr << "DAY date pushed: " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << ts.current().number_val << endl;
            		cur += ts.prev().number_val;
            		cur += ts.current().number_val - cur.dayofweek();

            		res.push_back(cur);
                    cur = sentdate;
            		ts.clear();
            	}
            	break;
            } 
            case Kind::MTH: {
                // only valid if no other valid types have been matched    
                cur.mth() = ts.current().number_val;
        		cerr << "MTH date found " << cur.mth() << endl;

            	break;
            }
            case Kind::REL: {
        		cerr << "RELation date found\n";
            	// next next monday
            	if (ts.prev().kind == Kind::REL) ts.current().number_val += ts.prev().number_val;
            }
            default: break;
        }
    }
    return res;
}