#include "Button.h"

Button::Button(const QString& name, int width, int height, QGraphicsItem *parent): QGraphicsRectItem(parent){
    setRect(0, 0, width, height);
    defaultBrush = QBrush(Qt::SolidPattern);
    defaultBrush.setColor(QColor(184, 251, 254));
    setBrush(defaultBrush);

    mwidth = width;
    mheight = height;

    selectedBrush = QBrush(Qt::SolidPattern);
    selectedBrush.setColor(QColor(151, 187, 242));

    defaultPen = QPen(QBrush(Qt::SolidPattern), 3, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    defaultPen.setColor(Qt::black);
    setPen(defaultPen);

    selectedPen = QPen(QBrush(Qt::SolidPattern), 5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
    selectedPen.setColor(Qt::black);

    text = new QGraphicsTextItem(name, this);

    int size = height * 0.35;
    defaultFont = QFont("Times New Roman", size);
    text->setFont(defaultFont);
    while (text->boundingRect().width() > rect().width()){
        defaultFont.setPointSize(defaultFont.pointSize() - 1);
        text->setFont(defaultFont);
    }

    text->setPos(rect().width()/2 - text->boundingRect().width()/2, rect().height()/2 - text->boundingRect().height()/2);

    setAcceptHoverEvents(true);
}

void Button::setText(const QString& newName){
    text->setPlainText(newName);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent* event){
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setBrush(selectedBrush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setBrush(defaultBrush);
}

QGraphicsTextItem* Button::getText() const{
    return text;
}

Button::~Button(){
    delete text;
}
