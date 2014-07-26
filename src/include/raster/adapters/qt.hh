#ifndef RASTER_ADAPTERS_QT_HH
#define RASTER_ADAPTERS_QT_HH

#include <QImage>
#include "../map.hh"

namespace raster {
    class const_qimage_adapter {
        const QImage *image;
    public:
        const_qimage_adapter(const QImage &img) : image(&img) {}
        
        QRgb get (int y, int x) const {
            return image->pixel(x, y);
        }
        
        int rows() const {
            return image->height();
        }
        
        int columns() const {
            return image->width();
        }
    };
    
    template <typename T = QRgb>
    class qimage_adapter {
        QImage *image;
    public:
        qimage_adapter(QImage &img) : image(&img) {}

        T get (int y, int x) const {
            return reinterpret_cast<T*>(image->scanLine(y))[x];
        }
        
        T set (int y, int x, T value) const {
            return reinterpret_cast<T*>(image->scanLine(y))[x] = value;
        }
        
        int rows() const {
            return image->height();
        }
        
        int columns() const {
            return image->width();
        }
    };
    
    inline const_qimage_adapter as_raster(QImage const& image) {
        return const_qimage_adapter(image);
    }
    
    template <typename T = QRgb>
    qimage_adapter<T> as_raster(QImage &image) {
        return qimage_adapter<T>(image);
    }

    template <typename M>
    auto qgray_raster(M &&m) -> decltype(mapped((int(*)(QRgb))qGray, std::forward<M>(m))) {
        return mapped((int(*)(QRgb))qGray, std::forward<M>(m));
    }

    inline auto qgray_raster(QImage &img) -> decltype(mapped((int(*)(QRgb))qGray, qimage_adapter<>(img))) {
        return mapped((int(*)(QRgb))qGray, qimage_adapter<>(img));
    }
    
    inline auto qgray_raster(const QImage &img) -> decltype(mapped((int(*)(QRgb))qGray, const_qimage_adapter(img))) {
        return mapped((int(*)(QRgb))qGray, const_qimage_adapter(img));
    }
}

#endif /* #ifndef RASTER_ADAPTERS_QT_HH */
