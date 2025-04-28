#include "mainwindow.hpp"
#include "settings.hpp"
#include "settingsdialog.hpp"

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>

namespace uiqt {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), networkManager(nullptr) {
    setupUi();
    setupMenuBar();
    setupStatusBar();
    setupNetwork();
    setupDatabase();
    loadSettings();
}

MainWindow::~MainWindow() {
    if (database.isOpen()) {
        database.close();
    }
}

void MainWindow::setupUi() {
    setWindowTitle(tr("Desktop Qt Demo"));
    resize(800, 600);

    auto* centralWidget = new QWidget(this);
    auto* layout = new QVBoxLayout(centralWidget);

    auto* networkButton = new QPushButton(tr("Test Network"), centralWidget);
    connect(networkButton, &QPushButton::clicked, this, &MainWindow::onNetworkRequest);

    auto* dbButton = new QPushButton(tr("Test Database"), centralWidget);
    connect(dbButton, &QPushButton::clicked, this, &MainWindow::onDatabaseQuery);

    layout->addWidget(networkButton);
    layout->addWidget(dbButton);
    layout->addStretch();

    setCentralWidget(centralWidget);
}

void MainWindow::setupMenuBar() {
    auto* menuBar = this->menuBar();

    // File menu
    auto* fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Settings"), this, &MainWindow::showSettings);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupNetwork() {
    networkManager = new QNetworkAccessManager(this);
}

void MainWindow::setupDatabase() {
    auto& settings = Settings::instance();
    database = QSqlDatabase::addDatabase(tr("QSQLITE"));
    database.setDatabaseName(settings.getDatabaseName());

    if (!database.open()) {
        qWarning()<<"Failed to open database: " << database.lastError().text().toStdString();
        QMessageBox::warning(this, tr("Database Error"), tr("Could not open database"));
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS demo (id INTEGER PRIMARY KEY, name TEXT)");
    query.exec("INSERT INTO demo (name) VALUES ('Test Item')");
}

void MainWindow::loadSettings() {
    auto& settings = Settings::instance();
    applyTheme(settings.getTheme());
}

void MainWindow::showSettings() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        loadSettings();
    }
}

void MainWindow::applyTheme(const QString& theme) {
    if (theme == "dark") {
        // 设置深色主题
        qApp->setStyleSheet(R"(
            QMainWindow { background-color: #2b2b2b; color: #ffffff; }
            QPushButton {
                background-color: #3d3d3d;
                color: #ffffff;
                border: none;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover { background-color: #4a4a4a; }
            QPushButton:pressed { background-color: #2d2d2d; }
        )");
    }
    else {
        // 设置浅色主题
        qApp->setStyleSheet(R"(
            QMainWindow { background-color: #f5f5f5; color: #000000; }
            QPushButton {
                background-color: #4a90e2;
                color: white;
                border: none;
                padding: 8px 16px;
                border-radius: 4px;
            }
            QPushButton:hover { background-color: #357abd; }
            QPushButton:pressed { background-color: #2a5f96; }
        )");
    }
}

void MainWindow::onNetworkRequest() {
    auto& settings = Settings::instance();
    QNetworkRequest request(QUrl(settings.getApiEndpoint()));
    request.setTransferTimeout(settings.getNetworkTimeout());

    statusBar()->showMessage(tr("Sending network request..."));
    auto* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString response = QString::fromUtf8(reply->readAll());
            QMessageBox::information(this, tr("Network Response"), response);
            statusBar()->showMessage(tr("Network request completed"), 3000);
        }
        else {
            qWarning()<<"Network error: "<< reply->errorString().toStdString();
            QMessageBox::warning(this, tr("Network Error"), reply->errorString());
            statusBar()->showMessage(tr("Network request failed"), 3000);
        }
        reply->deleteLater();
    });
}

void MainWindow::onDatabaseQuery() {
    statusBar()->showMessage(tr("Querying database..."));
    QSqlQuery query;
    query.exec("SELECT * FROM demo");

    QString result;
    while (query.next()) {
        result += QString("ID: %1, Name: %2\n").arg(query.value(0).toInt()).arg(query.value(1).toString());
    }

    QMessageBox::information(this, tr("Database Query Result"), result);
    statusBar()->showMessage(tr("Database query completed"), 3000);
}

} // namespace uiqt
