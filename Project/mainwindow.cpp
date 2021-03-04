#include "mainwindow.h"

#include "MyBaseDefine.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setup_ui();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setup_ui()
{


    setup_conf();

    setup_comp();

    setup_geo();

    setup_grap();

    setup_connect();


}

void MainWindow::setup_conf()
{
    bool flag = false;
    flag = m_configuration.load(MY_CONF_PATH);

    this->resize(QSize(m_configuration.get_windowWidth(), m_configuration.get_windowHeight()));

    if (!flag)
    {

    }
    else
    {

    }

    flag = m_library.link(MY_LIBRARY_PATH);///加载  !!
    if (!flag)
    {
        QMessageBox::critical(0, "Application Error!",
            QString("An internal error occurred!\n") +
            "Can't open \"" + MY_LIBRARY_PATH + "\"\n" +
            "Please contact our e-mail at " + MY_CONTACT_EMAIL + " and report\n" +
            "\n<Application> will now exit.");
        exit(0);
    }
    else
    {

    }
    if (!m_library.is_linked())
        exit(0);

    flag = this->m_mainWindow_backgroundImage.load(QString::fromStdString(m_configuration.get_backgroundImagePath()));
    if (!flag)
    {
        this->m_mainWindow_backgroundImage.load(":/res/background.png");
    }
    else
    {

    }

    m_userSystem.init();//初始化


}

void MainWindow::setup_comp()
{
    m_mainWindow_moveableArea = new MyWindowMoveArea(this, this);

    m_mainWindow_title = new QLabel(MY_VERSON, this);
    m_mainWindow_title->setAttribute(Qt::WA_TransparentForMouseEvents, true);//鼠标穿透

    m_mianWindow_mainmenuButton = new QPushButton(this);
    m_mianWindow_exitButton = new QPushButton(this);


    m_mainWindow_mainMenu = new QWidget(this);

    m_mainMenu_main = new QPushButton(m_mainWindow_mainMenu);
    m_mainMenu_work = new QPushButton(m_mainWindow_mainMenu);
    m_mainMenu_dict = new QPushButton(m_mainWindow_mainMenu);
    m_mainMenu_opti = new QPushButton(m_mainWindow_mainMenu);

    m_mainMenu_mainIcon = new QLabel(m_mainWindow_mainMenu);
    m_mainMenu_taskIcon = new QLabel(m_mainWindow_mainMenu);
    m_mainMenu_dictIcon = new QLabel(m_mainWindow_mainMenu);
    m_mainMenu_optiIcon = new QLabel(m_mainWindow_mainMenu);

    m_mainMenu_mainStr = new QLabel(QString::fromLocal8Bit(MY_STRING_0), m_mainWindow_mainMenu);
    m_mainMenu_taskStr = new QLabel(QString::fromLocal8Bit(MY_STRING_1), m_mainWindow_mainMenu);
    m_mainMenu_dictStr = new QLabel(QString::fromLocal8Bit(MY_STRING_2), m_mainWindow_mainMenu);
    m_mainMenu_optiStr = new QLabel(QString::fromLocal8Bit(MY_STRING_3), m_mainWindow_mainMenu);

    m_mainPage = new MyMainPage(this, &m_userSystem);
    m_taskPage = new MyWorkPage(this, &m_library, &m_userSystem);
    m_dictPage = new MyDictPage(this, &m_library);
    m_optiPage = new MyOptiPage(this, this, &m_configuration);

    m_mainPage->setup_comp();
    m_taskPage->setup_comp();
    m_dictPage->setup_comp();
    m_optiPage->setup_comp();

}

