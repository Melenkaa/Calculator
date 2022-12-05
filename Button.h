#ifndef BUTTON_H
#define BUTTON_H
#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsScene>

class Button: public QObject, public QGraphicsRectItem{
    Q_OBJECT
private:
    QGraphicsTextItem* text;
    QPen defaultPen;
    QPen selectedPen;
    QBrush defaultBrush;
    QBrush selectedBrush;
    int mwidth;
    int mheight;
    QFont defaultFont;
public:
    Button(const QString& name, int width = 200, int height = 50, QGraphicsItem* parent = nullptr);
    void setText(const QString& newName);
    QGraphicsTextItem* getText() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    ~Button();
signals:
    void clicked();
};

#endif // BUTTON_H
