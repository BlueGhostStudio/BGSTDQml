#ifndef HWVIEWPORT_H
#define HWVIEWPORT_H

#include <hwcanvas.h>
#include <hwpaper.h>

#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>

#include "hwviewportattachedtype.h"

// class HWGestureArea;

class HWViewport : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(HWViewportAttachedType)

    Q_CLASSINFO("HWQmlBaseType", "HWViewport")

    Q_PROPERTY(QQuickItem* paper READ paper WRITE setPaper NOTIFY paperChanged)
    /*Q_PROPERTY(
        HWCanvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)*/
    /*Q_PROPERTY(
        QQuickItem* gestureArea READ gestureArea NOTIFY gestureAreaChanged)*/
    Q_PROPERTY(bool isGestureInProgress READ isGestureInProgress NOTIFY
                   isGestureInProgressChanged)

    /*Q_PROPERTY(int singlePointState READ singlePointState NOTIFY
                   singlePointStateChanged)*/
    Q_PROPERTY(bool singlePointEnable READ singlePointEnable WRITE
                   setSinglePointEnable NOTIFY singlePointEnableChanged FINAL)
    Q_PROPERTY(QPointF lastPos READ lastPos NOTIFY lastPosChanged)

    Q_PROPERTY(bool panAniEnabled READ panAniEnabled WRITE setPanAniEnabled
                   NOTIFY panAniEnabledChanged)
    Q_PROPERTY(bool zoomed READ zoomed NOTIFY zoomedChanged)
    Q_PROPERTY(qreal zoomRatio READ zoomRatio NOTIFY zoomRatioChanged)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY
                   zoomFactorChanged)
    Q_PROPERTY(qreal maxZoomRatio READ maxZoomRatio WRITE setMaxZoomRatio NOTIFY
                   maxZoomRatioChanged)

public:
    HWViewport(QQuickItem* parent = nullptr);

    static HWViewportAttachedType* qmlAttachedProperties(QObject* obj);

    void classBegin() override;
    // void componentComplete() override;

    QQuickItem* paper() const;
    void setPaper(QQuickItem* paper);

    /*HWCanvas* canvas() const;
    void setCanvas(HWCanvas* canvas);*/

    // QQuickItem* gestureArea() const;

    bool isGestureInProgress() const;
    void setGestureInProgress(bool enable);

    // int singlePointState() const;

    bool singlePointEnable() const;
    void setSinglePointEnable(bool singlePointEnable);

    QPointF lastPos() const;
    void setLastPos(const QPointF& pos);

    bool panAniEnabled() const;
    void setPanAniEnabled(bool panAniEnabled);

    Q_INVOKABLE void pan(qreal dx, qreal dy);
    Q_INVOKABLE void panTo(qreal dx, qreal dy);
    Q_INVOKABLE void panToX(qreal dx, qreal margin = 0);
    Q_INVOKABLE void panToY(qreal dy, qreal margin = 0);
    Q_INVOKABLE void panLeft(qreal dx, qreal margin = 0);

    Q_INVOKABLE void zoom(qreal zoom, qreal zoomCenterX, qreal zoomCenterY,
                          bool viewportCenter = false);

    bool zoomed() const;
    qreal zoomRatio() const;

    qreal zoomFactor() const;
    void setZoomFactor(qreal zoomFactor);

    qreal maxZoomRatio() const;
    void setMaxZoomRatio(qreal maxZoomRatio);

signals:
    void paperChanged();
    void canvasChanged();
    // void gestureAreaChanged();
    void panAniEnabledChanged();

    void zoomedChanged();
    void zoomRatioChanged();
    void zoomFactorChanged();
    void maxZoomRatioChanged();

    void isGestureInProgressChanged();
    void lastPosChanged();

    void singlePointEnableChanged();

private:
    void clampPositionX(qreal x);
    void clampPositionY(qreal y);
    void clampPosition(qreal x, qreal y);

    void enableSinglePoint();
    void disableSinglePoint();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent*) override;

    void touchEvent(QTouchEvent* event) override;

protected:
    QQmlEngine* m_qmlEngine;
    HWSettings* m_settings;

    QQmlContext* m_context = nullptr;
    QQuickItem* m_paper = nullptr;
    // HWCanvas* m_canvas = nullptr;

    // int m_singlePointState = -1;
    bool m_singlePointEnable = true;
    QQuickItem* m_singlePointIndicator;

    bool m_panAniEnabled;

    qreal m_zoomFactor = 0.5;
    qreal m_maxZoomRatio = 50;

    QPointF m_lastPos;
    QPointF m_pressPos;

    qreal m_zoom = 1;
    qreal m_lastZoom = 1;

    qreal m_pressPinchDist = 0;

    bool m_isGestureInProgress = false;

    // QObject* m_paperAni = nullptr;
};

#endif  // HWVIEWPORT_H
