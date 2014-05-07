#include <QApplication>
#include <QImage>
#include <QLabel>
#include "rst/wrap.h"
#include "rst/filter/gaussian.h"
#include "rst/filter/canny.h"
#include "rst/adapters/qt.h"

static const rst::fgaussian<3> blur(1.0);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QImage in;
    in.load(argv[1]);
    
    QImage out(in.size(), QImage::Format_Indexed8);
    QVector<QRgb> grays;
    for (int i = 0; i < 256; ++i) {
        grays.push_back(qRgb(i,i,i));
    }
    out.setColorTable(grays);
    
    rst::canny(rst::bindmat(blur, rst::extwrappedmat<int>(rst::qgrayimageadp(&in))),
               rst::qmutableimageadp<unsigned char>(&out),
               (double(*)(double,double))std::hypot,
               1, 30);
    
    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(out));

    myLabel.show();

    return a.exec();
}