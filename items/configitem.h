#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include <QWidget>

namespace Ui {
class configitem;
}

class configitem : public QWidget
{
    Q_OBJECT

public:
    explicit configitem(QWidget *parent = nullptr);
    ~configitem();
    void setData(QString title, QString desc);

private:
    Ui::configitem *ui;
};

#endif // CONFIGITEM_H
