#ifndef GUI_CIRCLEWORKER_HH
#define GUI_CIRCLEWORKER_HH

#include <utility>
#include <QObject>

class QImage;
template <typename T> class QVector;

namespace mm {
    class CircleWorker : public QObject {
        Q_OBJECT
    public:
        enum mode { trigonometric, bresenham };
        
        CircleWorker(int, 
                     int,
                     int,
                     int,
                     const QVector<std::pair<int,int>> &,
                     QImage &,
                     QImage &,
                     mode = trigonometric);
        int count() const;
    public slots:
        void work();
    signals:
        void started();
        void finished();
    private:
        int rmin;
        int rmax;
        int smin;
        int smax;
        const QVector<std::pair<int,int>> *edges;
        QImage *in;
        QImage *out;
        int circles;
        mode plotmode;
    };
}

#endif /* #ifndef GUI_CIRCLEWORKER_HH */
