#include "modelchoose.h"
#include "ui_modelchoose.h"
#include<QComboBox>
#include<QDebug>
#include<QAbstractButton>
#include<QCheckBox>
ModelChoose::ModelChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelChoose)
{
        this->music=new QSound(":/new/prefix1/WaterDrop.wav");
    ui->setupUi(this);
    this->setWindowTitle(QStringLiteral("规则设置"));
    this->setWindowIcon(QIcon(":/new/prefix1/icon1.png"));
    this->setFixedSize(600,450);
    QStringList items;
    items.append("2");
    items.append("3");
    items.append("4");
    items.append("5");
    items.append("6");
    items.append("7");
    items.append("8");
    items.append("9");
    ui->comboBox->addItems(items);
    connect(ui->pushButton_ok,&QPushButton::clicked,this,[=](){
        QString str;
        str=ui->comboBox->currentText();
        qDebug()<<"str1="<<str;
        this->rangzi=str.toInt();
        qDebug()<<"rangzi="<<this->rangzi;
        str=ui->lineEdit->displayText();
        qDebug()<<"str1="<<str;
        this->tiemu=str.toDouble();
        qDebug()<<"tiemu"<<this->tiemu;
        this->isModelset=true;
        this->close();
            this->music->play();
    });
    connect(ui->pushButton_ok_2,&QPushButton::clicked,this,[=](){
        this->close();
        this->isModelset=true;
            this->music->play();
    });

}

ModelChoose::~ModelChoose()
{
    delete ui;
}


