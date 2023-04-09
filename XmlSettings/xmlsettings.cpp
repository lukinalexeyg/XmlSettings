#include "xmlsettings.h"

#include <QXmlStreamReader>

static const QLatin1Char s_separator('/');



XmlNode::XmlNode(const QString &name, const QByteArray &text, QObject *parent) :
    QObject(parent),
    tagName(name),
    subText(text)
{
}



QString XmlNode::fullPath() const
{
    const XmlNode *cur = this;
    QString path = tagName;

    while ((cur = qobject_cast<const XmlNode*>(cur->parent())) != nullptr)
        path.prepend(cur->tagName + s_separator);

    return path.mid(XmlSettings::rootTag.count() + 1);
}



bool XmlSettings::read(QIODevice &device, QSettings::SettingsMap &map)
{
    QXmlStreamReader xml(&device);
    XmlNode *curNode = nullptr;

    while (!xml.atEnd()) {
        switch(xml.readNext()) {

            case QXmlStreamReader::StartElement:
                if (curNode != nullptr) {
                    QString tag = xml.name().toString();
                    if (tag.startsWith(indexPrefix))
                        tag = tag.mid(indexPrefix.count());
                    curNode = new XmlNode(tag, QByteArray(), curNode);
                }
                else if (xml.name().toString() == rootTag)
                    curNode = new XmlNode(rootTag);
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



bool XmlSettings::write(QIODevice &device, const QSettings::SettingsMap &map)
{
    XmlNode *root = new XmlNode(rootTag);

    for (const QString &unsplitKey : map.keys()) {
        QStringList segs = unsplitKey.split(s_separator, Qt::SkipEmptyParts);
        QByteArray val = map[unsplitKey].toByteArray();
        XmlNode *cur = root;

        for (int i = 0; i < segs.length(); i++) {
            if (i == segs.length() - 1)
                new XmlNode(segs[i], val, cur);
            else {
                XmlNode *foundItem = nullptr;
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

        while ((cur = stack.takeLast()) == nullptr) {
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
            xml.writeStartElement(indexPrefix + cur->tagName);

        stack << nullptr;

        if (cur->children().isEmpty())
            xml.writeCharacters(cur->subText);
        else
            for (int i = cur->children().length()-1; i >= 0; i--)
                stack << qobject_cast<XmlNode*>(cur->children()[i]);
    }

    return false;
}
