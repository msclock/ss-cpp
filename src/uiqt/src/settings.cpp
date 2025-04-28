#include "settings.hpp"
#include <QCoreApplication>

namespace uiqt {

Settings& Settings::instance() {
    static Settings instance;
    return instance;
}

Settings::Settings()
    : m_settings(
          std::make_unique<QSettings>(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat)) {
    // 设置默认值
    if (!m_settings->contains("theme")) {
        m_settings->setValue("theme", "light");
    }
    if (!m_settings->contains("network/api_endpoint")) {
        m_settings->setValue("network/api_endpoint", "https://api.github.com/zen");
    }
    if (!m_settings->contains("network/timeout")) {
        m_settings->setValue("network/timeout", 5000);
    }
    if (!m_settings->contains("database/name")) {
        m_settings->setValue("database/name", ":memory:");
    }
}

QString Settings::getTheme() const {
    return m_settings->value("theme").toString();
}

void Settings::setTheme(const QString& theme) {
    m_settings->setValue("theme", theme);
}

QString Settings::getApiEndpoint() const {
    return m_settings->value("network/api_endpoint").toString();
}

void Settings::setApiEndpoint(const QString& endpoint) {
    m_settings->setValue("network/api_endpoint", endpoint);
}

QString Settings::getDatabaseName() const {
    return m_settings->value("database/name").toString();
}

void Settings::setDatabaseName(const QString& name) {
    m_settings->setValue("database/name", name);
}

int Settings::getNetworkTimeout() const {
    return m_settings->value("network/timeout").toInt();
}

void Settings::setNetworkTimeout(int timeout) {
    m_settings->setValue("network/timeout", timeout);
}

void Settings::sync() {
    m_settings->sync();
}

} // namespace uiqt
