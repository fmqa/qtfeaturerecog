#include <QApplication>
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
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QImage>
#include <QList>
#include <QUrl>
#include <QStatusBar>
#include <QTranslator>
#include "partials/edgeoptions.hh"
#include "partials/circleoptions.hh"
#include "partials/ellipseoptions.hh"
#include "partials/imagetabs.hh"
#include "ui.hh"

static void uiInitActions(mm::Ui *ui) {    
    ui->actions.open = new QAction(QIcon::fromTheme("document-open"), QString(), ui);
    ui->actions.open->setShortcuts(QKeySequence::Open);
    
    ui->actions.save = new QAction(QIcon::fromTheme("document-save"), QString(), ui);
    ui->actions.save->setShortcuts(QKeySequence::Save);
    
    ui->actions.savecirc = new QAction(QIcon::fromTheme("document-save"), QString(), ui);
    ui->actions.savecirc->setShortcuts(QKeySequence::Save);
    
    ui->actions.copy = new QAction(QIcon::fromTheme("edit-copy"), QString(), ui);
    ui->actions.copy->setShortcuts(QKeySequence::Copy);
    
    ui->actions.copycirc = new QAction(QIcon::fromTheme("edit-copy"), QString(), ui);
    ui->actions.copycirc->setShortcuts(QKeySequence::Copy);
    
    ui->actions.exit = new QAction(ui);
    ui->actions.exit->setShortcuts(QKeySequence::Quit);
    
    ui->actions.viewsrc = new QAction(ui);
    
    ui->actions.viewedges = new QAction(ui);

    ui->actions.viewtransfm = new QAction(ui);
    
    ui->actions.fullscr = new QAction(ui);
    ui->actions.fullscr->setCheckable(true);
    
    ui->actions.german = new QAction(ui);
    ui->actions.english = new QAction(ui);
    
    ui->actions.capture = new QAction(ui);
    
    ui->actions.tts = new QAction(ui);
    ui->actions.tts->setCheckable(true);
}

static void uiInitActionText(mm::Ui *ui) {
    ui->actions.open->setText(QObject::tr("&Open..."));
    ui->actions.open->setStatusTip(QObject::tr("Open an image file"));
    
    ui->actions.save->setText(QObject::tr("&Save..."));
    ui->actions.save->setStatusTip(QObject::tr("Save the active image"));
    
    ui->actions.savecirc->setText(QObject::tr("Save circle &mask..."));
    ui->actions.savecirc->setStatusTip(QObject::tr("Save the circle contents as an image"));
    
    ui->actions.copy->setText(QObject::tr("&Copy..."));
    ui->actions.copy->setStatusTip(QObject::tr("Copy the active image to clipboard"));
    
    ui->actions.copycirc->setText(QObject::tr("C&opy circle mask..."));
    ui->actions.copycirc->setStatusTip(QObject::tr("Save the circle contents as an image"));
    
    ui->actions.exit->setText(QObject::tr("E&xit"));
    ui->actions.exit->setStatusTip(QObject::tr("Exit HoughStudio"));
    
    ui->actions.viewsrc->setText(QObject::tr("O&riginal image"));
    ui->actions.viewsrc->setStatusTip(QObject::tr("Show the original image"));
    
    ui->actions.viewedges->setText(QObject::tr("E&dge Map"));
    ui->actions.viewedges->setStatusTip(QObject::tr("Show the edge map of the image"));
    
    ui->actions.viewtransfm->setText(QObject::tr("&Transformed View"));
    ui->actions.viewtransfm->setStatusTip(QObject::tr("Show the transformed image"));
    
    ui->actions.fullscr->setText(QObject::tr("F&ullscreen Mode"));
    ui->actions.fullscr->setStatusTip(QObject::tr("Activate the full-screen mode"));
    
    ui->actions.german->setText(QObject::tr("&German"));
    ui->actions.english->setText(QObject::tr("&English"));
    
    ui->actions.capture->setText(QObject::tr("Ca&pture"));
    ui->actions.tts->setText(QObject::tr("&Text-to-Speech"));
}

static void uiInitMenus(mm::Ui *ui) {
    ui->menus.file = ui->menuBar()->addMenu(QString());
    ui->menus.file->addAction(ui->actions.open);
    ui->menus.file->addAction(ui->actions.save);
    ui->menus.file->addAction(ui->actions.savecirc);
    ui->menus.file->addAction(ui->actions.capture);
    ui->menus.file->addSeparator();
    ui->menus.file->addAction(ui->actions.exit);
    
    ui->menus.edit = ui->menuBar()->addMenu(QString());
    ui->menus.edit->addAction(ui->actions.copy);
    ui->menus.edit->addAction(ui->actions.copycirc);
    
    ui->menus.view = ui->menuBar()->addMenu(QString());
    ui->menus.view->addAction(ui->actions.viewsrc);
    ui->menus.view->addAction(ui->actions.viewedges);
    ui->menus.view->addAction(ui->actions.viewtransfm);
    ui->menus.view->addSeparator();
    ui->menus.view->addAction(ui->actions.fullscr);
    
    ui->menus.extras = ui->menuBar()->addMenu(QString());
    ui->menus.extras->addAction(ui->actions.tts);
    
    ui->menus.language = ui->menuBar()->addMenu(QString());
    ui->menus.language->addAction(ui->actions.english);
    ui->menus.language->addAction(ui->actions.german);
}

