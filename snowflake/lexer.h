#pragma once
#include <string>
#include <iostream>
#include <map>  // unordered map
using namespace std;

enum class Kind : char {
    ABS = 'a', DIR = 'b', DAY = 'd', MTH = 'm', NUL = 'n', REL = 'r', CONT = 't', ACT = 'x'    // continuation
};

struct School_protocol {
    virtual bool start_coursecode(const string& s) = 0;
    virtual bool start_roomlocation(const string& s) = 0;
    virtual bool end_coursecode(const string& s) = 0;
    virtual bool end_roomlocation(const string& s) = 0;
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
    Token(const Token&) = default;
    Token(Kind k, int v, int p) : kind{k}, number_val{v}, pos{p} {}
    Token& operator=(const Token& other) = default;
};

class Token_stream {
public:
    Token_stream(istream& instream_ref, School_protocol* p = new Toronto_protocol{}) : ip{&instream_ref}, protocol(p), pos{-1}, crs_start{0}, start{0}, last{0}, owns{false} {}
    Token_stream(istream* instream_pt, School_protocol* p = new Toronto_protocol{}) : ip{instream_pt}, protocol(p), pos{-1}, crs_start{0}, start{0}, last{0}, owns{true} {}
    ~Token_stream() {if (owns) delete ip;}

    Token get();    // read and return next token
    Token& current() { return ct; } // most recently read token
    Token& cur() { return ct; } 
    Token& prev() { return pt; }
    int peek() const {return ip->peek();}
    bool eof() const { return ip->eof() || ip->peek() == -1; }
    void set_input(istream& instream_ref) {if (owns) delete ip; ip = &instream_ref; owns = false; start = index(); last = 0;}
    void set_input(istream* instream_pt) {if (owns) delete ip; ip = instream_pt; owns = true; start = index(); last = 0;}
    void clear() { ct = {}; pt = {}; }
    streampos index() {return ip->tellg();}
    streampos course_index() {return crs_start;}
    // for starting another stream
    void reset() {if (owns) delete ip; clear(); act = {}; pos = -1; owns = false;}


    Token& action() { return act; }
    const string& course() { return crs; }
    const string& location() { return loc; }

    streampos begin() const {return start;}
    streampos end() const {return last;}
    string header() {
        string res;
        ip->clear();
        ip->seekg(start);
        do {
            res += ip->get();
        } while (ip->tellg() != last && ip->tellg() != -1);
        return res;
    }

private:
    istream* ip;    // input stream pointer
    Token ct {};   // current token, default value in case of misuse
    Token pt {};
    Token act {Kind::ACT, 2, 0};   // 1 action per stream
    string crs;
    string loc;
    School_protocol* protocol;
    int pos;       // start at -1 to increment before usage in get (so 0 is first)
    streampos crs_start;
    streampos start;    // start of active sentence
    streampos last;
    bool owns;
};

extern Token_stream ts;
