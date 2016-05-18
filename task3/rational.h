class rational {
public:
    rational(int i);
    rational(int a, int b);
    int getNum() const;
    int getDenom() const;
    rational operator+(rational const &b) const;
    rational operator-(rational const &b) const;
    rational operator*(rational const &b) const;
    rational operator/(rational const &b) const;

private:
    int num;
    int den;
    void normalize();
};
