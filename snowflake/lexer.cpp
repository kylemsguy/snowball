#include <unordered_map>
#include "lexer.h"

constexpr int lifetime = 1;

using namespace std;

struct Date_val {
    Kind type;
    int val;
    Date_val() : type{Kind::NUL} {};
    Date_val(Kind t, int v) : type{t}, val{v} {}
};

std::unordered_map<string, Date_val> keywords {
    {"Jan",{Kind::MTH,1}}, {"jan",{Kind::MTH,1}}, {"January",{Kind::MTH,1}}, {"january",{Kind::MTH,1}},
    {"Feb",{Kind::MTH,2}}, {"feb",{Kind::MTH,2}}, {"February",{Kind::MTH,2}}, {"february",{Kind::MTH,2}},
    {"Mar",{Kind::MTH,3}}, {"March",{Kind::MTH,3}},
    {"Apr",{Kind::MTH,4}}, {"apr",{Kind::MTH,4}}, {"April",{Kind::MTH,4}}, {"april",{Kind::MTH,4}},
    {"May",{Kind::MTH,5}}, 
    {"Jun",{Kind::MTH,6}}, {"jun",{Kind::MTH,6}}, {"June",{Kind::MTH,6}}, {"june",{Kind::MTH,6}},
    {"Jul",{Kind::MTH,7}}, {"jul",{Kind::MTH,7}}, {"July",{Kind::MTH,7}}, {"july",{Kind::MTH,7}},
    {"Aug",{Kind::MTH,8}}, {"aug",{Kind::MTH,8}}, {"August",{Kind::MTH,8}}, {"august",{Kind::MTH,8}},
    {"Sep",{Kind::MTH,9}}, {"sep",{Kind::MTH,9}}, {"September",{Kind::MTH,9}}, {"september",{Kind::MTH,9}},
    {"Oct",{Kind::MTH,10}}, {"oct",{Kind::MTH,10}}, {"October",{Kind::MTH,10}}, {"october",{Kind::MTH,10}},
    {"Nov",{Kind::MTH,11}}, {"nov",{Kind::MTH,11}}, {"November",{Kind::MTH,11}}, {"november",{Kind::MTH,11}},
    {"Dec",{Kind::MTH,12}}, {"dec",{Kind::MTH,12}}, {"December",{Kind::MTH,12}}, {"december",{Kind::MTH,12}},
    {"This",{Kind::REL,0}}, {"this",{Kind::REL,0}}, {"Next",{Kind::REL,7}}, {"next",{Kind::REL,7}},
    {"Coming",{Kind::REL,7}}, {"coming",{Kind::REL,7}},
    {"Today",{Kind::DIR,0}}, {"today",{Kind::DIR,0}},
    {"Tomorrow",{Kind::DIR,1}},{"tomorrow",{Kind::DIR,1}},
    {"Sunday",{Kind::DAY,0}},{"sunday",{Kind::DAY,0}},
    {"Monday",{Kind::DAY,1}},{"monday",{Kind::DAY,1}},
    {"Tuesday",{Kind::DAY,2}},{"tuesday",{Kind::DAY,2}},
    {"Wednesday",{Kind::DAY,3}},{"wednesday",{Kind::DAY,3}},
    {"Thursday",{Kind::DAY,4}},{"thursday",{Kind::DAY,4}},
    {"Friday",{Kind::DAY,5}},{"friday",{Kind::DAY,5}},
    {"Saturday",{Kind::DAY,6}},{"saturday",{Kind::DAY,6}},
    {"Sun",{Kind::DAY,0}},{"sun",{Kind::DAY,0}},
    {"Mon",{Kind::DAY,1}},{"mon",{Kind::DAY,1}},
    {"Tues",{Kind::DAY,2}},{"tues",{Kind::DAY,2}},
    {"Wed",{Kind::DAY,3}},{"wed",{Kind::DAY,3}},
    {"Thur",{Kind::DAY,4}},{"thur",{Kind::DAY,4}},
    {"Fri",{Kind::DAY,5}},{"fri",{Kind::DAY,5}},
    {"Sat",{Kind::DAY,6}},{"sat",{Kind::DAY,6}},

    {"and",{Kind::CONT,0}},

    // action 0: cancelled 1: moved 
    {"Cancelled",{Kind::ACT,0}},{"Cancel",{Kind::ACT,0}},{"cancelled",{Kind::ACT,0}},{"cancel",{Kind::ACT,0}},
    {"Moved",{Kind::ACT,1}},{"Move",{Kind::ACT,1}},{"move",{Kind::ACT,1}},{"moved",{Kind::ACT,1}},

    };

Token_stream ts {nullptr};

Token Token_stream::get() {
    // read 1 char, decide what kind of token is incoming,
    // appropriately read more char then return Token
    ++pos;
    char c = 0;
    // update only if valuable
    if (ct.kind != Kind::NUL) {
        pt = ct;
        // throw out if too old
        if (pos > ct.pos + lifetime) ct = {};
    }

    do {  // skip all whitespace except newline
        if(!ip->get(c)) return ct = {};  // no char can be read from ip
    } while (c != '\n' && isspace(c));

    switch(c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ip->putback(c);
            *ip >> ct.number_val;
            cerr << "NUMBER: " << static_cast<char>(ct.kind) << ' ' << ct.number_val << endl;
            if (ct.number_val > 31 || ct.number_val < 0) ct.kind = Kind::NUL;
            else if (ct.kind == Kind::MTH || ct.kind == Kind::DAY || ct.kind == Kind::CONT || 
                (ip->peek() == 't') || (ip->peek() == 'r') || (ip->peek() == 's'))
                ct.kind = Kind::ABS;
            else ct.kind = Kind::NUL;   // 9 am, other unknown dates

            ct.pos = pos;
            return ct;
        default:    // name, name =, or error
            if (isalpha(c)) {
                // potential keyword
                string kw (1,c);
                while (ip->get(c) && isalpha(c))
                    kw += c;    // append each letter of name
                cerr << "STRING: " << static_cast<char>(ct.kind) << ' ' <<  kw << endl;
                ip->putback(c);     // while loop reads 1 extra char
                auto itr = keywords.find(kw);
                if (itr == keywords.end()) return {};
                Date_val date = itr->second;
                ct.kind = date.type;
                ct.number_val = date.val;

                ct.pos = pos;
                return ct;
            }
            return {};
    }
}

