#ifndef GUI_EDGEWORKER_HH
#define GUI_EDGEWORKER_HH

#include <utility>
#include <vector>
#include <QObject>
#include <QVector>
#include <QImage>

class QImage;

namespace mm {
    class edge_detector;
    
    class EdgeWorker : public QObject {
        Q_OBJECT
    public:
        EdgeWorker(int,
                   int,
                   int,
                   QImage);
        int x() const;
        int y() const;
        QImage result() const;
        QVector<std::pair<int,int>> list() const;
        std::vector<bool> bitmap() const;
        int count() const;
    public slots:
        void work();
    signals:
        void started();
        void finished();
    private:
        int minThresh;
        int maxThresh;
        int blurRadius;
        QVector<std::pair<int,int>> lst;
        std::vector<bool> bits;
        QImage in, out;
        int py;
        int px;
    };
}

#endif /* #ifndef GUI_EDGEWORKER_HH */
