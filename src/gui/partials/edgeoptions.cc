#include "edgeoptions.hh"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

mm::EdgeOptions::EdgeOptions(QWidget * parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    
    extGroup = new QGroupBox;
    QVBoxLayout *vbox = new QVBoxLayout;
    
    {
        {
            blurGroup = new QGroupBox;
            QVBoxLayout *ivbox = new QVBoxLayout;
            
            ivbox->addWidget(enableGaussianBlurCheckBox = new QCheckBox);
            connect(enableGaussianBlurCheckBox, SIGNAL(toggled(bool)), this, SLOT(onStateChanged(bool)));
            {
                QWidget *container = new QWidget;
                QHBoxLayout *hbox = new QHBoxLayout;
                    hbox->addWidget(lblRadius = new QLabel);
                    hbox->addWidget(gaussianBlurRadiusSpinBox = new QSpinBox);
                container->setLayout(hbox);
                ivbox->addWidget(container);
            }
            
            blurGroup->setLayout(ivbox);
            vbox->addWidget(blurGroup);
        }
        
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(lblMinThreshold = new QLabel);
                hbox->addWidget(cannyMinThresholdSpinBox = new QSpinBox);
                connect(cannyMinThresholdSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
            
        {
            QWidget *container = new QWidget;
            QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(lblMaxThreshold = new QLabel);
                hbox->addWidget(cannyMaxThresholdSpinBox = new QSpinBox);
                connect(cannyMaxThresholdSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
            container->setLayout(hbox);
            vbox->addWidget(container);
        }
    }
    
    extGroup->setLayout(vbox);
    mainLayout->addWidget(extGroup);
    
    setLayout(mainLayout);
    
    cannyMinThresholdSpinBox->setMaximum(99999999);
    cannyMaxThresholdSpinBox->setMaximum(99999999);
    
    cannyMinThresholdSpinBox->setValue(1);
    cannyMaxThresholdSpinBox->setValue(20);
    enableGaussianBlurCheckBox->setChecked(true);
    gaussianBlurRadiusSpinBox->setValue(1);
    
    retranslateUi();
}

void mm::EdgeOptions::onValueChanged(int) {
    emit changed();
}

void mm::EdgeOptions::onStateChanged(bool) {
    emit changed();
}

void mm::EdgeOptions::retranslateUi() {
    extGroup->setTitle(tr("Edge Extractor"));
    blurGroup->setTitle(tr("Blur"));
    enableGaussianBlurCheckBox->setText(tr("Enable"));
    lblRadius->setText(tr("Radius"));
    lblMinThreshold->setText(tr("Min. Threshold"));
    lblMaxThreshold->setText(tr("Max. Threshold"));
}

bool mm::EdgeOptions::blurEnabled() const {
    return enableGaussianBlurCheckBox->isChecked();
}

int mm::EdgeOptions::blurRadius() const {
    return gaussianBlurRadiusSpinBox->value();
}

int mm::EdgeOptions::minThreshold() const {
    return cannyMinThresholdSpinBox->value();
}

int mm::EdgeOptions::maxThreshold() const {
    return cannyMaxThresholdSpinBox->value();
}

mm::EdgeOptions& mm::EdgeOptions::setBlurEnabled(bool b) {
    return enableGaussianBlurCheckBox->setChecked(b), *this;
}

mm::EdgeOptions& mm::EdgeOptions::setBlurRadius(int r) {
    return gaussianBlurRadiusSpinBox->setValue(r), *this;
}

mm::EdgeOptions& mm::EdgeOptions::setMinThreshold(int minthresh) {
    return cannyMinThresholdSpinBox->setValue(minthresh), *this;
}

mm::EdgeOptions& mm::EdgeOptions::setMaxThreshold(int maxthresh) {
    return cannyMaxThresholdSpinBox->setValue(maxthresh), *this;
}

