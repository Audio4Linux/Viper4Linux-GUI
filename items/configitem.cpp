#include "configitem.h"
#include "ui_configitem.h"

configitem::configitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::configitem)
{
    ui->setupUi(this);
}

configitem::~configitem()
{
    delete ui;
}

void configitem::setData(QString title, QString desc){
    ui->title->setText(title);
    ui->desc->setText(desc);
}
