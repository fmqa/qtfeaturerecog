#include <QAction>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QStatusBar>
#include <QMouseEvent>
#include <QLabel>
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

static void reset(mm::Ui &ui, mm::controller::statedata &state) {
    ui.images->setTabPixmap(mm::ImageTabs::srctab, QPixmap::fromImage(state.source.image));
    ui.images->setTabPixmap(mm::ImageTabs::edgetab, QPixmap());
    ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap());
    ui.images->setTabPixmap(mm::ImageTabs::histogramtab, QPixmap());
    state.edges.image = QImage();
    state.edges.list = QVector<std::pair<int,int>>();
    state.edges.bitmap = std::vector<bool>();
    state.transfm.image = QImage();
    state.transfm.features = 0;
    state.histogram = QImage();
    state.histacc = QVector<int>();
    ui.images->switchTab(mm::ImageTabs::srctab);
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
    connect(&ui, SIGNAL(fileDropped(const QString &)), this, SLOT(loadDroppedFile(const QString &)));
    connect(&ui, SIGNAL(imageDropped(const QImage &)), this, SLOT(loadDroppedImage(const QImage &)));
    ui.images->widget(ImageTabs::histogramtab).setMouseTracking(true);
    ui.images->widget(ImageTabs::histogramtab).installEventFilter(this);
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
    
    reset(ui, state);
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
    if (state.source.image.isNull()) {
        ui.alertEmptySourceImage();
        return;
    }
    
    stop();
    
    thread = new QThread(this);
    timer = new QTimer(this);
    
    workers.edges = new EdgeWorker(
        ui.options.edges->minThreshold(),
        ui.options.edges->maxThreshold(),
        ui.options.edges->blurEnabled() ? ui.options.edges->blurRadius() : 0,
        state.source.image);
    
    connect(thread, SIGNAL(started()), workers.edges, SLOT(work()));
    connect(workers.edges, SIGNAL(started()), &ui, SLOT(disableControls()));
    connect(workers.edges, SIGNAL(started()), this, SLOT(clearAndSwitchToImageTab()));
    connect(workers.edges, SIGNAL(finished()), &ui, SLOT(enableControls()));
    connect(workers.edges, SIGNAL(finished()), this, SLOT(displayEdgeResults()));
    connect(workers.edges, SIGNAL(finished()), thread, SLOT(quit()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateEdgeProgress()));
    
    workers.edges->moveToThread(thread);
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
    
    workers.circles = new CircleWorker(
            ui.options.circles->minRadius(),
            ui.options.circles->maxRadius(),
            ui.options.circles->minScore(),
            ui.options.circles->maxScore(),
            state.edges.list,
            state.source.image,
            ui.options.circles->getColor(),
            ui.options.circles->histogram());
    
    connect(thread, SIGNAL(started()), workers.circles, SLOT(work()));
    connect(workers.circles, SIGNAL(started()), &ui, SLOT(disableControls()));
    connect(workers.circles, SIGNAL(started()), this, SLOT(clearAndSwitchToTransfmTab()));
    connect(workers.circles, SIGNAL(finished()), &ui, SLOT(enableControls()));
    connect(workers.circles, SIGNAL(finished()), this, SLOT(updateCircleProgress()));
    if (ui.options.circles->histogram()) {
        connect(workers.circles, SIGNAL(finished()), this, SLOT(updateHistogram()));
    }
    connect(workers.circles, SIGNAL(finished()), thread, SLOT(quit()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCircleProgress()));
    
    workers.circles->moveToThread(thread);
    thread->start();
    timer->start(100);
}

void mm::controller::applyellipsetransfm() {
}

void mm::controller::clearAndSwitchToImageTab() {
    ui.images->setTabPixmap(ImageTabs::edgetab, QPixmap());
    ui.images->switchTab(ImageTabs::edgetab);
}

void mm::controller::clearAndSwitchToTransfmTab() {
    ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap());
    ui.images->switchTab(mm::ImageTabs::transformtab);
}

void mm::controller::displayEdgeResults() {
    state.edges.image = workers.edges->result();
    state.edges.list = workers.edges->list();
    state.edges.bitmap = workers.edges->bitmap();
    ui.statusBar()->clearMessage();
    ui.statusBar()->showMessage(tr("%1 edges detected").arg(state.edges.list.size()));
    ui.images->setTabPixmap(ImageTabs::edgetab, QPixmap::fromImage(state.edges.image));
    ui.images->switchTab(ImageTabs::edgetab);
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
        thread->terminate();
    }
    if (timer && timer->isActive()) {
        timer->stop();
    }
    ui.enableControls();
}

void mm::controller::updateEdgeProgress() {
    if (thread->isRunning()) {
        ui.images->setTabPixmap(mm::ImageTabs::edgetab, QPixmap::fromImage(workers.edges->result()));
        ui.statusBar()->clearMessage();
        ui.statusBar()->showMessage(tr("%1 edges detected").arg(workers.edges->count()));
    }
}

void mm::controller::updateCircleProgress() {
    state.transfm.image = workers.circles->result();
    state.transfm.features = workers.circles->count();
    ui.statusBar()->clearMessage();
    ui.statusBar()->showMessage(tr("%1 circles detected").arg(state.transfm.features));
    ui.images->setTabPixmap(mm::ImageTabs::transformtab, QPixmap::fromImage(state.transfm.image));
}

void mm::controller::updateHistogram() {
    state.histogram = workers.circles->histogram();
    ui.images->setTabPixmap(ImageTabs::histogramtab, QPixmap::fromImage(state.histogram));
    state.histacc = workers.circles->histAcc();
}

void mm::controller::loadDroppedFile(const QString &file) {
    if (!load(file, state.source)) {
        ui.alertInvalidSourceImage(file);
        return;
    }
    
    reset(ui, state);
}

void mm::controller::loadDroppedImage(const QImage &image) {
    state.source.image = image;
    reset(ui, state);
}

bool mm::controller::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseMove && !state.histacc.empty()) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        int offset = mouseEvent->y() * state.histogram.width() + mouseEvent->x();
        if (offset < state.histacc.size()) {
            ui.coordLabel->setText(tr("(x: %1, y: %2) = Cumulative Score: %3)")
                .arg(mouseEvent->x()).arg(mouseEvent->y()).arg(state.histacc[offset]));
        }
    } else if (event->type() == QEvent::Leave) {
        ui.coordLabel->setText("");
    }
    return QObject::eventFilter(obj, event);
}