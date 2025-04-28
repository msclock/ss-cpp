#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "_version.hpp"
#include "distribution.hpp"
#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTranslator translator;
    auto lang = QString("uiqt_%1").arg(QLocale::system().name());
    qDebug() << "Loaded translation file: uiqt_" << lang << ".qm";
    if (translator.load(lang, ":/i18n")) {
        app.installTranslator(&translator);
    }

    uiqt::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
