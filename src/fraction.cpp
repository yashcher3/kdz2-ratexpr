#include <ratexpr/fraction.hpp>

#include <numeric>
#include <ostream>
#include <stdexcept>

namespace ratexpr {

Fraction::Fraction() : num_(0), den_(1) {}

Fraction::Fraction(std::int64_t numerator, std::int64_t denominator)
    : num_(numerator), den_(denominator) {
    if (den_ == 0) {
        throw std::invalid_argument("Fraction: zero denominator");
    }
    normalize();
}

void Fraction::normalize() {
    if (den_ < 0) {
        num_ = -num_;
        den_ = -den_;
    }
    const std::int64_t g = std::gcd(num_, den_);
    num_ /= g;
    den_ /= g;
}

Fraction& Fraction::operator+=(const Fraction& o) {
    const std::int64_t g = std::gcd(den_, o.den_);
    const std::int64_t n = num_ * (o.den_ / g) + o.num_ * (den_ / g);
    const std::int64_t d = den_ * (o.den_ / g);
    num_ = n;
    den_ = d;
    normalize();
    return *this;
}

Fraction& Fraction::operator-=(const Fraction& o) {
    return *this += -o;
}

Fraction& Fraction::operator*=(const Fraction& o) {
    const std::int64_t g1 = std::gcd(num_, o.den_);
    const std::int64_t g2 = std::gcd(o.num_, den_);
    const std::int64_t n = (num_ / g1) * (o.num_ / g2);
    const std::int64_t d = (den_ / g2) * (o.den_ / g1);
    num_ = n;
    den_ = d;
    normalize();
    return *this;
}

Fraction& Fraction::operator/=(const Fraction& o) {
    if (o.num_ == 0) {
        throw std::invalid_argument("Fraction: division by zero");
    }
    return *this *= Fraction(o.den_, o.num_);
}

Fraction& Fraction::operator+=(std::uint32_t k) { return *this += Fraction(k); }
Fraction& Fraction::operator-=(std::uint32_t k) { return *this -= Fraction(k); }
Fraction& Fraction::operator*=(std::uint32_t k) { return *this *= Fraction(k); }
Fraction& Fraction::operator/=(std::uint32_t k) { return *this /= Fraction(k); }

Fraction Fraction::operator-() const {
    return Fraction(-num_, den_);
}

Fraction operator+(const Fraction& a, const Fraction& b) { Fraction r = a; r += b; return r; }
Fraction operator+(const Fraction& a, std::uint32_t k) { Fraction r = a; r += k; return r; }
Fraction operator+(std::uint32_t k, const Fraction& b) { Fraction r(k); r += b; return r; }

Fraction operator-(const Fraction& a, const Fraction& b) { Fraction r = a; r -= b; return r; }
Fraction operator-(const Fraction& a, std::uint32_t k) { Fraction r = a; r -= k; return r; }
Fraction operator-(std::uint32_t k, const Fraction& b) { Fraction r(k); r -= b; return r; }

Fraction operator*(const Fraction& a, const Fraction& b) { Fraction r = a; r *= b; return r; }
Fraction operator*(const Fraction& a, std::uint32_t k) { Fraction r = a; r *= k; return r; }
Fraction operator*(std::uint32_t k, const Fraction& b) { Fraction r(k); r *= b; return r; }

Fraction operator/(const Fraction& a, const Fraction& b) { Fraction r = a; r /= b; return r; }
Fraction operator/(const Fraction& a, std::uint32_t k) { Fraction r = a; r /= k; return r; }
Fraction operator/(std::uint32_t k, const Fraction& b) { Fraction r(k); r /= b; return r; }

bool operator==(const Fraction& a, const Fraction& b) {
    return a.num_ == b.num_ && a.den_ == b.den_;
}

bool operator!=(const Fraction& a, const Fraction& b) {
    return !(a == b);
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    os << f.num_;
    if (f.den_ != 1) {
        os << '/' << f.den_;
    }
    return os;
}

}
