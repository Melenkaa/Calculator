#include "mainwindow.h"

QVector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

#define myVariant "01234567"//"BGHEADCF"

QFont titleFont("Times New Roman", 30);
QFont mediumFont("Times New Roman", 20);

#define setExpression(item, text) item->setHtml(QString("<span style=\"color:#808080\">") + text + "</span>")

MainWindow::MainWindow(QWidget *parent) : QGraphicsView(parent){
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene();
    scene -> setSceneRect(0, 0, 1920, 1080);
    setScene(scene);

    order.form();

    setRule();
}

//--------------------------------------------------------------------------METHODS-----------------------------------------------------------

void MainWindow::setRule(){
    QGraphicsTextItem* title = new QGraphicsTextItem();
    title->setHtml("<u>Enter the \"+1\" Rule:</u>");
    title->setFont(titleFont);
    title->setPos(10, 10);
    scene->addItem(title);

    QGraphicsTextItem* null = new QGraphicsTextItem("Choose the element β wich follows the rule: ∀x\tx+β=x, then click on buttons to set the result of x + 1, OR use the given order:");
    null->setFont(mediumFont);
    null->setPos(30, 20 + null->boundingRect().height());
    scene->addItem(null);

    textOrder.fill(nullptr, 8);
    for (auto& text: textOrder)
        text = new QGraphicsTextItem();
    textOrder[0]->setPos(60, null->pos().y() + null->boundingRect().height() + 20);

    QPoint buttonPosition(1820, 30);
    QVector<Button*> uiButtons;
    uiButtons.fill(nullptr, 8);
    for(int i = 0; i < uiButtons.size(); i++){
        uiButtons[i] = new Button(QChar(alphabet[i]), 70, 70);
        uiButtons[i]->setPos(buttonPosition);
        buttonPosition += QPoint(0, 100);
        connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(addElement()));
        scene->addItem(uiButtons[i]);
    }

    var16 = new Button(myVariant, 150, 60);
    var16->setPos(buttonPosition - QPoint(80, 0));
    connect(var16, SIGNAL(clicked()), this, SLOT(variant()));
    scene->addItem(var16);

    //variant(); //FOR DEBUG
}

void MainWindow::launchCalculator(){
    sumCarry.resize(order.size());
    for(auto& i : sumCarry)
        i.fill(order.front(), order.size());
    multCarry.resize(order.size());
    for(auto& i : multCarry)
        i.fill(order.front(), order.size());

    for(int i = 0; i < sumCarry.size(); i++)
        for(int j = 0; j < sumCarry[i].size(); j++)
            sumCarry[i][j] = order.at(order.carree(order.at(i), order.at(j)));

    Button* quit = new Button("Close", 100, 50);
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
    quit->setPos(1920 - 10 - quit->boundingRect().width(), 10);
    scene->addItem(quit);

    add = new Table(order, '+', tableAdd, 40);
    add->setPos(10, 10);
    scene->addItem(add);

    for(int i = 0; i < multCarry.size(); i++)
        for(int j = 0; j < multCarry[i].size(); j++)
            multCarry[i][j] = calculateMultCarry(i, j);

    Table* mult = new Table(order, '*', tableMult, 40);
    mult->setPos(add->pos().x() + add->boundingRect().width() + 20, add->pos().y());
    scene->addItem(mult);

    sub = new Table(order, '-', tableSubstract, 40);
    sub->setPos(mult->pos().x() + mult->boundingRect().width() + 20, mult->pos().y());
    scene->addItem(sub);

    displayKeyboard();
}

QChar MainWindow::calculateMultCarry(int first, int second){
    //Lets say we have C * E => 6 * 3
    if (first * second == 0)
        return order.front();
    QChar multiplee = order.at(std::max(first, second)); //Take the maximum of 2 and it will be the number we add to itself. multiplee = C
    int carree = 0; //Value of carree
    for(int i = 0; i < std::min(first, second) - 1; i++){ //We add C to itself 2 times (C + C + C)
        carree += order.find(sumCarry[order.find(multiplee)][order.find(order.at(std::max(first, second)))]); //Add the carree of the sum of multiplee + C to our carree
        multiplee = add->at(multiplee, order.at(std::max(first, second))); //mulpitlee = result of the sum according to the table
    }
    return order.at(carree);
    return '?';
}

