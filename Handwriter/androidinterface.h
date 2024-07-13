#ifndef ANDROIDINTERFACE_H
#define ANDROIDINTERFACE_H

#include <QObject>
#include <QRect>
#include <QQmlEngine>

class AndroidInterface : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(int screenOrientation READ screenOrientation WRITE
                   setScreenOrientation NOTIFY screenOrientationChanged)
    Q_PROPERTY(
        QRect cutout_boundingRectTop READ cutout_boundingRectTop CONSTANT)
    Q_PROPERTY(
        QRect cutout_boundingRectBottom READ cutout_boundingRectBottom CONSTANT)
    Q_PROPERTY(
        QRect cutout_boundingRectLeft READ cutout_boundingRectLeft CONSTANT)
    Q_PROPERTY(
        QRect cutout_boundingRectRight READ cutout_boundingRectRight CONSTANT)

public:
    explicit AndroidInterface(QObject* parent = nullptr);

    // Q_INVOKABLE setScreenOrientation

    int screenOrientation() const;
    void setScreenOrientation(int orientation);

    Q_INVOKABLE void setFullSreen();
    QRect cutout_boundingRectTop() const;
    QRect cutout_boundingRectBottom() const;
    QRect cutout_boundingRectLeft() const;
    QRect cutout_boundingRectRight() const;
    // Q_INVOKABLE void testCutout();

signals:
    void screenOrientationChanged();

private:
    QRect cutout_boundingRect(const char* method) const;
    qreal productVersion() const;
};

#endif  // ANDROIDINTERFACE_H
