#ifndef HWLSDPAPERMODEL_H
#define HWLSDPAPERMODEL_H

#include <QAbstractTableModel>
#include <QQmlEngine>

class HWLSDPaperModel : public QAbstractTableModel {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit HWLSDPaperModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex&) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;

    // QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex& index, int role) const override;

    bool insertRows(int row, int count,
                    const QModelIndex& parent = QModelIndex()) override;

    bool removeRows(int row, int count,
                    const QModelIndex& parent = QModelIndex()) override;

    Q_INVOKABLE bool select();

public slots:
    void appendPaperRecord(int pid);
    bool removePaperRecord(int pid);

private:
    QVariantList m_data;
};

#endif  // HWLSDPAPERMODEL_H
