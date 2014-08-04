#ifndef GUI_EDGEOPTIONS_HH
#define GUI_EDGEOPTIONS_HH

#include <QWidget>
class QCheckBox;
class QSpinBox;
class QGroupBox;
class QLabel;

namespace mm {
    class EdgeOptions : public QWidget {
        Q_OBJECT
    public:
        explicit EdgeOptions(QWidget * = 0);
        bool blurEnabled() const;
        int blurRadius() const;
        int minThreshold() const;
        int maxThreshold() const;
        EdgeOptions& setBlurEnabled(bool);
        EdgeOptions& setBlurRadius(int);
        EdgeOptions& setMinThreshold(int);
        EdgeOptions& setMaxThreshold(int);
        void retranslateUi();
    private slots:
        void onValueChanged(int);
        void onStateChanged(bool);
    signals:
        void changed();
    private:
        QCheckBox *enableGaussianBlurCheckBox;
        QSpinBox *gaussianBlurRadiusSpinBox;
        QSpinBox *cannyMinThresholdSpinBox;
        QSpinBox *cannyMaxThresholdSpinBox;
        QGroupBox *extGroup, *blurGroup;
        QLabel *lblRadius, *lblMinThreshold, *lblMaxThreshold;
    };
}

#endif /* #ifndef GUI_EDGEOPTIONS_HH */
