#ifndef GUI_CONTROLLER_HH
#define GUI_CONTROLLER_HH

#include <memory>
#include <utility>
#include <vector>
#include <functional>

#include <QObject>
#include <QImage>
#include <QString>
#include <QThread>
#include <QTimer>

#include "ui.hh"

namespace mm {
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
        };
    public:
        explicit controller();
    private:
        void bind();
        void applycircletransfm();
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
    private:
        Ui ui;
        statedata state;
        QThread *thread;
        QTimer *timer;
    };
}

#endif /* #ifndef GUI_CONTROLLER_HH */

