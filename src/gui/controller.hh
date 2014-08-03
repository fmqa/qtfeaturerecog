#ifndef GUI_CONTROLLER_HH
#define GUI_CONTROLLER_HH

#include <memory>
#include <utility>
#include <vector>
#include <functional>

#include <QObject>
#include <QImage>
#include <QString>
#include <QVector>
#include <QThread>
#include <QTimer>
#include <QEvent>

#include "ui.hh"

namespace mm {
    class EdgeWorker;
    class CircleWorker;
    class EllipseWorker;
    class ellipserange;
    
    class controller : public QObject {
        Q_OBJECT
    public:
        struct imagedata {
            QImage image;
            QString path;
        };
        
        struct edgedata {
            QImage image;
            QVector<std::pair<int,int>> list;
            std::vector<bool> bitmap;
        };
        
        struct transfmdata {
            QImage image;
            int features;
        };
        
        struct statedata {
            imagedata source;
            edgedata edges;
            transfmdata transfm;
            QImage histogram;
            QVector<int> histacc;
        };
        
        struct workerdata {
            EdgeWorker *edges;
            CircleWorker *circles;
            EllipseWorker *ellipses;
        };
    public:
        explicit controller();
    private:
        void bind();
        void applycircletransfm();
        void ellipsetransfm(const ellipserange &, const QVector<std::pair<int,int>> &);
        void applyellipsetransfm();
    private slots:
        void open();
        void save();
        void copy();
        void close();
        void viewsrc();
        void viewedges();
        void viewtransfm();
        void fullscr(bool);
        void detectedges();
        void applytransfm();
        void stop();
        void updateEdgeProgress();
        void updateCircleProgress();
        void updateEllipseProgress();
        void clearAndSwitchToImageTab();
        void clearAndSwitchToTransfmTab();
        void displayEdgeResults();
        void updateHistogram();
        void loadDroppedFile(const QString &);
        void loadDroppedImage(const QImage &);
    protected:
        bool eventFilter(QObject *, QEvent *) override;
    private:
        Ui ui;
        statedata state;
        workerdata workers;
        QThread *thread;
        QTimer *timer;
    };
}

#endif /* #ifndef GUI_CONTROLLER_HH */

