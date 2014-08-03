#include <QImage>
#include <QPainter>
#include "ellipseworker.hh" 
#include <raster/types.hh>
#include "../../include/util/circles.hh"
#include <raster/util/view2d.hh>

mm::EllipseWorker::EllipseWorker(const mm::ellipserange &r,
                                 QVector<std::pair<int,int>> edgelst,
                                 std::vector<bool> bv,
                                 QImage img,
                                 QColor m)
    : rng(r),
      lst(edgelst),
      bits(bv),
      io(img),
      marker(m),
      px(),
      py(),
      num()
      {}
      
void mm::EllipseWorker::work() {
    emit started();
    
    QPainter painter(&io);
    painter.setPen(marker);
    
    auto edgemap = raster::as2d(io.height(), io.width(), bits);
    
    mm::ellipsehough(rng, circles::unit.begin(), circles::unit.end(), lst.begin(), lst.end(),
                     [this,&edgemap](int x, int y) -> bool {
                         if (x >= 0 && y >= 0 && y < io.height() && x < io.width()) {
                             return get(edgemap, x, y);
                         }
                         return false;
                     },
                     [&](int cx, int cy, int smin, int smaj, double alpha) {
                        ++num;
                        painter.save();
                        painter.translate(cx, cy);
                        painter.rotate(static_cast<int>(alpha * 180 / M_PI));
                        painter.drawEllipse(-smaj, -smin, smaj * 2, smin * 2);
                        painter.restore();
                     },
                     [this](int x0, int y0, int x1, int y1, int x2, int y2) {
                        if (y0 > py) {
                            py = y0;
                            px = 0;
                        }
                        if (x0 > px) {
                            px = x0;
                        }
                     });
        /*
    model->apply(edgeList->begin(), edgeList->end(),
                 [this](int x, int y) -> bool {
                     if (x >= 0 && y >= 0 && y < in->height() && x < in->width()) {
                          return get(edgeMap, x, y);
                     }
                     return false;
                 },
                 [&](int cx, int cy, int smin, int smaj, double alpha) {
                     ++count;
                     painter.save();
                     painter.translate(cx, cy);
                     painter.rotate(static_cast<int>(alpha * 180 / M_PI));
                     painter.drawEllipse(-smaj, -smin, smaj * 2, smin * 2);
                     painter.restore();
                 },
                 [this](int x0, int y0, int x1, int y1, int x2, int y2) {
                     if (y0 > py) {
                         py = y0;
                         px = 0;
                     }
                     if (x0 > px) {
                         px = x0;
                     }
                 });*/
    emit finished();
}

int mm::EllipseWorker::currentX() const { return px; }
int mm::EllipseWorker::currentY() const { return py; }
int mm::EllipseWorker::count() const { return num; }
QImage mm::EllipseWorker::result() const { return io; }
