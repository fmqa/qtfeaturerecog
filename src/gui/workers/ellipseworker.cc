#include <QImage>
#include <QPainter>
#include "ellipseworker.hh" 
#include <raster/types.hh>
#include "../model/ellipse_detector.hh"
#include <raster/util/view2d.hh>

mm::EllipseWorker::EllipseWorker(const ellipse_detector &mdl,
                                 const QVector<std::pair<int,int>> &edgeList,
                                 const raster::view2d<std::vector<bool>,int> &edgeMap,
                                 QImage &imgIn,
                                 QImage &imgOut)
    : model(&mdl),
      edgeList(&edgeList),
      edgeMap(&edgeMap),
      in(&imgIn),
      out(&imgOut),
      px(),
      py(),
      count()
      {}
      
void mm::EllipseWorker::work() {
    emit started();
    
    QPainter painter(out);
    painter.setPen(qRgb(255,0,0));
    
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
                 });
    emit finished();
}

int mm::EllipseWorker::currentX() const { return px; }
int mm::EllipseWorker::currentY() const { return py; }
int mm::EllipseWorker::ellipseCount() const { return count; }
