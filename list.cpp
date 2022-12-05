#include "list.h"

Ring::Ring() {
    Head = new Node(QChar());
    Head->setStart();
    Tail = new Node(QChar());
    Head->pPrev = Tail;
    Head->pNext = Tail;
    Tail->pNext = Head;
    Tail->pPrev = Head;
    size_t = 0;
}

void Ring::form() {
    Ring();
}

const QChar Ring::at(int i) const {
    Node* tmp = Head;
    for(int j = 0; j < i; j++)
        tmp = tmp->pNext;
    return tmp->data;
}

int Ring::carree(QChar first, QChar second) const{
    int carry = 0;
    Node* add = Head;
    while (add->data != first)
        add = add->pNext;
    for(int i = 0; i < find(second); i++){
        add = add->pNext;
        if (add->isstart())
            carry += 1;
    }
    return carry;
}

int Ring::find(QChar c) const{
    Node* tmp = Head;
    int i = 0;
    while (tmp->data != c){
        tmp = tmp->pNext;
        i++;
    }
    return i;
}

QChar Ring::front() const {
    return Head->data;
}

QChar Ring::back() const {
    return Tail->data;
}

Ring::Ring(const QString& order){
    Head = new Node(order.front());
    Head->setStart();
    Tail = new Node(order.back());
    Head->pPrev = Tail;
    Head->pNext = Tail;
    Tail->pNext = Head;
    Tail->pPrev = Head;
    size_t = 2;
    wasDefault = false;
    for (auto it = order.begin() + 1; it != order.end() - 1; it++)
        append(*it);
}

void Ring::append(const QChar &c) {
    if (wasDefault)
        append_IfDefault(c);
    else
        append_IfKey(c);
}

void Ring::append_IfDefault(const QChar &c) {
    if (size_t == 0){
        Head->set(c);
        size_t++;
        return;
    }
    if (size_t == 1){
        Tail->set(c);
        size_t++;
        return;
    }
    Node* tmp = new Node(Tail->get());
    Tail->pPrev->pNext = tmp;
    tmp->pPrev = Tail->pPrev;
    Tail->pPrev = tmp;
    tmp->pNext = Tail;
    Tail->set(c);
    size_t++;
}

void Ring::clear(){
    Node* tmp = Head->pNext->pNext;
    while (!tmp->isstart()){
        delete tmp->pPrev;
        tmp = tmp->pNext;
    }
    size_t = 0;
    Head->set(QChar());
    Tail->set(QChar());
    Head->pNext = Tail;
    Tail->pPrev = Head;
}

int Ring::size() const{
    return size_t;
}

Ring::~Ring() {
    clear();
    delete Head;
    delete Tail;
}

void Ring::append_IfKey(const QChar &c) {
    Node* tmp = new Node(c);
    Tail->pPrev->pNext = tmp;
    tmp->pPrev = Tail->pPrev;
    Tail->pPrev = tmp;
    tmp->pNext = Tail;
    size_t++;
}
