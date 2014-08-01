#ifndef ELLIPSEWORKER_HH
#define ELLIPSEWORKER_HH

#include <utility>
#include <vector>
#include <QObject>
#include <QVector>
#include <QImage>
#include "../../util/ellipsehough.hh"


namespace mm {
    class EllipseWorker : public QObject {
        Q_OBJECT
    public:
        EllipseWorker(const ellipserange &r,
                      QVector<std::pair<int,int>>,
                      std::vector<bool>,
                      QImage);
        int currentX() const;
        int currentY() const;
        int count() const;
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
        int px;
        int py;
        int num;
    };
}

#endif /* #ifndef ELLIPSEWORKER_HH */
