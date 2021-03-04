#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/qapplication.h>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtCore/QVector>
#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>


#include "MyConfiguration.h"
#include "MyVirtualUserSystem.h"
#include "core/WordLibrary.h"
#include <iostream>

#include "MyPageMain.h"
#include "MyPageTask.h"
#include "MyPageDict.h"
#include "MyPageOpti.h"

class MyWindowMoveArea :public QWidget
{
    Q_OBJECT
public:
    QMainWindow* m_windows;

    MyWindowMoveArea(QWidget* parent, QMainWindow* window) :QWidget(parent), m_windows(window)
    {

    }

    //处理窗口移动
private:
    QPoint m_lastWindowsPos;
    QPoint m_lastMousePos;
    //鼠标按下
    void mousePressEvent(QMouseEvent *e)
    {
        if (e->pos().rx() > 10000 || e->pos().ry() > 5000)
            return;
        m_lastWindowsPos = m_windows->pos();
        m_lastMousePos = e->globalPos();
    }
    //鼠标移动
    void mouseMoveEvent(QMouseEvent *e)
    {
        if (e->pos().rx() > 10000 || e->pos().ry() > 5000)
            return;
        int dx = e->globalX() - m_lastMousePos.x();
        int dy = e->globalY() - m_lastMousePos.y();
        m_windows->move(m_lastWindowsPos.x() + dx, m_lastWindowsPos.y() + dy);
    }
    //鼠标释放
    void mouseReleaseEvent(QMouseEvent *e)
    {
        if (e->pos().rx() > 10000 || e->pos().ry() > 5000)
            return;
        int dx = e->globalX() - m_lastMousePos.x();
        int dy = e->globalY() - m_lastMousePos.y();
        if(m_lastWindowsPos.y() + dy >= 0)
            m_windows->move(m_lastWindowsPos.x() + dx, m_lastWindowsPos.y() + dy);
        else
            m_windows->move(m_lastWindowsPos.x() + dx, 0);
    }

public:

};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    
private:

	void setup_ui();

    void setup_conf();

    void setup_comp();

    void setup_geo();

    void setup_grap();

    void setup_connect();

    void set_mainWindow_background(QImage const& image);


public:
    MyConfiguration m_configuration;


    team::WordLibrary m_library;
    MyVirtualUserSystem m_userSystem;

    QImage m_mainWindow_backgroundImage;

    //标题栏
    MyWindowMoveArea* m_mainWindow_moveableArea;
    QLabel*           m_mainWindow_title;
    QPushButton*      m_mianWindow_mainmenuButton;
    QPushButton*      m_mianWindow_exitButton;

    //菜单
    QWidget*     m_mainWindow_mainMenu;
    bool m_mainWindow_mainMenu_onShow = false;
	void show_mainMenu(bool flag);
    QPushButton* m_mainMenu_main;
    QPushButton* m_mainMenu_work;
    QPushButton* m_mainMenu_dict;
    QPushButton* m_mainMenu_opti;

    QLabel*      m_mainMenu_mainIcon;
    QLabel*      m_mainMenu_taskIcon;
    QLabel*      m_mainMenu_dictIcon;
    QLabel*      m_mainMenu_optiIcon;

    QLabel*      m_mainMenu_mainStr;
    QLabel*      m_mainMenu_taskStr;
    QLabel*      m_mainMenu_dictStr;
    QLabel*      m_mainMenu_optiStr;

    MyMainPage* m_mainPage;
    MyWorkPage* m_taskPage;
    MyDictPage* m_dictPage;
    MyOptiPage* m_optiPage;

private:
	void resizeEvent(QResizeEvent* e);

public:
    void show_page(int id);
	void reconf_backgroundImage();

public slots:
    void slot_show_mainPage() { show_page(0); }
    void slot_show_taskPage() { show_page(1); }
    void slot_show_dictPage() { show_page(2); }
    void slot_show_optiPage() { show_page(3); }
    void slot_show_mainMenu(){if(m_mainWindow_mainMenu_onShow)show_mainMenu(false);else show_mainMenu(true);}
    void slot_exit_mainWindow(){ this->close(); }
    void slot_resize_mainWindow(){ setup_geo(); }
    void slot_reset_backgroundImage() { reconf_backgroundImage(); }

};


#endif // MAINWINDOW_H
