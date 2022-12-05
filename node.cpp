#include "node.h"

Node::Node(){
    pPrev = nullptr;
    pNext = nullptr;
    data = QChar();
}

Node::Node(const QChar& c){
    data = c;
    pPrev = nullptr;    //fixme
    pNext = nullptr;    //fixme
}

void Node::setStart(){
    start = true;
}

Node::~Node() {
    pNext = nullptr;
    pPrev = nullptr;
}

bool Node::isstart() const {
    return start;
}

void Node::set(QChar c) {
    data = c;
}

QChar Node::get() const {
    return data;
}
