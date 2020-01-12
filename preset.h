#ifndef PRESET_H
#define PRESET_H

#include <QDialog>
#include <QNetworkReply>
#include "converter.h"
#include "items/configitem.h"
#include "items/delegates.h"
#include "config/appconfigwrapper.h"

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
    void reloadRepo();
    void repoIndexChanged();
    void importAndroid();
    void exportAndroid(converter::configtype cmode);
    void importLinux();
    void exportLinux();
    void download();
    void nameChanged(const QString&);
    void showContextMenu(const QPoint &pos);
    void indexDownloaded(QNetworkReply*);
    void visitGithub();
    void performIRSDownload(QNetworkReply* reply);
    void performDownload(QNetworkReply* reply);

private:
    Ui::Preset *ui;
    AppConfigWrapper *appconf;
    QStringList irs;
    QString optimizeName(const QString& s);
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QString toCamelCase(const QString& s);
    QString pathAppend(const QString& path1, const QString& path2);

};
class FileObject
{
public:
    const QString url;
    const QString name;
};
#endif // PRESET_H
