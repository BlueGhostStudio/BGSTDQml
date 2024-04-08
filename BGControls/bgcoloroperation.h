#ifndef BGCOLOROPERATION_H
#define BGCOLOROPERATION_H

#include <QColor>
#include <QObject>
#include <QQmlEngine>

QColor BGColor_spin(const QColor& color, int amount);
QColor BGColor_shade(bool dark, const QColor& color, int shade, int mix = 10);
QColor BGColor_brightnessInversion(const QColor& color, qreal delta);
QColor BGColor_contrast(const QColor& color,
                        const QColor& dark = QColor(Qt::black),
                        const QColor& light = QColor(Qt::white),
                        qreal threshold = 0.5);
QColor BGColor_contrast2Color(const QColor& bgColor, const QColor& fgColor,
                              qreal delta = 0.6);
QColor BGColor_brightnessInversion(const QColor& color);

class BGColorOperation : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(BGColor)
    QML_SINGLETON

public:
    explicit BGColorOperation(QObject* parent = nullptr);

    Q_INVOKABLE QColor spin(const QColor& color, int amount);
    Q_INVOKABLE QColor shade(bool dark, const QColor& color, int shade,
                             int mix = 10);
    Q_INVOKABLE QColor brightnessInversion(const QColor& color, qreal delta);
    Q_INVOKABLE QColor contrast(const QColor& color,
                                const QColor& dark = QColor(Qt::black),
                                const QColor& light = QColor(Qt::white),
                                qreal threshold = 0.5);
    Q_INVOKABLE QColor contrast2Color(const QColor& bgColor,
                                      const QColor& fgColor, qreal delta = 0.6);
    Q_INVOKABLE QColor brightnessInversion(const QColor& color);

signals:
};

#endif  // BGCOLOROPERATION_H
