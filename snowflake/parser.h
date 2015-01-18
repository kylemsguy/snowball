#include <vector>
#include <iostream>
#include "date.h"
#include "lexer.h"

constexpr int lifeboost = 3;

using namespace std;

bool close_enough(int epsilon) {
    return ts.current().pos - ts.prev().pos < epsilon;
} 

struct Eval {
	vector<Date> dates;
    Date select;
    Date sentdate;
    Date cur;
    streampos last_index;
    bool closest;

    Eval(Date d) : sentdate(d), cur(sentdate), closest{true} {}

    void operator()() {    // add and subtract
        while (!ts.eof()) {
        	ts.get();
        	// //cerr << dates.size() << ' ' << static_cast<int>(ts.peek()) << endl;
            switch (ts.current().kind) {
                // action of the message found, expects 1 per message
                case Kind::ACT: {
                    if (ts.action().kind != Kind::NUL) //cerr << "Multiple ACTions inside message\n";
                    ts.action() = ts.cur();

                    //cerr << "ACTion found " << static_cast<char>(ts.prev().kind) << ' ' << ts.action().number_val << " (" << ts.index() << ")\n";

                    ts.cur() = {};  // clear
                    break;
                }
                // continuation such as and, extends life time of previous modifier
                case Kind::CONT: {
                    if (ts.prev().kind != Kind::NUL) {
                        //cerr << "CONTinuation phrase found " << ts.prev().pos;
                        ts.cur() = ts.prev();
                        ts.cur().pos += lifeboost;
                        //cerr << " increased to " << ts.cur().pos << " (" << ts.index() << ")\n"; 
                    }
                    break;
                }
            	// today, tomorrow, directly a date
            	case Kind::DIR: {
                    cur += ts.current().number_val;

            		//cerr << "DIRect date found " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";

                    push();
            		// dates.push_back(cur);
              //       cur = sentdate;
            		// ts.clear();
                    break;
            	}
            	// 4th, 5th, ... first absolute day in month
            	case Kind::ABS: {
                    if (ts.prev().kind == Kind::MTH || ts.prev().kind == Kind::NUL) {
                        cur.day() = ts.current().number_val;

                        //cerr << "ABSolute date found " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";

                        push();
                        // dates.push_back(cur);
                        // cur = sentdate;
                        // ts.clear();
                    }
                    // weird format like 4 Oct, do a lookahead
                    else if (ts.get().kind == Kind::MTH) {
                        cur.mth() = ts.current().number_val;
                        cur.day() = ts.prev().number_val;

                		//cerr << "ABSolute date found " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";

                        push();
                        // dates.push_back(cur);
                        // cur = sentdate;
                        // ts.clear();
                    }
            		break;
            	}
                case Kind::DAY: {
            		//cerr << "DAY date found\n";
                	// assume next if not specified, eg. the assignment is due [this] thursday
                	if (ts.prev().kind == Kind::NUL) {
                        cur +=  ts.current().number_val - cur.dayofweek();

                        //cerr << "DAY date pushed: " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";
                        push();
                        // dates.push_back(cur);
                        // cur = sentdate;
                        // ts.clear();
                    }
                    // next wednesday, need to know what day currently is
                    else if (ts.prev().kind == Kind::REL && close_enough(3)) {
                        cur += ts.prev().number_val;
                        cur += ts.current().number_val - cur.dayofweek();

                        //cerr << "DAY date pushed: " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";

                        push();
                        // dates.push_back(cur);
                        // cur = sentdate;
                        // ts.clear();
                    }
                    break;
                } 
                case Kind::MTH: {
                    // only valid if no other valid types have been matched    
                    cur.mth() = ts.current().number_val;
                    //cerr << "MTH date found " << cur.mth() << " (" << ts.index() << ")\n";
                    // for odd naming scheme like 23 Oct, close enough together
                    if (ts.prev().kind == Kind::DAY && close_enough(3)) {
                        cur.day() = ts.prev().number_val;

                        //cerr << "MTH date pushed: " << static_cast<char>(ts.prev().kind) << ' ' << cur.mth() << ' ' << cur.day() << " (" << ts.index() << ")\n";

                        push();
                        // dates.push_back(cur);
                        // cur = sentdate;
                        // ts.clear();
                    }


                	break;
                }
                case Kind::REL: {
            		//cerr << "RELation date found" << " (" << ts.index() << ")\n";
                	// next next monday
                	if (ts.prev().kind == Kind::REL) ts.current().number_val += ts.prev().number_val;
                }
                default: break;
            }
        }
    }

    void push() {
        dates.push_back(cur);
        if (ts.course_index() && closest && ts.index() - ts.course_index() < ts.course_index() - last_index) {
            // select = cur;
            closest = false;
            select = cur;
            cerr << "Finally selecting " << select << "-->" << ts.index() << endl;
        }
        // haven't gone past 
        else if (closest) {
            select = cur;
            cerr << "Selecting " << select << "-->" << ts.index() << endl;
            last_index = ts.index();
        }
        cur = sentdate;
        ts.clear();
    }
};
