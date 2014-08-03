#ifndef GUI_CIRCLEWORKER_HH
#define GUI_CIRCLEWORKER_HH

#include <utility>
#include <QObject>
#include <QImage>
#include <QVector>
#include <QColor>
#include "../../include/util/circles.hh"

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
                     bool = false,
                     mode = trigonometric);
        int count() const;
        QImage result() const;
        QImage histogram() const;
        QVector<int> histAcc() const;
        QVector<circles::parametric_circle<int>> circles() const;
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
        bool hist;
        QVector<int> histacc;
        QImage histimg;
        int num;
        mode plotmode;
        QVector<circles::parametric_circle<int>> circp;
    };
}

#endif /* #ifndef GUI_CIRCLEWORKER_HH */
