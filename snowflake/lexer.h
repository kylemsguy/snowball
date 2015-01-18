#pragma once
#include <string>
#include <iostream>
#include <map>  // unordered map
using namespace std;

enum class Kind : char {
    ABS = 'a', DIR = 'b', DAY = 'd', MTH = 'm', NUL = 'n', REL = 'r'
};
struct Token {
    Kind kind;
    int number_val;
    int pos;   // position in the text, used for proxity checking
    Token() : kind{Kind::NUL} {}
    Token(Kind k, int v, int p) : kind{k}, number_val{v}, pos{p} {}
};

class Token_stream {
public:
    Token_stream(istream& instream_ref) : ip{&instream_ref}, pos{-1}, owns{false} {}
    Token_stream(istream* instream_pt) : ip{instream_pt}, pos{-1}, owns{true} {}
    ~Token_stream() {if (owns) delete ip;}

    Token get();    // read and return next token
    Token& current() { return ct; } // most recently read token
    Token& prev() { return pt; }
    int peek() const {return ip->peek();}
    bool eof() const { return ip->eof() || ip->peek() == -1; }
    void set_input(istream& instream_ref) {if (owns) delete ip; ip = &instream_ref; owns = false;}
    void set_input(istream* instream_pt) {if (owns) delete ip; ip = instream_pt; owns = true;}
    void clear() { ct = {}; pt = {}; }

private:
    istream* ip;    // input stream pointer
    Token ct {};   // current token, default value in case of misuse
    Token pt {};
    int pos;       // start at -1 to increment before usage in get (so 0 is first)
    bool owns;
};

extern Token_stream ts;