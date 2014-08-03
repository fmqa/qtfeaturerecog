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
                               bool bhist,
                               mode m)
    : rmin(r0),
      rmax(r1),
      smin(s0),
      smax(s1),
      edges(e),
      out(io),
      marker(mrk),
      hist(bhist),
      histacc(),
      num(0),
      plotmode(m),
      circp()
      {}
      
void mm::CircleWorker::work() {
    emit started();
    
    
    QVector<int> accumulator(out.height() * out.width(), 0);
    QPainter painter(&out);
    
    painter.setPen(marker);
    
    if (hist) {
        histacc.resize(out.height() * out.width());
    }
    
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
                ++num;
                int y = j / out.width();
                int x = j % out.width();
                circp.push_back(circles::parametric_circle<int>{x, y, i});
                painter.drawEllipse(x - i, y - i, 2 * i, 2 * i);
            }
            if (hist) {
                histacc[j] += accumulator[j];
            }
        }
        accumulator.fill(0);
    }
    
    if (hist) {
        histimg = QImage(out.size(), QImage::Format_RGB32);
        int max = *std::max_element(histacc.begin(), histacc.end());
        for (int i = 0; i < histacc.size(); ++i) {
            int intensity = (histacc[i] / static_cast<float>(max)) * 255;
            int y = i / histimg.width();
            int x = i % histimg.width();
            histimg.setPixel(x, y, qRgb(intensity, intensity, intensity));
        }
    }
    
    emit finished();
}

int mm::CircleWorker::count() const { return num; }
QImage mm::CircleWorker::result() const { return out; }
QImage mm::CircleWorker::histogram() const { return histimg; }
QVector<int> mm::CircleWorker::histAcc() const { return histacc; }
QVector<circles::parametric_circle<int>> mm::CircleWorker::circles() const { return circp; }