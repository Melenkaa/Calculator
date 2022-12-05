#ifndef TABLE_H
#define TABLE_H

#include <QGraphicsRectItem>
#include "cell.h"
#include "list.h"

QChar tableAdd(const Ring& order, int row, int column);
QChar tableMult(const Ring& order, int row, int column);
QChar tableSubstract(const Ring& order, int row, int column);
QChar tableDivide(const Ring& order, int row, int column);

class Table : public QObject, public QGraphicsRectItem{
    Q_OBJECT
    int size;
    QVector<QVector<Cell*>> table;
public:
    Table(const Ring& order, const QChar action, QChar (*element)(const Ring& order, int row, int column), int cellSize = 50, QGraphicsItem* parent = nullptr);
    QChar at(QChar first, QChar second);
    ~Table();
};

#endif // TABLE_H
