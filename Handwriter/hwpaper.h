#ifndef HWPAPER_H
#define HWPAPER_H

#include <hwcanvas.h>
#include <hwguideline.h>

#include <QPainter>
#include <QQuickPaintedItem>

#include "hwdatastorage.h"

class HWSettings;

class HWPaper : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(
        HWCanvas* canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(QUrl background READ background WRITE setBackground NOTIFY
                   backgroundChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    Q_PROPERTY(HWGuideLine* guideLine READ guideLine CONSTANT)

    Q_PROPERTY(QSize paperSize READ paperSize WRITE setPaperSize NOTIFY
                   paperSizeChanged)
    Q_PROPERTY(QSizeF paperSizeInch READ paperSizeInch WRITE setPaperSizeInch
                   NOTIFY paperSizeInchChanged)
    Q_PROPERTY(QSizeF paperSizeMm READ paperSizeMm WRITE setPaperSizeMm NOTIFY
                   paperSizeMmChanged)

    Q_PROPERTY(
        int localStorageID READ localStorageID NOTIFY localStorageIDChanged)

public:
    HWPaper();
    ~HWPaper();

    void classBegin() override;
    void componentComplete() override;

    void paint(QPainter* painter) override;

    HWCanvas* canvas() const;
    void setCanvas(HWCanvas* newCanvas);

    QUrl background() const;
    void setBackground(const QUrl& background);

    QColor color() const;
    void setColor(const QColor& color);

    HWGuideLine* guideLine() const;

    QSizeF paperSizeMm() const;
    void setPaperSizeMm(const QSizeF& paperSize);

    QSizeF paperSizeInch() const;
    void setPaperSizeInch(const QSizeF& paperSize);

    QSize paperSize() const;
    void setPaperSize(const QSize& paperSize);

    int localStorageID() const;

    Q_INVOKABLE void export2Image(const QUrl& url);
    Q_INVOKABLE bool exportFile(const QUrl& url);
    Q_INVOKABLE QByteArray exportData();
    Q_INVOKABLE bool importFile(const QUrl& url);
    Q_INVOKABLE bool importData(const QByteArray& data);

    Q_INVOKABLE void newPaper();
    Q_INVOKABLE bool loadLocalStorageData(int id);
    Q_INVOKABLE bool saveLocalStorageData();

signals:
    void canvasChanged();

    void paperSizeMmChanged();

    void paperSizeInchChanged();

    void paperSizeChanged();

    void backgroundChanged();

    void colorChanged();

    void localStorageIDChanged();

private:
    void updatePaperSize();
    void load(const PaperData& data);
    // void updatePaperSize(const QSize& size);

private:
    HWSettings* m_settings;

    HWDataStorage* m_dataStorage = nullptr;
    HWCanvas* m_canvas = nullptr;
    // QQuickItem* m_background = nullptr;
    HWGuideLine* m_guideLine = nullptr;

    QUrl m_background;
    QImage m_backgroundImage;
    QColor m_color{ Qt::white };

    QSize m_paperSize;
    QSizeF m_paperSizeInch;

    int m_localStorageID = -1;
};

#endif  // HWPAPER_H
