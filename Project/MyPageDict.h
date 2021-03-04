#pragma once
#ifndef PAGE_DICT_H
#define PAGE_DICT_H
#include <iostream>

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
#include <QtWidgets/QCompleter>
#include <QtCore/QStringListModel>

#include "core/WordLibrary.h"


class MyDictPage:public QWidget
{
    Q_OBJECT
public:


    QLineEdit*        m_wordInput;
    QCompleter*       m_wordInputCompleter;
    QStringListModel* m_wordInputModel;
    QHBoxLayout*      m_mainLayout;

    int m_thickness;

    QWidget* m_searchWidget;
    QWidget* m_resultWidget;

    QLabel*      m_show_spellingLabel;
    QListWidget* m_show_exampleSentencesListWidget;
    QLabel*      m_show_translationsLabel[5];
    QLabel*      m_show_pronounciationsLabel[2];
    QLabel*      m_show_pronounceStylesLabel[2];


    team::WordLibrary* m_library;
    team::Word         m_wordData;

    MyDictPage(QWidget* parent, team::WordLibrary* library):QWidget(parent)
    {
        m_thickness = 35;
        m_library = library;
    }


    void setup_comp();
    void setup_geo();
    void setup_grap();
    void setup_connect();


    void reset_geo(QRect windowRect){setup_geo();}


private slots:
    void slot_search()
    {
        m_wordData = m_library->read_utf8(m_wordInput->text().toStdString());

        slot_comlete();

        if(!m_wordData.is_null())
            update_word_to_ui();
        else
            clear_ui();
    }

    void slot_comlete()
    {

        if(m_wordInput->text().isEmpty())
            return;

        int first_index = 0;

        if(!m_wordData.is_null())
        {
            first_index = m_library->index_at( m_wordData.spelling() );
            std::cout << "size  of "<< m_wordData.spelling() <<" = "<<m_wordData.spelling().size() <<std::endl;
            std::cout << "index of "<< m_wordData.spelling() <<" = "<<first_index <<std::endl;
        }
        else
        {
            first_index = m_library->fuzzy_query(m_wordInput->text().toStdString());
            std::cout << "first_index = "<<first_index <<std::endl;
        }


        std::cout << "first_index = "<<std::endl;

        if(!(first_index<0))
        {

            QStringList strlist;
            for (int i = 0; i < 10; i++) {
                std::string spe = m_library->read_spelling_at(first_index + i);
                if(spe.empty())
                    break;
                else
                    strlist.push_back( QString::fromStdString(spe)) ;

                std::cout<<first_index + i<<" = "<<spe<<std::endl;
            }
            m_wordInputModel->setStringList(strlist);
            m_wordInputCompleter->widget()->show();
        }

    }


private:
    void clear_ui()
    {
        m_show_spellingLabel->clear();
        m_show_exampleSentencesListWidget->clear();

        m_show_translationsLabel[0]->clear();
        m_show_translationsLabel[1]->clear();
        m_show_translationsLabel[2]->clear();
        m_show_translationsLabel[3]->clear();
        m_show_translationsLabel[4]->clear();
        m_show_pronounciationsLabel[0]->clear();
        m_show_pronounciationsLabel[1]->clear();
    }


    void update_word_to_ui()
    {
        m_show_spellingLabel->setText(QString::fromStdString(m_wordData.spelling()));
        m_show_exampleSentencesListWidget->clear();

        m_show_translationsLabel[0]->clear();
        m_show_translationsLabel[1]->clear();
        m_show_translationsLabel[2]->clear();
        m_show_translationsLabel[3]->clear();
        m_show_translationsLabel[4]->clear();
        m_show_pronounciationsLabel[0]->clear();
        m_show_pronounciationsLabel[1]->clear();

        for(int i =0;i<5;i++)
        {
            if(i>=m_wordData.sizeofTran())
                break;
            m_show_translationsLabel[i]->setText(QString::fromStdString(m_wordData.translation(i)));
        }

        if(m_wordData.pronounciationEnStyle().size()>0)
        {
            m_show_pronounciationsLabel[0]->setText(QString::fromStdString(m_wordData.pronounciationEnStyle()));
            m_show_pronounciationsLabel[1]->setText(QString::fromStdString(m_wordData.pronounciationUsStyle()));
        }

        for(int i = 0;i<m_wordData.sizeofSent();i++){
            QListWidgetItem *it_sent_en = new QListWidgetItem(QString::fromStdString(m_wordData.sentence_en(i)));
            QListWidgetItem *it_sent_cn = new QListWidgetItem(QString::fromStdString(m_wordData.sentence_cn(i)));
            it_sent_en->setBackground(QColor(50,50,50,50));
            m_show_exampleSentencesListWidget->addItem(it_sent_en);
            m_show_exampleSentencesListWidget->addItem(it_sent_cn);
        }

    }
};