void MainWindow::displayKeyboard() {
    QPoint buttonPosition(10, 800);
    QVector<Button*> letterButtons;
    letterButtons.fill(nullptr, 8);
    for(int i = 0; i < letterButtons.size(); i++){
        letterButtons[i] = new Button(order.at(i), 50, 50);
        letterButtons[i]->setPos(buttonPosition);
        buttonPosition += QPoint(letterButtons[i]->boundingRect().width() + 10, 0);
        connect(letterButtons[i], SIGNAL(clicked()), this, SLOT(addSymbol()));
        scene->addItem(letterButtons[i]);
    }
    buttonPosition = QPoint(10 + letterButtons.back()->boundingRect().width()/2, letterButtons.back()->pos().y() + letterButtons.back()->boundingRect().height() + 10);
    QVector<QString> operations = {"Clear", "+", "-", "×", "÷", "="};
    QVector<Button*> uiButtons;
    uiButtons.fill(nullptr, 6);
    for(int i = 0; i < uiButtons.size(); i++){
        uiButtons[i] = new Button(operations[i], 50, 50);
        uiButtons[i]->setPos(buttonPosition);
        buttonPosition += QPoint(uiButtons[i]->boundingRect().width() + 10, 0);
        if (i == 0)
            connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(clearExpression()));
        if ((i != 0) && (i < uiButtons.size() - 1))
            connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(addSymbol()));
        else if (i == uiButtons.size() - 1)
            connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(displayAnswer()));
        scene->addItem(uiButtons[i]);
    }

    pasteAnswer = new Button("Ans", 70, 50);
    pasteAnswer->setPos(buttonPosition);
    scene->addItem(pasteAnswer);

    userInput = new QGraphicsTextItem("");
    userInput->setPos(50, add->pos().y() + add->boundingRect().height() + 100);
    userInput->setFont(titleFont);
    setExpression(userInput, "Enter the expression...");
    scene->addItem(userInput);

    answer = new QGraphicsTextItem("=");
    answer->setPos(50, userInput->pos().y() + userInput->boundingRect().height() + 15);
    answer->setFont(titleFont);
    scene->addItem(answer);
}

QString MainWindow::sumResult(QVector<QString> additives, QVector<QChar> split) { //vector of additives, ex: ADDDE FGHE HGB A
    //This is a recursive function that returns the result of sum of all elements in additives
    if (additives.size() == 1) //If only one element exists => it is the answer
        return additives.front();

    //If there are at least 2 elements in initial vector, we copy first (size - 2) elements in a new vector thats gonna be passed in recursion
    QVector<QString> newAdditives;
    newAdditives.fill("", additives.size() - 1);
    for(int i = 0; i < newAdditives.size() - 1; i++)
        newAdditives[i] = additives[i];

    QVector<QChar> newSplit;
    newSplit.fill('\0', split.size() - 1);
    for(int i = 0; i < split.size() - 1; i++)
        newSplit[i] = split[i];

    //if the result of difference < 0 => last element in additives starts with - => get rid of - and change the last sign in our vector
    if (additives.back().startsWith('-')){
        if (newSplit.back() == '+')
            newSplit.back() = '-';
        else
            newSplit.back() = '+';
        QString newLast = "";
        for(int i = 1; i < additives.back().size(); i++)
            newLast += additives.back()[i];
        additives.back() = newLast;
    }

    //first and second are the last two elements in initial vector - we're gonna add them up and put the result as the last element in newAdditives
    QString first = additives[additives.size() - 2];
    QString second = additives.back();

    //append the result of sum of two last elements to the array:
    //if split contains only one sign => it is X + Y or X - Y
    if (split.size() == 1){
        if(split.back() == '+')
            newAdditives[newAdditives.size() - 1] = addTwo(first, second);
        else
            newAdditives[newAdditives.size() - 1] = substractTwo(first, second);
    //if vector has more than 1 sign => check them
    } else if (split[split.size() - 1] == split[split.size() - 2]) //if two last signs are the same - we add numbers
        newAdditives[newAdditives.size() - 1] = addTwo(first, second);
    else //else we find the difference
        newAdditives[newAdditives.size() - 1] = substractTwo(first, second);

    return sumResult(newAdditives, newSplit); //and call the function again passing the new array as a parameter
}

