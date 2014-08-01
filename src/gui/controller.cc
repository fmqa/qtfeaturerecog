#include <QAction>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QStatusBar>
#include <QDebug>
#include "partials/imagetabs.hh"
#include "controller.hh"
#include <raster/util/view2d.hh>
#include "workers/edgeworker.hh"
#include "workers/circleworker.hh"
#include "partials/edgeoptions.hh"
#include "partials/circleoptions.hh"

static bool load(const QString &img, mm::controller::imagedata &idata) {
    if (!idata.image.load(img)) {
        return false;
    }
    idata.path = img;
    return true;
}

mm::controller::controller() : QObject(), ui(), state(), thread(), timer() {
    bind();
    ui.show();
}

void mm::controller::bind() {
    connect(ui.actions.open, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui.actions.save, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui.actions.copy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui.actions.exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.actions.viewsrc, SIGNAL(triggered()), this, SLOT(viewsrc()));
    connect(ui.actions.viewedges, SIGNAL(triggered()), this, SLOT(viewedges()));
    connect(ui.actions.viewtransfm, SIGNAL(triggered()), this, SLOT(viewtransfm()));
    connect(ui.actions.fullscr, SIGNAL(triggered(bool)), this, SLOT(fullscr(bool)));
    connect(ui.controls.detectedges, SIGNAL(clicked()), this, SLOT(detectedges()));
    connect(ui.controls.applytransfm, SIGNAL(clicked()), this, SLOT(applytransfm()));
    connect(ui.controls.stop, SIGNAL(clicked()), this, SLOT(stop()));
}

void mm::controller::open() {
    QString file = ui.requestImage();
    
    if (file.isEmpty()) {
        return;
    }
    
    if (!load(file, state.source)) {
        ui.alertInvalidSourceImage(file);
        return;
    }
    
    ui.images->setTabPixmap(ImageTabs::srctab, QPixmap::fromImage(state.source.image));
    ui.images->setTabPixmap(ImageTabs::edgetab, QPixmap());
    ui.images->setTabPixmap(ImageTabs::transformtab, QPixmap());
    ui.images->setTabPixmap(ImageTabs::histogramtab, QPixmap());
    state.edges.image = QImage();
    state.edges.list = QVector<std::pair<int,int>>();
    state.edges.bitmap = std::vector<bool>();
    state.transfm.image = QImage();
    state.transfm.features = 0;
    ui.images->switchTab(ImageTabs::srctab);
}

void mm::controller::save() {
    ui.images->saveCurrentTabImage();
}

void mm::controller::copy() {
    ui.images->currentTabImageToClipboard();
}

void mm::controller::close() {
    ui.close();
}

void mm::controller::viewsrc() {
    ui.images->switchTab(ImageTabs::srctab);
}

void mm::controller::viewedges() {
    ui.images->switchTab(ImageTabs::edgetab);
}

void mm::controller::viewtransfm() {
    ui.images->switchTab(ImageTabs::transformtab);
}

void mm::controller::fullscr(bool checked) {
    if (checked) {
        ui.showFullScreen();
    } else {
        ui.showNormal();
    }
}

void mm::controller::detectedges() {
    stop();
    
    thread = new QThread(this);
    timer = new QTimer(this);
    
    mm::EdgeWorker *worker = new mm::EdgeWorker(
        ui.options.edges->minThreshold(),
        ui.options.edges->maxThreshold(),
        ui.options.edges->blurEnabled() ? ui.options.edges->blurRadius() : 0,
        state.source.image);
    
    QObject::connect(thread, &QThread::started, worker, &mm::EdgeWorker::work);
    
    QObject::connect(worker, &mm::EdgeWorker::started, &ui, &mm::Ui::disableControls);
    
    QObject::connect(worker, &mm::EdgeWorker::started, [this]() {
        ui.images->setTabPixmap(mm::ImageTabs::edgetab, QPixmap());
        ui.images->switchTab(mm::ImageTabs::edgetab);
    });
    
    QObject::connect(worker, &mm::EdgeWorker::finished, &ui, &mm::Ui::enableControls);
    
    QObject::connect(worker, &mm::EdgeWorker::finished, [this,worker]() {
        state.edges.image = worker->result();
        state.edges.list = worker->list();
        state.edges.bitmap = worker->bitmap();
        ui.statusBar()->showMessage(QObject::tr("%1 edges detected").arg(state.edges.list.size()));
        ui.images->setTabPixmap(mm::ImageTabs::edgetab, QPixmap::fromImage(state.edges.image));
        ui.images->switchTab(mm::ImageTabs::edgetab);
    });
    
    QObject::connect(worker, &mm::EdgeWorker::finished, thread, &QThread::quit);
    
    QObject::connect(timer, &QTimer::timeout, [this,worker](){
        if (thread->isRunning()) {
            ui.images->setTabPixmap(mm::ImageTabs::edgetab, QPixmap::fromImage(worker->result()));
            ui.statusBar()->showMessage(QObject::tr("%1 edges detected").arg(worker->count()));
        }
    });
    
    worker->moveToThread(thread);
    thread->start();
    timer->start(100);
}

void mm::controller::applycircletransfm() {
    if (state.edges.list.isEmpty()) {
        ui.alertExtractEdges();
        return;
    }
    
    stop();
    
    thread = new QThread(this);
    timer = new QTimer(this);
    
    mm::CircleWorker *worker = new mm::CircleWorker(
            ui.options.circles->minRadius(),
            ui.options.circles->maxRadius(),
            ui.options.circles->minScore(),
            ui.options.circles->maxScore(),
            state.edges.list,
            state.source.image,
            ui.options.circles->getColor());
    
    QObject::connect(thread, &QThread::started, worker, &mm::CircleWorker::work);
    
    QObject::connect(worker, &mm::CircleWorker::started, &ui, &mm::Ui::disableControls);
    
    QObject::connect(worker, &mm::CircleWorker::started, [this]() {
        ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap());
        ui.images->switchTab(mm::ImageTabs::transformtab);
    });
    
    QObject::connect(worker, &mm::CircleWorker::finished, &ui, &mm::Ui::enableControls);
    
    QObject::connect(worker, &mm::CircleWorker::finished, [this,worker]() {
        state.transfm.image = worker->result();
        state.transfm.features = worker->count();
        ui.statusBar()->showMessage(QObject::tr("%1 circles detected").arg(state.transfm.features));
        ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap::fromImage(state.transfm.image));
    });
    
    QObject::connect(worker, &mm::CircleWorker::finished, thread, &QThread::quit);
    
    QObject::connect(timer, &QTimer::timeout, [this,worker](){
        if (thread->isRunning()) {
            ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap::fromImage(worker->result()));
            ui.statusBar()->showMessage(QObject::tr("%1 circles detected").arg(worker->count()));
        }
    });
    
    worker->moveToThread(thread);
    thread->start();
    timer->start(100);
}

void mm::controller::applyellipsetransfm() {
}

void mm::controller::applytransfm() {
    if (ui.transfmtabs->currentWidget() == ui.options.circles) {
        applycircletransfm();
    } else {
        applyellipsetransfm();
    }
}

void mm::controller::stop() {
    if (thread && thread->isRunning()) {
        thread->quit();
    }
    if (timer && timer->isActive()) {
        timer->stop();
    }
}

