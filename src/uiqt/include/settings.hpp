#pragma once

#include <QSettings>
#include <QString>
#include <memory>

namespace uiqt {

class Settings {
public:
    static Settings& instance();

    QString getTheme() const;
    void setTheme(const QString& theme);

    QString getApiEndpoint() const;
    void setApiEndpoint(const QString& endpoint);

    QString getDatabaseName() const;
    void setDatabaseName(const QString& name);

    int getNetworkTimeout() const;
    void setNetworkTimeout(int timeout);

    void sync();

private:
    Settings();
    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    std::unique_ptr<QSettings> m_settings;
};

} // namespace uiqt
