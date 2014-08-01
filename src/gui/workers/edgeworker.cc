#include <QImage>
#include "edgeworker.hh"
#include <raster/adapters/qt.hh>
#include <raster/util/facade.hh>
#include <raster/flt/canny.hh>
#include <raster/flt/gaussian.hh>
#include <raster/util/view2d.hh>
#include <raster/wrap.hh>

mm::EdgeWorker::EdgeWorker(int mint,
                           int maxt,
                           int blur,
                           QImage image)
    : minThresh(mint),
      maxThresh(maxt),
      blurRadius(blur),
      lst(), 
      bits(image.height() * image.width(), false),
      in(image), 
      out(in.size(), QImage::Format_RGB32),
      py(),
      px()
      {}

void mm::EdgeWorker::work() {
    class observer : public raster::facade<raster::view2d<std::vector<bool>,int>,observer> {
        QVector<std::pair<int,int>> *edges;
        int *px;
        int *py;
        QImage *outp;
    public:
        observer(raster::view2d<std::vector<bool>,int> t, QVector<std::pair<int,int>> &e, int &x, int &y, QImage &o) 
         : raster::facade<raster::view2d<std::vector<bool>,int>,observer>(t), 
           edges(&e), 
           px(&x), 
           py(&y),
           outp(&o)
           {}
    private:
        friend raster::facade<raster::view2d<std::vector<bool>,int>,observer>;
        void write(int y, int x) { 
            edges->append(std::make_pair(x, y)); 
            outp->setPixel(x, y, qRgb(255, 255, 255));
            if (y > *py) {
                *py = y;
                *px = 0;
            }
            if (x > *px) {
                *px = x;
            }
        }
        void read(int,int) const {}
    };
    
    emit started();
    
    if (blurRadius > 0) {
        std::array<std::array<double,3>,3> k;
        raster::krn::gaussian(&k, blurRadius);
        raster::canny(raster::convolved(&k, raster::extend(raster::qgray_raster(in))),
                      observer(raster::as2d(in.height(), in.width(), bits), lst, px, py, out),
                      [](double x, double y){return std::sqrt(x * x + y * y);},
                      minThresh, maxThresh);
    } else {
        raster::canny(raster::extend(raster::qgray_raster(in)),
                      observer(raster::as2d(in.height(), in.width(), bits), lst, px, py, out),
                      [](double x, double y){return std::sqrt(x * x + y * y);},
                      minThresh, maxThresh);
    }
    
    emit finished();
}

int mm::EdgeWorker::count() const { return lst.size(); }
int mm::EdgeWorker::x() const { return px; }
int mm::EdgeWorker::y() const { return py; }
QImage mm::EdgeWorker::result() const { return out; }
QVector<std::pair<int,int>> mm::EdgeWorker::list() const { return lst; }
std::vector<bool> mm::EdgeWorker::bitmap() const { return bits; }
