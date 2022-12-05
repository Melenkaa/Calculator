#include "table.h"

Table::Table(const Ring& order, const QChar action, QChar (*element)(const Ring&, int, int), int cellSize, QGraphicsItem* parent) : QGraphicsRectItem(parent){
    table.resize(order.size() + 1);
    for(auto& i : table)
        i.fill(nullptr, order.size() + 1);

    size = cellSize;

    setRect(0, 0, cellSize * (order.size() + 1), cellSize * (order.size() + 1));
    for(int i = 0; i < table.size(); i++){
        for(int j = 0; j < table[i].size(); j++){
            if (i*j == 0){
                if (i + j == 0){
                    table[i][j] = new Cell(action, cellSize, this);
                    table[i][j]->setPos(0, 0);
                }else{
                    table[i][j] = new Cell(order.at(i + j - 1), cellSize, this);
                    table[i][j]->setPos(QPoint(i, j) * cellSize);
                }
            }else{
                table[i][j] = new Cell(element(order, i, j), cellSize, this, true, 3);
                table[i][j]->setPos(QPoint(i, j) * cellSize);
            }
        }
    }

    setAcceptHoverEvents(true);
}

QChar Table::at(QChar first, QChar second) {
    int row = 0;
    QChar tmp = '\0';
    while (tmp != first)
        tmp = table[++row][0]->getText()->toPlainText().front();
    int column = 0;
    tmp = '\0';
    while (tmp != second)
        tmp = table[0][++column]->getText()->toPlainText().front();
    return table[row][column]->getText()->toPlainText().front();
}

Table::~Table() {
    for(auto& row : table)
        for(auto& element : row)
            delete element;
}

QChar tableAdd(const Ring& order, int row, int column) {
    return order.at(row + column - 2); //Sum Table is just a shift of order with each new line
    return '?';
}

QChar tableMult(const Ring& order, int row, int column) {
    if ((row - 1) * (column - 1) == 0)
        return order.front();
    if ((row == 2) || (column == 2))
        return order.at(std::max(row, column) - 1);
    //if we * 0 => return 0; if we * 1 => return x
    //By this point, minimal multiplicator is at least 2

    //Lets say we have 6 * 3
    QChar result = tableAdd(order, std::max(row, column), std::max(row, column)); //Here result equals 6 * 2 = 6 + 6
    //For every remaining time we need to add 6
    for(int i = 0; i < order.find(order.at(std::min(row - 1, column - 1))) - 2; i++)
        result = tableAdd(order, std::max(row, column), order.find(result) + 1); //We add 6
    return result;
    return '?';
}

QChar tableSubstract(const Ring &order, int row, int column) {
    return order.at(abs(row - column));
    return '?';
}
