#ifndef BGCONTROLS_H
#define BGCONTROLS_H

#include <QJSValue>
#include <QObject>
#include <QQuickItem>
#include <functional>

#include "bgrefreshconnection.h"

class BGControls : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(BGControls)
    Q_DISABLE_COPY(BGControls)

    Q_PROPERTY(QObject* parentContainer READ parentContainer CONSTANT)

    Q_PROPERTY(bool isContainer READ isContainer WRITE setIsContainer NOTIFY
                   isContainerChanged)
    Q_PROPERTY(QQuickItem* contentItem READ contentItem WRITE setContentItem
                   NOTIFY contentItemChanged)

    Q_PROPERTY(QJSValue options READ options WRITE setOptions NOTIFY optionsChanged)

    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged)

    Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY darkChanged)

    Q_PROPERTY(
        QColor primary READ primary WRITE setPrimary NOTIFY primaryChanged)
    Q_PROPERTY(QColor accent READ accent WRITE setAccent NOTIFY accentChanged)
    Q_PROPERTY(QColor base READ base WRITE setBase NOTIFY baseChanged)
    Q_PROPERTY(int shade READ shade WRITE setShade NOTIFY shadeChanged)
    Q_PROPERTY(int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY(bool covered READ covered WRITE setCovered NOTIFY coveredChanged)

    Q_PROPERTY(QColor foreground READ foreground NOTIFY foregroundChanged)
    Q_PROPERTY(QColor background READ background NOTIFY backgroundChanged)

    Q_PROPERTY(ColorRoles role READ role WRITE setRole NOTIFY roleChanged)

public:
    explicit BGControls(QObject* parent = nullptr);
    ~BGControls() override;

    enum Theme { NOTHEME, NORMAL, BRIGHTNESS_INVERSION };
    Q_ENUM(Theme)

    enum ColorRoles { NOROLE, BASE, PRIMARY, ACCENT };
    Q_ENUM(ColorRoles)

    static BGControls* qmlAttachedProperties(QObject* obj);

    QObject* parentContainer(QObject* obj = nullptr) const;
    Q_INVOKABLE QObject* backtrackContainer(const QJSValue& cb) const;

    bool isContainer() const;
    void setIsContainer(bool is);
    QQuickItem* contentItem() const;
    void setContentItem(QQuickItem* contentItem);

    QJSValue options() const;
    void setOptions(const QJSValue& data);

    Theme theme() const;
    void setTheme(Theme theme);

    bool dark() const;
    void setDark(bool dark);

    QColor primary() const;
    void setPrimary(const QColor& color);

    QColor accent() const;
    void setAccent(const QColor& color);

    QColor base() const;
    void setBase(const QColor& color);

    int shade() const;
    void setShade(int shade);

    int hue() const;
    void setHue(int hue);

    bool covered() const;
    void setCovered(bool covered);

    QColor foreground() const;
    QColor background() const;

    ColorRoles role() const;
    void setRole(ColorRoles role);

signals:
    void isContainerChanged();
    void contentItemChanged();
    void optionsChanged();
    void themeChanged();
    void darkChanged();
    void primaryChanged();
    void accentChanged();
    void baseChanged();
    void shadeChanged();
    void hueChanged();
    void coveredChanged();
    void foregroundChanged();
    void backgroundChanged();
    void roleChanged();
    void forceRefresh();

private:
    QObject* backtrackContainer(
        std::function<bool(QObject*, BGControls*)> callback) const;
    QColor backtrackColor(ColorRoles target) const;

private:
    QQmlEngine* m_qmlEngine = nullptr;
    BGRefreshConnection* m_refreshConnection = nullptr;

    QQuickItem* m_contentItem = nullptr;
    Theme m_theme = Theme::NOTHEME;
    ColorRoles m_role = ColorRoles::NOROLE;
    QColor m_primary;
    QColor m_accent;
    QColor m_base;
    int m_shade = 0XFFFF;
    int m_hue = 0XFFFF;
    bool m_covered = false;
    QJSValue m_options;
};

#endif  // BGCONTROLS_H
