#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H

#pragma once

#include <QSettings>

bool readXmlSettings(QIODevice &device, QSettings::SettingsMap &map);
bool writeXmlSettings(QIODevice &device, const QSettings::SettingsMap &map);

static const QSettings::Format XML_SETTINGS_FORMAT = QSettings::registerFormat("xml", &readXmlSettings, &writeXmlSettings);
static const QString XML_SETTINGS_ROOT_TAG = "settings";
static const QString XML_SETTINGS_INDEX_PREFIX = "index_";

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
