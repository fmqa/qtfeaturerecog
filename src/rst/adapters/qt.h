#ifndef RST_ADAPTERS_QT_H
#define RST_ADAPTERS_QT_H

#include <QImage>
#include "compose.h"

namespace rst {
    class qimageadp {
        const QImage *image;
    public:
        qimageadp (const QImage *img) : image(img) {}
        
        QRgb operator() (int y, int x) const {
            return image->pixel(x, y);
        }
        
        int rows() const {
            return image->height();
        }
        
        int columns() const {
            return image->width();
        }
    };
    
    class qgrayimageadp : public qimageadp {
    public:
        qgrayimageadp(const QImage *img) : qimageadp(img) {}
        QRgb operator() (int y, int x) const {
            return qGray(qimageadp::operator()(y, x));
        }
    };
    
    template <typename T = QRgb>
    class qmutableimageadp {
        QImage *image;
    public:
        qmutableimageadp (QImage *img) : image(img) {}
        
        T& operator() (int y, int x) const {
            return static_cast<T*>(image->scanLine(y))[x];
        }
        
        int rows() const {
            return image->height();
        }
        
        int columns() const {
            return image->width();
        }
    };
}

#endif /* #ifndef RST_ADAPTERS_QT_H */
