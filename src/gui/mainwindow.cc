#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>

#include <QLabel>
#include <QPixmap>

#include <QPainter>

#include <QThread>
#include <QTimer>

#include <raster/adapters/qt.hh>
#include <raster/util/view2d.hh>
#include <raster/util/facade.hh>

#include "partials/edgeoptions.hh"
#include "partials/circleoptions.hh"
#include "partials/ellipseoptions.hh"
#include "partials/imagetabs.hh"
#include "modeladapter.hh"
#include "mainwindow.hh"


#include "workers/edgeworker.hh"
#include "workers/circleworker.hh"
#include "workers/ellipseworker.hh"

#include <QDebug>
#include <QCoreApplication>

mm::MainWindow::MainWindow() {
    createActions();
    createMenus();
    createToolBars();
    
    QVBoxLayout *toolboxLayout = new QVBoxLayout;
    
    toolboxLayout->addWidget(statusLabel = new QLabel(tr("No file loaded")));
    statusLabel->setAlignment(Qt::AlignCenter);
    toolboxLayout->addWidget(edgeOptions = new EdgeOptions);
    toolboxLayout->addWidget(edgeCountLabel = new QLabel(tr("0 edges detected")));
    edgeCountLabel->setAlignment(Qt::AlignCenter);
    connect(edgeOptions, SIGNAL(changed()), this, SLOT(onEdgeOptionChange()));
    
    optionTabWidget = new QTabWidget;
    optionTabWidget->addTab(circleOptions = new CircleOptions, tr("Circles"));
    optionTabWidget->addTab(ellipseOptions = new EllipseOptions, tr("Ellipses"));
    
    toolboxLayout->addWidget(optionTabWidget);
    
    toolboxLayout->addWidget(featureCountLabel = new QLabel(tr("0 features detected")));
    featureCountLabel->setAlignment(Qt::AlignCenter);
    
    toolboxLayout->addWidget(showEdgesButton = new QPushButton(tr("Edge Map")));
    connect(showEdgesButton, SIGNAL(clicked()), this, SLOT(extractEdgesAndDisplay()));
    
    {
        QWidget *container = new QWidget;
        QHBoxLayout *hbox = new QHBoxLayout;
        
        hbox->addWidget(startTransformButton = new QPushButton(tr("Detect")));
        connect(startTransformButton, SIGNAL(clicked()), this, SLOT(dispatchExtractionMethod()));
        
        hbox->addWidget(stopButton = new QPushButton(tr("Stop")));
        stopButton->setEnabled(false);
        connect(stopButton, SIGNAL(clicked()), this, SLOT(stopProcessing()));
        
        container->setLayout(hbox);
        toolboxLayout->addWidget(container);
    }
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    
    QWidget *toolbox = new QWidget;
    toolbox->setLayout(toolboxLayout);
    mainLayout->addWidget(toolbox);
    
    imageTabsWidget = new ImageTabs;
    
    mainLayout->addWidget(imageTabsWidget);
    
    QWidget *content = new QWidget;
    content->setLayout(mainLayout);
    
    setCentralWidget(content);
    
    model_to_options(ellipseModel, *ellipseOptions);
}

void mm::MainWindow::open() {
    sourceImagePath = QFileDialog::getOpenFileName(this);
    if (!sourceImagePath.isEmpty()) {
        loadSourceImage();
    }
    imageTabsWidget->switchTab(ImageTabs::srctab);
}

void mm::MainWindow::save() {
    imageTabsWidget->saveCurrentTabImage();
}

void mm::MainWindow::copyToClipboard() {
    imageTabsWidget->currentTabImageToClipboard();
}

