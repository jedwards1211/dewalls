#ifndef SIGNUM_H
#define SIGNUM_H

template <typename T> int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif // SIGNUM_H

