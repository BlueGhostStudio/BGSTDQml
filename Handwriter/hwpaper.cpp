#include "hwpaper.h"

#include <hwsettings.h>

#include <QQuickItemGrabResult>

HWPaper::HWPaper() {}

void
HWPaper::classBegin() {
    QQuickItem::classBegin();
    m_settings = qmlEngine(this)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");

    QObject::connect(m_settings, &HWSettings::dpiChanged, this,
                     [=]() { updatePaperSize(); });

    /*m_guideLine = new HWGuideLine(this);
    m_guideLine->setZ(1);*/
}

void
HWPaper::componentComplete() {
    QQuickItem::componentComplete();
    updatePaperSize();
}

HWCanvas*
HWPaper::canvas() const {
    return m_canvas;
}

void
HWPaper::setCanvas(HWCanvas* canvas) {
    if (m_canvas != canvas) {
        m_canvas = canvas;
        m_canvas->setParentItem(this);
        m_canvas->setZ(2);

        QObject* anchors = qobject_cast<QObject*>(
            m_canvas->property("anchors").value<QObject*>());
        if (anchors)
            anchors->setProperty("fill", QVariant::fromValue(this));

        emit canvasChanged();
    }
}

QQuickItem*
HWPaper::background() const {
    return m_background;
}

void
HWPaper::setBackground(QQuickItem* background) {
    if (m_background != background) {
        m_background = background;
        m_background->setParentItem(this);
        m_background->setZ(0);

        QObject* anchors = qobject_cast<QObject*>(
            m_background->property("anchors").value<QObject*>());
        if (anchors)
            anchors->setProperty("fill", QVariant::fromValue(this));

        emit backgroundChanged();
    }
}

/*HWGuideLine* HWPaper::guideLine() const {
    return m_guideLine;
}*/

QSize
HWPaper::paperSize() const {
    return m_paperSize;
}

void
HWPaper::setPaperSize(const QSize& paperSize) {
    if (m_paperSize != paperSize) {
        m_paperSize = paperSize;
        m_paperSizeInch = QSizeF();
        // updatePaperSize();

        emit paperSizeChanged();
    }
}

void
HWPaper::export2Image(const QUrl& url) {
    QSharedPointer<QQuickItemGrabResult> result = grabToImage();
    QObject::connect(
        result.data(), &QQuickItemGrabResult::ready, this,
        [=]() {
            QString filename = url.toLocalFile();
            result->image().save(filename);
        },
        Qt::SingleShotConnection);
}

QSizeF
HWPaper::paperSizeInch() const {
    return m_paperSizeInch;
}

void
HWPaper::setPaperSizeInch(const QSizeF& paperSize) {
    if (m_paperSizeInch != paperSize) {
        m_paperSize = QSize();
        m_paperSizeInch = paperSize;
        updatePaperSize();

        emit paperSizeInchChanged();
        emit paperSizeMmChanged();
    }
}

QSizeF
HWPaper::paperSizeMm() const {
    return m_paperSizeInch / 0.03937008;
}

void
HWPaper::setPaperSizeMm(const QSizeF& paperSize) {
    QSizeF inchSize = paperSize * 0.03937008;
    if (m_paperSizeInch != inchSize) {
        setPaperSizeInch(inchSize);
    }
}

void
HWPaper::updatePaperSize() {
    QSizeF paperSize = m_paperSize;

    if (m_paperSizeInch.isValid()) {
        if (m_canvas) {
            paperSize = (m_paperSizeInch * m_settings->dpi()).toSize();
            emit paperSizeChanged();
        }
    }

    if (paperSize.isValid()) {
        setSize(paperSize);
        setImplicitSize(paperSize.width(), paperSize.height());
        if (m_canvas) {
            // m_canvas->updateCanvasSize(m_paperSize);
            m_canvas->updateCanvas();
        }
        /*if (m_background) {
            m_background->setSize(m_paperSize);
        }*/
    }
    // updatePaperSize((m_paperSizeInch * m_canvas->dpi()).toSize());
}

/*void
HWPaper::updatePaperSize(const QSize& size) {
    if (m_paperSize != size) {
        m_paperSize = size;
        setSize(m_paperSize);

        setImplicitSize(m_paperSize.width(), m_paperSize.height());

        m_canvas->updateCanvasSize(m_paperSize);
        m_canvas->updateCanvas();

        emit paperSizeChanged();
        emit paperSizeInchChanged();
        emit paperSizeMmChanged();
    }
}*/
