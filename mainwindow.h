#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include "Button.h"
#include "table.h"

class MainWindow : public QGraphicsView{
    Q_OBJECT
private:
    QGraphicsScene* scene = nullptr;
    QVector<QGraphicsTextItem*> textOrder;
    QVector<QVector<QChar>> sumCarry;
    QVector<QVector<QChar>> multCarry;
    Ring order;
    QGraphicsTextItem* userInput = nullptr;
    QGraphicsTextItem* answer = nullptr;
    Button* var16 = nullptr;
    Button* pasteAnswer = nullptr;
    Table* add = nullptr;
    Table* sub = nullptr;
//---------------------------------------Members----------------------------------------
    void setRule();
    void launchCalculator();
    QChar calculateMultCarry(int first, int second);
    void displayKeyboard();
    QString sumResult(QVector<QString> additives, QVector<QChar> split); //parameter - vector of additives, ex: "FGH", "HAB", "HHG". Returns the result of addition
    QString addTwo(const QString& firstAdd, const QString& secondAdd); //returns result of firstAdd + secondAdd
    QString substractTwo(const QString& firstSub, const QString& secondSub); //returns result of firstSub - secondSub
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void apply();
    void addElement();
    void displayAnswer();
    void useAnswer();
    void clearExpression();
    void variant();
    void addSymbol();
};
#endif // MAINWINDOW_H
