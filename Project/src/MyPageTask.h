#pragma once
#ifndef PAGE_WORK_H
#define PAGE_WORK_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QValidator>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTextEdit>

#include "core/WordLibrary.h"
#include "MyVirtualUserSystem.h"

class MyLogProcessor
{
    struct ATask // 一个待背单词
    {
        //前两项由 MyLogProcessor 根据单词本、本地日志 生成
        unsigned m_indices[4];// 四个选项
        unsigned char m_resultIndex;// 正确选项的索引值，取值为0-3

        //后三项由 ui 系统根据用户使用情况生成，作为反馈信息回传到 MyLogProcessor
        bool m_finished; //【今天】已经背过
        bool m_correct;//用户选择是否正确
        int m_userResultIndex;//用户选择的选项索引
    };

public:
    //生成任务数组 ，ui将重复调用，直到返回值std::vector<ATask>.size() == 0为止
    std::vector<ATask> create_today_task();

    //当create_today_task()生成的单词数组使用完毕时 ui将调用此函数回传用户背单词的对错情况
    void process_today_task(const std::vector<ATask>&);

};







class MyTaskPage_option :public QWidget
{
    Q_OBJECT
public:
    QPalette m_defaultBtPe;
    QPalette m_wrongBtPe;
    QPalette m_rightBtPe;

    team::WordLibrary* m_library;

    QProgressBar* m_progress;
    QLabel* m_spelling;
    QTextEdit* m_question;
    QPushButton* m_option[4];

    MyVirtualWord* m_currentWork;

    MyTaskPage_option(QWidget* parent, team::WordLibrary* libary) :QWidget(parent)
    {
        m_library = libary;
        m_currentWork = nullptr;
    }

    void setup_comp()
    {
        m_progress = new QProgressBar(this);
        m_spelling = new QLabel(this);
        m_question = new QTextEdit(this);
        m_question->setReadOnly(true);

        m_option[0] = new QPushButton(this);
        m_option[1] = new QPushButton(this);
        m_option[2] = new QPushButton(this);
        m_option[3] = new QPushButton(this);

        m_defaultBtPe = m_option[0]->palette();
        m_defaultBtPe.setColor(QPalette::Button, QColor(200, 200, 200, 200));///
        m_wrongBtPe = m_defaultBtPe;
        m_rightBtPe = m_defaultBtPe;
        m_wrongBtPe.setColor(QPalette::Button, QColor(150, 0, 0, 180));///
        m_rightBtPe.setColor(QPalette::Button, QColor(0, 150, 0, 180));///
        m_wrongBtPe.setColor(QPalette::ButtonText, QColor(255, 255, 255));
        m_rightBtPe.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    }

    void setup_geo()
    {
        int awidth = parentWidget()->width() - 200;
        awidth = awidth > 720 ? 720 : awidth;
        int start_x = (parentWidget()->width() - awidth) / 2;
        int start_y = (parentWidget()->height() - 400) / 2;

        this->setGeometry(start_x, start_y, awidth, 400);

        m_progress->setGeometry(28, 5, width()-30, 20);

        m_spelling->setGeometry(0, 30, width(), 95);

        m_question->setGeometry(0, 125, width(), 80);

        m_option[0]->setGeometry(0, 205, awidth, 45);
        m_option[1]->setGeometry(0, m_option[0]->y() + 44, awidth, 45);
        m_option[2]->setGeometry(0, m_option[1]->y() + 44, awidth, 45);
        m_option[3]->setGeometry(0, m_option[2]->y() + 44, awidth, 45);
    }

