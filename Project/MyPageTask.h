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




class MyTaskPage_option :public QWidget
{
    Q_OBJECT
public:

    team::WordLibrary* m_library;

    QPalette m_defaultBtPalette;
    QPalette m_wrongBtPalette;
    QPalette m_rightBtPalette;

    QProgressBar* m_progressProgressBar;
    QLabel*       m_spellingLabel;
    QTextEdit*    m_exampleSentenceTextEdit;
    QPushButton*  m_optionsButton[4];

    MyVirtualWord* m_currentWord;


    MyTaskPage_option(QWidget* parent, team::WordLibrary* libary) :QWidget(parent)
    {
        m_library = libary;
        m_currentWord = nullptr;
    }

    void setup_comp()
    {
        m_progressProgressBar = new QProgressBar(this);
        m_spellingLabel = new QLabel(this);
        m_exampleSentenceTextEdit = new QTextEdit(this);
        m_exampleSentenceTextEdit->setReadOnly(true);

        m_optionsButton[0] = new QPushButton(this);
        m_optionsButton[1] = new QPushButton(this);
        m_optionsButton[2] = new QPushButton(this);
        m_optionsButton[3] = new QPushButton(this);

        m_defaultBtPalette = m_optionsButton[0]->palette();
        m_defaultBtPalette.setColor(QPalette::Button, QColor(200, 200, 200, 200));///
        m_wrongBtPalette = m_defaultBtPalette;
        m_rightBtPalette = m_defaultBtPalette;
        m_wrongBtPalette.setColor(QPalette::Button, QColor(150, 0, 0, 180));///
        m_rightBtPalette.setColor(QPalette::Button, QColor(0, 150, 0, 180));///
        m_wrongBtPalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
        m_rightBtPalette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
    }

    void setup_geo()
    {
        int awidth = parentWidget()->width() - 200;
        awidth = awidth > 720 ? 720 : awidth;
        int start_x = (parentWidget()->width() - awidth) / 2;
        int start_y = (parentWidget()->height() - 400) / 2;

        this->setGeometry(start_x, start_y, awidth, 400);

        m_progressProgressBar->setGeometry(28, 5, width()-30, 20);

        m_spellingLabel->setGeometry(0, 30, width(), 95);

        m_exampleSentenceTextEdit->setGeometry(0, 125, width(), 80);

        m_optionsButton[0]->setGeometry(0, 205, awidth, 45);
        m_optionsButton[1]->setGeometry(0, m_optionsButton[0]->y() + 44, awidth, 45);
        m_optionsButton[2]->setGeometry(0, m_optionsButton[1]->y() + 44, awidth, 45);
        m_optionsButton[3]->setGeometry(0, m_optionsButton[2]->y() + 44, awidth, 45);
    }

    void setup_grap()
    {
        QFont font = this->font();
        QPalette pe = this->palette();

        this->setAutoFillBackground(true);
        pe.setColor(QPalette::Window, QColor(150, 150, 150, 200));
        this->setPalette(pe);

        pe = m_progressProgressBar->palette();
        pe.setColor(QPalette::WindowText, QColor(255, 255, 255));
        m_progressProgressBar->setPalette(pe);



        m_spellingLabel->setAutoFillBackground(true);
        pe = m_spellingLabel->palette();
        pe.setColor(QPalette::Window, QColor(255, 255, 255, 225));
        m_spellingLabel->setPalette(pe);

        font.setPixelSize(45);
        m_spellingLabel->setFont(font);

        font.setPixelSize(18);
        m_exampleSentenceTextEdit->setFont(font);
        font.setPixelSize(16);
        m_optionsButton[0]->setFont(font);
        m_optionsButton[1]->setFont(font);
        m_optionsButton[2]->setFont(font);
        m_optionsButton[3]->setFont(font);

        m_optionsButton[0]->setFlat(true);
        m_optionsButton[1]->setFlat(true);
        m_optionsButton[2]->setFlat(true);
        m_optionsButton[3]->setFlat(true);

        m_optionsButton[0]->setAutoFillBackground(true);
        m_optionsButton[1]->setAutoFillBackground(true);
        m_optionsButton[2]->setAutoFillBackground(true);
        m_optionsButton[3]->setAutoFillBackground(true);

        m_optionsButton[0]->setStyleSheet("text-align: left;");
        m_optionsButton[1]->setStyleSheet("text-align: left;");
        m_optionsButton[2]->setStyleSheet("text-align: left;");
        m_optionsButton[3]->setStyleSheet("text-align: left;");

        default_grap();
    }

