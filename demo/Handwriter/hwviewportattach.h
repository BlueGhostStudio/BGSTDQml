#ifndef HWVIEWPORTATTACH_H
#define HWVIEWPORTATTACH_H

#include <QObject>
#include <QQmlEngine>

class HWViewportAttach : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Attach type")
    QML_ATTACHED(HWViewportAttach)

    Q_PROPERTY(QObject* viewport READ viewport CONSTANT)

public:
    explicit HWViewportAttach(QObject *parent = nullptr);

    static HWViewportAttach* qmlAttachedProperties(QObject* obj);

    QObject* viewport() const;

signals:
};

#endif  // HWVIEWPORTATTACH_H
