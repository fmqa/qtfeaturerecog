#ifndef ELLIPSEWORKER_HH
#define ELLIPSEWORKER_HH

#include <QObject>
#include <QVector>
#include <raster/util/view2d.hh>

class QImage;

namespace mm {
    class ellipse_detector;
    
    class EllipseWorker : public QObject {
        Q_OBJECT
    public:
        EllipseWorker(const ellipse_detector &,
                      const QVector<std::pair<int,int>> &,
                      const raster::view2d<std::vector<bool>,int> &,
                      QImage &,
                      QImage &);
        int currentX() const;
        int currentY() const;
        int ellipseCount() const;
    public slots:
        void work();
    signals:
        void started();
        void finished();
    private:
        const ellipse_detector *model;
        const QVector<std::pair<int,int>> *edgeList;
        const raster::view2d<std::vector<bool>,int> *edgeMap;
        QImage *in;
        QImage *out;
        int px;
        int py;
        int count;
    };
}

#endif /* #ifndef ELLIPSEWORKER_HH */