void MainWindow::setup_geo()
{

    //this->setMinimumSize(this->size());
    //this->setMaximumSize(this->size());

    int stick = 35;
    m_mainWindow_moveableArea->setGeometry(QRect(0, 0, this->width(), stick));
    m_mainWindow_title->setGeometry(58, 0, m_mainWindow_moveableArea->width() - 58 * 2, m_mainWindow_moveableArea->height());
    int button_width = 1.618f * stick;
    m_mianWindow_mainmenuButton->setGeometry(QRect(0, 0, button_width, stick));
    m_mianWindow_exitButton->setGeometry(QRect(this->width() - button_width, 0, button_width, stick));

    m_mainWindow_mainMenu->setGeometry(QRect(0, stick, this->width(), this->height() - stick));

    int mainmenu_btwidth = 140;
    int mainmenu_btheight = 230;

    int t_y = (m_mainWindow_mainMenu->height() - mainmenu_btheight) / 2;
    int t_sx = (m_mainWindow_mainMenu->width() - 80 * 3 - mainmenu_btwidth * 4) / 2;

    m_mainMenu_main->setGeometry(QRect(t_sx, t_y, mainmenu_btwidth, mainmenu_btheight));
    m_mainMenu_work->setGeometry(QRect(t_sx + 1 * (mainmenu_btwidth + 80), t_y, mainmenu_btwidth, mainmenu_btheight));
    m_mainMenu_dict->setGeometry(QRect(t_sx + 2 * (mainmenu_btwidth + 80), t_y, mainmenu_btwidth, mainmenu_btheight));
    m_mainMenu_opti->setGeometry(QRect(t_sx + 3 * (mainmenu_btwidth + 80), t_y, mainmenu_btwidth, mainmenu_btheight));

    m_mainMenu_mainIcon->setGeometry(QRect(m_mainMenu_main->x(), m_mainMenu_main->y(), mainmenu_btwidth, mainmenu_btwidth));
    m_mainMenu_taskIcon->setGeometry(QRect(m_mainMenu_work->x(), m_mainMenu_work->y(), mainmenu_btwidth, mainmenu_btwidth));
    m_mainMenu_dictIcon->setGeometry(QRect(m_mainMenu_dict->x(), m_mainMenu_dict->y(), mainmenu_btwidth, mainmenu_btwidth));
    m_mainMenu_optiIcon->setGeometry(QRect(m_mainMenu_opti->x(), m_mainMenu_opti->y(), mainmenu_btwidth, mainmenu_btwidth));

    m_mainMenu_mainStr->setGeometry(QRect(m_mainMenu_main->x(), m_mainMenu_main->y() + mainmenu_btwidth, mainmenu_btwidth, mainmenu_btheight - mainmenu_btwidth));
    m_mainMenu_taskStr->setGeometry(QRect(m_mainMenu_work->x(), m_mainMenu_work->y() + mainmenu_btwidth, mainmenu_btwidth, mainmenu_btheight - mainmenu_btwidth));
    m_mainMenu_dictStr->setGeometry(QRect(m_mainMenu_dict->x(), m_mainMenu_dict->y() + mainmenu_btwidth, mainmenu_btwidth, mainmenu_btheight - mainmenu_btwidth));
    m_mainMenu_optiStr->setGeometry(QRect(m_mainMenu_opti->x(), m_mainMenu_opti->y() + mainmenu_btwidth, mainmenu_btwidth, mainmenu_btheight - mainmenu_btwidth));


    m_mainPage->setup_geo();
    m_taskPage->setup_geo();
    m_dictPage->setup_geo();
    m_optiPage->setup_geo();


}

void MainWindow::setup_grap()
{
    setWindowFlags(Qt::FramelessWindowHint); // 去掉标题栏,去掉任务栏显示，窗口置顶

    setAutoFillBackground(true);
    set_mainWindow_background(this->m_mainWindow_backgroundImage);

    QPalette palette = this->palette();

    //标题栏
    m_mainWindow_moveableArea->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(255, 255, 255, 150)));
    m_mainWindow_moveableArea->setPalette(palette);

    QFont font = m_mainWindow_title->font();
    //font.setBold(true);
    font.setPixelSize(16);
    m_mainWindow_title->setFont(font);


    QIcon iocn;
    iocn.addFile(":/res/default_ui/res/bt_menu.png");/// !!
    m_mianWindow_mainmenuButton->setIcon(iocn);
    m_mianWindow_mainmenuButton->setIconSize(m_mianWindow_mainmenuButton->size());
    m_mianWindow_mainmenuButton->setFlat(true);
    iocn.addFile(":/res/default_ui/res/bt_exit.png");/// !!
    m_mianWindow_exitButton->setIcon(iocn);
    m_mianWindow_exitButton->setIconSize(m_mianWindow_exitButton->size());
    m_mianWindow_exitButton->setFlat(true);



    //主菜单
    m_mainWindow_mainMenu->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(30, 30, 30, 240)));
    m_mainWindow_mainMenu->setPalette(palette);

    m_mainMenu_mainStr->setAlignment(Qt::AlignCenter);
    m_mainMenu_taskStr->setAlignment(Qt::AlignCenter);
    m_mainMenu_dictStr->setAlignment(Qt::AlignCenter);
    m_mainMenu_optiStr->setAlignment(Qt::AlignCenter);

    m_mainMenu_mainIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_taskIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_dictIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_optiIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);//

    m_mainMenu_mainStr->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_taskStr->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_dictStr->setAttribute(Qt::WA_TransparentForMouseEvents, true);//
    m_mainMenu_optiStr->setAttribute(Qt::WA_TransparentForMouseEvents, true);//


    m_mainPage->setup_grap();
    m_taskPage->setup_grap();
    m_dictPage->setup_grap();
    m_optiPage->setup_grap();


    m_mainPage->show();
    m_taskPage->hide();
    m_dictPage->hide();
    m_optiPage->hide();
    m_mainWindow_mainMenu->raise();///
    show_mainMenu(false);

}


