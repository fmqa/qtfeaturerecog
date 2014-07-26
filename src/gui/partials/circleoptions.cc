#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include "circleoptions.hh"

mm::CircleOptions::CircleOptions(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    QGroupBox *group = new QGroupBox(tr("Circles"));
    QVBoxLayout *vbox = new QVBoxLayout;
    {
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(new QLabel(tr("Radius min/max:")));
                hbox->addWidget(minRadiusSpinBox = new QSpinBox);
                hbox->addWidget(maxRadiusSpinBox = new QSpinBox);
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
    }
    group->setLayout(vbox);
    mainLayout->addWidget(group);
    
    setLayout(mainLayout);
    
    minRadiusSpinBox->setMinimum(1);
    maxRadiusSpinBox->setMinimum(1);
    minScoreSpinBox->setMaximum(99999999);
    maxScoreSpinBox->setMaximum(99999999);
    
    connect(minScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(minRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxRadiusSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(maxScoreSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    
    minRadiusSpinBox->setValue(5);
    maxRadiusSpinBox->setValue(30);
    minScoreSpinBox->setValue(100);
    maxScoreSpinBox->setValue(380);
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