    void setup_grap()
    {
        QFont font = this->font();
        QPalette pe = this->palette();

        this->setAutoFillBackground(true);
        pe.setColor(QPalette::Window, QColor(150, 150, 150, 200));
        this->setPalette(pe);

        pe = m_progress->palette();
        pe.setColor(QPalette::WindowText, QColor(255, 255, 255));
        m_progress->setPalette(pe);



        m_spelling->setAutoFillBackground(true);
        pe = m_spelling->palette();
        pe.setColor(QPalette::Window, QColor(255, 255, 255, 225));
        m_spelling->setPalette(pe);

        font.setPixelSize(45);
        m_spelling->setFont(font);

        font.setPixelSize(18);
        m_question->setFont(font);
        font.setPixelSize(16);
        m_option[0]->setFont(font);
        m_option[1]->setFont(font);
        m_option[2]->setFont(font);
        m_option[3]->setFont(font);

        m_option[0]->setFlat(true);
        m_option[1]->setFlat(true);
        m_option[2]->setFlat(true);
        m_option[3]->setFlat(true);

        m_option[0]->setAutoFillBackground(true);
        m_option[1]->setAutoFillBackground(true);
        m_option[2]->setAutoFillBackground(true);
        m_option[3]->setAutoFillBackground(true);

        m_option[0]->setStyleSheet("text-align: left;");
        m_option[1]->setStyleSheet("text-align: left;");
        m_option[2]->setStyleSheet("text-align: left;");
        m_option[3]->setStyleSheet("text-align: left;");

        default_grap();
    }

    void setup_connect()
    {
        connect(m_option[0], SIGNAL(clicked()), this, SLOT(slot_select_A()));
        connect(m_option[1], SIGNAL(clicked()), this, SLOT(slot_select_B()));
        connect(m_option[2], SIGNAL(clicked()), this, SLOT(slot_select_C()));
        connect(m_option[3], SIGNAL(clicked()), this, SLOT(slot_select_D()));
    }


    void set_work(MyVirtualWord* work)
    {
        m_currentWork = work;
        default_grap();
        for (int i = 0; i < 4; i++)
        {
            auto w = m_library->read_utf8(work->optionWordIndices[i]);
            m_option[i]->setText(QString::fromStdString(w.translation(0)));
            if (i == work->correctOptionIndex)
            {
                m_spelling->setText(QString::fromStdString(w.spelling()));
                m_question->setText(QString::fromStdString(w.sentence_en(0)));
            }
            if (work->isChecked)
            {
                if (i == work->correctOptionIndex)
                    m_option[i]->setPalette(m_rightBtPe);
                if (i == work->userChoiceOptionIndex && i != work->correctOptionIndex)
                    m_option[i]->setPalette(m_wrongBtPe);
            }
        }
    }

private:

    void select(int index)
    {
        if (m_currentWork && (!m_currentWork->isChecked))
        {
            m_currentWork->userChoiceOptionIndex = index;
            if (m_currentWork->correctOptionIndex == index)
                m_currentWork->isCorrect = true;
            else
                m_currentWork->isCorrect = false;
            m_currentWork->isChecked = true;
            emit mysignal_optionSelected();
        }
    }



    void default_grap()
    {
        for (int i = 0; i < 4; i++)
            m_option[i]->setPalette(m_defaultBtPe);
    }


public slots:
    void slot_select_A() { select(0); }
    void slot_select_B() { select(1); }
    void slot_select_C() { select(2); }
    void slot_select_D() { select(3); }

signals:
    void mysignal_optionSelected();

};



class MyWorkPage_result :public QWidget
{
    Q_OBJECT
public:
    QPalette m_defaultBtPe;
    team::WordLibrary* m_library;

    QLabel* m_showSpelling;
    QLabel* m_translation[5];

    QLabel* m_rightTip;
    QLabel* m_wrongTip;

    QListWidget* m_listWidget;

    MyWorkPage_result(QWidget* parent, team::WordLibrary* library) :QWidget(parent)
    {
        m_library = library;
    }

