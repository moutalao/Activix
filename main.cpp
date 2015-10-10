#include <QApplication>
#include "serveur.h"
#include "ui_serveur.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
        QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

    Serveur fenetre;
    fenetre.show();

    return app.exec();
}
