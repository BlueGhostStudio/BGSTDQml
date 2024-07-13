#ifndef HWVIEWPORTATTACHEDTYPE_H
#define HWVIEWPORTATTACHEDTYPE_H

#include <QObject>
#include <QQmlEngine>

class HWViewport;

class HWViewportAttachedType : public QObject {
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(HWViewport* viewport READ viewport NOTIFY viewportChanged)

public:
    explicit HWViewportAttachedType(QObject* parent = nullptr);

    HWViewport* viewport() const;

signals:
    void viewportChanged();
};

#endif  // HWVIEWPORTATTACHEDTYPE_H
