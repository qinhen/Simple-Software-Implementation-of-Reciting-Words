
#include "mainwindow.h"

#include "MyPageOpti.h"

MyOptiPage::MyOptiPage(QWidget * parent, MainWindow * window, MyConfiguration * conf) :QWidget(parent)
{
    m_windows = window;
    m_conf = conf;
}

void MyOptiPage::setup_comp()
{

    m_filterWidget = new QWidget(this);
    m_filterLabel = new QLabel(QString::fromLocal8Bit(" 设置"), m_filterWidget);
    m_filterContainerWidget = new QWidget(m_filterWidget);
    m_filterLayout = new QVBoxLayout(m_filterContainerWidget);

    //视觉
    m_visualFilterButton = new QPushButton(QString::fromLocal8Bit(" 视觉"), m_filterWidget);
    m_filterLayout->addWidget(m_visualFilterButton);

    m_filterLayout->addStretch();
    //

    m_settingAreaBackgroundWidget = new QWidget(this);

    m_visualSettingWidget = new QWidget(m_settingAreaBackgroundWidget);
    m_visualSettingLabelWidget = new QWidget(m_visualSettingWidget);
    m_visualSettingCompWidget = new QWidget(m_visualSettingWidget);
    //1
    m_sizeOptionLabel = new QLabel(QString::fromLocal8Bit("分辨率"), m_visualSettingLabelWidget);
    m_sizeOptionBox = new QComboBox(m_visualSettingCompWidget);
    m_sizeOptionBox->addItem(QString::fromLocal8Bit("自定义"));
    m_sizeOptionBox->addItem("1920x1080");
    m_sizeOptionBox->addItem("1600x900");
    m_sizeOptionBox->addItem("1440x900");
    m_sizeOptionBox->addItem("1366x768");
    m_sizeOptionBox->addItem("1280x720");
    m_sizeOptionBox->addItem("1024x768");
    m_sizeOptionButton = new QPushButton(QString::fromLocal8Bit("自定义"), m_visualSettingCompWidget);
    //2
    m_backgroundPathLabel = new QLabel(QString::fromLocal8Bit("背景图片"), m_visualSettingLabelWidget);
    m_pathLabel = new QLabel(QString::fromStdString(m_conf->get_backgroundImagePath()), m_visualSettingCompWidget);
    m_pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_getPathButton = new QPushButton(QString::fromLocal8Bit("从本地文件选择"), m_visualSettingCompWidget);

}

void MyOptiPage::setup_geo()
{
    this->setGeometry(0, 35, parentWidget()->width(), parentWidget()->height() - 35);

    m_filterWidget->setGeometry(0, 0, 300, this->height());
    m_filterLabel->setGeometry(0, 0, m_filterWidget->width(), 20);//0 + 20
    m_filterContainerWidget->setGeometry(QRect(0, 20, m_filterWidget->width(), m_filterWidget->height() - 20));

    m_settingAreaBackgroundWidget->setGeometry(300, 0, this->width() - 300, this->height());

    m_visualSettingWidget->setGeometry(0, 0, m_settingAreaBackgroundWidget->width(), m_settingAreaBackgroundWidget->height());
    m_visualSettingLabelWidget->setGeometry(20, 20, m_visualSettingWidget->width() / 3, m_settingAreaBackgroundWidget->height() - 20);
    m_visualSettingCompWidget->setGeometry(m_visualSettingWidget->width() / 2, 20, m_visualSettingWidget->width() / 2 - 20, m_visualSettingWidget->height() - 20);

    //1
    m_sizeOptionLabel->setGeometry(0, 0, m_visualSettingLabelWidget->width(), 30);
    m_sizeOptionBox->setGeometry(0, 0, m_visualSettingCompWidget->width() * 2 / 3, 30);
    m_sizeOptionButton->setGeometry(m_sizeOptionBox->width(), 0, m_visualSettingCompWidget->width() / 3, 30);
    //2
    m_backgroundPathLabel->setGeometry(0, 30, m_visualSettingLabelWidget->width(), 30);
    m_pathLabel->setGeometry(0, 30, m_visualSettingCompWidget->width() * 2 / 3, 30);
    m_getPathButton->setGeometry(m_pathLabel->width(), 30, m_visualSettingCompWidget->width() / 3, 30);
}

