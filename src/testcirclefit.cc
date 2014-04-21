#include <iostream>
#include <iomanip>
#include <cassert>
#include "image/circlefit.h"
#include "monocircle.xbm"

template <typename T>
class range {
public:
    class iterator {
        friend class range;
    public:
        T operator *() const { return i_; }
        const iterator &operator++ () { ++i_; return *this; }
        iterator operator++ (int) { iterator copy(*this); ++i_; return copy; }

        bool operator == (const iterator &other) const { return i_ == other.i_; }
        bool operator != (const iterator &other) const { return i_ != other.i_; }

    protected:
        iterator (T start) : i_ (start) { }

    private:
        T i_;
    };

    iterator begin () const { return begin_; }
    iterator end () const { return end_; }
    range (T begin, T end) : begin_(begin), end_(end) {}
private:
    iterator begin_;
    iterator end_;
};

static int acc[14][monocircle_height][monocircle_width];

static int & param (int x, int y, int r) {
    assert (x < 64 && y < 64);
    return acc[r - 3][y][x];
}

static int circlepget (int x, int y) {
    assert (x < 64 && y < 64);
    return (monocircle_bits[(y * monocircle_width + x) / 8] & (1 << ((y * monocircle_width + x) % 8))) ? 1 : 0;
}

int main (int argc, char *argv[]) {
    range<int> radii(3, 17);
    img::circlefit (radii.begin(), radii.end(), 64, 64, circlepget, param);
    
    for (int r = 3; r <= 16; ++r) {
        std::cout << "r = " << r << " score matrix" << std::endl;
        for (int i = 0; i < 64; ++i) {
            for (int j = 0; j < 64; ++j) {
                if (acc[r - 3][i][j] >= 360) {
                    std::cerr << "CANDIDATE: " << "x=" << j << ", y=" << i << ", r=" << r 
                              << ", score=" << acc[r - 3][i][j] << std::endl;
                }
                std::cout << std::setw(3) << acc[r - 3][i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    return 0;
}

