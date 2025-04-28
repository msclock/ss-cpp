#pragma once

#include <QComboBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

namespace uiqt {

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void saveSettings();

private:
    void setupUi();
    void loadSettings();

    QComboBox* themeComboBox;
    QLineEdit* apiEndpointEdit;
    QSpinBox* timeoutSpinBox;
    QLineEdit* databaseNameEdit;
};

} // namespace uiqt
