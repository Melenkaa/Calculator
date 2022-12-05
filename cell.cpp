#include "cell.h"

Cell::Cell(const QString& name, int size, QGraphicsItem* parent, bool canSelect, int penWidth): QGraphicsRectItem(parent){
    defaultBrush = QBrush(Qt::white, Qt::SolidPattern);
    selectedBrush = QBrush(QColor(122, 122, 122), Qt::SolidPattern);
    QPen pen(QBrush(Qt::black, Qt::SolidPattern), penWidth, Qt::SolidLine);
    setPen(pen);

    setRect(0, 0, size, size);
    setBrush(defaultBrush);
    selectable = canSelect;

    text = new QGraphicsTextItem(name, this);

    int psize = size * 0.35;
    QFont font("Times New Roman", psize);
    text->setFont(font);
    while (text->boundingRect().width() > rect().width()){
        font.setPointSize(font.pointSize() - 1);
        text->setFont(font);
    }

    text->setPos(rect().width()/2 - text->boundingRect().width()/2, rect().height()/2 - text->boundingRect().height()/2);

    setAcceptHoverEvents(true);
}

bool Cell::isSelectable() const {
    return selectable;
}

QGraphicsTextItem *Cell::getText() const {
    return text;
}

void Cell::setSelected(){
    if (selectable)
        setBrush(selectedBrush);
}

void Cell::setDefault(){
    if (selectable)
        setBrush(defaultBrush);
}

Cell::~Cell() {
    delete text;
}
