#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT
public:
    Label(QWidget *parent = 0)
        : QLabel(parent)
    {
    }
signals:
    void click();
protected:
    void mousePressEvent(QMouseEvent *ev)
    {
        emit click();
        return QLabel::mousePressEvent(ev);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void right();
    void left();

protected:
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *);
};


#endif // MAINWINDOW_H
