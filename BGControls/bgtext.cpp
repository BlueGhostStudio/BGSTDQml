#include "bgtext.h"

BGText::BGText(QObject* parent) : QObject{ parent } {}

BGText*
BGText::qmlAttachedProperties(QObject* obj) {
    return new BGText(obj);
}
void
BGText::setTextScale(TextType textScale) {
    m_textScale = textScale;
    emit textScaleChanged();
    emit textFormatChanged();
    emit fontChanged();
}

BGText::TextType
BGText::textScale() const {
    return m_textScale;
}

QVariantMap
BGText::textFormat() const {
    return m_scaleRoles[m_textScale];
}

QFont
BGText::font() const {
    QFont theFont;
    QVariantMap theFormat = m_scaleRoles[m_textScale];

    theFont.setPointSizeF(theFormat["size"].toReal());
    qreal letterSpacing = 100 + theFormat["letterSpacing"].toReal() * 10;
    theFont.setLetterSpacing(QFont::PercentageSpacing, letterSpacing);
    theFont.setWeight((QFont::Weight)theFormat["weight"].toInt());
    theFont.setCapitalization((QFont::Capitalization)theFormat["cap"].toInt());

    return theFont;
}
