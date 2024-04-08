#ifndef HWPAPER_H
#define HWPAPER_H

#include <hwcanvas.h>
#include <hwguideline.h>

#include <QQuickItem>

class HWSettings;

class HWPaper : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        HWCanvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(QQuickItem* background READ background WRITE setBackground NOTIFY
                   backgroundChanged)

    // Q_PROPERTY(HWGuideLine* guideLine READ guideLine CONSTANT)

    Q_PROPERTY(QSize paperSize READ paperSize WRITE setPaperSize NOTIFY
                   paperSizeChanged)
    Q_PROPERTY(QSizeF paperSizeInch READ paperSizeInch WRITE setPaperSizeInch
                   NOTIFY paperSizeInchChanged)
    Q_PROPERTY(QSizeF paperSizeMm READ paperSizeMm WRITE setPaperSizeMm NOTIFY
                   paperSizeMmChanged)

public:
    HWPaper();

    void classBegin() override;
    void componentComplete() override;

    HWCanvas* canvas() const;
    void setCanvas(HWCanvas* newCanvas);

    QQuickItem* background() const;
    void setBackground(QQuickItem* background);

    // HWGuideLine* guideLine() const;

    QSizeF paperSizeMm() const;
    void setPaperSizeMm(const QSizeF& paperSize);

    QSizeF paperSizeInch() const;
    void setPaperSizeInch(const QSizeF& paperSize);

    QSize paperSize() const;
    void setPaperSize(const QSize& paperSize);

    Q_INVOKABLE void export2Image(const QUrl& url);

signals:
    void canvasChanged();

    void paperSizeMmChanged();

    void paperSizeInchChanged();

    void paperSizeChanged();

    void backgroundChanged();

private:
    void updatePaperSize();
    // void updatePaperSize(const QSize& size);

protected:
    HWSettings* m_settings;

private:
    HWCanvas* m_canvas = nullptr;
    QQuickItem* m_background = nullptr;
    // HWGuideLine* m_guideLine = nullptr;

    QSize m_paperSize;
    QSizeF m_paperSizeInch;
};

#endif  // HWPAPER_H
