#ifndef HWGESTUREAREA_H
#define HWGESTUREAREA_H

#include <QQuickItem>

class HWViewport;

class HWGestureArea : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isGestureInProgress READ isGestureInProgress NOTIFY
                   isGestureInProgressChanged)
public:
    HWGestureArea(QQuickItem* parent = nullptr);

    void classBegin() override;

    bool isGestureInProgress() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent*) override;

    void touchEvent(QTouchEvent* event) override;

signals:
    void isGestureInProgressChanged();

private:
    HWViewport* m_viewport;

    QPointF m_lastPos;

    QPointF m_pressPos;
    qreal m_zoom = 1;
    qreal m_lastZoom = 1;

    qreal m_pressPinchDist = 0;

    bool m_isGestureInProgress = false;
};

#endif  // HWGESTUREAREA_H