    void setup_connect()
    {
        connect(m_optionsButton[0], SIGNAL(clicked()), this, SLOT(slot_select_A()));
        connect(m_optionsButton[1], SIGNAL(clicked()), this, SLOT(slot_select_B()));
        connect(m_optionsButton[2], SIGNAL(clicked()), this, SLOT(slot_select_C()));
        connect(m_optionsButton[3], SIGNAL(clicked()), this, SLOT(slot_select_D()));
    }


    void set_work(MyVirtualWord* work)
    {
        m_currentWord = work;
        default_grap();
        for (int i = 0; i < 4; i++)
        {
            auto w = m_library->read_utf8(work->optionWordIndices[i]);
            m_optionsButton[i]->setText(QString::fromStdString(w.translation(0)));
            if (i == work->correctOptionIndex)
            {
                m_spellingLabel->setText(QString::fromStdString(w.spelling()));
                m_exampleSentenceTextEdit->setText(QString::fromStdString(w.sentence_en(0)));
            }
            if (work->isChecked)
            {
                if (i == work->correctOptionIndex)
                    m_optionsButton[i]->setPalette(m_rightBtPalette);
                if (i == work->userChoiceOptionIndex && i != work->correctOptionIndex)
                    m_optionsButton[i]->setPalette(m_wrongBtPalette);
            }
        }
    }

private:

    void select(int index)
    {
        if (m_currentWord && (!m_currentWord->isChecked))
        {
            m_currentWord->userChoiceOptionIndex = index;
            if (m_currentWord->correctOptionIndex == index)
                m_currentWord->isCorrect = true;
            else
                m_currentWord->isCorrect = false;
            m_currentWord->isChecked = true;
            emit mysignal_optionSelected();
        }
    }



