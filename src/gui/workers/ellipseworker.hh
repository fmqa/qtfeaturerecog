#ifndef ELLIPSEWORKER_HH
#define ELLIPSEWORKER_HH

#include <utility>
#include <vector>
#include <QObject>
#include <QVector>
#include <QImage>
#include <QColor>
#include "../../util/ellipsehough.hh"


namespace mm {
    class EllipseWorker : public QObject {
        Q_OBJECT
    public:
        EllipseWorker(const ellipserange &r,
                      QVector<std::pair<int,int>>,
                      std::vector<bool>,
                      QImage,
                      QColor);
        
        int currentX() const;
        int currentY() const;
        int count() const;
        QImage result() const;
    public slots:
        void work();
    signals:
        void started();
        void finished();
    private:
        ellipserange rng;
        QVector<std::pair<int,int>> lst;
        std::vector<bool> bits;
        QImage io;
        QColor marker;
        int px;
        int py;
        int num;
    };
}

#endif /* #ifndef ELLIPSEWORKER_HH */
