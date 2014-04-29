#include <QApplication>
#include <QImage>
#include <QLabel>
#include "rst/wrap.h"
#include "rst/filter/gaussian.h"
#include "rst/filter/canny.h"

static rst::gaussian_filter<3> stdgaussian(1.0);

struct ImageProxy {
    ImageProxy(QImage *img) : image(img) {}
    
    unsigned char get(int y, int x) const { return qGray(image->pixel(x, y)); }
    unsigned char operator() (int y, int x) const { return get(y, x); }
    void set(int y, int x, char v) const { image->setPixel(x, y, qRgb(v,v,v)); }
    
private:
    QImage *image;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QImage in;
    in.load(argv[1]);
    
    QImage out(in.size(), QImage::Format_RGB888);
    
    auto wrapper = rst::make_ext_wrapper(ImageProxy(&in), in.height(), in.width());
    rst::canny([&wrapper](int y, int x){return stdgaussian.apply(wrapper,y,x);},
               ImageProxy(&out),
               (double(*)(double,double))std::hypot,
               in.height(), in.width(), 1, 30);
    
    QLabel myLabel;
    myLabel.setPixmap(QPixmap::fromImage(out));

    myLabel.show();

    return a.exec();
}