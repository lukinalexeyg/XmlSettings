#include <QCoreApplication>
#include <QDebug>
#include "xmlsettings.h"

struct User
{
    QString name;
    QString password;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("XmlSettings example");
    QCoreApplication::setOrganizationName("Alexey Lukin");

    QSettings settings("settings.xml", XML_SETTINGS_FORMAT);

    qDebug() << "write";

    settings.beginGroup("app");
    settings.setValue("app_name", QCoreApplication::applicationName());
    settings.endGroup();

    const QList<User> USERS = {
        {"Alex", "ldEwMbCywgTsNztbl4Jj"},
        {"Martha", "j5uCqdyhNJ_FyQ2bwFFv"},
        {"Michael", "mIZSvSGHMlXk9gGcP0_B"}
    };

    settings.beginWriteArray("users");
    for (int i = 0; i < 3; ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", USERS.at(i).name);
        settings.setValue("password", USERS.at(i).password);
        qDebug() << USERS.at(i).name << USERS.at(i).password;
    }
    settings.endArray();

    qDebug() << "";
    qDebug() << "read";

    settings.beginReadArray("users");
    for (int i = 0; i < 3; ++i) {
        settings.setArrayIndex(i);
        qDebug() << settings.value("name").toString() << settings.value("password").toString();
    }
    settings.endArray();

    return app.exec();
}
