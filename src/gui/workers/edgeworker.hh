#ifndef GUI_EDGEWORKER_HH
#define GUI_EDGEWORKER_HH

#include <utility>
#include <QObject>
#include <QVector>
#include <raster/util/view2d.hh>

class QImage;

namespace mm {
    class edge_detector;
    
    class EdgeWorker : public QObject {
        Q_OBJECT
    public:
        EdgeWorker(int,
                   int,
                   int,
                   QVector<std::pair<int,int>> &,
                   raster::view2d<std::vector<bool>,int> &,
                   QImage &);
        int currentX() const;
        int currentY() const;
    public slots:
        void work();
    signals:
        void started();
        void finished();
    private:
        int minThresh;
        int maxThresh;
        int blurRadius;
        QVector<std::pair<int,int>> *edgeList;
        raster::view2d<std::vector<bool>,int> *edgeMap;
        QImage *in;
        int py;
        int px;
    };
}

#endif /* #ifndef GUI_EDGEWORKER_HH */
