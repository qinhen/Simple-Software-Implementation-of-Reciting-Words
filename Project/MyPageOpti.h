﻿#pragma once
#ifndef PAGE_OPTI_H
#define PAGE_OPTI_H

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
#include<QtWidgets/QLineEdit>
#include<QtWidgets/QScrollArea>
#include<QtWidgets/QHBoxLayout>
#include<QtGui/QValidator>
#include<QtWidgets/QListWidget>
#include<QtWidgets/QListWidgetItem>
#include<QtWidgets/QComboBox>
#include<QtWidgets/QFileDialog>
#include<QtWidgets/QItemDelegate>

#include "MyConfiguration.h"

class MainWindow;


class MyOptiPage:public QWidget
{
    Q_OBJECT
public:
    MainWindow*      m_windows;
    MyConfiguration* m_conf;

    QWidget*     m_filterWidget;
    QWidget*     m_filterContainerWidget;
    QLabel*      m_filterLabel;
    QVBoxLayout* m_filterLayout;
    QPushButton* m_visualFilterButton;


    QWidget* m_settingAreaBackgroundWidget;
    //视觉面板


    QWidget* m_visualSettingWidget;
    QWidget* m_visualSettingLabelWidget;
    QWidget* m_visualSettingCompWidget;

        //1
    QLabel*      m_sizeOptionLabel;
    QComboBox*   m_sizeOptionBox;
    QPushButton* m_sizeOptionButton;
        //2
    QLabel*      m_backgroundPathLabel;
    QLabel*      m_pathLabel;
    QPushButton* m_getPathButton;


    MyOptiPage(QWidget* parent, MainWindow* window, MyConfiguration* conf);

    void setup_comp();

    void setup_geo();

    void setup_connect();

    void setup_grap();



public slots:
    void slot_change_window_size();

    void slot_select_path();

    void slot_diy_window_size();
};


#endif // PAGE_OPTI_H
