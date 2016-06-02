//
// Created by 1 on 18.05.2016.
//

#include <iostream>
#include <mutex>
#include "lazy_string.h"

using namespace std;

threads::threads() : readers(0), locked(false), rec(0) {}

threads::threads(const threads &lock) :
        readers((int) lock.readers), locked((bool) lock.locked), rec((int) lock.rec) {}

inline void threads::readLock() const {
    if(this->threadId == this_thread::get_id()) {
        ++this->readers;
        return;
    }
    while(locked)
        this_thread::yield();
    ++this->readers;
}

inline void threads::readUnlock() const {
    --this->readers;
}

inline void threads::writeLock() {
    thread::id id = this_thread::get_id();
    if(id == this->threadId) {
        ++this->rec;
        return;
    }
    while(atomic_exchange_explicit(&locked, true, memory_order_acquire))
        this_thread::yield();
    while(this->readers > 0)
        this_thread::yield();
    this->threadId = id;
    ++this->rec;
}

inline void threads::writeUnlock() {
    if(--this->rec == 0) {
        this->threadId = thread::id();
        atomic_store_explicit(&locked, false, memory_order_release);
    }
}

lazy_string::lazy_string() {
    (*this).start = (*this).number = 0;
    this->word = make_shared<string>("");
    this->lock = make_shared<threads>(threads());
}

lazy_string::lazy_string(const string &str) {
    (*this).start = 0;
    (*this).number = str.size();
    (*this).word = make_shared<string>(str);
    (*this).lock = make_shared<threads>(threads());
}

size_t lazy_string::size() const {
    return number;
}

size_t lazy_string::length() const {
    return number;
}

lazy_string::lazy_string(size_t begin, size_t size, shared_ptr<string> str) {
    (*this).start = begin;
    (*this).number = size;
    (*this).word = str;
    (*this).lock = make_shared<threads>(threads());

}

lazy_string lazy_string::substr(size_t pos, size_t length) {
    return lazy_string(start + pos, (pos + length > number) ? (number - pos) : length, word);
}

lazy_string::operator std::string() {
    return (*word).substr(start, number);
}

char lazy_string::at(size_t i) const {
    return (*word)[start + i];
}

lazy_string::get_char lazy_string::at(size_t i) {
    return get_char(this, i);
}

char lazy_string::operator[](size_t i) const {
    return (*word)[start + i];
}

lazy_string::get_char lazy_string::operator[](size_t i) {
    return get_char(this, i);
}

lazy_string::get_char::get_char(lazy_string *str, size_t i) : lazy_str(str), index(i) { }

lazy_string::get_char &lazy_string::get_char::operator=(char c) {
    threads &current = (*this->lazy_str->lock);
    current.writeLock();
    bool updating = false;
    if ((*lazy_str).word.use_count() > 1) {
        (*lazy_str).start = 0;
        (*lazy_str).word = make_shared<string>(lazy_str->word->substr((*lazy_str).start, (*lazy_str).number));
        threads newLock;
        newLock.writeLock();
        this->lazy_str->lock = make_shared<threads>(newLock);
        updating = true;
    }
    (*lazy_str->word)[(*lazy_str).start + index] = c;
    current.writeUnlock();
    if(updating)
        this->lazy_str->lock->writeUnlock();
    return *this;
}

lazy_string::get_char::operator char() const {
    return (*lazy_str->word)[lazy_str->start + index];
}

istream &operator>>(istream &input, lazy_string &str) {
    str.lock->writeLock();
    auto s = make_shared<string>();
    input >> *s;
    str.word = s;
    str.start = 0;
    str.number = (*s).size();
    str.lock->writeUnlock();
    return input;
}

ostream &operator<<(ostream &output, lazy_string &str) {
    str.lock->readLock();
    for (size_t i = str.start; i < str.start + str.size(); i++)
        output << str[i];
    str.lock->readUnlock();
    return output;
}
