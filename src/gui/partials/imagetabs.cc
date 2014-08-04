#include <QTabWidget>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include "imagetabs.hh"

mm::ImageTabs::ImageTabs(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    tabWidget = new QTabWidget;
    
    scrollAreaSrc = new QScrollArea;
    labelSrc = new QLabel;
    labelSrc->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollAreaSrc->setWidget(labelSrc);
    tabWidget->addTab(scrollAreaSrc, QString());
    
    scrollAreaEdges = new QScrollArea;
    labelEdges = new QLabel;
    labelEdges->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollAreaEdges->setWidget(labelEdges);
    tabWidget->addTab(scrollAreaEdges, QString());
    
    scrollAreaTransform = new QScrollArea;
    labelTransform = new QLabel;
    labelTransform->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollAreaTransform->setWidget(labelTransform);
    tabWidget->addTab(scrollAreaTransform, QString());
    
    scrollAreaHistogram = new QScrollArea;
    labelHistogram = new QLabel;
    labelHistogram->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollAreaHistogram->setWidget(labelHistogram);
    tabWidget->addTab(scrollAreaHistogram, QString());
    
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

mm::ImageTabs::tab mm::ImageTabs::currentTab() const {
    if (tabWidget->currentWidget() == scrollAreaSrc) {
        return srctab;
    } else if (tabWidget->currentWidget() == scrollAreaEdges) {
        return edgetab;
    } else if (tabWidget->currentWidget() == scrollAreaTransform) {
        return transformtab;
    } else if (tabWidget->currentWidget() == scrollAreaHistogram) {
        return histogramtab;
    } else {
        return invalidtab;
    }
}

void mm::ImageTabs::switchTab(mm::ImageTabs::tab tabid) {
    switch (tabid) {
        case srctab: tabWidget->setCurrentWidget(scrollAreaSrc); break;
        case edgetab: tabWidget->setCurrentWidget(scrollAreaEdges); break;
        case transformtab: tabWidget->setCurrentWidget(scrollAreaTransform); break;
        case histogramtab: tabWidget->setCurrentWidget(scrollAreaHistogram); break;
        default: break;
    }
}

void mm::ImageTabs::setTabPixmap(mm::ImageTabs::tab tabid, QPixmap const &pixmap) {
    switch (tabid) {
        case srctab: labelSrc->setPixmap(pixmap); labelSrc->adjustSize(); break;
        case edgetab: labelEdges->setPixmap(pixmap); labelEdges->adjustSize(); break;
        case transformtab: labelTransform->setPixmap(pixmap); labelTransform->adjustSize(); break;
        case histogramtab: labelHistogram->setPixmap(pixmap); labelHistogram->adjustSize(); break;
        default: break;
    }
}

void mm::ImageTabs::saveCurrentTabImage() {
    const QPixmap *pmap = 0;
    
    switch (currentTab()) {
        case srctab: pmap = labelSrc->pixmap(); break;
        case edgetab: pmap = labelEdges->pixmap(); break;
        case transformtab: pmap = labelTransform->pixmap(); break;
        case histogramtab: pmap = labelHistogram->pixmap(); break;
        default: break;
    }
    
    if (pmap && !pmap->isNull()) {
        QFileDialog dlg(this, tr("Save image..."), QString(), tr("Images (*.png *.jpg)"));
        dlg.setDefaultSuffix(".png");
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        
        if (dlg.exec()) {
            QStringList selection = dlg.selectedFiles();
            
            if (selection.empty()) {
                return;
            }
            
            const QString &saveFile = selection.first();
            
            if (!pmap->save(saveFile)) {
                QMessageBox msg(QMessageBox::Warning,
                                tr("HoughStudio error"),
                                tr("Error saving file"),
                                0, this);
                msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
                msg.exec();
            }
        }
    } else {
        QMessageBox msg(QMessageBox::Warning,
                        tr("HoughStudio error"),
                        tr("Can't save empty image"),
                        0, this);
        msg.addButton(tr("&Ok"), QMessageBox::AcceptRole);
        msg.exec();
    }
}

void mm::ImageTabs::currentTabImageToClipboard() {
    const QPixmap *pmap = 0;
    
    switch (currentTab()) {
        case srctab: pmap = labelSrc->pixmap(); break;
        case edgetab: pmap = labelEdges->pixmap(); break;
        case transformtab: pmap = labelTransform->pixmap(); break;
        case histogramtab: pmap = labelHistogram->pixmap(); break;
        default: break;
    }
    
    if (pmap && !pmap->isNull()) {
        QApplication::clipboard()->setPixmap(*pmap);
    }
}

QLabel& mm::ImageTabs::widget(mm::ImageTabs::tab t) {
    switch (t) {
        case srctab: return *labelSrc; break;
        case edgetab: return *labelEdges; break;
        case transformtab: return *labelTransform; break;
        case histogramtab: return *labelHistogram; break;
        default: return *labelSrc;
    }
}

void mm::ImageTabs::retranslateUi() {
    tabWidget->setTabText(tabWidget->indexOf(scrollAreaSrc), tr("Source"));
    tabWidget->setTabText(tabWidget->indexOf(scrollAreaEdges), tr("Edges"));
    tabWidget->setTabText(tabWidget->indexOf(scrollAreaTransform), tr("Transform"));
    tabWidget->setTabText(tabWidget->indexOf(scrollAreaHistogram), tr("Histogram"));
}

