
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
	m_filterContainer = new QWidget(m_filterWidget);
	m_filterLayout = new QVBoxLayout(m_filterContainer);

	//视觉
	m_visualFilter = new QPushButton(QString::fromLocal8Bit(" 视觉"), m_filterWidget);
	m_filterLayout->addWidget(m_visualFilter);

	m_filterLayout->addStretch();
	//


	m_settingWidgetBackground = new QWidget(this);



	m_visualSettingWidget = new QWidget(m_settingWidgetBackground);
	m_visualSettingLabelWidget = new QWidget(m_visualSettingWidget);
	m_visualSettingCompWidget = new QWidget(m_visualSettingWidget);
	//1
	m_label_sizeOption = new QLabel(QString::fromLocal8Bit("分辨率"), m_visualSettingLabelWidget);
	m_button_sizeOption = new QComboBox(m_visualSettingCompWidget);
	m_button_sizeOption->addItem("1920x1080");
	m_button_sizeOption->addItem("1600x900");
	m_button_sizeOption->addItem("1440x900");
	m_button_sizeOption->addItem("1366x768");
	m_button_sizeOption->addItem("1280x720");
	//2
	m_label_backgroundPath = new QLabel(QString::fromLocal8Bit("背景图片"), m_visualSettingLabelWidget);
	m_label_path = new QLabel(QString::fromStdString(m_conf->get_backgroundImagePath()), m_visualSettingCompWidget);
    m_label_path->setTextInteractionFlags(Qt::TextSelectableByMouse);
	m_button_getPath = new QPushButton(QString::fromLocal8Bit("从本地文件选择"), m_visualSettingCompWidget);

}

void MyOptiPage::setup_geo()
{
	this->setGeometry(0, 35, parentWidget()->width(), parentWidget()->height() - 35);

	m_filterWidget->setGeometry(0, 0, 300, this->height());
	m_filterLabel->setGeometry(0, 0, m_filterWidget->width(), 20);//0 + 20
	m_filterContainer->setGeometry(QRect(0, 20, m_filterWidget->width(), m_filterWidget->height() - 20));


	m_settingWidgetBackground->setGeometry(300, 0, this->width() - 300, this->height());


	m_visualSettingWidget->setGeometry(0, 0, m_settingWidgetBackground->width(), m_settingWidgetBackground->height());
	m_visualSettingLabelWidget->setGeometry(20, 20, m_visualSettingWidget->width() / 3, m_settingWidgetBackground->height() - 20);
	m_visualSettingCompWidget->setGeometry(m_visualSettingWidget->width() / 2, 20, m_visualSettingWidget->width() / 2 - 20, m_visualSettingWidget->height() - 20);

	//1
	m_label_sizeOption->setGeometry(0, 0, m_visualSettingLabelWidget->width(), 30);
	m_button_sizeOption->setGeometry(0, 0, m_visualSettingCompWidget->width(), 30);
	//2
	m_label_backgroundPath->setGeometry(0, 30, m_visualSettingLabelWidget->width(), 30);
	m_label_path->setGeometry(0, 30, m_visualSettingCompWidget->width() * 2 / 3, 30);
	m_button_getPath->setGeometry(m_label_path->width(), 30, m_visualSettingCompWidget->width() / 3, 30);
}

void MyOptiPage::setup_connect()
{
	connect(m_button_sizeOption, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_change_window_size()));
	connect(m_button_getPath, SIGNAL(clicked()), this, SLOT(slot_select_path()));
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


	m_visualFilter->setFlat(true);
	palette = m_label_sizeOption->palette();
	palette.setColor(QPalette::WindowText, QColor(255, 255, 255));
	palette.setColor(QPalette::Window, QColor(255, 255, 255, 50));
	m_label_sizeOption->setPalette(palette);//1
	m_label_backgroundPath->setPalette(palette);//2
	m_label_path->setPalette(palette);//2

	m_label_sizeOption->setAutoFillBackground(true);
	m_label_backgroundPath->setAutoFillBackground(true);
	m_label_path->setAutoFillBackground(true);

	palette = m_settingWidgetBackground->palette();
	m_settingWidgetBackground->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window, QBrush(QColor(50,50,50,180)));
	m_settingWidgetBackground->setPalette(palette);

}

//public slots:

void MyOptiPage::slot_change_window_size()
{
	switch (m_button_sizeOption->currentIndex())
	{
	case 0:
		m_windows->resize(1920, 1080);
		break;
	case 1:
		m_windows->resize(1600, 900);
		break;
	case 2:
		m_windows->resize(1440, 900);
		break;
	case 3:
		m_windows->resize(1366, 768);
		break;
	case 4:
		m_windows->resize(1280, 720);
		break;
	}
	m_conf->set_windowWidth(m_windows->width());
	m_conf->set_windowHeight(m_windows->height());
    m_conf->save();
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
		m_label_path->setText(QString::fromStdString(m_conf->get_backgroundImagePath()));
	}
}

//public slots:

