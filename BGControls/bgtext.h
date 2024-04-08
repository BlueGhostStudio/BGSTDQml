#ifndef BGTEXT_H
#define BGTEXT_H

#include <QFont>
#include <QObject>
#include <QQmlEngine>

class BGText : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_ATTACHED(BGText)

    Q_PROPERTY(TextType textScale READ textScale WRITE setTextScale NOTIFY
                   textScaleChanged)
    Q_PROPERTY(QVariantMap textFormat READ textFormat NOTIFY textFormatChanged)
    Q_PROPERTY(QFont font READ font NOTIFY fontChanged)

public:
    explicit BGText(QObject* parent = nullptr);

    enum TextType {
        H1,
        H2,
        H3,
        H4,
        H5,
        H6,
        Subtitle1,
        Subtitle2,
        Body1,
        Body2,
        Button,
        Caption,
        Overline
    };
    Q_ENUM(TextType)

    static BGText* qmlAttachedProperties(QObject* obj);

    void setTextScale(TextType textScale);
    TextType textScale() const;

    QVariantMap textFormat() const;

    QFont font() const;

signals:
    void textScaleChanged();
    void textFormatChanged();

    void fontChanged();

private:
    QMap<TextType, QVariantMap> m_scaleRoles = {
        { H1,
          { { "size", 72 },
            { "letterSpacing", -1.5 },
            { "weight", QFont::Light },
            { "cap", QFont::MixedCase } } },
        { H2,
          { { "size", 45 },
            { "letterSpacing", -0.5 },
            { "weight", QFont::Light },
            { "cap", QFont::MixedCase } } },
        { H3,
          { { "size", 36 },
            { "letterSpacing", 0 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { H4,
          { { "size", 25.8 },
            { "letterSpacing", 0.25 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { H5,
          { { "size", 18 },
            { "letterSpacing", 0 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { H6,
          { { "size", 15 },
            { "letterSpacing", 0.15 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { Subtitle1,
          { { "size", 12 },
            { "letterSpacing", 0.15 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { Subtitle2,
          { { "size", 10.5 },
            { "letterSpacing", 0.1 },
            { "weight", QFont::Medium },
            { "cap", QFont::MixedCase } } },
        { Body1,
          { { "size", 12 },
            { "letterSpacing", 0.5 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { Body2,
          { { "size", 10.5 },
            { "letterSpacing", 0.25 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { Button,
          { { "size", 10.5 },
            { "letterSpacing", 1.25 },
            { "weight", QFont::Medium },
            { "cap", QFont::AllUppercase } } },
        { Caption,
          { { "size", 9 },
            { "letterSpacing", 0.4 },
            { "weight", QFont::Normal },
            { "cap", QFont::MixedCase } } },
        { Overline,
          { { "size", 7.5 },
            { "letterSpacing", 1.5 },
            { "weight", QFont::Normal },
            { "cap", QFont::AllUppercase } } }
    };

    TextType m_textScale = Body1;
};

#endif  // BGTEXT_H