static void uiInitMenuText(mm::Ui *ui) {
    ui->menus.file->setTitle(QObject::tr("&File"));
    ui->menus.edit->setTitle(QObject::tr("&Edit"));
    ui->menus.view->setTitle(QObject::tr("&View"));
    ui->menus.extras->setTitle(QObject::tr("E&xtras"));
    ui->menus.language->setTitle(QObject::tr("&Language"));
}

static void uiInitToolbars(mm::Ui *ui) {
    ui->toolbars.file = ui->addToolBar(QString());
    ui->toolbars.file->addAction(ui->actions.open);
    ui->toolbars.file->addAction(ui->actions.save);
    ui->toolbars.file->addAction(ui->actions.copy);
}

static void uiInitToolbarText(mm::Ui *ui) {
    ui->toolbars.file->setWindowTitle(QObject::tr("File"));
}

static void uiInitOptions(mm::Ui *ui) {
    ui->options.edges = new mm::EdgeOptions;
    ui->options.circles = new mm::CircleOptions;
    ui->options.ellipses = new mm::EllipseOptions;
    ui->transfmtabs = new QTabWidget;
    ui->transfmtabs->addTab(ui->options.circles, QString());
    ui->transfmtabs->addTab(ui->options.ellipses, QString());
}

static void uiInitOptionsText(mm::Ui *ui) {
    ui->transfmtabs->setTabText(ui->transfmtabs->indexOf(ui->options.circles), QObject::tr("Circles"));
    ui->transfmtabs->setTabText(ui->transfmtabs->indexOf(ui->options.ellipses), QObject::tr("Ellipses (Experimental)"));
    ui->options.edges->retranslateUi();
    ui->options.circles->retranslateUi();
    ui->options.ellipses->retranslateUi();
    ui->images->retranslateUi();
}

static void uiInitControls(mm::Ui *ui) {
    ui->controls.detectedges = new QPushButton;
    ui->controls.applytransfm = new QPushButton;
    ui->controls.stop = new QPushButton;
}

static void uiInitControlsText(mm::Ui *ui) {
    ui->controls.detectedges->setText(QObject::tr("Detect edges"));
    ui->controls.applytransfm->setText(QObject::tr("Apply"));
    ui->controls.stop->setText(QObject::tr("Stop"));
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

mm::Ui::Ui() : language("en") {
    uiInitActions(this);
    uiInitMenus(this);
    uiInitToolbars(this);
    uiInitOptions(this);
    uiInitControls(this);
    setCentralWidget(uiCreateMainPanel(this));
    setAcceptDrops(true);
    statusBar()->addPermanentWidget(coordLabel = new QLabel);
    
    translator = new QTranslator(this);
    
    connect(actions.german, SIGNAL(triggered()), this, SLOT(changeLanguageGerman()));
    connect(actions.english, SIGNAL(triggered()), this, SLOT(changeLanguageEnglish()));
    
    retranslateUi();
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

QString mm::Ui::requestSaveImage() {
    QFileDialog dialog(this, tr("Save image..."), QString(), tr("Images (*.png *.jpg)"));
    dialog.setDefaultSuffix(".png");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    
    if (dialog.exec()) {
        QStringList selection = dialog.selectedFiles();
        
        if (!selection.empty()) {
            return selection.first();
        }
    }
    
    return QString();
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

void mm::Ui::changeLanguageEnglish() {
    qApp->removeTranslator(translator);
    language = "en";
}

void mm::Ui::changeLanguageGerman() {
    translator->load("languages/houghstudio_de");
    qApp->installTranslator(translator);
    language = "de";
}

void mm::Ui::retranslateUi() {
    uiInitActionText(this);
    uiInitMenuText(this);
    uiInitToolbarText(this);
    uiInitOptionsText(this);
    uiInitControlsText(this);
}

void mm::Ui::alertEmptySourceImage() {
    QMessageBox msg(QMessageBox::Warning,
                    tr("HoughStudio error"),
                    tr("Please load an image first"),
                    0, this);
    msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
    msg.exec();
}

void mm::Ui::alertNoParametricCircles() {
    QMessageBox msg(QMessageBox::Warning,
                    tr("HoughStudio error"),
                    tr("No circles in parameter buffer. Please run the detector first"),
                    0, this);
    msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
    msg.exec();
}

void mm::Ui::dragEnterEvent(QDragEnterEvent *event) {
    event->accept();
}

void mm::Ui::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        
        if (urlList.size() == 1) {
            emit fileDropped(urlList.first().toLocalFile());
        }
    } else if (mimeData->hasImage()) {
        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        if (!image.isNull()) {
            emit imageDropped(image);
        }
    }
}

void mm::Ui::changeEvent(QEvent *event) {
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
    QMainWindow::changeEvent(event);
}

void mm::Ui::alertSaveError(const QString &file) {
    QMessageBox msg(QMessageBox::Warning,
                    tr("HoughStudio error"),
                    tr("Error saving file: %1").arg(file),
                    0, this);
    msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
    msg.exec();
}

QString mm::Ui::currentLanguage() const { return language; }