QString MainWindow::addTwo(const QString &firstAdd, const QString &secondAdd) {
    QString first = firstAdd;
    QString second = secondAdd;

    //Reverse the elements in order to sum them in columns
    std::reverse(first.begin(), first.end());
    std::reverse(second.begin(), second.end());

    QChar carree = order.front(); //the value that's carried to the next digit
    QString result = ""; //the result of sum first + second

    int index;

    //While each number has a digit at a given index:
    for(index = 0; index < std::min(first.size(), second.size()); index++){
        result += QString(add->at(carree, add->at(first.at(index), second.at(index)))); //the value of result's same digit is digit1 + digit2 + carree

        //Calculate the carree for the next digit
        if (sumCarry[order.find(first.at(index))][order.find(second.at(index))] == order.at(1))
            carree = sumCarry[order.find(first.at(index))][order.find(second.at(index))];
        else
            carree = sumCarry[order.find(add->at(first.at(index), second.at(index)))][order.find(carree)];
    }

    //Add a leftover carree to the leftover number
    if (first.size() == second.size()){
        if (carree != order.front())
            result += carree;
    } else if (first.size() > second.size()){
        for(int i = index; i < first.size(); i++){
            result += QString(add->at(carree, first.at(i)));
            carree = sumCarry[order.find(first.at(index))][order.find(carree)];
        }
    } else {
        for(int i = index; i < second.size(); i++){
            result += QString(add->at(carree, second.at(i)));
            carree = sumCarry[order.find(second.at(index))][order.find(carree)];
        }
    }

    std::reverse(result.begin(), result.end()); //Since we reversed the numbers and calculated the sum in reverse, we need to reverse it back

    return result;
}

QString MainWindow::substractTwo(const QString &firstSub, const QString &secondSub) {
    QString first = firstSub;
    QString second = secondSub;

    //We will calculate first - second

    QString result = "";
    bool isResultNegative = false;

    //determine the sign of difference
    if (first.size() < second.size()){
        isResultNegative = true;
        std::swap(first, second);
    }
    else if (first.size() == second.size()){
        for(int i = 0; i < first.size(); i++){
            if (order.find(first[i]) < order.find(second[i])){
                isResultNegative = true;
                std::swap(first, second);
                break;
            }
        }
    }

    std::reverse(first.begin(), first.end());
    std::reverse(second.begin(), second.end());

    //second is less than first, so we adgust the lengths =>
            //if we had first = HHFFEAB and second = AB => second becomes BBBBBAB
    while (second.size() != first.size())
        second += order.front();

    //we guess what number we need to add to second to get first
    QChar carree = order.front();
    for(int index = 0; index < first.size(); index++){
        QChar tmpAdd = order.front();
        //we find the digit that suits the condition
        while (add->at(carree, add->at(tmpAdd, second.at(index))) != first.at(index))
            tmpAdd = order.at(order.find(tmpAdd) + 1);
        result += tmpAdd;
        carree = sumCarry[order.find(tmpAdd)][order.find(second.at(index))];
    }

    //delete zeros at the beginning (remember the result is reversed, so the beginning of the number is at the end of QString)
    while (result.back() == order.front())
        result.chop(1);

    if (isResultNegative)
        result += "-";

    std::reverse(result.begin(), result.end());
    return result;
}

//--------------------------------------------------------------------------SLOTS-----------------------------------------------------------

void MainWindow::apply(){
    delete scene;
    scene = new QGraphicsScene();
    scene -> setSceneRect(0, 0, 1920, 1080);
    setScene(scene);
    launchCalculator();
}

