#include "rational.h"

rational::rational(int a) {
    num = a;
    den = 1;
}

int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
}

int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}

void rational::normalize() {
    int v = gcd(num, den);
    num /= v;
    den /= v;
}

rational::rational(int a, int b) {
    num = a;
    den = b;
    normalize();
}
 
int rational::getNum() const { return num; }

int rational::getDenom() const { return den; }

rational rational::operator+(rational const &b) const {
    int mem = lcm(b.den, den);
    return rational(num * mem / den + b.num * mem / b.den, mem);
}

rational rational::operator-(rational const &b) const {
    int mem = lcm(b.den, den);
    return rational(num * mem / den - b.num * mem / b.den, mem);
}


rational rational::operator*(rational const &b) const {
    return rational(num * b.num, den * b.den);
}

rational rational::operator/(rational const &b) const {
    return rational(num * b.den, den * b.num);
}
