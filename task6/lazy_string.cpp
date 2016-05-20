//
// Created by 1 on 18.05.2016.
//

#include <iostream>
#include "lazy_string.h"

using namespace std;

lazy_string::lazy_string() {
    (*this).start = (*this).number = 0;
    this->word = make_shared<string>("");
}

lazy_string::lazy_string(const string &str) {
    (*this).start = 0;
    (*this).number = str.size();
    (*this).word = make_shared<string>(str);
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
    if ((*lazy_str).word.use_count() > 1) {
        (*lazy_str).start = 0;
        (*lazy_str).word = make_shared<string>(lazy_str->word->substr((*lazy_str).start, (*lazy_str).number));
    }
    (*lazy_str->word)[(*lazy_str).start + index] = c;
    return *this;
}

lazy_string::get_char::operator char() const {
    return (*lazy_str->word)[lazy_str->start + index];
}

istream &operator>>(istream &input, lazy_string &str) {
    auto s = make_shared<string>();
    input >> *s;
    str.word = s;
    str.start = 0;
    str.number = (*s).size();
    return input;
}

ostream &operator<<(ostream &output, lazy_string &str) {
    for (size_t i = str.start; i < str.start + str.size(); i++)
        output << str[i];
    return output;
}
 
