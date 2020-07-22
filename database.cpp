#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <tuple>
#include <iomanip>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
    Date();
    explicit Date(const string& input_date);

    int GetYear() const { return year_; }
    int GetMonth() const { return month_; }
    int GetDay() const { return day_; }

    string DateToString() const;

    bool CheckDigit(stringstream& ss, int& number);
    void CheckFormat(const string& input);
    void CheckDate() const;

private:
    int year_;
    int month_;
    int day_;
};

Date::Date() {
    year_ = 0;
    month_ = 0;
    day_ = 0;
}

Date::Date(const string &input_date) {
    CheckFormat(input_date);
    CheckDate();
}

string Date::DateToString() const {
    stringstream sout;
    char separator = '-';
    sout << setfill('0');
    if (year_ < 0) {
        sout << '-';
    }
    sout << setw(4) << abs(year_);
    sout << separator;
    sout << setw(2) << month_;
    sout << separator;
    sout << setw(2) << day_;
    return sout.str();
}

bool Date::CheckDigit(stringstream &ss, int &number) {
    char sign = '+';
    if (ss.peek() == '+' || ss.peek() == '-') {
        sign = ss.get();
    }
    if (ss.peek() >= '0' && ss.peek() <= '9') {
        ss >> number;
        if (sign == '-') {
            number = -number;
        }
    }
    else {
        return false;
    }
    return true;
}

void Date::CheckFormat(const string &input) {
    stringstream ss(input);
    int count_data = 3;
    int date_data[3] = {-1, -1, -1};
    year_ = date_data[0];
    month_ = date_data[1];
    day_ = date_data[2];

    if (!CheckDigit(ss, date_data[0])) {
        stringstream so;
        so << "Wrong date format: " << input;
        throw runtime_error(so.str());
    }

    char input_date_separator = '-';
    for (int i = 1; i < count_data; ++i) {
        if (ss.peek() != input_date_separator) {
            stringstream so;
            so << "Wrong date format: " << input;
            throw runtime_error(so.str());
        }
        else {
            ss.ignore(1);
            if (!CheckDigit(ss, date_data[i])) {
                stringstream so;
                so << "Wrong date format: " << input;
                throw runtime_error(so.str());
            }
        }
    }

    if (ss.rdbuf()->in_avail() != 0) {
        stringstream so;
        so << "Wrong date format: " << input;
        throw runtime_error(so.str());
    }
    year_ = date_data[0];
    month_ = date_data[1];
    day_ = date_data[2];
}

void Date::CheckDate() const {
    if (month_ < 1 || month_ > 12) {
        string error_msg = "Month value is invalid: " + to_string(month_);
        throw runtime_error(error_msg);
    }
    if (day_ < 1 || day_ > 31) {
        string error_msg = "Day value is invalid: " + to_string(day_);
        throw runtime_error(error_msg);
    }
}

bool operator<(const Date& lhs, const Date& rhs) {
    int y1 = lhs.GetYear(), m1 = lhs.GetMonth(), d1 = lhs.GetDay();
    int y2 = rhs.GetYear(), m2 = rhs.GetMonth(), d2 = rhs.GetDay();
    return tie(y1, m1, d1) < tie(y2, m2, d2);
}

class Database {
public:
    void AddEvent(const Date& date, const string& event);
    bool DeleteEvent(const Date& date, const string& event);
    int  DeleteDate(const Date& date);

    set<string> Find(const Date& date) const;

    void Print() const;

private:
    map<Date, set<string>> database_;
};

void Database::AddEvent(const Date &date, const string &event) {
    database_[date].insert(event);
}

bool Database::DeleteEvent(const Date &date, const string &event) {
    try {
        return database_.at(date).erase(event);
    } catch (exception& ex) {
        return false;
    }
}

int Database::DeleteDate(const Date &date) {
    int num_deleted = 0;
    try {
        num_deleted = database_.at(date).size();
        database_.erase(date);
        return num_deleted;
    } catch (exception& ex) {
        return num_deleted;
    }
}

set<string> Database::Find(const Date &date) const {
    set<string> events;
    try {
        events = database_.at(date);
        return events;
    } catch (exception& ex) {
        return events;
    }
}

void Database::Print() const {
    for (const auto& [date, events] : database_) {
        for (const auto& event : events) {
            cout << date.DateToString() << " " << event << endl;
        }
    }
}

void Add(const string& raw_date, const string& event, Database& db) {
    try {
        Date date(raw_date);
        db.AddEvent(date, event);
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        return;
    }
}

void Delete(const string& raw_date, const string& event, Database& db) {
    try {
        Date ex_date(raw_date);
        if (db.DeleteEvent(ex_date, event)) {
            cout << "Deleted successfully" << endl;
        }
        else {
            cout << "Event not found" << endl;
        }
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        return;
    }
}

void DelDate(const string& raw_date, Database& db){
    try {
        Date ex_date(raw_date);
        int count = db.DeleteDate(ex_date);
        cout << "Deleted " << count << " events" << endl;
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        return;
    }
}

void Find(const string& raw_date, Database& db) {
    try {
        Date ex_date(raw_date);
        set<string> result = db.Find(ex_date);
        if (!result.empty()) {
            for (const auto& event : result) {
                cout << event << endl;
            }
        }
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
        return;
    }
}

void Print(Database& db) {
    db.Print();
}

int main() {
    Database db;

    string command;
    Date a;
    while (getline(cin, command)) {
        if (command.empty()) continue;
        stringstream ss(command);
        string command_name;
        ss >> command_name;
        if (command_name == "Add") {
            string raw_date, event;
            ss >> raw_date >> event;
            Add(raw_date, event, db);
        }
        else if (command_name == "Del") {
            string raw_date, event;
            ss >> raw_date >> event;
            if (event.empty()) {
                DelDate(raw_date, db);
            }
            else {
                Delete(raw_date, event, db);
            }
        }
        else if (command_name == "Find") {
            string raw_date;
            ss >> raw_date;
            Find(raw_date, db);
        }
        else if (command_name == "Print") {
            Print(db);
        }
        else {
            cout << "Unknown command: " << command_name << endl;
        }
    }
    return 0;
}