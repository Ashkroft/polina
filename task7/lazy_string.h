//
// Created by 1 on 18.05.2016.
//

#ifndef LAZY_STRING_LAZY_STRING_H
#define LAZY_STRING_LAZY_STRING_H

#include <string>
#include <iostream>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>

using namespace std;

class threads {

private:
    mutable atomic<int> readers;
    mutable atomic<bool> locked;
    atomic<thread::id> threadId;
    atomic<int> rec;

public:

    threads();
    threads(const threads &lock);

    inline void readLock() const;
    inline void readUnlock() const;
    inline void writeLock();
    inline void writeUnlock();

};


class lazy_string {
    struct get_char {
        friend class lazy_string;
        operator char() const;
        get_char &operator=(char);

    private:
        get_char(lazy_string *, size_t);
        const size_t index;
        lazy_string *const lazy_str;
    };

private:
    size_t start, number;
    shared_ptr<string> word;
    lazy_string(size_t begin, size_t size, shared_ptr<string> str);
 //create lazy string from lazy string
//    lazy_string(size_t begin, size_t size, const lazy_string &str);
    shared_ptr<string> present;
    mutable shared_ptr<threads> lock;
//    unique_lock<mutex> lk(my_mutex);

public:

    //Create empty lazy string
    lazy_string();

    //Create lazy string from std::string
    lazy_string(const string &str);

    //Create string copy from lazy string
    operator std::string();

    //Return length of lazy sting
    size_t size() const;
    size_t length() const;

    //Return the substring from position start to start + length (or to the end of a lazy string)
    lazy_string substr(size_t start = 0, size_t length = string::npos);

    //Return the character of lazy string
    get_char at(size_t i);
    char at(size_t i) const;
    get_char operator[](size_t i);
    char operator[](size_t i) const;

    //Read lazy string from input stream
    friend istream &operator>>(istream &input, lazy_string &str);

    //Sends lazy string to output stream
    friend ostream &operator<<(ostream &output, lazy_string &str);

};


#endif //LAZY_STRING_LAZY_STRING_H
