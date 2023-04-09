#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#include <QSettings>

namespace XmlSettings {
    bool read(QIODevice &device, QSettings::SettingsMap &map);
    bool write(QIODevice &device, const QSettings::SettingsMap &map);

    static const QLatin1String extension("xml");
    static const QSettings::Format format = QSettings::registerFormat(extension, &read, &write);
    static const QString rootTag("settings");
    static const QString indexPrefix("index_");
}

class XmlNode : public QObject
{
    Q_OBJECT

public:
    QString tagName;
    QByteArray subText;

    XmlNode(const QString &name, const QByteArray &text = QByteArray(), QObject *parent = nullptr);
    QString fullPath() const;
};

#endif // XMSETTINGS_H
