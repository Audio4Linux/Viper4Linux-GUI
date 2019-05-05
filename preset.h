#ifndef PRESET_H
#define PRESET_H

#include <QDialog>

namespace Ui {
class Preset;
}

class Preset : public QDialog
{
    Q_OBJECT

public:
    explicit Preset(QWidget *parent = nullptr);
    ~Preset();

private:
    Ui::Preset *ui;
};

#endif // PRESET_H
