#ifndef HWPAPERPREVIEW_H
#define HWPAPERPREVIEW_H

#include <QQuickItem>
#include <QQuickPaintedItem>

#include "hwdatastorage.h"
#include "hwoutput.h"
#include "hwsettings.h"

class HWPaperPreview : public QQuickPaintedItem, public HWOutput {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int localStorageID READ localStorageID NOTIFY
                   localStorageIDChanged FINAL)
public:
    HWPaperPreview();

    void classBegin() override;

    void paint(QPainter* painter) override;

    int localStorageID() const;

    Q_INVOKABLE bool load(const QUrl& url);
    Q_INVOKABLE void loadData(const QByteArray& data);
    Q_INVOKABLE bool loadLocalStorageData(int id);
    Q_INVOKABLE void reloadLocalStorageData();

signals:
    void localStorageIDChanged();

private:
    void load(const PaperData& data);

private:
    HWDataStorage* m_dataStorage;
    HWSettings* m_settings;

    int m_localStorageID = -1;
};

#endif  // HWPAPERPREVIEW_H
