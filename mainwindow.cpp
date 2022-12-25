#include "mainwindow.h"

QVector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

#define myVariant "01234567"//        "BGHEADCF"

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

    //setRule();            //For release

    //For DEBUG
    for(int i = 0; i < 8; i++)
        order.append(myVariant[i]);
    launchCalculator();
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

    mult = new Table(order, '*', tableMult, 40);
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
    QPoint buttonPosition(45, 670);
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
    QVector<QString> operations = {"Clear", "←", "+", "-", "×", "÷", "="};
    QVector<Button*> uiButtons;
    uiButtons.fill(nullptr, 7);
    for(int i = 0; i < uiButtons.size(); i++){
        uiButtons[i] = new Button(operations[i], 50, 50);
        uiButtons[i]->setPos(buttonPosition);
        buttonPosition += QPoint(uiButtons[i]->boundingRect().width() + 10, 0);
        if (i == 0)
            connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(clearExpression()));
        if (i == 1)
            connect(uiButtons[i], SIGNAL(clicked()), this, SLOT(removeLastSymbol()));
        if ((i != 0) && (i != 1) && (i < uiButtons.size() - 1))
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
    if (additives.size() == 1){ //If only one element exists => it is the answer
        if (!additives.front().contains("×") && !additives.front().contains("÷"))
            return additives.front();
        else
            return multResult(additives.front());
    }

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

        if (newSplit.size() != 0){
            if (newSplit.back() == '+')
               newSplit.back() = '-';
            else
               newSplit.back() = '+';
        } else {
            if (split.back() == '+')
               split.back() = '-';
            else
               split.back() = '+';
        }

        QString newLast = "";
        for(int i = 1; i < additives.back().size(); i++)
            newLast += additives.back()[i];
        additives.back() = newLast;
    }

    //first and second are the last two elements in initial vector - we're gonna add them up and put the result as the last element in newAdditives
    QString first = additives[additives.size() - 2];
    QString second = additives.back();

    //check if first or second contain multiplication sign
    if (first.contains("\u00d7") || first.contains("\u00f7"))
        first = multResult(first);
    if (second.contains("\u00d7") || second.contains("\u00f7"))
        second = multResult(second);

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
        if (carree != order.front())
            result += carree;
    } else {
        for(int i = index; i < second.size(); i++){
            result += QString(add->at(carree, second.at(i)));
            carree = sumCarry[order.find(second.at(index))][order.find(carree)];
        }
        if (carree != order.front())
            result += carree;
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

    //second is less than first, so we adjust the lengths =>
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

        //calculating the next carree:
        if (carree != order.front()){ //if the CURRENT carree is not zero => we need to determine whether adding together carree, digit from SECOND and our tmpAdd creates a non-zero carree
            if (addTwo(addTwo(carree, tmpAdd), second.at(index)).size() != 1) //if the sum of three is not 1 digit long => there is a carree
                carree = order.at(1);
            else //else - no carree
                carree = order.front();
        } else // if CURRENT carree is zero => next carre is got from the table
            carree = sumCarry[order.find(tmpAdd)][order.find(second.at(index))];
    }

    //delete zeros at the beginning (remember the result is reversed, so the beginning of the number is at the end of QString)
    if (result != QString(order.front()))
        while (result.back() == order.front())
            result.chop(1);

    if (isResultNegative)
        result += "-";

    std::reverse(result.begin(), result.end());
    return result;
}

