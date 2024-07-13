#include "hwpaper.h"

#include <hwsettings.h>

#include <QQuickItemGrabResult>
#include <QQuickWindow>

HWPaper::HWPaper() {}

HWPaper::~HWPaper() {
    if (m_canvas)
        m_canvas->deleteLater();
    if (m_guideLine)
        m_guideLine->deleteLater();
}

void
HWPaper::classBegin() {
    QQuickPaintedItem::classBegin();
    m_settings = qmlEngine(this)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");
    m_dataStorage = qmlEngine(this)->singletonInstance<HWDataStorage*>(
        "BGStudio.Handwriter", "HWDataStorage");

    QObject::connect(m_settings, &HWSettings::dpiChanged, this,
                     [=]() { updatePaperSize(); });

    /*m_guideLine = new HWGuideLine(this);
    m_guideLine->setZ(1);*/

    m_guideLine = new HWGuideLine(this);
    m_guideLine->setZ(0);
    m_guideLine->setVisible(false);

    QObject* anchors = qobject_cast<QObject*>(
        m_guideLine->property("anchors").value<QObject*>());
    if (anchors) anchors->setProperty("fill", QVariant::fromValue(this));
}

void
HWPaper::componentComplete() {
    QQuickPaintedItem::componentComplete();
    updatePaperSize();
}

void
HWPaper::paint(QPainter* painter) {
    painter->eraseRect(0, 0, width(), height());

    if (!m_backgroundImage.isNull())
        painter->drawImage(QRectF(0, 0, width(), height()), m_backgroundImage);
    else {
        QPen pen(QColor(Qt::gray), 1);
        painter->setPen(pen);
        painter->setBrush(m_color);
        painter->drawRect(0, 0, width() - 1, height() - 1);
    }
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
        m_canvas->setZ(1);

        QObject* anchors = qobject_cast<QObject*>(
            m_canvas->property("anchors").value<QObject*>());
        if (anchors) anchors->setProperty("fill", QVariant::fromValue(this));

        emit canvasChanged();
    }
}

QUrl
HWPaper::background() const {
    return m_background;
}

void
HWPaper::setBackground(const QUrl& background) {
    if (m_background != background) {
        m_background = background;

        if (m_background.isEmpty())
            m_backgroundImage = QImage();
        else if (m_background.scheme() == "qrc")
            m_backgroundImage.load(":" + m_background.path());
        else
            m_backgroundImage.load(m_background.path());

        emit backgroundChanged();
        update();
    }
}

QColor
HWPaper::color() const {
    return m_color;
}

