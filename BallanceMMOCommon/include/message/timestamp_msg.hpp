#ifndef BALLANCEMMOSERVER_TIMESTAMP_MSG_HPP
#define BALLANCEMMOSERVER_TIMESTAMP_MSG_HPP
#include "message.hpp"

namespace bmmo {
    class timestamp_t {
        uint32_t v[2]{};
    public:
        operator int64_t() const {
            return (int64_t)v[0] << 32 | v[1];
        }
        timestamp_t(int64_t t = 0) {
            *this = t;
        }
        void operator=(int64_t t) {
            v[0] = t >> 32;
            v[1] = (uint32_t) (t & UINT32_MAX);
        }
        int64_t operator+(const int64_t t) const {
            return ((int64_t)v[0] << 32 | v[1]) + t;
        }
        int64_t operator-(const int64_t t) const {
            return ((int64_t)v[0] << 32 | v[1]) - t;
        }
        void operator+=(const int64_t t) {
            *this = *this + t;
        }
        bool operator<(const timestamp_t& t) const {
            return ((int64_t)v[0] << 32 | v[1]) < ((int64_t)t.v[0] << 32 | t.v[1]);
        }
        bool operator==(const int64_t t) const {
            return ((int64_t)v[0] << 32 | v[1]) == t;
        }
        bool is_zero() const {
            return v[0] == 0 && v[1] == 0;
        }
    };

    typedef struct message<timestamp_t, Timestamp> timestamp_msg;
}

#endif //BALLANCEMMOSERVER_TIMESTAMP_MSG_HPP
