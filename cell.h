#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>

class Cell: public QGraphicsRectItem {
    //Q_OBJECT
    QBrush defaultBrush;
    QBrush selectedBrush;
    QGraphicsTextItem* text = nullptr;
    bool selectable = false;
public:
    Cell(const QString& name, int size = 50, QGraphicsItem* parent = nullptr, bool canSelect = false, int penWidth = 5);
    bool isSelectable() const;
    QGraphicsTextItem* getText() const;
    QBrush getSelectedBrush() const;
    QBrush getDefaultBrush() const;
    ~Cell();
    void setDefault();
    void setSelected();
};

#endif // CELL_H
