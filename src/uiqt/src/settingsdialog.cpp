#include "settingsdialog.hpp"
#include "settings.hpp"

#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace uiqt {

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent) {
    setupUi();
    loadSettings();
}

void SettingsDialog::setupUi() {
    setWindowTitle(tr("Settings"));

    auto* layout = new QVBoxLayout(this);
    auto* formLayout = new QFormLayout;

    // Theme selection
    themeComboBox = new QComboBox(this);
    themeComboBox->addItem(tr("Light"), "light");
    themeComboBox->addItem(tr("Dark"), "dark");
    formLayout->addRow(tr("Theme:"), themeComboBox);

    // API endpoint
    apiEndpointEdit = new QLineEdit(this);
    formLayout->addRow(tr("API Endpoint:"), apiEndpointEdit);

    // Network timeout
    timeoutSpinBox = new QSpinBox(this);
    timeoutSpinBox->setRange(1000, 30000);
    timeoutSpinBox->setSingleStep(1000);
    timeoutSpinBox->setSuffix(tr(" ms"));
    formLayout->addRow(tr("Network Timeout:"), timeoutSpinBox);

    // Database name
    databaseNameEdit = new QLineEdit(this);
    formLayout->addRow(tr("Database Name:"), databaseNameEdit);

    layout->addLayout(formLayout);

    // Buttons
    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    layout->addWidget(buttonBox);
}

void SettingsDialog::loadSettings() {
    auto& settings = Settings::instance();

    // Set current theme
    int themeIndex = themeComboBox->findData(settings.getTheme());
    themeComboBox->setCurrentIndex(themeIndex);

    apiEndpointEdit->setText(settings.getApiEndpoint());
    timeoutSpinBox->setValue(settings.getNetworkTimeout());
    databaseNameEdit->setText(settings.getDatabaseName());
}

void SettingsDialog::saveSettings() {
    auto& settings = Settings::instance();

    settings.setTheme(themeComboBox->currentData().toString());
    settings.setApiEndpoint(apiEndpointEdit->text());
    settings.setNetworkTimeout(timeoutSpinBox->value());
    settings.setDatabaseName(databaseNameEdit->text());

    settings.sync();
    accept();
}

} // namespace uiqt