void MainWindow::setup_connect()
{
    connect(m_mianWindow_mainmenuButton, SIGNAL(clicked()), this, SLOT(slot_show_mainMenu()));
    connect(m_mianWindow_exitButton, SIGNAL(clicked()), this, SLOT(slot_exit_mainWindow()));

    connect(m_mainMenu_main, SIGNAL(clicked()), this, SLOT(slot_show_mainPage()));
    connect(m_mainMenu_work, SIGNAL(clicked()), this, SLOT(slot_show_taskPage()));
    connect(m_mainMenu_dict, SIGNAL(clicked()), this, SLOT(slot_show_dictPage()));
    connect(m_mainMenu_opti, SIGNAL(clicked()), this, SLOT(slot_show_optiPage()));

    m_mainPage->setup_connect();
    m_taskPage->setup_connect();
    m_dictPage->setup_connect();
    m_optiPage->setup_connect();
}

void MainWindow::set_mainWindow_background(QImage const & image)
{
    if (image.isNull())
        return;
    QRect res_rect;
    int res_width;
    int res_height;
    res_width = image.width();
    res_height = res_width * this->height() / this->width();
    int temp = image.height() - res_height;
    if (temp >= 0)
        res_rect = QRect(0, temp / 2, res_width, res_height);
    else {
        res_height = image.height();
        res_width = res_height * this->width() / this->height();
        temp = image.width() - res_width;
        if (temp >= 0)
            res_rect = QRect(temp / 2, 0, res_width, res_height);
        else
            res_rect = QRect(0, 0, image.width(), image.height());
    }

    QImage image_fixed = image.copy(res_rect).scaled(
        this->size(),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation
    );

    QPalette palette = this->palette();
    palette.setBrush(
        QPalette::Window,
        QBrush(image_fixed)
    );
    this->setPalette(palette);
}

void MainWindow::show_mainMenu(bool flag) { if (flag) { m_mainWindow_mainMenu_onShow = true; m_mainWindow_mainMenu->show(); } else { m_mainWindow_mainMenu_onShow = false; m_mainWindow_mainMenu->hide(); } }


void MainWindow::resizeEvent(QResizeEvent * e)
{
    setup_geo();
    set_mainWindow_background(this->m_mainWindow_backgroundImage);
}

void MainWindow::show_page(int id)
{
    if (id >= 0 && id < 4)
    {
        m_mainPage->hide();
        m_taskPage->hide();
        m_dictPage->hide();
        m_optiPage->hide();
        switch (id)
        {
        case 0: m_mainPage->show();
            break;
        case 1: m_taskPage->show();
            break;
        case 2: m_dictPage->show();
            break;
        case 3: m_optiPage->show();
            break;
        }
        show_mainMenu(false);
    }
}

void MainWindow::reconf_backgroundImage() {
    QImage image(QString::fromStdString(m_configuration.get_backgroundImagePath()));
    if (!image.isNull())
    {
        m_mainWindow_backgroundImage = image;
        set_mainWindow_background(m_mainWindow_backgroundImage);
        m_configuration.save();
    }
    else
    {
        m_configuration.set_backgroundImagePath(":/res/background.png");//
    }
}