QString MainWindow::multResult(const QString &expression) { //expression can have undefined amount of splits, ex: 64 * 2635732 * 755 * ...
    QVector<QString> multiplees;
    QVector<QString> splits;

    multiplees.resize(0);
    splits.resize(0);

    QString tmp = "";
    for(auto& c : expression){
        if ((c == "\u00d7") || (c == "\u00f7")){
            splits.append(c);
            multiplees.append(tmp);
            tmp = "";
        }
        else
            tmp += c;
    }
    multiplees.append(tmp);
    //now we have 2 vectors consisting of 1) signs *, and 2) numbers that are to be multiplied
    //it is guaranteed that expression does not end with a sign, so after the loop ends we append tmp to the vector, since tmp is the last number

    if(multiplees.size() == 1) //if there are no * or / => expression = answer
        return expression;

    QString first = multiplees.front();
    QString second = multiplees[1];

    //we are going to multiply from left to right, so the operands are always 1st and 2nd elements of the array.
    QString newExpression = "";
    if (splits.front() == "\u00d7") // u00d7 is unicode for ×. If thats the symbol => we multiply
        newExpression = multiply(first, second);
    else //else we divide
        newExpression = divide(first, second);

    //creating the new expression:  Before: 111 * 222 / 33 * 444
    //                              After: 333 / 33 * 444
    for(int i = 1; i < splits.size(); i++){
        newExpression += splits[i];
        newExpression += multiplees[i + 1];
    }

    return multResult(newExpression);
}

QString MainWindow::multiply(const QString &firstM, const QString &secondM) {
    QString first = firstM;
    QString second = secondM;

    std::reverse(first.begin(), first.end());
    std::reverse(second.begin(), second.end());

    QVector<QString> resultsOfMultiplication;
    resultsOfMultiplication.resize(0);
    //We are gonna multiply in columns. This vector will contain results of multiplication of FIRST by each digit in second,
                                                                                    //and then they will all be added together to get the answer

    if (first.size() < second.size()) //we gonna multiply by SECOND so we want it to be a shorter number
        std::swap(first, second);

    for(int index = 0; index < second.size(); index++){ //going through each digit in SECOND

        if (second[index] == order.front()){ //if a digit is zero => the result of multiplication is zero
            resultsOfMultiplication.append(order.front());
            continue;
        }

        QString newNumber = ""; //the result of FIRST * digit from SECOND
        QChar multcarree = order.front();

        for(int firstDigit = 0; firstDigit < first.size(); firstDigit++){

            newNumber += QString(add->at(mult->at(second[index], first[firstDigit]), multcarree)); //new digit in the result is FIRST[i] * SECOND[j] + carree from previous digit

            //calculating the next carree

            //if the sum  newDigit + multcarree  creates another carree,
            if (sumCarry[order.find(mult->at(second[index], first[firstDigit]))][order.find(multcarree)] != order.front())
                //the next carree is the sum of carrees from multiplication and addition
                multcarree = add->at(multCarry[order.find(first[firstDigit])][order.find(second[index])], sumCarry[order.find(mult->at(second[index], first[firstDigit]))][order.find(multcarree)]);
            else //if it does not => next carree is just the multiplication carree
                multcarree = multCarry[order.find(first[firstDigit])][order.find(second[index])];

        }

        if (multcarree != order.front()) //add a non zero carree to the first digit
            newNumber += multcarree;

        resultsOfMultiplication.append(newNumber);
    }

    //since everything is reversed, we need to reverse it back and add zeros to the end of some numbers:
    //lets say we have 23 * 18:
    //23 * 8 = 184, 23 * 1 = 23, so our vector would have "184" and "23", but the result of multiplication is 184 + 230 = 414, so we need to add a zero at the end of "23"
    //The amount of zeros to be added is equal to the index of a given number
    for (int i = 0; i < resultsOfMultiplication.size(); i++){
        if (resultsOfMultiplication[i] != QString(order.front())){
            std::reverse(resultsOfMultiplication[i].begin(), resultsOfMultiplication[i].end()); //reverse the number back
            for(int j = 0; j < i; j++)
                resultsOfMultiplication[i] += order.front(); //add zeros
        }
    }

    QVector<QChar> temp;
    temp.fill('+', resultsOfMultiplication.size() - 1);

    return sumResult(resultsOfMultiplication, temp); //result of multiplication is the result of the sum
}

