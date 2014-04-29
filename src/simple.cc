#include <QApplication>
#include <QImage>
#include <QLabel>
#include "rst/wrap.h"
#include "rst/filter/gaussian.h"
#include "rst/filter/canny.h"

static rst::gaussian_filter<3> stdgaussian(1.0);

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
    
    auto wrapper = rst::make_ext_wrapper([&in](int y, int x){return qGray(in.pixel(x, y));},
                                         in.height(), in.width());
    rst::canny([&wrapper](int y, int x){return stdgaussian.apply(wrapper,y,x);},
               [&out](int y, int x)->unsigned char & {return out.scanLine(y)[x];},
               (double(*)(double,double))std::hypot,
               in.height(), in.width(), 1, 30);
    
    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(out));

    myLabel.show();

    return a.exec();
}