void MyOptiPage::setup_connect()
{
    connect(m_sizeOptionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_change_window_size()));
    connect(m_sizeOptionButton, SIGNAL(clicked()), this, SLOT(slot_diy_window_size()));
    connect(m_getPathButton, SIGNAL(clicked()), this, SLOT(slot_select_path()));
}

void MyOptiPage::setup_grap()
{
    QPalette palette = this->palette();
    QFont font = this->font();

    palette = m_filterWidget->palette();
    m_filterWidget->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(255, 255, 255, 235)));
    m_filterWidget->setPalette(palette);


    palette = m_filterLabel->palette();
    m_filterLabel->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(200, 200, 200, 235)));
    m_filterLabel->setPalette(palette);


    m_visualFilterButton->setFlat(true);
    palette = m_sizeOptionLabel->palette();
    palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
    palette.setColor(QPalette::Window, QColor(255, 255, 255, 50));
    m_sizeOptionLabel->setPalette(palette);//1
    m_backgroundPathLabel->setPalette(palette);//2
    m_pathLabel->setPalette(palette);//2

    m_sizeOptionLabel->setAutoFillBackground(true);
    m_backgroundPathLabel->setAutoFillBackground(true);
    m_pathLabel->setAutoFillBackground(true);

    palette = m_settingAreaBackgroundWidget->palette();
    m_settingAreaBackgroundWidget->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(50,50,50,180)));
    m_settingAreaBackgroundWidget->setPalette(palette);

}

//public slots:

void MyOptiPage::slot_change_window_size()
{
    switch (m_sizeOptionBox->currentIndex())
    {
    case 0:
    {

        break;
    }

    case 1:
        m_windows->resize(1920, 1080);
        break;
    case 2:
        m_windows->resize(1600, 900);
        break;
    case 3:
        m_windows->resize(1440, 900);
        break;
    case 4:
        m_windows->resize(1366, 768);
        break;
    case 5:
        m_windows->resize(1280, 720);
        break;
    case 6:
        m_windows->resize(1024, 768);
        break;
    }
    m_conf->set_windowWidth(m_windows->width());
    m_conf->set_windowHeight(m_windows->height());
    m_conf->save();
}

void MyOptiPage::slot_diy_window_size()
{

    bool ok = FALSE;
    QString gotten_text = QInputDialog::getText(
        this,
        QString::fromLocal8Bit("输入一个分辨率"),
        "eg. 1280x720 ",
        QLineEdit::Normal, "", &ok);
    if (ok && !gotten_text.isEmpty())// 用户输入一些东西并且按下OK
    {
        auto lower_text = gotten_text.toLower();
        int index_x = -1;
        for (int i = 0; i < lower_text.size(); i++)
        {
            if (lower_text[i] == 'x')
            {
                index_x = i;
                break;
            }
        }

        if (index_x < 0)
            return;

        int gotton_width_str_size = index_x - 1;
        int gotton_height_str_size = lower_text.size() - index_x - 1;
        if (gotton_width_str_size <= 0 || gotton_height_str_size <= 0)
            return;

        auto gotton_width_str = lower_text.left(gotton_width_str_size);
        auto gotton_height_str = lower_text.left(gotton_height_str_size);

        auto gotton_width = gotton_width_str.toInt(&ok, 10);
        if (!ok)
            return;
        auto gotton_height = gotton_height_str.toInt(&ok, 10);
        if (!ok)
            return;

        gotton_width = gotton_width < 600 ? 600 : gotton_width;
        gotton_height = gotton_height < 400 ? 400 : gotton_height;

        m_windows->resize(gotton_width, gotton_height);

        m_sizeOptionBox->setCurrentIndex(0);
    }
    else // 用户不输入任何东西或者按下Cancel
    {

    }

}


void MyOptiPage::slot_select_path() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Select an image file for background."),
        "F:",
        tr("Image File(*png *jpg *bmp *jpeg *tga)"
        ));
    if (!fileName.isNull()) {
        m_conf->set_backgroundImagePath(fileName.toStdString());
        m_windows->reconf_backgroundImage();
        m_pathLabel->setText(QString::fromStdString(m_conf->get_backgroundImagePath()));
    }
}

//public slots:

