#include <bits/stdc++.h>

template <auto N1, int K>
class Fixed {
public:
    using StorageType = std::conditional_t< 
            (N1 <= 8), std::conditional_t<std::is_same_v<std::decay_t<decltype(N1)>, int_fast8_t>, int_fast8_t, int8_t>, 
            std::conditional_t< 
                (N1 <= 16), std::conditional_t<std::is_same_v<std::decay_t<decltype(N1)>, int_fast16_t>, int_fast16_t, int16_t>, 
                std::conditional_t< 
                    (N1 <= 32), std::conditional_t<std::is_same_v<std::decay_t<decltype(N1)>, int_fast32_t>, int_fast32_t, int32_t>, 
                    std::conditional_t<std::is_same_v<std::decay_t<decltype(N1)>, int_fast64_t>, int_fast64_t, int64_t>
                > 
            > 
        >;
    constexpr Fixed(int v): v(v) {}
    constexpr Fixed(float f): v(f * (1ULL << K)) {}
    constexpr Fixed(double f): v(f * (1ULL << K)) {}
    constexpr Fixed(): v(0) {}

    static constexpr Fixed from_raw(int32_t x) {
        Fixed ret;
        ret.v = x;
        return ret;
    }

    StorageType v;
    
    template <auto N2, int K2>
    operator Fixed<N2, K2>() const {
        if constexpr (K > K2) {
            return Fixed<N2, K2>::from_raw(v >> (K - K2));
        } else {
            return Fixed<N2, K2>::from_raw(v << (K2 - K));
        }
    }

    operator double() const { 
        return static_cast<double>(v) / (1ULL << K); 
    }

    operator float() const {
        return static_cast<float>(v) / (1ULL << K);
    }


    template <auto M, size_t L> 
    Fixed& operator=(const Fixed<M, L>& other) { 
        double asDouble = static_cast<double>(other); 
        v = static_cast<StorageType>(asDouble * (1ULL << K)); 
        return *this; 
    }

    Fixed& operator=(const Fixed& other) = default;

    Fixed abs(Fixed x) {
        if (x.v < 0) {
            x.v = -x.v;
        }
        return x;
    }
};

template<auto N1, int K1, auto N2, int K2>
bool operator==(const Fixed<N1, K1>& a, const Fixed<N2, K2>& b) {
    return a.v == b.v;
}

template<auto N1, int K>
bool operator==(const Fixed<N1, K>& a, int b) {
    return a.v == Fixed<N1, K>(b).v;
}

template<auto N1, int K>
bool operator==(int a, const Fixed<N1, K>& b) {
    return Fixed<N1, K>(a).v == b.v;
}

template<auto N1, int K>
bool operator==(const Fixed<N1, K>& a, float b) {
    return a.v == Fixed<N1, K>(b).v;
}

template<auto N1, int K>
bool operator==(float a, const Fixed<N1, K>& b) {
    return Fixed<N1, K>(a).v == b.v;
}

template<auto N1, int K>
bool operator==(const Fixed<N1, K>& a, double b) {
    return a.v == Fixed<N1, K>(b).v;
}

template<auto N1, int K>
bool operator==(double a, const Fixed<N1, K>& b) {
    return Fixed<N1, K>(a).v == b.v;
}

template<auto N1, int K, auto N2, int K2>
auto operator<=>(Fixed<N1, K> a, Fixed<N2, K2> b) {
    return a.v <=> b.v;
}

template<auto N1, int K>
auto operator<=>(int a, Fixed<N1, K> b) {
    return (static_cast<typename Fixed<N1, K>::StorageType>(a) << K) <=> b.v;
}

template<auto N1, int K>
auto operator<=>(float a, Fixed<N1, K> b) {
    return (Fixed<N1, K>(a).v) <=> b.v;
}

