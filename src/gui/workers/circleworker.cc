#include <QImage>
#include <QPainter>
#include <raster/util/flat_accumulator.hh>
#include <util/circles.hh>
#include "circleworker.hh"

mm::CircleWorker::CircleWorker(int r0, 
                               int r1,
                               int s0,
                               int s1,
                               QVector<std::pair<int,int>> e,
                               QImage io,
                               QColor mrk,
                               mode m)
    : rmin(r0),
      rmax(r1),
      smin(s0),
      smax(s1),
      edges(e),
      out(io),
      marker(mrk),
      circles(0),
      plotmode(m)
      {}
      
void mm::CircleWorker::work() {
    emit started();
    
    
    QVector<int> accumulator(out.height() * out.width(), 0);
    QPainter painter(&out);
    
    painter.setPen(marker);
    
    for (int i = rmin; i < rmax; ++i) {
        switch (plotmode) {
            case trigonometric:
                circles::hough(i, 
                               edges.begin(), 
                               edges.end(),
                               raster::as_constrained_flat_acc(out.height(), out.width(), accumulator),
                               circles::trig(circles::unit.begin(), circles::unit.end()),
                               std::mem_fn(&std::pair<int,int>::first),
                               std::mem_fn(&std::pair<int,int>::second));
                break;
            case bresenham:
                circles::hough(i, 
                               edges.begin(), 
                               edges.end(),
                               raster::as_constrained_flat_acc(out.height(), out.width(), accumulator),
                               circles::bres(),
                               std::mem_fn(&std::pair<int,int>::first),
                               std::mem_fn(&std::pair<int,int>::second));
                break;                
        }
        for (int j = 0; j < accumulator.size(); ++j) {
            if (accumulator[j] >= smin && accumulator[j] <= smax) {
                ++circles;
                int y = j / out.width();
                int x = j % out.width();
                painter.drawEllipse(x - i, y - i, 2 * i, 2 * i);
            }
        }
        accumulator.fill(0);
    }
    
    emit finished();
}

int mm::CircleWorker::count() const { return circles; }
QImage mm::CircleWorker::result() const { return out; }
