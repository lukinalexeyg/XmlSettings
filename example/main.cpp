#include "xmlsettings.h"

#include <QSettings>
#include <QDebug>

static const QLatin1String s_users("users");
static const QLatin1String s_name("name");
static const QLatin1String s_password("password");

struct User
{
    QString name;
    QString password;
};

int main()
{
    QSettings settings("settings." + XmlSettings::extension, XmlSettings::format);

    qDebug() << "write";

    settings.beginGroup(QStringLiteral("app"));
    settings.setValue(s_name, QStringLiteral("XmlSettingsExample"));
    settings.endGroup();

    const QVector<User> users = {
        {"Alex", "ldEwMbCywgTsNztbl4Jj"},
        {"Martha", "j5uCqdyhNJ_FyQ2bwFFv"},
        {"Michael", "mIZSvSGHMlXk9gGcP0_B"}
    };

    settings.beginWriteArray(s_users);
    for (int i = 0; i < users.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(s_name, users.at(i).name);
        settings.setValue(s_password, users.at(i).password);
        qDebug() << users.at(i).name << users.at(i).password;
    }
    settings.endArray();

    qDebug() << "";
    qDebug() << "read";

    settings.beginReadArray(s_users);
    for (int i = 0; i < users.count(); ++i) {
        settings.setArrayIndex(i);
        qDebug() << settings.value(s_name).toString() << settings.value(s_password).toString();
    }
    settings.endArray();
}
