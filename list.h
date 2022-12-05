#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "node.h"

class Ring{
    Node* Head;
    Node* Tail;
    int size_t;
    bool wasDefault = true;
    void append_IfDefault(const QChar& c);
    void append_IfKey(const QChar& c);
public:
    Ring();
    void form();
    const QChar at(int i) const;
    int carree(QChar first, QChar second) const;
    int find(QChar c) const;
    QChar front() const;
    QChar back() const;
    explicit Ring(const QString& order);
    int size() const;
    ~Ring();
    void append(const QChar& c);
    void clear();
};

