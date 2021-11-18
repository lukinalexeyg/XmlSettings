#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#pragma once

#include <QSettings>

namespace XmlSettings {
    bool read(QIODevice &device, QSettings::SettingsMap &map);
    bool write(QIODevice &device, const QSettings::SettingsMap &map);

    static const QString extension = QStringLiteral("xml");
    static const QSettings::Format format = QSettings::registerFormat(extension, &read, &write);
    static const QString rootTag = QStringLiteral("settings");
    static const QString indexPrefix = QStringLiteral("index_");
};

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