void MainWindow::addElement(){
    Button* sender = qobject_cast<Button*>(QObject::sender());

    if (order.size() == 0) {
        disconnect(var16, 0, 0, 0);
        scene->removeItem(var16);
        delete var16;
    }
    if (order.size() != 0)
        textOrder[order.size() - 1]->setPlainText(textOrder[order.size() - 1]->toPlainText() + sender->getText()->toPlainText().front());
    if (order.size() == alphabet.size() - 1){
        textOrder[order.size()]->setPlainText(sender->getText()->toPlainText() + " + 1 = " + order.front());
        textOrder[order.size()]->setFont(mediumFont);
        textOrder[order.size()]->setPos(textOrder.front()->pos().x(), textOrder.front()->pos().y() + (textOrder.front()->boundingRect().height() + 10) * order.size());
        scene->addItem(textOrder[order.size()]);
        order.append(sender->getText()->toPlainText().front());

        Button* applyOrder = new Button("Apply", 150, 60);
        applyOrder->setPos(1740, 990);
        connect(applyOrder, SIGNAL(clicked()), this, SLOT(apply()));
        scene->addItem(applyOrder);
        scene->removeItem(sender);
        delete sender;
        return;
    }
    textOrder[order.size()]->setPlainText(sender->getText()->toPlainText().front() + " + 1 = ");
    textOrder[order.size()]->setFont(mediumFont);
    textOrder[order.size()]->setPos(textOrder.front()->pos().x(), textOrder.front()->pos().y() + (textOrder.front()->boundingRect().height() + 10) * order.size());
    scene->addItem(textOrder[order.size()]);
    order.append(sender->getText()->toPlainText().front());
    scene->removeItem(sender);
    delete sender;
}

void MainWindow::displayAnswer() {
    if (userInput->toPlainText() == QString("Enter the expression...")) //expression is empty => do nothing
        return;
    if ((userInput->toPlainText().back() == '+') || (userInput->toPlainText().back() == '-') ||\
            (userInput->toPlainText().endsWith("×")) || (userInput->toPlainText().endsWith("÷"))) //expression ends with an operator sign => do nothing
        return;

    QVector<QString> additives;
    QVector<QChar> splits;
    additives.resize(0);
    splits.resize(0);

    QString entry = "";
    for(auto& c : userInput->toPlainText()){
        if ((c == '+') || (c == '-')){
            additives.append(entry);
            splits.append(c);
            entry = "";
        } else
            entry += c;
    }
    additives.append(entry);

    answer->setPlainText("=" + sumResult(additives, splits));
    connect(pasteAnswer, SIGNAL(clicked()), this, SLOT(useAnswer()));
    return;
}

void MainWindow::useAnswer() {
    QString ans = "";
    for(int i = 1; i < answer->toPlainText().size(); i++)
        ans += answer->toPlainText().at(i);
    setExpression(userInput, ans);
    answer->setPlainText("=");
    disconnect(pasteAnswer, 0, 0, 0);
}

void MainWindow::clearExpression() {
    setExpression(userInput, "Enter the expression...");
    answer->setPlainText("=");
}

void MainWindow::variant() {
    for(int i = 0; i < var16->getText()->toPlainText().length(); i++)
        order.append(var16->getText()->toPlainText()[i]);
    disconnect(var16, 0, 0, 0);
    scene->removeItem(var16);
    delete var16;
    apply();
}

void MainWindow::addSymbol() {
    Button* sender = qobject_cast<Button*>(QObject::sender());
    QString previous = userInput->toPlainText(); //current expression
    QString symbol = sender->getText()->toPlainText(); //symbol we want to add
    if(userInput->toPlainText() == QString("Enter the expression...")){
        if ((symbol == "+") || (symbol == "-") || (symbol == "×") || (symbol == "÷"))
            return; //we don't add + - × or ÷ to an empty expression
        userInput->setPlainText(sender->getText()->toPlainText()); //if the expression is empty - clear the text and set it to our symbol
        answer->setPlainText("=");
    } else {
        if ((symbol == "+") || (symbol == "-") || (symbol == "×") || (symbol == "÷")){
            if ((userInput->toPlainText().back() == "+") || (userInput->toPlainText().back() == "-") || (userInput->toPlainText().back() == "×") || (userInput->toPlainText().back() == "÷"))
                return; //we don't add an operation after another operation
        } else {
            //if (userInput->toPlainText().back() == order.front())
                //return; //we don't add A B C D E F G or H behind our zero unless it is in a number, we add only + - × or ÷ //FIXME
        }
        userInput->setPlainText(userInput->toPlainText() + symbol); //if all clear - add the symbol to the end of expression
    }
    if (userInput->pos().x() + userInput->boundingRect().width() > 1920)
        userInput->setPlainText(previous); //we don't add a symbol if it goes beyond the screen
    //reset the font and color
    answer->setPlainText("=");
    userInput->setFont(titleFont);
    setExpression(userInput, userInput->toPlainText());
}

//------------------------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow(){
    delete scene;
}
