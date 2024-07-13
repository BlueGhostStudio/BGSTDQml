#ifndef HWGRABPREVIEW_H
#define HWGRABPREVIEW_H

#include <hwcanvas.h>
#include <hwpaper.h>

#include <QPicture>
#include <QQmlEngine>
#include <QQuickPaintedItem>

#include "hwoutput.h"

class HWGrabPreview : public QQuickPaintedItem, public HWOutput {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(
        HWCanvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(HWPaper* paper READ paper WRITE setPaper NOTIFY paperChanged)
    Q_PROPERTY(HWCVType::ContentVisible grabRange READ grabRange WRITE
                   setGrabRange NOTIFY grabRangeChanged)
    Q_PROPERTY(QUrl background READ background WRITE setBackground NOTIFY
                   backgroundChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool mono READ mono WRITE setMono NOTIFY monoChanged)
    Q_PROPERTY(bool outline READ outline WRITE setOutline NOTIFY outlineChanged)

public:
    HWGrabPreview();

    void paint(QPainter* painter) override;

    HWCanvas* canvas() const;
    void setCanvas(HWCanvas* canvas, bool set = true);

    HWPaper* paper() const;
    void setPaper(HWPaper* paper);

    HWCVType::ContentVisible grabRange() const;
    void setGrabRange(HWCVType::ContentVisible grabRange);

    void setBackground(const QUrl& background);
    void setColor(const QColor& newColor);
    void setMono(bool mono);
    void setOutline(bool outline);

    Q_INVOKABLE void grab();

signals:
    void canvasChanged();
    void paperChanged();

    void grabRangeChanged();
    void backgroundChanged();

    void colorChanged();
    void monoChanged();
    void outlineChanged();

private:
    HWCanvas* m_canvas = nullptr;
    HWPaper* m_paper = nullptr;
    HWCVType::ContentVisible m_grabRange = HWCVType::VisibleFullContent;

    /*QUrl m_background;
    QImage m_backgroundImage;
    QPicture m_contentPicture;

    QPainter m_renderPainter;
    QColor m_color{ Qt::white };*/
};

#endif  // HWGRABPREVIEW_H
