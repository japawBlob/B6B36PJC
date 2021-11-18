#include "tiny-05.hpp"

namespace pjc {



complex::complex(double real, double imaginary) :
    m_real(real),
    m_imag(imaginary) {}

double complex::real() const {
    return m_real;
}

void complex::real(double d) {
    m_real = d;
}

double complex::imag() const {
    return m_imag;
}

void complex::imag(double d) {
    m_imag = d;
}
complex operator+(const double lhs, const complex& rhs) {
    return complex(lhs + rhs.real(),
                   rhs.imag());
}
complex operator+(const complex& lhs, const double rhs) {
    return complex(lhs.real() + rhs,
                   lhs.imag());
}
complex operator+(const complex& lhs, const complex& rhs) {
    return complex(lhs.real() + rhs.real(),
                   lhs.imag() + rhs.imag());
}

complex operator-(const double lhs, const complex& rhs) {
    return complex(lhs - rhs.real(),
                   -rhs.imag());
}
complex operator-(const complex& lhs, const double rhs) {
    return complex(lhs.real() - rhs,
                   lhs.imag());
}
complex operator-(const complex& lhs, const complex& rhs) {
    return complex(lhs.real() - rhs.real(),
                   lhs.imag() - rhs.imag());
}
complex operator*(const double lhs, const complex& rhs) {
    return complex(lhs * rhs.real(),
                   lhs * rhs.imag());
}
complex operator*(const complex& lhs, const double rhs) {
    return complex(lhs.real() * rhs,
                   lhs.imag() * rhs);
}
complex operator*(const complex& lhs, const complex& rhs) {
    return complex(lhs.real() * rhs.real() - lhs.imag() * rhs.imag(),
                   lhs.real() * rhs.imag() + lhs.imag() * rhs.real());
}
}