    void set_work(MyVirtualWord* work)
    {

        set_tip(work->isCorrect);
        if (work)
        {
            auto w = m_library->read_utf8(work->optionWordIndices[work->correctOptionIndex]);
            m_showSpelling->setText(QString::fromStdString(w.spelling()));
            for (int i = 0; i < 5; i++)
                m_translation[i]->clear();
            for (int i = 0; i < 5; i++)
            {
                if (i < w.sizeofTran())
                    m_translation[i]->setText(QString::fromStdString(w.translation(i)));
                else {
                    break;
                }
            }
            m_listWidget->clear();
            for(int i = 0;i<w.sizeofSent();i++){
                QListWidgetItem *it_sent_en = new QListWidgetItem(QString::fromStdString(w.sentence_en(i)));
                QListWidgetItem *it_sent_cn = new QListWidgetItem(QString::fromStdString(w.sentence_cn(i)));
                it_sent_cn->setBackground(QColor(235,235,235));
                m_listWidget->addItem(it_sent_en);
                m_listWidget->addItem(it_sent_cn);
            }

        }
    }

    void setup_comp()
    {
        m_showSpelling = new QLabel(this);
        for (int i = 0; i < 5; i++)
            m_translation[i] = new QLabel(this);

        m_rightTip = new QLabel(QString::fromLocal8Bit("选择正确"), this);
        m_wrongTip = new QLabel(QString::fromLocal8Bit("选择错误"), this);
        m_rightTip->hide();
        m_wrongTip->hide();


         m_listWidget = new QListWidget(this);
         //m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    void setup_geo()
    {
        int awidth = parentWidget()->width() - 200;
        awidth = awidth > 720 ? 720 : awidth;
        int start_x = (parentWidget()->width() - awidth) / 2;
        int start_y = (parentWidget()->height() - 400) / 2;

        this->setGeometry(start_x, start_y, awidth, 400);

        m_rightTip->setGeometry(0, 0, 100, 30);
        m_wrongTip->setGeometry(0, 0, 100, 30);
        m_showSpelling->setGeometry(0, 30, width(), 60);
        m_translation[0]->setGeometry(0, 90, width(), 25);
        m_translation[1]->setGeometry(0, 115, width(), 25);
        m_translation[2]->setGeometry(0, 140, width(), 25);
        m_translation[3]->setGeometry(0, 165, width(), 25);
        m_translation[4]->setGeometry(0, 190, width(), 25);

        m_listWidget->setGeometry(0,215,width(),185);
        //m_listWidget->setGeometry(0,0,width(),1500);

    }
    void setup_grap()
    {

        QPalette pe = this->palette();

        this->setAutoFillBackground(true);
        pe.setColor(QPalette::Window, QColor(100, 100, 100, 150));
        this->setPalette(pe);



        QFont font = this->font();
        font.setPixelSize(45);
        m_showSpelling->setAutoFillBackground(true);
        m_showSpelling->setFont(font);
        font.setPixelSize(16);
        for (int i = 0; i < 5; i++)
        {
            m_translation[i]->setFont(font);
            m_translation[i]->setAutoFillBackground(true);
        }


        pe.setColor(QPalette::Window, QColor(255, 255, 255, 225));
        m_showSpelling->setPalette(pe);
        for (int i = 0; i < 5; i++)
            m_translation[i]->setPalette(pe);


        pe = m_rightTip->palette();
        pe.setColor(QPalette::Window, QColor(0, 100, 0, 180));///
        pe.setColor(QPalette::Text, QColor(255, 255, 255, 255));///
        m_rightTip->setPalette(pe);
        pe = m_wrongTip->palette();
        pe.setColor(QPalette::Window, QColor(100, 0, 0, 180));
        pe.setColor(QPalette::Text, QColor(255, 255, 255, 255));
        m_wrongTip->setPalette(pe);

        m_wrongTip->setAutoFillBackground(true);
        m_rightTip->setAutoFillBackground(true);

        m_wrongTip->setAlignment(Qt::AlignCenter);
        m_rightTip->setAlignment(Qt::AlignCenter);


        font = m_listWidget->font();
        font.setPixelSize(14);
        m_listWidget->setFont(font);

    }
    void setup_connect()
    {



    }

    void set_tip(bool flag)
    {
        if (flag)
        {
            m_rightTip->show();
            m_wrongTip->hide();
        }
        else {
            m_rightTip->hide();
            m_wrongTip->show();
        }
    }



};





class MyWorkPage :public QWidget
{
    Q_OBJECT

public:

    team::WordLibrary* m_library;
    MyVirtualUserSystem* m_userSystem;

    QWidget* m_startCard;
    QTextEdit* m_text;
    QPushButton* m_pushbuttonStart;

    QPushButton* m_btToNext;
    QPushButton* m_btToLast;

    MyTaskPage_option* m_optionPage;
    MyWorkPage_result* m_resultPage;
    bool on_work;

    MyWorkPage(QWidget* parent, team::WordLibrary* libary, MyVirtualUserSystem* userSystem) :QWidget(parent)
    {
        m_library = libary;
        m_userSystem = userSystem;

        on_work = false;
        m_state = STOP;
        m_currentTask = 0;
        m_showOption = false;
    }

private:

    std::vector<MyVirtualWord> m_Tasks;

    unsigned m_currentTask;

    enum MyState
    {
        STOP = 0,
        OPTION = 1,
        RESULT = 2,
        RUN = 3,
    };

    MyState m_state;
    bool m_showOption;



    void to_next_state()
    {
        if (m_state == RUN)
        {
            if (m_currentTask < m_Tasks.size()) {
                m_state = RUN;
                to_next_page();
            }
            else {

            }
        }

    }


    void to_last_page()
    {
        if (!(m_state == RUN))return;
        if (m_showOption)
        {
            if (m_currentTask <= 0)
            {

            }
            else {
                m_showOption = false;
                m_currentTask--;
                m_resultPage->set_work(&m_Tasks[m_currentTask]);
                m_optionPage->hide();
                m_resultPage->show();
            }
        }
        else {
            m_showOption = true;
            m_optionPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->show();
            m_resultPage->hide();
        }
        update_progressBar();
    }

    void to_next_page()
    {
        //std::cout<<"to_next_page"<<std::endl;
        if (!(m_state == RUN))return;
        if (m_showOption)
        {
            m_showOption = false;
            m_resultPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->hide();
            m_resultPage->show();
        }
        else {
            if (m_currentTask < m_Tasks.size() - 1) {
                m_showOption = true;
                m_currentTask++;
                m_optionPage->set_work(&m_Tasks[m_currentTask]);
                m_optionPage->show();
                m_resultPage->hide();
            }
            else {

                if(m_userSystem->is_logined()){
                    m_userSystem->feedback(m_Tasks);
                    m_Tasks = m_userSystem->request();
                    if(m_Tasks.size() == 0)
                    {
                         endup();
                         m_state = STOP;
                    }
                    else
                    {
                        restart();
                    }

                }
                else {
                    endup();
                }
            }
        }
        update_progressBar();
    }



public:


    void  update_progressBar()
    {
        if (m_Tasks.size())
        {
            m_optionPage->m_progress->setMinimum(0);
            m_optionPage->m_progress->setMaximum(m_Tasks.size());
            m_optionPage->m_progress->setValue(m_currentTask);
        }
    }

    void restart()
    {

        if(!m_userSystem->is_logined())
        {
            //未登录
        }
        else
        {
            m_Tasks = m_userSystem->request();
        }

        if(m_Tasks.size()==0)
        {
            //没有任务
        }
        else
        {
            m_startCard->hide();
            m_btToLast->show();
            m_btToNext->show();

            m_currentTask = 0;
            m_optionPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->show();
            m_resultPage->hide();
            m_showOption = true;
            m_state = RUN;
        }

    }

    void endup()
    {
        m_startCard->show();
        m_optionPage->hide();
        m_resultPage->hide();
        m_btToLast->hide();
        m_btToNext->hide();
    }


