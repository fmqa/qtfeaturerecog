#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QColorDialog>
#include <QPainter>
#include <QPushButton>
#include "ellipseoptions.hh"

mm::EllipseOptions::EllipseOptions(QWidget *parent) : QWidget(parent), markerImage(10, 5, QImage::Format_RGB32), markerColor(qRgb(255,0,0)) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    QGroupBox *group = new QGroupBox(tr("Ellipses"));
    QVBoxLayout *vbox = new QVBoxLayout;
    {
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(new QLabel(tr("Major min/max:")));
                hbox->addWidget(minMajorSpinBox = new QSpinBox);
                hbox->addWidget(maxMajorSpinBox = new QSpinBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(new QLabel(tr("Minor min/max:")));
                hbox->addWidget(minMinorSpinBox = new QSpinBox);
                hbox->addWidget(maxMinorSpinBox = new QSpinBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(new QLabel(tr("Score min/max:")));
                hbox->addWidget(minScoreSpinBox = new QSpinBox);
                hbox->addWidget(maxScoreSpinBox = new QSpinBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(new QLabel(tr("Marker color:")));
                hbox->addWidget(colorLabel = new QLabel);
                hbox->addWidget(colorPickBtn = new QPushButton("..."));
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(chkRHT = new QCheckBox(tr("RHT")));
                hbox->addWidget(rhtEdgePercentage = new QSpinBox);
                hbox->addWidget(new QLabel("%"));
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
    }
    group->setLayout(vbox);
    mainLayout->addWidget(group);
    
    setLayout(mainLayout);
    
    minMajorSpinBox->setMinimum(1);
    minMajorSpinBox->setMaximum(99999999);
    
    maxMajorSpinBox->setMinimum(1);
    maxMajorSpinBox->setMaximum(99999999);
    
    minMinorSpinBox->setMaximum(1);
    minMinorSpinBox->setMaximum(99999999);
    
    maxMinorSpinBox->setMaximum(1);
    maxMinorSpinBox->setMaximum(99999999);
    
    minScoreSpinBox->setMaximum(99999999);
    maxScoreSpinBox->setMaximum(99999999);
    
    rhtEdgePercentage->setMaximum(100);
    rhtEdgePercentage->setMinimum(0);
    rhtEdgePercentage->setEnabled(false);
    
    connect(minMajorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxMajorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(minMinorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxMinorSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    
    connect(minScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    
    connect(chkRHT, SIGNAL(stateChanged(int)), this, SLOT(rhtStateChanged(int)));
    connect(colorPickBtn, SIGNAL(clicked()), this, SLOT(pickColor()));
    
    setColor();
}

void mm::EllipseOptions::onValueChanged(int) {
    emit changed();
}

void mm::EllipseOptions::rhtStateChanged(int state) {
    rhtEdgePercentage->setEnabled(state == Qt::Checked);
}

int mm::EllipseOptions::minMajor() const {
    return minMajorSpinBox->value();
}

int mm::EllipseOptions::maxMajor() const {
    return maxMajorSpinBox->value();
}

int mm::EllipseOptions::minMinor() const {
    return minMinorSpinBox->value();
}

int mm::EllipseOptions::maxMinor() const {
    return maxMinorSpinBox->value();
}

int mm::EllipseOptions::minScore() const {
    return minScoreSpinBox->value();
}

int mm::EllipseOptions::maxScore() const {
    return maxScoreSpinBox->value();
}

bool mm::EllipseOptions::rhtEnabled() const {
    return chkRHT->isChecked();
}

int mm::EllipseOptions::rhtPercentage() const {
    return rhtEdgePercentage->value();
}

QColor mm::EllipseOptions::getColor() const {
    return markerColor;
}

mm::EllipseOptions& mm::EllipseOptions::setMinMajor(int value) {
    return minMajorSpinBox->setValue(value), *this;
}

mm::EllipseOptions& mm::EllipseOptions::setMaxMajor(int value) {
    return maxMajorSpinBox->setValue(value), *this;
}

mm::EllipseOptions& mm::EllipseOptions::setMinMinor(int value) {
    return minMinorSpinBox->setValue(value), *this;
}

mm::EllipseOptions& mm::EllipseOptions::setMaxMinor(int value) {
    return maxMinorSpinBox->setValue(value), *this;
}

mm::EllipseOptions& mm::EllipseOptions::setMinScore(int value) {
    return minScoreSpinBox->setValue(value), *this;
}

mm::EllipseOptions& mm::EllipseOptions::setMaxScore(int value) {
    return maxScoreSpinBox->setValue(value), *this;
}

void mm::EllipseOptions::pickColor() {
    markerColor = QColorDialog::getColor(markerColor, this, tr("Pick marker color"));
    setColor();
}

void mm::EllipseOptions::setColor() {
    {
        QPainter painter(&markerImage);
        painter.fillRect(0, 0, 10, 5, markerColor);
    }
    colorLabel->setPixmap(QPixmap::fromImage(markerImage));
}