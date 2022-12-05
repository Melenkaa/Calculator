#pragma once
#include <QChar>
#include <QString>

class Node{
    QChar data;
    Node* pNext;
    Node* pPrev;
    bool start = false;
public:
    Node();
    void setStart();
    bool isstart() const;
    void set(QChar c);
    QChar get() const;
    explicit Node(const QChar& c);
    friend class Ring;
    ~Node();
};
