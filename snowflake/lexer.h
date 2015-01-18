#pragma once
#include <string>
#include <iostream>
#include <map>  // unordered map
using namespace std;

enum class Kind : char {
    ABS = 'a', DIR = 'b', DAY = 'd', MTH = 'm', NUL = 'n', REL = 'r', CONT = 't', ACT = 'x'    // continuation
};

struct School_protocol {
    virtual bool start_coursecode(const string& s);
    virtual bool start_roomlocation(const string& s);
    virtual bool end_coursecode(const string& s);
    virtual bool end_roomlocation(const string& s);
};
struct Toronto_protocol : School_protocol {
    bool start_coursecode(const string& s) override {
        return s.size() == 3;
    }
    bool start_roomlocation(const string& s) override {
        return s.size() == 2;
    }
    bool end_coursecode(const string& s) override {
        return s.size() != 6 || s.size() != 7;
    }
    bool end_roomlocation(const string& s) override {
        return s.size() > 4 && s.size() < 7;
    }
};

struct Token {
    Kind kind;
    int number_val;
    int pos;   // position in the text, used for proxity checking
    Token() : kind{Kind::NUL}, pos{-1} {}
    Token(Kind k, int v, int p) : kind{k}, number_val{v}, pos{p} {}
    Token& operator=(const Token& other) = default;
};

class Token_stream {
public:
    Token_stream(istream& instream_ref, School_protocol* p = new Toronto_protocol{}) : ip{&instream_ref}, protocol(p), pos{-1}, owns{false} {}
    Token_stream(istream* instream_pt, School_protocol* p = new Toronto_protocol{}) : ip{instream_pt}, protocol(p), pos{-1}, owns{true} {}
    ~Token_stream() {if (owns) delete ip;}

    Token get();    // read and return next token
    Token& current() { return ct; } // most recently read token
    Token& cur() { return ct; } 
    Token& prev() { return pt; }
    int peek() const {return ip->peek();}
    bool eof() const { return ip->eof() || ip->peek() == -1; }
    void set_input(istream& instream_ref) {if (owns) delete ip; ip = &instream_ref; owns = false;}
    void set_input(istream* instream_pt) {if (owns) delete ip; ip = instream_pt; owns = true;}
    void clear() { ct = {}; pt = {}; }
    // for starting another stream
    void reset() {if (owns) delete ip; clear(); act = {}; pos = -1; owns = false;}


    Token& action() { return act; }
    const string& course() { return crs; }
    const string& location() { return loc; }

private:
    istream* ip;    // input stream pointer
    Token ct {};   // current token, default value in case of misuse
    Token pt {};
    Token act {};   // 1 action per stream
    string crs;
    string loc;
    School_protocol* protocol;
    int pos;       // start at -1 to increment before usage in get (so 0 is first)
    bool owns;
};

extern Token_stream ts;