QString MainWindow::divide(const QString &dividend, const QString &divider) {
    QString result = order.front();
    QString div = dividend;
    //The result of division is going to be calculated by continiously subtracting divider from dividend
    //                                                  and keeping track of the amount of subtractions
    while (substractTwo(div, divider).front() != '-'){
        result = addTwo(result, order.at(1));
        div = substractTwo(div, divider);
    }
    //if we divide with remainder AND the remainder is not 0 => display "AAA + B in remainder."
    if (divideWithRemainder && (div != QString(order.front())))
        result += QString(" + " + div + " in remainder.");
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
    //if the expression contains division by zero => do nothing.
    if (answer->toPlainText() == "= Error: Division by Zero.")
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

    if ((splits.size() == 0) && (entry.count("\u00f7") == 1) && (entry.count("\u00d7") == 0))
        divideWithRemainder = true;

    answer->setPlainText("=" + sumResult(additives, splits));
    divideWithRemainder = false;
    connect(pasteAnswer, SIGNAL(clicked()), this, SLOT(useAnswer()));
    return;
}

void MainWindow::useAnswer() {
    QString ans = "";

    //if expression contains division by zero => do nothing
    if (answer->toPlainText() == "= Error: Division by Zero.")
        return;

    // Check if the last action was division with remainder => then we use only the integer part of the answer.
    //ex.   answer:         =45 + 4 in remainder.       New expression = 45.
    if (answer->toPlainText().contains(" in remainder.")){
        int i = 1;
        while (answer->toPlainText().at(i) != ' '){
            ans += answer->toPlainText().at(i);
            i += 1;
        }
    }
    else
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

    //if we add a zero after division sign => display error message
    if ((userInput->toPlainText().back() == "÷") && (symbol == order.front())){
        userInput->setPlainText(userInput->toPlainText() + symbol);
        answer->setPlainText("= Error: Division by Zero.");
        userInput->setFont(titleFont);
        setExpression(userInput, userInput->toPlainText());
        return;
    }

    //if expression contains division by zero => do nothing.
    if (userInput->toPlainText().contains(QString("÷") + order.front()))
        return;

    if(userInput->toPlainText() == QString("Enter the expression...")){
        if ((symbol == "+") || (symbol == "-") || (symbol == "×") || (symbol == "÷"))
            return; //we don't add + - × or ÷ to an empty expression
        userInput->setPlainText(sender->getText()->toPlainText()); //if the expression is empty - clear the text and set it to our symbol
        answer->setPlainText("=");
    } else {
        if ((symbol == "+") || (symbol == "-") || (symbol == "×") || (symbol == "÷")){
            if((userInput->toPlainText().back() == "+") || (userInput->toPlainText().back() == "-") || (userInput->toPlainText().back() == "×") || (userInput->toPlainText().back() == "÷"))
                return; //we don't add an operation after another operation
        } else {
            if (userInput->toPlainText().back() == order.front()){
                if (userInput->toPlainText().size() == 1)
                    return; //we dont add a zero behind another zero at the beginning of expression
                if ((userInput->toPlainText().at(userInput->toPlainText().size() - 2) == "+") || \
                    (userInput->toPlainText().at(userInput->toPlainText().size() - 2) == "-") || \
                    (userInput->toPlainText().at(userInput->toPlainText().size() - 2) == "×") || \
                    (userInput->toPlainText().at(userInput->toPlainText().size() - 2) == "÷") )
               return;  //we don't add digits behind our zero unless it is in a number, we add only + - × or ÷
            }
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

void MainWindow::removeLastSymbol() {
    if (userInput->toPlainText() == QString("Enter the expression..."))
        return;
    setExpression(userInput, userInput->toPlainText().chopped(1));
    if (userInput->toPlainText().size() == 0)
        setExpression(userInput, "Enter the expression...");
    //if after the removal there is no division by zero => remove error message.
    if (!userInput->toPlainText().contains(QString("÷") + order.front())){
        answer->setPlainText("=");
        return;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow(){
    delete scene;
}
