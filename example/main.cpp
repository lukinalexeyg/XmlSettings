#include "xmlsettings.h"

#include <QDebug>

static const QLatin1String s_users("users");
static const QLatin1String s_name("name");
static const QLatin1String s_hash("hash");

struct User
{
    QString name;
    QString hash;
};

int main()
{
    QSettings settings(QStringLiteral("%1.%2").arg(XmlSettings::rootTag, XmlSettings::extension), XmlSettings::format);

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
        settings.setValue(s_hash, users.at(i).hash);
        qDebug() << users.at(i).name << users.at(i).hash;
    }
    settings.endArray();

    qDebug() << "";
    qDebug() << "read";

    settings.beginReadArray(s_users);
    for (int i = 0; i < users.count(); ++i) {
        settings.setArrayIndex(i);
        qDebug() << settings.value(s_name).toString() << settings.value(s_hash).toString();
    }
    settings.endArray();
}
