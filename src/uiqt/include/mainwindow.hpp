#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QSqlDatabase>

namespace uiqt {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNetworkRequest();
    void onDatabaseQuery();
    void showSettings();
    void applyTheme(const QString &theme);

private:
    void setupUi();
    void setupNetwork();
    void setupDatabase();
    void setupMenuBar();
    void setupStatusBar();
    void loadSettings();

    QNetworkAccessManager *networkManager;
    QSqlDatabase database;
};

} // namespace uiqt
