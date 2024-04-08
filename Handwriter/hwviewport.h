#ifndef HWVIEWPORT_H
#define HWVIEWPORT_H

#include <hwcanvas.h>
#include <hwpaper.h>

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

#include "hwviewportattachedtype.h"

class HWGestureArea;

class HWViewport : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(HWViewportAttachedType)

    Q_PROPERTY(HWPaper* paper READ paper WRITE setPaper NOTIFY paperChanged)
    Q_PROPERTY(
        HWCanvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(
        QQuickItem* gestureArea READ gestureArea NOTIFY gestureAreaChanged)
    Q_PROPERTY(bool panAniEnabled READ panAniEnabled WRITE setPanAniEnabled
                   NOTIFY panAniEnabledChanged)
    Q_PROPERTY(bool zoomed READ zoomed NOTIFY zoomedChanged)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY
                   zoomFactorChanged)
    Q_PROPERTY(qreal maxZoomRatio READ maxZoomRatio WRITE setMaxZoomRatio NOTIFY
                   maxZoomRatioChanged)

public:
    HWViewport(QQuickItem* parent = nullptr);

    static HWViewportAttachedType* qmlAttachedProperties(QObject* obj);

    void classBegin() override;

    HWPaper* paper() const;
    void setPaper(HWPaper* paper);

    HWCanvas* canvas() const;
    void setCanvas(HWCanvas* canvas);

    QQuickItem* gestureArea() const;

    bool panAniEnabled() const;
    void setPanAniEnabled(bool panAniEnabled);

    Q_INVOKABLE void pan(qreal dx, qreal dy);
    Q_INVOKABLE void panTo(qreal dx, qreal dy);
    Q_INVOKABLE void panToX(qreal dx, qreal margin = 0);
    Q_INVOKABLE void panToY(qreal dy, qreal margin = 0);
    Q_INVOKABLE void panLeft(qreal dx, qreal margin = 0);

    Q_INVOKABLE void zoom(qreal zoom, qreal zoomCenterX, qreal zoomCenterY);

    bool zoomed() const;

    qreal zoomFactor() const;
    void setZoomFactor(qreal zoomFactor);

    qreal maxZoomRatio() const;
    void setMaxZoomRatio(qreal maxZoomRatio);

signals:
    void paperChanged();
    void canvasChanged();
    void gestureAreaChanged();
    void panAniEnabledChanged();

    void zoomedChanged();
    void zoomFactorChanged();
    void maxZoomRatioChanged();

private:
    void clampPositionX(qreal x);
    void clampPositionY(qreal y);
    void clampPosition(qreal x, qreal y);

protected:
    QQmlEngine* m_qmlEngine;

    QQmlContext* m_context = nullptr;
    HWPaper* m_paper = nullptr;
    HWCanvas* m_canvas = nullptr;
    // QQuickItem* m_gestureArea = nullptr;

    HWGestureArea* m_gestureArea = nullptr;

    bool m_panAniEnabled;

    qreal m_zoomFactor = 0.5;
    qreal m_maxZoomRatio = 50;
};

#endif  // HWVIEWPORT_H
