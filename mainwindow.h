#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "gamecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setButtonsStatus();
    void setView();
    void initBackground();

    bool eventFilter(QObject* object, QEvent* event) override;

private:
    Ui::MainWindow *ui;
    QAction* start;
    QAction* quit;
    QAction* load;
    QAction* save;
    QAction* pause;
    QAction* restart;
    QAction* unpause;

    QGraphicsView* view;
    QGraphicsScene* scene;

    friend class gamecontroller;
    gamecontroller* game;

};
#endif // MAINWINDOW_H
