#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maskwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pMaskWidget = new MaskWidget();
    m_pMaskWidget->setParent(this);
    m_pMaskWidget->SetMainWidget(this);
    m_pMaskWidget->hide();

    m_pDlg = new QWidget(this);
    m_pDlg->setObjectName("m_pDlg");
    m_pDlg->setStyleSheet("QWidget#m_pDlg{background-color: #ffffff;border-radius: 6px;}");
    m_pMaskWidget->SetShowDialog(m_pDlg);
    m_pDlg->installEventFilter(m_pMaskWidget);

    QWidget* titleWidget = new QWidget;
    titleWidget->setObjectName("titleWidget");
    titleWidget->setStyleSheet("QWidget#titleWidget{background-color: #00ff00;}");
    titleWidget->setFixedHeight(50);

    QHBoxLayout* pTitleLyout = new QHBoxLayout(titleWidget);
    pTitleLyout->setContentsMargins(6, 0, 6, 0);
    QLabel* title = new QLabel;
    pTitleLyout->addWidget(title);

    pTitleLyout->addStretch();

    QPushButton* closeBtn = new QPushButton("x");
    closeBtn->setFixedSize(30, 30);
    pTitleLyout->addWidget(closeBtn);
    connect(closeBtn, &QPushButton::clicked, m_pDlg, &QWidget::hide);

    QVBoxLayout* vLyout = new QVBoxLayout(m_pDlg);
    vLyout->setSpacing(0);
    vLyout->setContentsMargins(18, 0, 18, 0);

    QWidget *body = new QWidget;
    body->setObjectName("body");
    body->setStyleSheet("QWidget#body{background-color: #ffff00;}");
    vLyout->addWidget(titleWidget);
    vLyout->addWidget(body);

    m_pDlg->setLayout(vLyout);
    m_pDlg->resize(300, 400);
    m_pDlg->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_pDlg->show();
}