    void default_grap()
    {
        for (int i = 0; i < 4; i++)
            m_optionsButton[i]->setPalette(m_defaultBtPalette);
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

    team::WordLibrary* m_library;

    QPalette m_defaultBtPe;

    QLabel* m_showSpellingLabel;
    QLabel* m_translationsLabel[5];

    QLabel* m_rightTipLabel;
    QLabel* m_wrongTipLabel;

    QListWidget* m_exampleSentencesListWidget;

    MyWorkPage_result(QWidget* parent, team::WordLibrary* library) :QWidget(parent)
    {
        m_library = library;
    }

    void set_work(MyVirtualWord* work)
    {

        set_tip(work->isCorrect);
        if (work)
        {
            auto word_task = m_library->read_utf8(work->optionWordIndices[work->correctOptionIndex]);
            m_showSpellingLabel->setText(QString::fromStdString(word_task.spelling()));
            for (int i = 0; i < 5; i++)
                m_translationsLabel[i]->clear();
            for (int i = 0; i < 5; i++)
            {
                if (i < word_task.sizeofTran())
                    m_translationsLabel[i]->setText(QString::fromStdString(word_task.translation(i)));
                else {
                    break;
                }
            }
            m_exampleSentencesListWidget->clear();
            for(int i = 0;i<word_task.sizeofSent();i++){
                QListWidgetItem *item_sentence_en = new QListWidgetItem(QString::fromStdString(word_task.sentence_en(i)));
                QListWidgetItem *item_sentence_cn = new QListWidgetItem(QString::fromStdString(word_task.sentence_cn(i)));
                item_sentence_cn->setBackground(QColor(235,235,235));
                m_exampleSentencesListWidget->addItem(item_sentence_en);
                m_exampleSentencesListWidget->addItem(item_sentence_cn);
            }

        }
    }

    void setup_comp()
    {
        m_showSpellingLabel = new QLabel(this);
        for (int i = 0; i < 5; i++)
            m_translationsLabel[i] = new QLabel(this);

        m_rightTipLabel = new QLabel(QString::fromLocal8Bit("选择正确"), this);
        m_wrongTipLabel = new QLabel(QString::fromLocal8Bit("选择错误"), this);
        m_rightTipLabel->hide();
        m_wrongTipLabel->hide();


         m_exampleSentencesListWidget = new QListWidget(this);
         //m_exampleSentencesListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    void setup_geo()
    {
        int fixed_width = parentWidget()->width() - 200;
        fixed_width = fixed_width > 720 ? 720 : fixed_width;
        int start_x = (parentWidget()->width() - fixed_width) / 2;
        int start_y = (parentWidget()->height() - 400) / 2;

        this->setGeometry(start_x, start_y, fixed_width, 400);

        m_rightTipLabel->setGeometry(0, 0, 100, 30);
        m_wrongTipLabel->setGeometry(0, 0, 100, 30);
        m_showSpellingLabel->setGeometry(0, 30, width(), 60);
        m_translationsLabel[0]->setGeometry(0, 90, width(), 25);
        m_translationsLabel[1]->setGeometry(0, 115, width(), 25);
        m_translationsLabel[2]->setGeometry(0, 140, width(), 25);
        m_translationsLabel[3]->setGeometry(0, 165, width(), 25);
        m_translationsLabel[4]->setGeometry(0, 190, width(), 25);

        m_exampleSentencesListWidget->setGeometry(0,215,width(),185);
        //m_exampleSentencesListWidget->setGeometry(0,0,width(),1500);

    }
    void setup_grap()
    {

        QPalette palette = this->palette();

        this->setAutoFillBackground(true);
        palette.setColor(QPalette::Window, QColor(100, 100, 100, 150));
        this->setPalette(palette);


        QFont font = this->font();
        font.setPixelSize(45);
        m_showSpellingLabel->setAutoFillBackground(true);
        m_showSpellingLabel->setFont(font);
        font.setPixelSize(16);
        for (int i = 0; i < 5; i++)
        {
            m_translationsLabel[i]->setFont(font);
            m_translationsLabel[i]->setAutoFillBackground(true);
        }


        palette.setColor(QPalette::Window, QColor(255, 255, 255, 225));
        m_showSpellingLabel->setPalette(palette);
        for (int i = 0; i < 5; i++)
            m_translationsLabel[i]->setPalette(palette);


        palette = m_rightTipLabel->palette();
        palette.setColor(QPalette::Window, QColor(0, 100, 0, 180));///
        palette.setColor(QPalette::Text, QColor(255, 255, 255, 255));///
        m_rightTipLabel->setPalette(palette);
        palette = m_wrongTipLabel->palette();
        palette.setColor(QPalette::Window, QColor(100, 0, 0, 180));
        palette.setColor(QPalette::Text, QColor(255, 255, 255, 255));
        m_wrongTipLabel->setPalette(palette);

        m_wrongTipLabel->setAutoFillBackground(true);
        m_rightTipLabel->setAutoFillBackground(true);

        m_wrongTipLabel->setAlignment(Qt::AlignCenter);
        m_rightTipLabel->setAlignment(Qt::AlignCenter);


        font = m_exampleSentencesListWidget->font();
        font.setPixelSize(14);
        m_exampleSentencesListWidget->setFont(font);

    }

    void setup_connect()
    {

    }

    void set_tip(bool flag)
    {
        if (flag)
        {
            m_rightTipLabel->show();
            m_wrongTipLabel->hide();
        }
        else {
            m_rightTipLabel->hide();
            m_wrongTipLabel->show();
        }
    }


};




class MyWorkPage :public QWidget
{
    Q_OBJECT

public:

    team::WordLibrary*   m_library;
    MyVirtualUserSystem* m_userSystem;

    QWidget*     m_startPageWidget;
    QTextEdit*   m_startPageNotesTextEdit;
    QPushButton* m_startTaskButton;

    QPushButton* m_toNextButton;
    QPushButton* m_toLastButton;

    MyTaskPage_option* m_optionPage;
    MyWorkPage_result* m_resultPage;
    bool m_on_work;

    MyWorkPage(QWidget* parent, team::WordLibrary* libary, MyVirtualUserSystem* userSystem) :QWidget(parent)
    {
        m_library = libary;
        m_userSystem = userSystem;

        m_on_work = false;
        m_state = STOP;
        m_currentTask = 0;
        m_on_showOption = false;
    }

private:

    std::vector<MyVirtualWord> m_Tasks;

    int m_currentTask;

    enum MyState
    {
        STOP = 0,
        RUN = 1,
    };

    MyState m_state;
    bool    m_on_showOption;

    void to_next_state()
    {
        if (m_state == RUN)
        {
            if (m_currentTask < m_Tasks.size()) {
                m_state = RUN;
                to_next_page();
            }
            else
            {

            }
        }

    }


    void to_start_page()
    {
        m_Tasks.clear();
        m_state = STOP;
        m_optionPage->hide();
        m_resultPage->hide();
        m_toNextButton->hide();
        m_toLastButton->hide();
        m_startPageWidget->show();
        m_currentTask = 0;
    }

    void to_last_page()
    {
        if (!(m_state == RUN))return;
        if (m_on_showOption)
        {
            if (m_currentTask <= 0)
            {

            }
            else {
                m_on_showOption = false;
                m_currentTask--;
                m_resultPage->set_work(&m_Tasks[m_currentTask]);
                m_optionPage->hide();
                m_resultPage->show();
            }
        }
        else {
            m_on_showOption = true;
            m_optionPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->show();
            m_resultPage->hide();
        }
        update_progressBar();
    }

    void to_next_page()
    {
        std::cout<<"to_next_page"<<std::endl;
        if (!(m_state == RUN))
        {
            to_start_page();
            return;
        }
        if (m_on_showOption)
        {
            m_on_showOption = false;
            if(!(m_currentTask<m_Tasks.size()))
                to_start_page();
            m_resultPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->hide();
            m_resultPage->show();
        }
        else {


            if (m_currentTask < m_Tasks.size()-1) {
                m_on_showOption = true;
                m_currentTask++;
                if(!m_currentTask<m_Tasks.size())
                {
                    to_start_page();
                }
                else
                {
                    m_optionPage->set_work(&m_Tasks[m_currentTask]);
                    m_optionPage->show();
                    m_resultPage->hide();
                }

            }
            else {

                if(m_userSystem->is_logined()){

                    m_userSystem->feedback_w(m_Tasks[m_Tasks.size()-1]);
                    auto word = m_userSystem->request_w();
                    if(word.isNull)
                    {
                        to_start_page();
                    }
                    else
                    {
                        m_Tasks.push_back(word);
                        m_on_showOption = true;
                        m_currentTask++;
                        m_optionPage->set_work(&m_Tasks[m_currentTask]);
                        m_optionPage->show();
                        m_resultPage->hide();
                    }


                    if(m_Tasks.size() == 0)
                    {
                         endup();
                         m_state = STOP;
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
            m_optionPage->m_progressProgressBar->setMinimum(0);

            if(AccountManageSystem::ar)
            {
                std::cout<<"count total = " <<AccountManageSystem::ar->wl_daily.count_total() <<std::endl;
                std::cout<<"count done  = " <<AccountManageSystem::ar->wl_daily.count_done() <<std::endl;

                m_optionPage->m_progressProgressBar->setMaximum(AccountManageSystem::ar->wl_daily.count_total());
                m_optionPage->m_progressProgressBar->setValue(AccountManageSystem::ar->wl_daily.count_done());
            }

        }
    }

    void restart()
    {

        if(!m_userSystem->is_logined())
        {
            std::cout<<"请登录一个用户..."<<std::endl;
            return;
            //未登录
        }
        else
        {
            auto word = m_userSystem->request_w();
            if(word.isNull)
            {
                std::cout<<"reques_w failed!"<<std::endl;
                if(AccountManageSystem::ar)
                {
                    std::cout<<"count total = " <<AccountManageSystem::ar->wl_daily.count_total() <<std::endl;
                    std::cout<<"count done  = " <<AccountManageSystem::ar->wl_daily.count_done() <<std::endl;
                }

                to_start_page();
            }
            else
            {
                m_Tasks.push_back(word);
                m_on_showOption = true;
                m_optionPage->set_work(&m_Tasks[0]);
                m_optionPage->show();
                m_resultPage->hide();
            }
        }

        if(m_Tasks.size()==0)
        {
            std::cout<<"没有任务..."<<std::endl;
            return ;
            //没有任务
        }
        else
        {
            m_startPageWidget->hide();
            m_toLastButton->show();
            m_toNextButton->show();

            m_currentTask = 0;
            m_optionPage->set_work(&m_Tasks[m_currentTask]);
            m_optionPage->show();
            m_resultPage->hide();
            m_on_showOption = true;
            m_state = RUN;
        }

    }

    void endup()
    {
        m_startPageWidget->show();
        m_optionPage->hide();
        m_resultPage->hide();
        m_toLastButton->hide();
        m_toNextButton->hide();
    }


    void setup_connect()
    {
        connect(m_startTaskButton, SIGNAL(clicked()), this, SLOT(slot_start()));
        connect(m_toLastButton, SIGNAL(clicked()), this, SLOT(slot_bt_to_last()));
        connect(m_toNextButton, SIGNAL(clicked()), this, SLOT(slot_bt_to_next()));
        connect(m_optionPage, SIGNAL(mysignal_optionSelected()), this, SLOT(slot_process_state()));

        m_optionPage->setup_connect();
        m_resultPage->setup_connect();
    }

    void setup_comp()
    {
        m_startPageWidget = new QWidget(this);
        m_startPageNotesTextEdit = new QTextEdit(m_startPageWidget);
        m_startPageNotesTextEdit->setText(QString("\n") +
            "                            FBI WARNING\n\n" +
            "   Federal Law provides severe civil and criminal penalties for\n" +
            "   the unauthorized reproduction, distribution, or exhibition of\n" +
            "   copyrighted motion pictures (Title 17, United States Code.\n" +
            "   Sections 501 and 508). The Federal Bureau of Investigation\n" +
            "   investigates allegations of criminal copyright infringement\n" +
            "           (Title 17, United States Code Section 506)."
        );
        m_startPageNotesTextEdit->setReadOnly(true);
        m_startTaskButton = new QPushButton(QString::fromLocal8Bit("开始任务"), m_startPageWidget);

        m_optionPage = new MyTaskPage_option(this, m_library);
        m_optionPage->setup_comp();
        m_resultPage = new MyWorkPage_result(this, m_library);
        m_resultPage->setup_comp();
        m_optionPage->hide();
        m_resultPage->hide();

        m_toNextButton = new QPushButton("next", this);
        m_toLastButton = new QPushButton("back", this);
        m_toLastButton->hide();
        m_toNextButton->hide();
    }

    void setup_geo()
    {
        this->setGeometry(0, 35, parentWidget()->width(), parentWidget()->height() - 35);
        m_startPageWidget->setGeometry(0, 0, this->width(), this->height());
        m_startPageNotesTextEdit->setGeometry((this->width() - 600) / 2, (this->height() - 380) / 2, 600, 380);
        m_startTaskButton->setGeometry((this->width() - 100) / 2, m_startPageNotesTextEdit->y() + m_startPageNotesTextEdit->height() + 20, 100, 30);

        m_toLastButton->setGeometry(0, 0, 100, height());
        m_toNextButton->setGeometry(width() - 100, 0, 100, height());

        m_optionPage->setup_geo();
        m_resultPage->setup_geo();
    }

    void setup_grap()
    {
        QFont font = m_startPageNotesTextEdit->font();
        QPalette palette = m_startPageNotesTextEdit->palette();
        font.setPixelSize(18);
        m_startPageNotesTextEdit->setFont(font);
        palette.setColor(QPalette::Base, QColor(10, 10, 10));
        palette.setColor(QPalette::Text, QColor(255, 0, 0));
        palette.setColor(QPalette::Highlight, QColor(50, 50, 50));
        palette.setColor(QPalette::HighlightedText, QColor(200, 200, 200));
        m_startPageNotesTextEdit->setPalette(palette);


        palette = m_toLastButton->palette();
        palette.setColor(QPalette::Button, QColor(200, 200, 200, 210));
        m_toLastButton->setAutoFillBackground(true);
        m_toNextButton->setAutoFillBackground(true);
        m_toLastButton->setFlat(true);
        m_toNextButton->setFlat(true);
        m_toLastButton->setPalette(palette);
        m_toNextButton->setPalette(palette);


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
