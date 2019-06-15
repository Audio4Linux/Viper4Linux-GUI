#ifndef PRESET_H
#define PRESET_H

#include <QDialog>
#include <QNetworkReply>
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
    void upload();
    void import();
    void download();
    void nameChanged(QString);
    void showContextMenu(const QPoint &pos);
    void indexDownloaded(QNetworkReply*);
    void visitGithub();
    void performIRSDownload(QNetworkReply* reply);
    void performDownload(QNetworkReply* reply);
private:
    Ui::Preset *ui;
    QStringList irs;
    QString optimizeName(QString s);
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
