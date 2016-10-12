#include "mainwindow.h"
#include <QDebug>
#include <QtMath>
#include <QState>
#include <QStateMachine>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QPixmap pix(":/images/background.jpg");
    this->setFixedSize(pix.size());

    QPalette palette;
    palette.setBrush(QPalette::Background, pix);
    this->setPalette(palette);

    QStringList strList;
    strList<<":/images/accessories-calculator.png";
    strList<<":/images/accessories-text-editor.png";
    strList<<":/images/help-browser.png";
    strList<<":/images/internet-group-chat.png";
    strList<<":/images/internet-mail.png";
    strList<<":/images/internet-web-browser.png";
    strList<<":/images/office-calendar.png";
    strList<<":/images/system-users.png";

    int width = this->width();
    int hight = this->height();
    QList<QPoint> pointList;
    QList<QLabel*> labelList;
    for (int i=0; i<strList.count(); i++)
    {
        Label *pLabel = new Label(this);
        labelList<<pLabel;
        pLabel->setPixmap(QPixmap(strList[i]));
        pLabel->resize(QPixmap(strList[i]).size());

        double x = width/2 - 30 + 170*qCos(i*3.14/4 + 20);
        double y = hight/2 - 30 + 100*qSin(i*3.14/4 + 20);

        pointList<<QPoint(x,y);
        pLabel->move(x,y);
    }
    
    QState *parentState = new QState();
    QList<QState*> stateList;
    for (int i=0; i<strList.count(); i++)
    {
        QState *pState = new QState(parentState);
        stateList<<pState;
        for (int j=0; j<strList.count(); j++)
        {
            pState->assignProperty(labelList[j],"pos", pointList[(i+j)%pointList.count()]);
        }
        if (i == 0)parentState->setInitialState(pState);
    }

    for (int i=0; i<stateList.count(); i++)
    {
        stateList[i]->addTransition(this, SIGNAL(right()), stateList[(i+1)%stateList.count()]);
        stateList[i]->addTransition(this, SIGNAL(left()), stateList[(i-1+stateList.count())%stateList.count()]);
    }

    for (int i=0; i<stateList.count(); i++)
    {
        for (int j=0; j<labelList.count(); j++)
        {
            stateList[i]->addTransition(static_cast<Label*>(labelList[j]), SIGNAL(click()), stateList[(stateList.count()-j+2)%stateList.count()]);
        }
    }

    QStateMachine *machine = new QStateMachine;
    machine->addState(parentState);
    machine->setInitialState(parentState);
    for(int i=0; i<labelList.count(); i++)
    {
        machine->addDefaultAnimation(new QPropertyAnimation(labelList.at(i), "pos"));
    }
    machine->start();


    //////
}

MainWindow::~MainWindow()
{

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Down:
        emit left();
        break;
    case Qt::Key_Right:
    case Qt::Key_Up:
        emit right();
        break;
    default:
        break;
    }
    return QWidget::keyPressEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QLinearGradient line(0,0, this->width(), this->height());
    line.setColorAt(0.1, QColor(255,255,255,0));
    line.setColorAt(0.9, QColor(255,255,255,255));

    //p.setBrush(line);
    p.fillRect(rect(), line);
}
