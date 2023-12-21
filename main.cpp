#include <QApplication>

#include "widget.hpp"

int main(int argc, char* argv[]) {
        QApplication a(argc, argv);
        widget w;
        w.show();
        return QApplication::exec();
}
