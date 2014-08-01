#include <QApplication>
#include "controller.hh"

int main(int argc, char *argv[]) {
    //Q_INIT_RESOURCE(application);
    
    QApplication app(argc, argv);
    
    app.setOrganizationName("EMMS-HTW-SS2014");
    app.setApplicationName("Hough Filter Studio");
    
    mm::controller controller;
    
    return app.exec();
}

    
