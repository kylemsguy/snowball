#ifndef johnsonalgo_date
#define johnsonalgo_date
#include <iostream>
#include <stdexcept>
using namespace std;


bool leap_year(size_t year) {
	return ((year % 4 == 0) && !((year % 100 == 0) ^ (year % 400 == 0)))? true : false;
}

size_t days_in_mth(size_t month, size_t year) {
	//						Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	static size_t days[] {0, 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2) return leap_year(year)? 29 : 28;
	return days[month];
}

class Date {
	int d{0}, m {0}, y{0};
public:
	Date() = default;
	Date(int dd) : d{dd} {}
	Date(int dd, int mm) : d{dd}, m{mm} { if (!is_valid()) throw runtime_error("Invalid date"); }
	Date(int dd, int mm, int yy) : d{dd}, m{mm}, y{yy} { if (!is_valid()) throw runtime_error("Invalid date"); }
	Date(const Date& date) : d{date.d}, m{date.m}, y{date.y} {}
	Date(Date&& date) : d{date.d}, m{date.m}, y{date.y} {}

	Date& operator=(Date&& date) {
		d = date.d;
		m = date.m;
		y = date.y;
		return *this;
	}

	// accessors
	int day() const { return d; }
	int mth() const { return m; }
	int month() const { return m; }
	int& day() { return d; }
	int& mth() { return m; }
	int year() const { return y; }
	int mth_days() const { return days_in_mth(m, y); }

	Date& operator+=(int d);
	Date& operator+=(const Date& a);
	Date& operator-=(int d);
	Date& operator-=(const Date& a);

	bool operator<(const Date& b) const;
	bool is_valid() const;

	size_t inc_day(int to_inc);
	size_t inc_mth(int to_inc);
	size_t dec_day(int to_dec);
	size_t dec_mth(int to_dec);
	int distance(const Date& future) const;

	int dayofweek() const {
	    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	    int yy = y;
	    yy -= m < 3;
	    return ( yy + yy/4 - yy/100 + yy/400 + t[m-1] + d) % 7;
	}

	friend ostream& operator<<(ostream& os, const Date& a);
	friend istream& operator>>(istream& is, Date& a);
};


size_t Date::inc_day(int to_inc) {
	if (to_inc < 0) return dec_day(-to_inc);
	size_t max_days {days_in_mth(m, y)};
	if (d+ to_inc <= static_cast<int>(max_days)) d+= to_inc;
	else {
		to_inc -= max_days - d;	// days to go in the current month
		for (size_t next_month = days_in_mth(inc_mth(1), y); to_inc > static_cast<int>(next_month);) {
			to_inc -= next_month;	
			next_month = days_in_mth(inc_mth(1), y);
		}
		d= to_inc;	// left over days to add
	}
	return d;
}
size_t Date::dec_day(int to_dec) {
	if (to_dec < 0) return inc_day(-to_dec);
	else if (d- to_dec > 0) d-= to_dec;
	else {
		to_dec -= d;
		while (to_dec >= 0) {
			to_dec -= days_in_mth(dec_mth(1), y);
		}
		d= -to_dec;	// rest of the days in m, to_dec is negative
	}
	return d;
}

size_t Date::inc_mth(int to_inc) {
	if (to_inc < 0) return dec_mth(-to_inc);
	else if (m + to_inc < 13) m += to_inc;
	else {
		to_inc -= 12 - m;	// subtract left over to increments
		++y;
		while (to_inc > 12) {
			to_inc -= 12;
			++y;
		}
		m = to_inc;
	}
	return m;
}

size_t Date::dec_mth(int to_dec) {
	if (to_dec < 0) return inc_mth(-to_dec);
	else if (m - to_dec > 0) m -= to_dec;
	else {
		to_dec -= m;	
		--y;
		while (to_dec > 11) {
			to_dec -= 12;
			--y;
		}
		m = 12 - to_dec; // to_dec between 0 to 11
	}
	return m;
}

Date& Date::operator+=(int d) {
	inc_day(d);
	return *this;
}
Date& Date::operator-=(int d) {
	dec_day(d);
	return *this;
}
// moves the date by another date
Date& Date::operator+=(const Date& a) {
	inc_day(a.d);
	inc_mth(a.m);
	y+= a.y;
	return *this;
}
Date& Date::operator-=(const Date& a) {
	dec_day(a.d);
	dec_mth(a.m);
	y-= a.y;
	return *this;
}

bool Date::operator<(const Date& b) const {
	return (y < b.y)? true : ((y > b.y)? false : 
		((m < b.m)? true : ((m > b.m)? false : (d < b.d))));
}

bool Date::is_valid() const {
	return (m > 12 || m < 1)? false : ((d > static_cast<int>(days_in_mth(m, y)) || d < 1)? false : true);
}

Date& operator+(Date a, int d) {
	return a += d;	// copy of a
}
Date& operator-(Date a, int d) {
	return a -= d;
}

int Date::distance(const Date& b) const {
	Date past {*this};
	Date future {b};

	bool neg_dist {false};	// if b is before current
	if (future < past) { swap(past, future), neg_dist = true; }

	int days_to {-past.d + 1};	// go to start of month
	past.d= 1;

	int mths_to_add {12 * (future.y - past.y) + (future.m - past.m)};
	for (int i = 0; i < mths_to_add; ++i) {
		days_to += days_in_mth(past.m, past.y);	// add # days in current month
		past.inc_mth(1);
	}
	// first da month ahead of future
	days_to += future.d - 1;
	return (neg_dist)? -days_to : days_to;
}


// distance between 2 dates
int operator-(const Date& a, const Date& b) {
	return b.distance(a);
}


// IO
ostream& operator<<(ostream& os, const Date& a) {
	return os << a.d << ':' << a.m << ':' << a.y;
}
istream& operator>>(istream& is, Date& a) {
	return is >> a.d >> a.m >> a.y;
}
#endif