#ifndef BGFIELDITEMATTACHEDTYPE_H
#define BGFIELDITEMATTACHEDTYPE_H

#include <QObject>
#include <QQmlEngine>

class BGFieldItem;

class BGFieldItemAttachedType : public QObject {
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(BGFieldItem* field READ field CONSTANT)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation
                   NOTIFY orientationChanged)
    Q_PROPERTY(bool fillWidth READ fillWidth WRITE setFillWidth NOTIFY
                   fillWidthChanged)

public:
    explicit BGFieldItemAttachedType(QObject* parent = nullptr);

    BGFieldItem* field() const;
    void setField(BGFieldItem* field);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    bool fillWidth() const;
    void setFillWidth(bool fillWidth);

signals:
    void orientationChanged();
    void fillWidthChanged();

private:
    BGFieldItem* m_field = nullptr;

    bool m_orientationChanged = false;
    Qt::Orientation m_orientation = Qt::Horizontal;

    bool m_fillWidthChanged = false;
    bool m_fillWidth = false;
};

#endif  // BGFIELDITEMATTACHEDTYPE_H
