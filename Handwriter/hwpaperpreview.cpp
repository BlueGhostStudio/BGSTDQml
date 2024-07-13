#include "hwpaperpreview.h"

#include <QQuickWindow>

HWPaperPreview::HWPaperPreview() {}

void
HWPaperPreview::classBegin() {
    QQuickPaintedItem::classBegin();

    m_settings = qmlEngine(this)->singletonInstance<HWSettings*>(
        "BGStudio.Handwriter", "HWSettings");
    m_dataStorage = qmlEngine(this)->singletonInstance<HWDataStorage*>(
        "BGStudio.Handwriter", "HWDataStorage");

    QObject::connect(m_dataStorage, &HWDataStorage::paperUpdated,
                     this, [=](int id) {
                         if (m_localStorageID == id) reloadLocalStorageData();
                     });
    QObject::connect(m_dataStorage, &HWDataStorage::paperDeleted,
                     this, [=](int id) {
                         if (m_localStorageID == id) {
                             m_localStorageID = -1;
                             load(PaperData{});
                         }
                     });
}

void
HWPaperPreview::paint(QPainter* painter) {
    output(painter, QSize(implicitWidth(), implicitHeight()), QPointF(0, 0),
           window() ? window()->devicePixelRatio() : 1);
}

int
HWPaperPreview::localStorageID() const {
    return m_localStorageID;
}

bool
HWPaperPreview::load(const QUrl& url) {
    PaperData thePaperData = m_dataStorage->readFile(url);

    if (thePaperData.m_isValid) load(thePaperData);

    return thePaperData.m_isValid;
}

void
HWPaperPreview::loadData(const QByteArray& data) {
    PaperData thePaperData = m_dataStorage->readData(data);

    load(thePaperData);
}

bool
HWPaperPreview::loadLocalStorageData(int id) {
    PaperData thePaperData = m_dataStorage->readLocalStorageData(id);
    if (thePaperData.m_isValid) {
        m_localStorageID = id;
        emit localStorageIDChanged();

        load(thePaperData);
    }

    return thePaperData.m_isValid;
}

void
HWPaperPreview::reloadLocalStorageData() {
    if (m_localStorageID > 0) loadLocalStorageData(m_localStorageID);
}

void
HWPaperPreview::load(const PaperData& data) {
    setPalette(data.m_palette);
    setBackground(data.m_background);
    QSizeF implicitSize = data.m_paperSize * m_settings->dpi();
    setImplicitSize(implicitSize.width(), implicitSize.height());

    HWOutput::paint(data.m_strokeList);
    update();
}