void
HWPaper::setColor(const QColor& color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

HWGuideLine*
HWPaper::guideLine() const {
    return m_guideLine;
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

int
HWPaper::localStorageID() const {
    return m_localStorageID;
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

bool
HWPaper::exportFile(const QUrl& url) {
    /*QFile file(url.path());

     if (file.open(QIODevice::WriteOnly)) {
         QJsonObject jsoPaper;

          QJsonArray jsaPalette;
          QList<QColor> palette = m_canvas->canvasPalette();
          foreach (const QColor& color, palette) {
              jsaPalette.append(color.name());
          }
          jsoPaper["palette"] = jsaPalette;
          jsoPaper["size"] =
              QJsonObject{ { "width", m_paperSizeInch.width() },
                           { "height", m_paperSizeInch.height() } };
          jsoPaper["background"] = m_background.toString();
          jsoPaper["guideline"] =
              qmlEngine(this)->fromScriptValue<QJsonValue>(m_guideLine->layout());

         file.write(QJsonDocument(jsoPaper).toJson(QJsonDocument::Compact));
         file.write("\n");

          file.write(sl2b(m_canvas->strokes(HWCVType::VisibleFullContent)));

         file.close();

          return true;
      } else
          return false;*/

    PaperData theData{ m_paperSizeInch, m_background, m_canvas->canvasPalette(),
                       m_guideLine->layout(),
                       m_canvas->strokes(HWCVType::VisibleFullContent) };

    return m_dataStorage->saveFile(url, theData);
}

QByteArray
HWPaper::exportData() {
    return m_dataStorage->saveData(
        { m_paperSizeInch, m_background, m_canvas->canvasPalette(),
          m_guideLine->layout(),
          m_canvas->strokes(HWCVType::VisibleFullContent) });
}

bool
HWPaper::importFile(const QUrl& url) {
    PaperData thePaperData = m_dataStorage->readFile(url);

    if (thePaperData.m_isValid) {
        load(thePaperData);
    }

    return thePaperData.m_isValid;
    /*QFile file(url.path());

     if (file.open(QIODevice::ReadOnly)) {
         m_canvas->clear();
         QJsonObject jsoData =
     QJsonDocument::fromJson(file.readLine()).object(); QJsonObject jsoSize =
     jsoData["size"].toObject(); setPaperSizeInch(
             QSizeF(jsoSize["width"].toDouble(), jsoSize["height"].toDouble()));

          QJsonArray jsaPalette = jsoData["palette"].toArray();
          QList<QColor> palette;
          foreach(QJsonValue jsvColor, jsaPalette) {
              palette.append(QColor::fromString(jsvColor.toString()));
          }
          m_canvas->setCanvasPalette(palette);
          setBackground(jsoData["background"].toString());

         m_guideLine->setLayout(
             qmlEngine(this)->toScriptValue(jsoData["guideline"].toObject()));

          m_canvas->drawStrokes(b2sl(file.readAll()));

         file.close();
         return true;
     } else
         return false;*/
}

bool
HWPaper::importData(const QByteArray& data) {
    PaperData thePaperData = m_dataStorage->readData(data);

    if (thePaperData.m_isValid) {
        load(thePaperData);
    }

    return thePaperData.m_isValid;
}

void
HWPaper::newPaper() {
    m_canvas->clear();
    m_localStorageID = -1;
}

bool
HWPaper::loadLocalStorageData(int id) {
    PaperData thePaperData = m_dataStorage->readLocalStorageData(id);
    if (thePaperData.m_isValid) {
        m_localStorageID = id;
        emit localStorageIDChanged();

        load(thePaperData);
    }

    return thePaperData.m_isValid;
}

bool
HWPaper::saveLocalStorageData() {
    PaperData thePaperData{ m_paperSizeInch, m_background,
                            m_canvas->canvasPalette(), m_guideLine->layout(),
                            m_canvas->strokes(HWCVType::VisibleFullContent) };
    qDebug() << "保存" << m_localStorageID;
    if (m_localStorageID > 0) {
        return m_dataStorage->updatePaper(m_localStorageID, thePaperData);
    } else {
        m_localStorageID = m_dataStorage->insertPaper(thePaperData);
        emit localStorageIDChanged();
        return m_localStorageID > 0;
    }
}

void
HWPaper::updatePaperSize() {
    // QSizeF paperSize = m_paperSize;

    if (m_paperSizeInch.isValid()) {
        if (m_canvas) {
            m_paperSize = (m_paperSizeInch * m_settings->dpi()).toSize();
            emit paperSizeChanged();
        }
    }

    if (m_paperSize.isValid()) {
        setSize(m_paperSize);
        setImplicitSize(m_paperSize.width(), m_paperSize.height());
        if (m_canvas) {
            // m_canvas->updateCanvasSize(m_paperSize);
            m_canvas->updateCanvas();
        }
        /*if (m_background) {
            m_background->setSize(m_paperSize);
        }*/
    }

    update();
    // updatePaperSize((m_paperSizeInch * m_canvas->dpi()).toSize());
}

void
HWPaper::load(const PaperData& data) {
    m_canvas->clear();
    setPaperSizeInch(data.m_paperSize);
    m_canvas->setCanvasPalette(data.m_palette);
    setBackground(data.m_background);
    m_guideLine->setLayout(data.m_guidelineLayout);

    m_canvas->drawStrokes(data.m_strokeList);
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