inline void MyDictPage::setup_comp()
{

    m_searchWidget = new QWidget(this);
    m_resultWidget = new QWidget(this);

    m_show_exampleSentencesListWidget = new QListWidget(m_resultWidget);
    //水平滚动条不可见，只能通过鼠标滑动
    m_show_exampleSentencesListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //m_resultDisplayArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);


    m_wordInput = new QLineEdit(m_searchWidget);
    m_wordInput->setPlaceholderText(QString::fromLocal8Bit("请输入拼写"));
    m_wordInputCompleter = new QCompleter;
    m_wordInputCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_wordInput->setCompleter(m_wordInputCompleter);
    m_wordInputModel = new QStringListModel(m_wordInput);
    m_wordInputCompleter->setModel(m_wordInputModel);


    m_show_spellingLabel = new QLabel(m_resultWidget);

    m_show_translationsLabel[0] = new QLabel(m_resultWidget);
    m_show_translationsLabel[1] = new QLabel(m_resultWidget);
    m_show_translationsLabel[2] = new QLabel(m_resultWidget);
    m_show_translationsLabel[3] = new QLabel(m_resultWidget);
    m_show_translationsLabel[4] = new QLabel(m_resultWidget);
    m_show_pronounciationsLabel[0] = new QLabel(m_resultWidget);
    m_show_pronounciationsLabel[1] = new QLabel(m_resultWidget);
    m_show_pronounceStylesLabel[0] = new QLabel(QString::fromLocal8Bit("英式："),m_resultWidget);
    m_show_pronounceStylesLabel[1] = new QLabel(QString::fromLocal8Bit("美式："),m_resultWidget);

    m_show_translationsLabel[0]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_translationsLabel[1]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_translationsLabel[2]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_translationsLabel[3]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_translationsLabel[4]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_pronounciationsLabel[0]->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_show_pronounciationsLabel[1]->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

inline void MyDictPage::setup_geo()
{
    this->setGeometry(QRect(0,m_thickness,parentWidget()->width(),parentWidget()->height()-m_thickness));///

    m_searchWidget->setGeometry(QRect(0,0,300,height()));///

    m_wordInput->setGeometry(10,10,m_searchWidget->width()-20,35);///

    m_resultWidget->setGeometry(QRect(m_searchWidget->width(),0,width() - m_searchWidget->width(),height()));///

    // show geo
    m_show_spellingLabel->setGeometry(QRect(20,20,m_resultWidget->width()*3/5,70));///

    m_show_exampleSentencesListWidget->setGeometry(QRect(0,200,m_resultWidget->width(),m_resultWidget->height()-200));///


    int trans_label_width = m_resultWidget->width() - 40;

    m_show_translationsLabel[0]->setGeometry(20,98,trans_label_width,20);
    m_show_translationsLabel[1]->setGeometry(20,115,trans_label_width,20);
    m_show_translationsLabel[2]->setGeometry(20,135,trans_label_width,20);
    m_show_translationsLabel[3]->setGeometry(20,155,trans_label_width,20);
    m_show_translationsLabel[4]->setGeometry(20,175,trans_label_width,20);

    m_show_pronounceStylesLabel[0]->setGeometry(40+m_show_spellingLabel->width(),20,40,35);
    m_show_pronounceStylesLabel[1]->setGeometry(40+m_show_spellingLabel->width(),55,40,35);

    m_show_pronounciationsLabel[0]->setGeometry(80+m_show_spellingLabel->width(),20,m_resultWidget->width()-100-m_show_spellingLabel->width(),35);
    m_show_pronounciationsLabel[1]->setGeometry(m_show_pronounciationsLabel[0]->x(),m_show_pronounciationsLabel[0]->y()+35,m_show_pronounciationsLabel[0]->width(),35);

}


inline void MyDictPage::setup_grap()
{
    QFont font = m_wordInput->font();
    font.setPixelSize(14);
    m_wordInput->setFont(font);
    m_wordInput->setTextMargins(4,0,0,0);
    m_wordInput->setStyleSheet("height:35px;border:1px solid #eaeaea;border-radius:14px;");
    m_wordInput->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z]+$")));

    m_searchWidget->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(QColor(75,75,75,150)));
    m_searchWidget->setPalette(palette);


    m_resultWidget->setAutoFillBackground(true);
    palette = m_resultWidget->palette();
    palette.setBrush(QPalette::Window,QBrush(QColor(255,255,255,225)));//
    m_resultWidget->setPalette(palette);

    //m_resultDisplayArea->setStyleSheet("background-color:transparent");
    m_show_exampleSentencesListWidget->setStyleSheet("background-color:transparent");


    font = m_show_spellingLabel->font();
    font.setPixelSize(50);
    font.setBold(true);
    m_show_spellingLabel->setFont(font);
    //m_show_spellingLabel->setAlignment(Qt::AlignCenter);

    m_show_spellingLabel->setAutoFillBackground(true);
    palette =  m_show_spellingLabel->palette();
    palette.setBrush(QPalette::Window,QBrush(QColor(50,50,50,50)));
    m_show_spellingLabel->setPalette(palette);


    font = m_show_pronounciationsLabel[0]->font();
    font.setPixelSize(20);
    font.setBold(true);
    m_show_pronounciationsLabel[0]->setFont(font);
    m_show_pronounciationsLabel[1]->setFont(font);


    font = m_show_exampleSentencesListWidget->font();
    font.setPixelSize(16);
    m_show_exampleSentencesListWidget->setFont(font);
}

inline void MyDictPage::setup_connect()
{
    connect(m_wordInput,SIGNAL(textChanged(QString)),this,SLOT(slot_search()));
    //connect(m_wordInput,SIGNAL(returnPressed()),this,SLOT(slot_comple()));
}



#endif // PAGE_DICT_H
