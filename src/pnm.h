#ifndef PNM_H
#define PNM_H

#include <istream>
#include <ostream>
#include <cstdio>

namespace pnm {
    template <typename F>
    bool readpnm (std::istream &is, F f) 
    {
        char header[5];
        
        is.getline (header, 5);
        
        if (header[0] == 'P' && (header[1] == '1' || 
                                 header[1] == '2' || 
                                 header[1] == '3' ||
                                 header[1] == '4' ||
                                 header[1] == '5' ||
                                 header[1] == '6')) 
        {
            char line[64];
            size_t width, height, maxcolor;
            
            do {
                is.getline (line, 64);
            } while (line[0] == '#');
            
            std::sscanf (line, "%ld %ld", &width, &height);
            
            do {
                is.getline (line, 64);
            } while (line[0] == '#');
            
            std::sscanf (line, "%ld", &maxcolor);
            
            auto result = f(header[1], width, height, maxcolor);
            bool flag = result.first;
            auto os = result.second;
            
            if (flag) {
                if (header[1] == '1' || header[1] == '2' || header[1] == '3') {
                    int value;
                    while (is.good()) {
                        is >> value;
                        *os++ = (char)value;
                    }
                } else {
                    while (is.good()) {
                        *os++ = is.get();
                    }
                }
                return is.bad();
            }
        }
        
        return false;
    }

    template <typename InputIterator>
    bool writepnm (std::ostream &os,
                   char type,
                   size_t width,
                   size_t height,
                   size_t maxcolor,
                   InputIterator begin, 
                   InputIterator end)
    {
        os << 'P' << type << std::endl;
        os << width << ' ' << height << std::endl;
        os << maxcolor << std::endl;
        
        if (type == '1' || type == '2' || type == '3') {
            for (; begin != end; ++begin) {
                os << *begin;
            }
            return os.bad();
        } else if (type == '4' || type == '5' || type == '6') {
            for (; begin != end; ++begin) {
                os << (char)*begin;
            }
            return os.bad();
        } else {
            return false;
        }
    }
}

#endif /* #ifndef PNM_H */
