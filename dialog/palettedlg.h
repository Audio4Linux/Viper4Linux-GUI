#ifndef PALETTE_H
#define PALETTE_H

#include <QDialog>
#include <config/appconfigwrapper.h>

namespace Ui {
class palette;
}

class PaletteEditor : public QDialog
{
    Q_OBJECT

public:
    explicit PaletteEditor(AppConfigWrapper *_appconf, QWidget *parent = nullptr);
    ~PaletteEditor();

private:
    Ui::palette *ui;
    AppConfigWrapper *appconf;
private slots:
    int loadColor(int index,int rgb_index);
    void closeWin();
    void Reset();
    void updateBase();
    void updateBack();
    void updateFore();
    void updateIcons();
    void updateSelection();
    void updateDisabled();
    void saveColor(int index,const QColor& color);
};

#endif // PALETTE_H