template<auto N1, int K>
auto operator<=>(double a, Fixed<N1, K> b) {
    return (Fixed<N1, K>(a).v) <=> b.v;
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> operator+(Fixed<N1, K> a, Fixed<N2, K2> b) {
    return Fixed<N1, K>::from_raw(a.v + b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(Fixed<N1, K> a, int b) {
    return Fixed<N1, K>::from_raw(a.v + Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(int a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v + b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(Fixed<N1, K> a, float b) {
    return Fixed<N1, K>::from_raw(a.v + Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(float a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v + b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(Fixed<N1, K> a, double b) {
    return Fixed<N1, K>::from_raw(a.v + Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator+(double a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v + b.v);
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> operator-(Fixed<N1, K> a, Fixed<N2, K2> b) {
    return Fixed<N1, K>::from_raw(a.v - b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(Fixed<N1, K> a, int b) {
    return Fixed<N1, K>::from_raw(a.v - Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(int a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v - b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(Fixed<N1, K> a, float b) {
    return Fixed<N1, K>::from_raw(a.v - Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(float a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v - b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(Fixed<N1, K> a, double b) {
    return Fixed<N1, K>::from_raw(a.v - Fixed<N1, K>(b).v);
}

template<auto N1, int K>
Fixed<N1, K> operator-(double a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(Fixed<N1, K>(a).v - b.v);
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> operator*(Fixed<N1, K> a, Fixed<N2, K2> b) {
    return Fixed<N1, K>::from_raw(((int64_t) a.v * b.v) >> K);
}

template<auto N1, int K>
Fixed<N1, K> operator*(Fixed<N1, K> a, int b) {
    return Fixed<N1, K>::from_raw(a.v * b);
}

template<auto N1, int K>
Fixed<N1, K> operator*(int a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(a * b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator*(Fixed<N1, K> a, float b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>((static_cast<float>(a.v) * b)));
}

template<auto N1, int K>
Fixed<N1, K> operator*(float a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(a * static_cast<float>(b.v)));
}

template<auto N1, int K>
Fixed<N1, K> operator*(Fixed<N1, K> a, double b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>((static_cast<double>(a.v) * b)));
}

template<auto N1, int K>
Fixed<N1, K> operator*(double a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(a * static_cast<double>(b.v)));
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> operator/(Fixed<N1, K> a, Fixed<N2, K2> b) {
    return Fixed<N1, K>::from_raw(((int64_t) a.v << K) / b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator/(Fixed<N1, K> a, int b) {
    return Fixed<N1, K>::from_raw(a.v / b);
}

template<auto N1, int K>
Fixed<N1, K> operator/(int a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw((a << K) / b.v);
}

template<auto N1, int K>
Fixed<N1, K> operator/(Fixed<N1, K> a, float b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(static_cast<float>(a.v) / b));
}

template<auto N1, int K>
Fixed<N1, K> operator/(float a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(a / static_cast<float>(b.v)));
}

template<auto N1, int K>
Fixed<N1, K> operator/(Fixed<N1, K> a, double b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(static_cast<double>(a.v) / b));
}

template<auto N1, int K>
Fixed<N1, K> operator/(double a, Fixed<N1, K> b) {
    return Fixed<N1, K>::from_raw(static_cast<int32_t>(a / static_cast<double>(b.v)));
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> &operator+=(Fixed<N1, K> &a, Fixed<N2, K2> b) {
    return a = a + b;
}

template<auto N1, int K>
Fixed<N1, K> &operator+=(Fixed<N1, K> &a, int b) {
    return a = a + b;
}

template<auto N1, int K>
Fixed<N1, K> &operator+=(Fixed<N1, K> &a, float b) {
    return a = a + b;
}

template<auto N1, int K>
Fixed<N1, K> &operator+=(float a, Fixed<N1, K> &b) {
    return b = a + b;
}

template<auto N1, int K>
Fixed<N1, K> &operator+=(Fixed<N1, K> &a, double b) {
    return a = a + b;
}

template<auto N1, int K>
Fixed<N1, K> &operator+=(double a, Fixed<N1, K> &b) {
    return b = a + b;
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> &operator-=(Fixed<N1, K> &a, Fixed<N2, K2> b) {
    return a = a - b;
}

template<auto N1, int K>
Fixed<N1, K> &operator-=(Fixed<N1, K> &a, int b) {
    return a = a - b;
}

template<auto N1, int K>
Fixed<N1, K> &operator-=(Fixed<N1, K> &a, float b) {
    return a = a - b;
}

template<auto N1, int K>
Fixed<N1, K> &operator-=(float a, Fixed<N1, K> &b) {
    return b = a - b;
}


template<auto N1, int K>
Fixed<N1, K> &operator-=(Fixed<N1, K> &a, double b) {
    return a = a - b;
}

template<auto N1, int K>
Fixed<N1, K> &operator-=(double a, Fixed<N1, K> &b) {
    return b = a - b;
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> &operator*=(Fixed<N1, K> &a, Fixed<N2, K2> b) {
    return a = a * b;
}

template<auto N1, int K>
Fixed<N1, K> &operator*=(Fixed<N1, K> &a, int b) {
    return a = a * b;
}

template<auto N1, int K>
Fixed<N1, K> &operator*=(Fixed<N1, K> &a, float b) {
    return a = a * b;
}

template<auto N1, int K>
Fixed<N1, K> &operator*=(Fixed<N1, K> &a, double b) {
    return a = a * b;
}

template<auto N1, int K, auto N2, int K2>
Fixed<N1, K> &operator/=(Fixed<N1, K> &a, Fixed<N2, K2> b) {
    return a = a / b;
}

template<auto N1, int K>
Fixed<N1, K> &operator/=(Fixed<N1, K> &a, int b) {
    return a = a / b;
}

template<auto N1, int K>
Fixed<N1, K> &operator/=(Fixed<N1, K> &a, float b) {
    return a = a / b;
}

template<auto N1, int K>
Fixed<N1, K> &operator/=(Fixed<N1, K> &a, double b) {
    return a = a / b;
}

template<auto N1, int K>
Fixed<N1, K> operator-(Fixed<N1, K> x) {
    return Fixed<N1, K>::from_raw(-x.v);
}

template<auto N1, int K>
std::ostream &operator<<(std::ostream &out, Fixed<N1, K> x) {
    return out << x.v / (double) (1ULL << K);
}
