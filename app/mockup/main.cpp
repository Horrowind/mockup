#include "cstdlib"
#include "cstdio"

#include <QApplication>
#include "main_window.hpp"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    //Mockup::MainWindow w((argc == 2) ? strtol(argv[1], NULL, 16) : 0x105);
    Mockup::MainWindow w;
    w.setGeometry(25,25,1024,768);
    w.show();
    return app.exec();
}
