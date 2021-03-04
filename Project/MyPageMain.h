#pragma once
#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

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
#include "MyVirtualUserSystem.h"





class MyMainPage:public QWidget
{
Q_OBJECT
public:

    QWidget* m_leftPart;
    QWidget* m_userShowWidget;
    QWidget* m_userHeadImage;
    QLabel* m_userName;
    QPushButton* m_logoutUser;
    QPushButton* m_deleteUser;


    QWidget* m_loginShowWidget;
    QListWidget* m_usersList;


    QWidget* m_loginCard;
    QLineEdit* m_loginCard_Account;
    QLineEdit* m_loginCard_Password;
    QPushButton* m_loginCard_login;
    QPushButton* m_loginCard_reg;


    QWidget* m_newsShowWidget;


    MyVirtualUserSystem* m_userSystem;


    MyMainPage(QWidget* parent, MyVirtualUserSystem* userSystem):QWidget(parent)
    {
        m_userSystem = userSystem;
    }

    void setup_connect()
    {
        connect(m_usersList,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(slot_select_user(QListWidgetItem *)));

        connect(m_loginCard_login,SIGNAL(clicked()),this,SLOT(slot_bt_login()));
        connect(m_loginCard_reg,SIGNAL(clicked()),this,SLOT(slot_bt_reg()));

        connect(m_logoutUser,SIGNAL(clicked()),this,SLOT(slot_bt_logout()));
        connect(m_deleteUser,SIGNAL(clicked()),this,SLOT(slot_bt_del()));
    }

    void setup_geo()
    {
        this->setGeometry(0,35,parentWidget()->width(),parentWidget()->height()-35);
        m_leftPart->setGeometry(0,0,300,height());

        m_userShowWidget->setGeometry(0,0,300,height());
        m_userHeadImage->setGeometry(100,100,100,100);
        m_userName->setGeometry(0,200,300,40);
        m_logoutUser->setGeometry(100,250,100,25);
        m_deleteUser->setGeometry(100,280,100,25);


        m_loginShowWidget->setGeometry(0,0,300,height());
        m_usersList->setGeometry(20,80,m_loginShowWidget->width()-40,m_loginShowWidget->height()/2 - 110);
        m_loginCard->setGeometry(20,m_loginShowWidget->height()/2+(m_loginShowWidget->height()/2-150)/2,m_loginShowWidget->width()-40,150);
        m_loginCard_Account->setGeometry(0,0,m_loginCard->width(),30);
        m_loginCard_Password->setGeometry(0,40,m_loginCard->width(),30);
        m_loginCard_login->setGeometry(0,80,m_loginCard->width()/2-5,25);
        m_loginCard_reg->setGeometry(m_loginCard->width()/2+5,80,m_loginCard->width()/2-5,25);


        m_newsShowWidget->setGeometry(300,0,width()-300,height());
    }

    void setup_comp()
    {

        m_leftPart = new QWidget(this);

        m_userShowWidget = new QWidget(m_leftPart);
        m_userHeadImage = new QWidget(m_userShowWidget);
        m_userName = new QLabel(m_userShowWidget);
        m_userName->setAlignment(Qt::AlignCenter);
        m_logoutUser = new QPushButton(QString::fromLocal8Bit("Logout"),m_userShowWidget);
        m_deleteUser = new QPushButton(QString::fromLocal8Bit("Delete User"),m_userShowWidget);
        m_userShowWidget->hide();


        m_loginShowWidget = new QWidget(m_leftPart);
        m_usersList = new QListWidget(m_loginShowWidget);
        m_loginCard = new QWidget(m_loginShowWidget);
        m_loginCard_Account = new QLineEdit(m_loginCard) ;
        m_loginCard_Account->setPlaceholderText(QString::fromLocal8Bit("Account"));
        m_loginCard_Password = new QLineEdit(m_loginCard) ;
        m_loginCard_Password->setPlaceholderText(QString::fromLocal8Bit("Password"));
        m_loginCard_login = new QPushButton(QString::fromLocal8Bit("Login"),m_loginCard);
        m_loginCard_reg = new QPushButton(QString::fromLocal8Bit("Register"),m_loginCard);



        m_newsShowWidget = new QWidget(this);


        update_userNameList();
        update_userInfo();
    }


    void setup_grap()
    {
        QPalette palette = this->palette();


        palette = m_leftPart->palette();
        palette.setColor(QPalette::Window,QColor(225,225,225,250));
        m_leftPart->setAutoFillBackground(true);
        m_leftPart->setPalette(palette);


        palette = m_userHeadImage->palette();
        palette.setColor(QPalette::Window,QColor(225,225,225,250));
        m_userHeadImage->setAutoFillBackground(true);
        m_userHeadImage->setPalette(palette);


        palette = m_newsShowWidget->palette();
        palette.setColor(QPalette::Window,QColor(50,50,50,180));
        m_newsShowWidget->setAutoFillBackground(true);
        m_newsShowWidget->setPalette(palette);


    }



private:

    void update_userInfo()
    {
        m_userName->setText(QString::fromStdString(m_userSystem->get_userName()));

        QImage image(QString::fromStdString(m_userSystem->get_userHeadImagePath()));
        if(image.isNull())
            image.load(":/res/user_head_image.png");// 引用资源
        QPalette pe = m_userHeadImage->palette();
        pe.setBrush(QPalette::Window,QBrush(image));
        m_userHeadImage->setPalette(pe);

    }
    void update_userNameList()
    {
        auto user_names_list = m_userSystem->get_all_userNames();
        m_usersList->clear();
        for(auto i:user_names_list)
              m_usersList->addItem(QString::fromStdString(i));
    }

    void to_loginPage()
    {
        m_loginShowWidget->show();
        m_userShowWidget->hide();
    }

    void to_userPage()
    {
        m_userShowWidget->show();
        m_loginShowWidget->hide();
    }

public slots:

    void slot_select_user(QListWidgetItem *item)
    {
        m_loginCard_Account->setText(item->text());
    }

    void slot_bt_login()
    {
        QString account = m_loginCard_Account->text();
        QString password = m_loginCard_Password->text();
        if(account.isNull()||password.isNull())
            return;
        else
        {
            bool success = m_userSystem->login(account.toStdString(),password.toStdString());
            if(success)
            {
                update_userInfo();
                to_userPage();
            }
        }
    }

    void slot_bt_reg()
    {
        QString account = m_loginCard_Account->text();
        QString password = m_loginCard_Password->text();
        if(account.isNull()||password.isNull())
            return;
        else
        {
            bool success = m_userSystem->reg(account.toStdString(),password.toStdString());
            if(success)
            {
                update_userNameList();
                m_loginCard_Account->clear();
                m_loginCard_Password->clear();
                to_loginPage();
            }
        }
    }

    void slot_bt_logout()
    {
        m_userSystem->logout();
        to_loginPage();
    }

    void slot_bt_del()
    {
        if(m_userSystem->is_logined())
        {
            m_userSystem->del();
            if(m_userSystem->is_logined())
            {
                update_userNameList();
                to_loginPage();
            }

        }

    }



};

#endif // PAGE_MAIN_H
