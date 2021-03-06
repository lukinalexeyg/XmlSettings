#include <QXmlStreamReader>
#include "xmlsettings.h"



XmlNode::XmlNode(const QString &name, const QByteArray &text, QObject *parent) :
    QObject(parent),
    tagName(name),
    subText(text)
{}



QString XmlNode::fullPath() const
{
    const XmlNode *cur = this;
    QString path = tagName;

    while ((cur = qobject_cast<const XmlNode*>(cur->parent())) != Q_NULLPTR)
        path.prepend(cur->tagName + "/");

    return path.mid(XML_SETTINGS_ROOT_TAG.count() + 1);
};



bool readXmlSettings(QIODevice &device, QSettings::SettingsMap &map)
{
    QXmlStreamReader xml(&device);
    XmlNode *curNode = Q_NULLPTR;

    while (!xml.atEnd()) {
        switch(xml.readNext()) {

            case QXmlStreamReader::StartElement:
                if (curNode != Q_NULLPTR) {
                    QString tag = xml.name().toString();
                    if (tag.startsWith(XML_SETTINGS_INDEX_PREFIX))
                        tag = tag.mid(XML_SETTINGS_INDEX_PREFIX.count());
                    curNode = new XmlNode(tag, QByteArray(), curNode);
                }
                else if (xml.name().toString() == XML_SETTINGS_ROOT_TAG)
                    curNode = new XmlNode(XML_SETTINGS_ROOT_TAG);
                else
                    return false;
                break;

            case QXmlStreamReader::EndElement:
                if (!curNode->parent()) {
                    delete curNode;
                    return true;
                }
                else
                    curNode = qobject_cast<XmlNode*>(QScopedPointer<XmlNode>(curNode)->parent());
                break;

            case QXmlStreamReader::Characters:
                if (!xml.isWhitespace()) {
                    QString text = xml.text().toString();
                    map[curNode->fullPath()] = text;
                }
                break;
        }
    }

    map.clear();
    return false;
}



bool writeXmlSettings(QIODevice &device, const QSettings::SettingsMap &map)
{
    XmlNode *root = new XmlNode(XML_SETTINGS_ROOT_TAG);

    for (const QString &unsplitKey : map.keys()) {
        QStringList segs = unsplitKey.split("/", QString::SkipEmptyParts);
        QByteArray val = map[unsplitKey].toByteArray();
        XmlNode *cur = root;

        for (int i = 0; i < segs.length(); i++) {
            if (i == segs.length() - 1)
                new XmlNode(segs[i], val, cur);
            else {
                XmlNode *foundItem = Q_NULLPTR;
                for (QObject *object : cur->children()) {
                    XmlNode *child = qobject_cast<XmlNode*>(object);
                    if (QString::compare(child->tagName, segs[i], Qt::CaseInsensitive) == 0) {
                        foundItem = child;
                        break;
                    }
                }

                if (!foundItem)
                    foundItem = new XmlNode(segs[i], QByteArray(), cur);

                cur = foundItem;
            }
        }
    }

    QXmlStreamWriter xml(&device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(-1);
    xml.writeStartDocument();

    QList<XmlNode*> stack;
    stack << root;

    while (true) {
        XmlNode *cur;

        while ((cur = stack.takeLast()) == Q_NULLPTR) {
            xml.writeEndElement();

            if (stack.isEmpty()) {
                xml.writeEndDocument();
                delete root;
                return true;
            }
        }

        bool isNumber;
        cur->tagName.toInt(&isNumber);

        if (!isNumber)
            xml.writeStartElement(cur->tagName);
        else
            xml.writeStartElement(XML_SETTINGS_INDEX_PREFIX + cur->tagName);

        stack << Q_NULLPTR;

        if (cur->children().size() == 0)
            xml.writeCharacters(cur->subText);
        else
            for (int i = cur->children().length()-1; i >= 0; i--)
                stack << qobject_cast<XmlNode*>(cur->children()[i]);
    }

    return false;
}
