#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include "partials/edgeoptions.hh"
#include "partials/circleoptions.hh"
#include "partials/ellipseoptions.hh"
#include "partials/imagetabs.hh"
#include "ui.hh"

static void uiInitActions(mm::Ui *ui) {    
    ui->actions.open = new QAction(QIcon::fromTheme("document-open"), QObject::tr("&Open..."), ui);
    ui->actions.open->setShortcuts(QKeySequence::Open);
    ui->actions.open->setStatusTip(QObject::tr("Open an image file"));
    
    ui->actions.save = new QAction(QIcon::fromTheme("document-save"), QObject::tr("&Save..."), ui);
    ui->actions.save->setShortcuts(QKeySequence::Save);
    ui->actions.save->setStatusTip(QObject::tr("Save the active image"));
    
    ui->actions.copy = new QAction(QIcon::fromTheme("edit-copy"), QObject::tr("&Copy..."), ui);
    ui->actions.copy->setShortcuts(QKeySequence::Copy);
    ui->actions.copy->setStatusTip(QObject::tr("Copy the active image to clipboard"));
    
    ui->actions.exit = new QAction(QObject::tr("E&xit"), ui);
    ui->actions.exit->setShortcuts(QKeySequence::Quit);
    ui->actions.exit->setStatusTip(QObject::tr("Exit HoughStudio"));
    
    ui->actions.viewsrc = new QAction(QObject::tr("O&riginal image"), ui);
    ui->actions.viewsrc->setStatusTip(QObject::tr("Show the original image"));
    
    ui->actions.viewedges = new QAction(QObject::tr("E&dge Map"), ui);
    ui->actions.viewedges->setStatusTip(QObject::tr("Show the edge map of the image"));

    ui->actions.viewtransfm = new QAction(QObject::tr("T&ransformed View"), ui);
    ui->actions.viewtransfm->setStatusTip(QObject::tr("Show the transformed image")); 
    
    ui->actions.fullscr = new QAction(QObject::tr("F&ullscreen Mode"), ui);
    ui->actions.fullscr->setStatusTip(QObject::tr("Activate the full-screen mode"));
    ui->actions.fullscr->setCheckable(true);
}

static void uiInitMenus(mm::Ui *ui) {
    ui->menus.file = ui->menuBar()->addMenu(QObject::tr("&File"));
    ui->menus.file->addAction(ui->actions.open);
    ui->menus.file->addAction(ui->actions.save);
    ui->menus.file->addAction(ui->actions.copy);
    ui->menus.file->addSeparator();
    ui->menus.file->addAction(ui->actions.exit);
    
    ui->menus.view = ui->menuBar()->addMenu(QObject::tr("&View"));
    ui->menus.view->addAction(ui->actions.viewsrc);
    ui->menus.view->addAction(ui->actions.viewedges);
    ui->menus.view->addAction(ui->actions.viewtransfm);
    ui->menus.view->addSeparator();
    ui->menus.view->addAction(ui->actions.fullscr);
}

static void uiInitToolbars(mm::Ui *ui) {
    ui->toolbars.file = ui->addToolBar(QObject::tr("File"));
    ui->toolbars.file->addAction(ui->actions.open);
    ui->toolbars.file->addAction(ui->actions.save);
    ui->toolbars.file->addAction(ui->actions.copy);
}

static void uiInitOptions(mm::Ui *ui) {
    ui->options.edges = new mm::EdgeOptions;
    ui->options.circles = new mm::CircleOptions;
    ui->options.ellipses = new mm::EllipseOptions;
    ui->transfmtabs = new QTabWidget;
    ui->transfmtabs->addTab(ui->options.circles, QObject::tr("Circles"));
    ui->transfmtabs->addTab(ui->options.ellipses, QObject::tr("Ellipses"));
}

static void uiInitControls(mm::Ui *ui) {
    ui->controls.detectedges = new QPushButton(QObject::tr("Detect edges"));
    ui->controls.applytransfm = new QPushButton(QObject::tr("Apply"));
    ui->controls.stop = new QPushButton(QObject::tr("Stop"));
}

static void uiAssignTransformControls(QLayout *layout, mm::Ui *ui) {
    layout->addWidget(ui->controls.applytransfm);
    layout->addWidget(ui->controls.stop);
}

static QWidget *uiCreateTransformPanel(mm::Ui *ui) {
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    uiAssignTransformControls(layout, ui);
    container->setLayout(layout);
    return container;
}

static void uiAssignToolboxControls(QLayout *layout, mm::Ui *ui) {
    layout->addWidget(ui->options.edges);
    layout->addWidget(ui->transfmtabs);
    layout->addWidget(ui->controls.detectedges);
    layout->addWidget(uiCreateTransformPanel(ui));
}

static QWidget *uiCreateToolbox(mm::Ui *ui) {
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    uiAssignToolboxControls(layout, ui);
    container->setLayout(layout);
    container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    return container;
}

static QWidget *uiCreateMainPanel(mm::Ui *ui) {
    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(uiCreateToolbox(ui));
    ui->images = new mm::ImageTabs;
    layout->addWidget(ui->images);
    container->setLayout(layout);
    return container;
}

mm::Ui::Ui() {
    uiInitActions(this);
    uiInitMenus(this);
    uiInitToolbars(this);
    uiInitOptions(this);
    uiInitControls(this);
    setCentralWidget(uiCreateMainPanel(this));
}

QString mm::Ui::requestImage() {
    QFileDialog dialog(this, tr("Open image..."), QString(), tr("Images (*.png *.jpg *.jpeg *.gif *.bmp)"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    
    if (!dialog.exec()) {
        return QString();
    }
    
    QStringList selection = dialog.selectedFiles();
        
    if (selection.empty()) {
        return QString();
    }
    
    return selection.first();
}

void mm::Ui::alertInvalidSourceImage(const QString &file) {
    QMessageBox msg(QMessageBox::Warning,
                    tr("HoughStudio error"),
                    tr("Error loading image file: %1").arg(file),
                    0, this);
    msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
    msg.exec();
}

void mm::Ui::alertExtractEdges() {
    QMessageBox msg(QMessageBox::Warning,
                    tr("HoughStudio error"),
                    tr("Edge map is empty. Run the edge extractor first"),
                    0, this);
    msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
    msg.exec();
}

void mm::Ui::enableControls() {
    controls.detectedges->setEnabled(true);
    controls.applytransfm->setEnabled(true);
}

void mm::Ui::disableControls() {
    controls.detectedges->setEnabled(false);
    controls.applytransfm->setEnabled(false);
}