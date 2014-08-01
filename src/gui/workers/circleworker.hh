#ifndef GUI_CIRCLEWORKER_HH
#define GUI_CIRCLEWORKER_HH

#include <utility>
#include <QObject>
#include <QImage>
#include <QVector>
#include <QColor>

namespace mm {
    class CircleWorker : public QObject {
        Q_OBJECT
    public:
        enum mode { trigonometric, bresenham };
        
        CircleWorker(int, 
                     int,
                     int,
                     int,
                     QVector<std::pair<int,int>>,
                     QImage,
                     QColor,
                     mode = trigonometric);
        int count() const;
        QImage result() const;
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
        QVector<std::pair<int,int>> edges;
        QImage out;
        QColor marker;
        int circles;
        mode plotmode;
    };
}

#endif /* #ifndef GUI_CIRCLEWORKER_HH */
