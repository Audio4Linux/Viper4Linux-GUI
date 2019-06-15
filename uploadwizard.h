#ifndef UPLOADWIZARD_H
#define UPLOADWIZARD_H

#include <QWizard>
#include <QUrlQuery>
namespace Ui {
class UploadWizard;
}

class UploadWizard : public QWizard
{
    Q_OBJECT

public:
    explicit UploadWizard(QWidget *parent = nullptr);
    ~UploadWizard();
private slots:
    void openOAuth();
    void openRepo();
    void openPRs();
    void openOAuthSettings();
    void UpdateList();
    void PageChanged(int);
    bool DoPR(QString);
    bool DoClone(QString);
    bool DoChanges(QString);
    bool DoPush(QString);
    void DoFork();
    bool CheckFork(QString);
    QString getRequest(QString);
private:
    Ui::UploadWizard *ui;
    QString forkurl="";
    bool ongoing_request = false;
    QString irsPathVar = "";
    QString irsPath = "";
};

#endif // UPLOADWIZARD_H
