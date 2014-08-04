#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QColorDialog>
#include <QCheckBox>
#include "circleoptions.hh"

mm::CircleOptions::CircleOptions(QWidget *parent) : QWidget(parent), markerImage(10, 5, QImage::Format_RGB32), markerColor(qRgb(255,0,0)) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    circlesGroup = new QGroupBox;
    QVBoxLayout *vbox = new QVBoxLayout;
    
    {
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(lblRadiusMinMax = new QLabel);
                hbox->addWidget(minRadiusSpinBox = new QSpinBox);
                hbox->addWidget(maxRadiusSpinBox = new QSpinBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(lblScoreMinMax = new QLabel);
                hbox->addWidget(minScoreSpinBox = new QSpinBox);
                hbox->addWidget(maxScoreSpinBox = new QSpinBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(lblMarkerColor = new QLabel);
                hbox->addWidget(colorLabel = new QLabel);
                hbox->addWidget(colorPickBtn = new QPushButton("..."));
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(chkHistogram = new QCheckBox);
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
    }
    
    circlesGroup->setLayout(vbox);
    mainLayout->addWidget(circlesGroup);
    
    setLayout(mainLayout);
    
    minRadiusSpinBox->setMinimum(1);
    maxRadiusSpinBox->setMinimum(1);
    minScoreSpinBox->setMaximum(99999999);
    maxScoreSpinBox->setMaximum(99999999);
    
    connect(minScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(minRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(colorPickBtn, SIGNAL(clicked()), this, SLOT(pickColor()));
    
    minRadiusSpinBox->setValue(5);
    maxRadiusSpinBox->setValue(30);
    minScoreSpinBox->setValue(100);
    maxScoreSpinBox->setValue(380);
    
    setColor();
    retranslateUi();
}

void mm::CircleOptions::onValueChanged(int) {
    emit changed();
}

void mm::CircleOptions::setMaxRadiusBound(int value)  {
    minRadiusSpinBox->setMaximum(value);
    maxRadiusSpinBox->setMaximum(value);
}

int mm::CircleOptions::minRadius() const {
    return minRadiusSpinBox->value();
}

int mm::CircleOptions::maxRadius() const {
    return maxRadiusSpinBox->value();
}

int mm::CircleOptions::minScore() const {
    return minScoreSpinBox->value();
}

int mm::CircleOptions::maxScore() const {
    return maxScoreSpinBox->value();
}

QColor mm::CircleOptions::getColor() const {
    return markerColor;
}

mm::CircleOptions& mm::CircleOptions::setMinRadius(int value) {
    return minRadiusSpinBox->setValue(value), *this;
}

mm::CircleOptions& mm::CircleOptions::setMaxRadius(int value) {
    return maxRadiusSpinBox->setValue(value), *this;
}

mm::CircleOptions& mm::CircleOptions::setMinScore(int value) {
    return minScoreSpinBox->setValue(value), *this;
}

mm::CircleOptions& mm::CircleOptions::setMaxScore(int value) {
    return maxScoreSpinBox->setValue(value), *this;
}

void mm::CircleOptions::pickColor() {
    markerColor = QColorDialog::getColor(markerColor, this, tr("Pick marker color"));
    setColor();
}

void mm::CircleOptions::setColor() {
    {
        QPainter painter(&markerImage);
        painter.fillRect(0, 0, 10, 5, markerColor);
    }
    colorLabel->setPixmap(QPixmap::fromImage(markerImage));
}

bool mm::CircleOptions::histogram() const {
    return chkHistogram->isChecked();
}

void mm::CircleOptions::retranslateUi() {
    circlesGroup->setTitle(tr("Circles"));
    lblRadiusMinMax->setText(tr("Radius min/max:"));
    lblScoreMinMax->setText(tr("Score min/max:"));
    lblMarkerColor->setText(tr("Marker color:"));
    chkHistogram->setText(tr("Histogram"));
}
