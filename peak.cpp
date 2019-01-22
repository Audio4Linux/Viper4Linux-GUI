#include "peak.h"
#include "ui_peak.h"

peak::peak(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::peak)
{
    ui->setupUi(this);
}

peak::~peak()
{
    delete ui;
}
