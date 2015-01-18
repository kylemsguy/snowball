#pragma once
#include <string>
#include <iostream>
#include <map>  // unordered map
using namespace std;

enum class Kind : char {
    NUL, ABS, MTH, DAY, REL, DIR
};
struct Token {
    Kind kind;
    int number_val;
    Token() : kind{Kind::NUL} {}
    Token(Kind k, int v) : kind{k}, number_val{v} {}
};

class Token_stream {
public:
    Token_stream(istream& instream_ref) : ip{&instream_ref}, owns{false} {}
    Token_stream(istream* instream_pt) : ip{instream_pt}, owns{true} {}
    ~Token_stream() {if (owns) delete ip;}

    Token get();    // read and return next token
    Token& current() { return ct; } // most recently read token
    Token& prev() { return pt; }
    int peek() const {return ip->peek();}
    bool eof() const { return ip->eof() || ip->peek() == -1; }
    void set_input(istream& instream_ref) {if (owns) delete ip; ip = &instream_ref;}
    void set_input(istream* instream_pt) {if (owns) delete ip; ip = instream_pt;}
    void clear() { ct = {}; pt = {}; }

private:
    istream* ip;    // input stream pointer
    Token ct {};   // current token, default value in case of misuse
    Token pt {};
    bool owns;
};

extern Token_stream ts;