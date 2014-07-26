#include <QImage>
#include "edgeworker.hh"
#include <raster/adapters/qt.hh>
#include <raster/util/facade.hh>
#include <raster/flt/canny.hh>
#include <raster/flt/gaussian.hh>
#include <raster/wrap.hh>

mm::EdgeWorker::EdgeWorker(int mint,
                           int maxt,
                           int blur,
                           QVector<std::pair<int,int>> &list,
                           raster::view2d<std::vector<bool>,int> &bv,
                           QImage &image)
    : minThresh(mint),
      maxThresh(maxt),
      blurRadius(blur),
      edgeList(&list), 
      edgeMap(&bv),
      in(&image), 
      py(),
      px()
      {}

void mm::EdgeWorker::work() {
    class observer : public raster::facade<raster::view2d<std::vector<bool>,int>*,observer> {
        QVector<std::pair<int,int>> *edges;
        int *px;
        int *py;
    public:
        observer(raster::view2d<std::vector<bool>,int> &t, QVector<std::pair<int,int>> &e, int &x, int &y) 
         : raster::facade<raster::view2d<std::vector<bool>,int>*,observer>(&t), 
           edges(&e), 
           px(&x), 
           py(&y) 
           {}
    private:
        friend raster::facade<raster::view2d<std::vector<bool>,int>*,observer>;
        void write(int y, int x) { 
            edges->append(std::make_pair(x, y)); 
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
        raster::canny(raster::convolved(&k, raster::extend(raster::qgray_raster(*in))),
                      observer(*edgeMap, *edgeList, px, py),
                      [](double x, double y){return std::sqrt(x * x + y * y);},
                      minThresh, maxThresh);
    } else {
        raster::canny(raster::extend(raster::qgray_raster(*in)),
                      observer(*edgeMap, *edgeList, px, py),
                      [](double x, double y){return std::sqrt(x * x + y * y);},
                      minThresh, maxThresh);
    }
    
    emit finished();
}

int mm::EdgeWorker::currentX() const { return px; }
int mm::EdgeWorker::currentY() const { return py; }