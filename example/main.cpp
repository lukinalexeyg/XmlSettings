#include "xmlsettings.h"

#include <QSettings>
#include <QDebug>

struct User
{
    QString name;
    QString password;
};

int main()
{
    QSettings settings("settings." + XmlSettings::extension, XmlSettings::format);

    qDebug() << "write";

    settings.beginGroup("app");
    settings.setValue("app_name", "XmlSettingsExample");
    settings.endGroup();

    const QVector<User> users = {
        {"Alex", "ldEwMbCywgTsNztbl4Jj"},
        {"Martha", "j5uCqdyhNJ_FyQ2bwFFv"},
        {"Michael", "mIZSvSGHMlXk9gGcP0_B"}
    };

    settings.beginWriteArray("users");
    for (int i = 0; i < users.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", users.at(i).name);
        settings.setValue("password", users.at(i).password);
        qDebug() << users.at(i).name << users.at(i).password;
    }
    settings.endArray();

    qDebug() << "";
    qDebug() << "read";

    settings.beginReadArray("users");
    for (int i = 0; i < users.count(); ++i) {
        settings.setArrayIndex(i);
        qDebug() << settings.value("name").toString() << settings.value("password").toString();
    }
    settings.endArray();
}