void mm::MainWindow::createActions() {
    openAct = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an image file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    
    saveAct = new QAction(QIcon::fromTheme("document-save"), tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the active image"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    
    copyAct = new QAction(QIcon::fromTheme("edit-copy"), tr("&Copy..."), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the active image to clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit HoughStudio"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
    
    viewSourceImageAct = new QAction(tr("O&riginal image"), this);
    viewSourceImageAct->setStatusTip(tr("Show the original image"));
    connect(viewSourceImageAct, SIGNAL(triggered()), this, SLOT(displaySourceImage()));
    
    viewEdgeImageAct = new QAction(tr("E&dge Map"), this);
    viewEdgeImageAct->setStatusTip(tr("Show the edge map of the image"));
    connect(viewEdgeImageAct, SIGNAL(triggered()), this, SLOT(displayEdgeImage()));

    viewTransformedImageAct = new QAction(tr("T&ransformed View"), this);
    viewTransformedImageAct->setStatusTip(tr("Show the transformed image"));
    connect(viewTransformedImageAct, SIGNAL(triggered()), this, SLOT(displayTransformedImage()));    
    
    activateFullScreenAct = new QAction(tr("F&ullscreen Mode"), this);
    activateFullScreenAct->setStatusTip(tr("Activate the full-screen mode"));
    activateFullScreenAct->setCheckable(true);
    connect(activateFullScreenAct, SIGNAL(triggered(bool)), this, SLOT(activateFullScreen(bool)));
}

void mm::MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(copyAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(viewSourceImageAct);
    viewMenu->addAction(viewEdgeImageAct);
    viewMenu->addAction(viewTransformedImageAct);
    viewMenu->addSeparator();
    viewMenu->addAction(activateFullScreenAct);
}

void mm::MainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
}

void mm::MainWindow::loadSourceImage() {
    resetState();
    if (!sourceImage.load(sourceImagePath)) {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("HoughStudio error"),
                           "Error loading image file: " + sourceImagePath + " - not a valid image",
                           0, this);
        msgBox.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    circleOptions->setMaxRadiusBound(std::min(sourceImage.width(), sourceImage.height()));
    statusLabel->setText("Loaded: " + sourceImagePath);
    imageTabsWidget->setTabPixmap(ImageTabs::srctab, QPixmap::fromImage(sourceImage));
}

void mm::MainWindow::displaySourceImage() {
    /*if (sourceImage.isNull()) {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("HoughStudio error"),
                           "No image loaded",
                           0, this);
        msgBox.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }*/
    imageTabsWidget->switchTab(ImageTabs::srctab);
}

void mm::MainWindow::displayEdgeImage() {
    imageTabsWidget->switchTab(ImageTabs::edgetab);
}

void mm::MainWindow::displayTransformedImage() {
    /*if (transformedImage.isNull()) {
        return;
    }*/
    //transformedImageWidget->setPixmap(QPixmap::fromImage(transformedImage));
    //transformedImageWidget->adjustSize();
    imageTabsWidget->switchTab(ImageTabs::transformtab);
}

void mm::MainWindow::extractEdgesAndDisplay() {
    if (sourceImage.isNull()) {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("HoughStudio error"),
                           "No image loaded",
                           0, this);
        msgBox.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    
    displayEdgeImage();
    if (edgeImage.isNull()) {
        extractEdges();
        fillEdgeImage();
    }
    
    imageTabsWidget->setTabPixmap(ImageTabs::edgetab, QPixmap::fromImage(edgeImage));
}

void mm::MainWindow::extractCirclesAndDisplay() {
    if (sourceImage.isNull()) {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("HoughStudio error"),
                           "No image loaded",
                           0, this);
        msgBox.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    displayTransformedImage();
    extractCircles();
}

void mm::MainWindow::extractCircles() {
    if (edgeList.isEmpty()) {
        extractEdges();
    }
    
    if (edgeList.isEmpty()) {
        return;
    }
    
    setControlState(false);
    
    transformedImage = edgeImage.copy();
    
    workerThread = new QThread(this);
    CircleWorker *worker = new CircleWorker(circleOptions->minRadius(), 
                                            circleOptions->maxRadius(), 
                                            circleOptions->minScore(), 
                                            circleOptions->maxScore(), 
                                            edgeList, 
                                            sourceImage, 
                                            transformedImage);
    
    progressTimer = new QTimer(this);
    
    const int pixelCount = sourceImage.width() * sourceImage.height();
    
    connect(progressTimer, &QTimer::timeout, [this,worker,pixelCount]() {
        if (workerThread->isRunning()) {
            featureCountLabel->setText(
                tr("%1 circles detected")
                    .arg(worker->count()));
        }
    });
    
    connect(worker, &CircleWorker::finished, [this,worker]() { 
        workerThread->quit();
        progressTimer->stop();
        stopButton->setEnabled(false);
        setControlState(true);
        featureCountLabel->setText(tr("%1 circles detected").arg(worker->count()));
        imageTabsWidget->setTabPixmap(ImageTabs::transformtab, QPixmap::fromImage(transformedImage)); 
    });
    
    connect(worker, &CircleWorker::started, [this]() {
        stopButton->setEnabled(true);
    });
    
    worker->moveToThread(workerThread);
    workerThread->start();
    
    progressTimer->start(100);
    QMetaObject::invokeMethod(worker, "work", Qt::QueuedConnection);
    
    /*
    QVector<int> accumulator(sourceImage.height() * sourceImage.width(), 0);
    
    fillEdgeImage();
    transformedImage = edgeImage.copy();
    QPainter painter(&transformedImage);
    
    painter.setPen(qRgb(255,0,0));
    
    circleModel.apply(edgeList.begin(), edgeList.end(),
                      iter::fn_iter([&](const std::pair<int,int> &p) {
                          if (p.first >= 0 && p.second >= 0 && p.second < sourceImage.height() && p.first < sourceImage.width()) {
                            if (accumulator[p.second * sourceImage.width() + p.first] < circleModel.maximum_score()) {
                                ++accumulator[p.second * sourceImage.width() + p.first];
                            }
                          }
                      }),
                      [&](int radius) -> bool {
                          for (int i = 0; i < accumulator.size(); ++i) {
                              if (accumulator[i] >= circleModel.minimum_score() && accumulator[i] <= circleModel.maximum_score()) {
                                  int y = i / sourceImage.width();
                                  int x = i % sourceImage.width();
                                  painter.drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
                              }
                          }
                          accumulator.fill(0);
                          return true;
                      });*/
    
}

void mm::MainWindow::extractEllipsesAndDisplay() {
    if (sourceImage.isNull()) {
        QMessageBox msgBox(QMessageBox::Warning,
                           tr("HoughStudio error"),
                           "No image loaded",
                           0, this);
        msgBox.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msgBox.exec();
        return;
    }
    displayTransformedImage();
    extractEllipses();
}

void mm::MainWindow::extractEllipses() {
    if (edgeList.isEmpty()) {
        extractEdges();
    }
    
    if (edgeList.isEmpty()) {
        return;
    }
    
    setControlState(false);
    
    options_to_model(*ellipseOptions, ellipseModel);
    
    transformedImage = edgeImage.copy();
    
    workerThread = new QThread(this);
    EllipseWorker *worker = new EllipseWorker(ellipseModel, edgeList, edgeMap, sourceImage, transformedImage);
    
    progressTimer = new QTimer(this);
    
    const int pixelCount = sourceImage.width() * sourceImage.height();
    
    connect(progressTimer, &QTimer::timeout, [this,worker,pixelCount]() {
        if (workerThread->isRunning()) {
            featureCountLabel->setText(
                tr("%1 ellipses detected (%2%)")
                    .arg(worker->ellipseCount())
                    .arg(static_cast<int>(((worker->currentY() * sourceImage.width() + worker->currentX()) / static_cast<double>(pixelCount)) * 100)));
        }
    });
    
    connect(worker, &EllipseWorker::finished, [this,worker]() {
        workerThread->quit();
        progressTimer->stop();
        stopButton->setEnabled(false);
        featureCountLabel->setText(tr("%1 ellipses detected").arg(worker->ellipseCount()));
        imageTabsWidget->setTabPixmap(ImageTabs::transformtab, QPixmap::fromImage(transformedImage)); 
        setControlState(true);
    });
    
    connect(worker, &EllipseWorker::started, [this]() {
        stopButton->setEnabled(true);
    });
    
    worker->moveToThread(workerThread);
    workerThread->start();
    
    progressTimer->start(100);
    QMetaObject::invokeMethod(worker, "work", Qt::QueuedConnection);
    
    /*
    fillEdgeImage();
    transformedImage = edgeImage.copy();
    
    QPainter painter(&transformedImage);
    
    painter.setPen(qRgb(255,0,0));
    
    ellipseModel.apply(edgeList.begin(), edgeList.end(),
                       [this](int x, int y){return x >= 0 && y >= 0 &&
                                                   y < sourceImage.height() &&
                                                   x < sourceImage.width() 
                                                   ? get(edgeMap, x, y) : false;},
                       [&](int cx, int cy, int smin, int smaj, double alpha) {
                           painter.save();
                           painter.translate(cx, cy);
                           painter.rotate(static_cast<int>(alpha * 180 / M_PI));
                           painter.drawEllipse(-smaj, -smin, smaj * 2, smin * 2);
                           painter.restore();
                       });*/
    
}

void mm::MainWindow::onEdgeOptionChange() {
    edgeImage = QImage();
    edgeList.clear();
    bits = std::vector<bool>();
    edgeMap = raster::view2d<std::vector<bool>,int>();
}

void mm::MainWindow::extractEdges() {
    if (sourceImage.isNull()) {
        return;
    }
    
    setControlState(false);
    
    bits = std::vector<bool>(sourceImage.height() * sourceImage.width(), false);
    edgeMap = raster::as2d(sourceImage.height(), sourceImage.width(), bits);
    
    edgeList.clear();
    
    workerThread = new QThread(this);
    EdgeWorker *worker = new EdgeWorker(edgeOptions->minThreshold(),
                                        edgeOptions->maxThreshold(),
                                        edgeOptions->blurEnabled() ? edgeOptions->blurRadius() : 0,
                                        edgeList, 
                                        edgeMap, 
                                        sourceImage);
    
    progressTimer = new QTimer(this);
    
    const int pixelCount = sourceImage.width() * sourceImage.height();
    
    connect(progressTimer, &QTimer::timeout, [this,worker,pixelCount]() {
        if (workerThread->isRunning()) {
            edgeCountLabel->setText(
                tr("%1 edges detected (%2%)")
                    .arg(edgeList.size())
                    .arg(
                        static_cast<int>(((worker->currentY() * sourceImage.width() + worker->currentX()) / static_cast<double>(pixelCount)) * 100)));
        }
    });
    
    connect(worker, &EdgeWorker::finished, [this]() { 
        workerThread->quit();
        progressTimer->stop();
        setControlState(true);
        stopButton->setEnabled(false);
        edgeCountLabel->setText(tr("%1 edges detected").arg(edgeList.size()));
        fillEdgeImage(); 
        imageTabsWidget->setTabPixmap(ImageTabs::edgetab, QPixmap::fromImage(edgeImage)); 
    });
    
    connect(worker, &EdgeWorker::started, [this]() {
        stopButton->setEnabled(true);
    });
    
    worker->moveToThread(workerThread);
    workerThread->start();
    
    progressTimer->start(100);
    QMetaObject::invokeMethod(worker, "work", Qt::QueuedConnection);
    
    while (workerThread->isRunning()) {
        QCoreApplication::processEvents();
    }
}

void mm::MainWindow::fillEdgeImage() {
    if (sourceImage.isNull()) {
        return;
    }
    
    edgeImage = QImage(sourceImage.size(), QImage::Format_RGB32);
    edgeImage.fill(qRgb(0,0,0));
    for (const auto &xy : edgeList) {
        edgeImage.setPixel(xy.first, xy.second, qRgb(255,255,255));
    }
}

void mm::MainWindow::dispatchExtractionMethod() {
    if (optionTabWidget->currentWidget() == circleOptions) {
        extractCirclesAndDisplay();
    } else if (optionTabWidget->currentWidget() == ellipseOptions) {
        extractEllipsesAndDisplay();
    }
}

void mm::MainWindow::resetState() {
    edgeCountLabel->setText(tr("0 edges detected"));
    featureCountLabel->setText(tr("0 features detected"));
    sourceImage = QImage();
    edgeImage = QImage();
    transformedImage = QImage();
    bits = std::vector<bool>();
    edgeMap = raster::view2d<std::vector<bool>,int>();
    edgeList.clear();
    statusLabel->setText(tr("No file loaded"));
    imageTabsWidget->setTabPixmap(ImageTabs::srctab, QPixmap::fromImage(sourceImage));
    imageTabsWidget->setTabPixmap(ImageTabs::edgetab, QPixmap::fromImage(edgeImage));
    imageTabsWidget->setTabPixmap(ImageTabs::transformtab, QPixmap::fromImage(transformedImage));
}

void mm::MainWindow::setControlState(bool s) {
    edgeOptions->setEnabled(s);
    optionTabWidget->setEnabled(s);
    showEdgesButton->setEnabled(s);
    startTransformButton->setEnabled(s);
}

void mm::MainWindow::stopProcessing() {
    if (workerThread->isRunning()) {
        workerThread->quit();
        setControlState(true);
        edgeCountLabel->setText(tr("0 edges detected"));
        featureCountLabel->setText(tr("0 features detected"));
    }
    if (progressTimer->isActive()) {
        progressTimer->stop();
    }
    stopButton->setEnabled(false);
}

void mm::MainWindow::activateFullScreen(bool checked) {
    if (checked) {
        showFullScreen();
    } else {
        showNormal();
    }
}

