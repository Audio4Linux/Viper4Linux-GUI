#include "preset.h"
#include "ui_preset.h"

Preset::Preset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preset)
{
    ui->setupUi(this);
}

Preset::~Preset()
{
    delete ui;
}
