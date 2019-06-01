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
    void UpdateList();
    ~Preset();
private slots:
    void reject();
    void add();
    void remove();
    void load();
    void import();
private:
    Ui::Preset *ui;
    QString pathAppend(const QString& path1, const QString& path2);

};

#endif // PRESET_H
