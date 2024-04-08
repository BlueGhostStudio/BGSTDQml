#ifndef BGBORDERBACKGROUND_H
#define BGBORDERBACKGROUND_H

#include <QQmlEngine>
#include <QQuickPaintedItem>

class BGBorderBackground : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor background2D READ background2D WRITE setBackground2D
                   NOTIFY background2DChanged)
    Q_PROPERTY(Shape shape READ shape WRITE setShape NOTIFY shapeChanged);
    Q_PROPERTY(Style style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius NOTIFY
                   cornerRadiusChanged)
    Q_PROPERTY(bool border READ border WRITE setBorder NOTIFY borderChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY
                   borderWidthChanged)

public:
    BGBorderBackground(QQuickItem* parent = nullptr);

    enum Shape {
        Fill,
        Rectangle,
        RoundedRectangle,
        Ellipse,
        Lower,
        Upper,
        LowerV,
        UpperV
    };
    Q_ENUM(Shape)

    enum Style { TwoDimensional, ThreeDimensional };
    Q_ENUM(Style)

    QColor color() const;
    void setColor(const QColor& color);

    QColor background2D() const;
    void setBackground2D(const QColor& color);

    Shape shape() const;
    void setShape(Shape shape);

    Style style() const;
    void setStyle(Style style);

    int cornerRadius() const;
    void setCornerRadius(int radius);

    bool border() const;
    void setBorder(bool border);

    qreal borderWidth() const;
    void setBorderWidth(qreal width);

    void paint(QPainter* painter) override;

signals:
    void colorChanged();
    void background2DChanged();
    void shapeChanged();
    void styleChanged();
    void cornerRadiusChanged();
    void borderChanged();
    void borderWidthChanged();

private:
    QColor highlightColor() const;
    QColor shadowColor() const;

    void fill(QPainter* painter);
    void drawRectangle(QPainter* painter);
    void drawRoundedRectangle(QPainter* painter);
    void drawEllipse(QPainter* painter);
    void drawLower(QPainter* painter);
    void drawUpper(QPainter* painter);
    void drawLowerV(QPainter* painter);
    void drawUpperV(QPainter* painter);

private:
    QColor m_color = QColor("white");
    QColor m_background2D = QColor(Qt::transparent);
    Shape m_shape = Rectangle;
    Style m_style = ThreeDimensional;
    int m_cornerRadius = 5;
    bool m_border = true;
    int m_borderWidth = 1;
};

#endif  // BGBORDERBACKGROUND_H