    void setup_connect()
    {

        connect(m_pushbuttonStart, SIGNAL(clicked()), this, SLOT(slot_start()));
        connect(m_btToLast, SIGNAL(clicked()), this, SLOT(slot_bt_to_last()));
        connect(m_btToNext, SIGNAL(clicked()), this, SLOT(slot_bt_to_next()));
        connect(m_optionPage, SIGNAL(mysignal_optionSelected()), this, SLOT(slot_process_state()));

        m_optionPage->setup_connect();
        m_resultPage->setup_connect();
    }

    void setup_comp()
    {
        m_startCard = new QWidget(this);
        m_text = new QTextEdit(m_startCard);
        m_text->setText(QString("\n") +
            "                            FBI WARNING\n\n" +
            "   Federal Law provides severe civil and criminal penalties for\n" +
            "   the unauthorized reproduction, distribution, or exhibition of\n" +
            "   copyrighted motion pictures (Title 17, United States Code.\n" +
            "   Sections 501 and 508). The Federal Bureau of Investigation\n" +
            "   investigates allegations of criminal copyright infringement\n" +
            "           (Title 17, United States Code Section 506)."
        );
        m_text->setReadOnly(true);
        m_pushbuttonStart = new QPushButton(QString::fromLocal8Bit("开始任务"), m_startCard);

        m_optionPage = new MyTaskPage_option(this, m_library);
        m_optionPage->setup_comp();
        m_resultPage = new MyWorkPage_result(this, m_library);
        m_resultPage->setup_comp();
        m_optionPage->hide();
        m_resultPage->hide();

        m_btToNext = new QPushButton("next", this);
        m_btToLast = new QPushButton("back", this);
        m_btToLast->hide();
        m_btToNext->hide();
    }

    void setup_geo()
    {
        this->setGeometry(0, 35, parentWidget()->width(), parentWidget()->height() - 35);
        m_startCard->setGeometry(0, 0, this->width(), this->height());
        m_text->setGeometry((this->width() - 600) / 2, (this->height() - 380) / 2, 600, 380);
        m_pushbuttonStart->setGeometry((this->width() - 100) / 2, m_text->y() + m_text->height() + 20, 100, 30);

        m_btToLast->setGeometry(0, 0, 100, height());
        m_btToNext->setGeometry(width() - 100, 0, 100, height());

        m_optionPage->setup_geo();
        m_resultPage->setup_geo();
    }

    void setup_grap()
    {
        QFont font = m_text->font();
        QPalette palette = m_text->palette();
        font.setPixelSize(18);
        m_text->setFont(font);
        palette.setColor(QPalette::Base, QColor(10, 10, 10));
        palette.setColor(QPalette::Text, QColor(255, 0, 0));
        palette.setColor(QPalette::Highlight, QColor(50, 50, 50));
        palette.setColor(QPalette::HighlightedText, QColor(200, 200, 200));
        m_text->setPalette(palette);


        palette = m_btToLast->palette();
        palette.setColor(QPalette::Button, QColor(200, 200, 200, 210));
        m_btToLast->setAutoFillBackground(true);
        m_btToNext->setAutoFillBackground(true);
        m_btToLast->setFlat(true);
        m_btToNext->setFlat(true);
        m_btToLast->setPalette(palette);
        m_btToNext->setPalette(palette);


        m_optionPage->setup_grap();
        m_resultPage->setup_grap();
    }



private slots:
    void slot_process_state() { to_next_state(); }
    void slot_bt_to_next() {


        if (m_currentTask < m_Tasks.size())
        {
            if (!m_Tasks[m_currentTask].isChecked)
            {

            }
            else {
                to_next_page();
            }
        }
    }
    void slot_bt_to_last() { to_last_page(); }
    void slot_start() {
        restart();
    }
};

#endif // PAGE_MAIN_H
