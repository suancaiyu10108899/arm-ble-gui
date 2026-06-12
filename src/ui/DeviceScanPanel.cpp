#include "DeviceScanPanel.h"
#include "BleScanner.h"
#include "BleConnection.h"
#include "StyleConstants.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

DeviceScanPanel::DeviceScanPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    updateButtonStates();
}

void DeviceScanPanel::setScanner(armble::BleScanner *scanner)
{
    m_scanner = scanner;
    if (m_scanner) {
        connect(m_scanner, &armble::BleScanner::deviceDiscovered,
                this, &DeviceScanPanel::onDeviceDiscovered);
        connect(m_scanner, &armble::BleScanner::scanFinished,
                this, &DeviceScanPanel::onScanFinished);
        connect(m_scanner, &armble::BleScanner::scanError,
                this, &DeviceScanPanel::onScanError);
    }
}

void DeviceScanPanel::setConnection(armble::BleConnection *connection)
{
    m_connection = connection;
    if (m_connection) {
        connect(m_connection, &armble::BleConnection::stateChanged,
                this, &DeviceScanPanel::onConnectionStateChanged);
    }
}

void DeviceScanPanel::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    // 标题
    auto *titleLabel = new QLabel(QStringLiteral("BLE 设备扫描"), this);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(titleLabel);

    // 状态标签
    m_statusLabel = new QLabel(QStringLiteral("⚪ 已断开"), this);
    m_statusLabel->setStyleSheet(
        QString("color: %1; padding: 4px;").arg(Style::kDisconnectedColor));
    mainLayout->addWidget(m_statusLabel);

    // 设备列表
    m_deviceList = new QListWidget(this);
    m_deviceList->setStyleSheet(Style::kPanelFrameStyle);
    mainLayout->addWidget(m_deviceList, 1);  // stretch=1 填满剩余空间

    // 按钮行
    auto *btnLayout = new QHBoxLayout();

    m_scanBtn = new QPushButton(QStringLiteral("🔍 扫描"), this);
    m_scanBtn->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_scanBtn, &QPushButton::clicked,
            this, &DeviceScanPanel::onScanClicked);
    btnLayout->addWidget(m_scanBtn);

    m_connectBtn = new QPushButton(QStringLiteral("连接"), this);
    m_connectBtn->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_connectBtn, &QPushButton::clicked,
            this, &DeviceScanPanel::onConnectClicked);
    btnLayout->addWidget(m_connectBtn);

    m_disconnectBtn = new QPushButton(QStringLiteral("断开"), this);
    m_disconnectBtn->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_disconnectBtn, &QPushButton::clicked,
            this, &DeviceScanPanel::onDisconnectClicked);
    btnLayout->addWidget(m_disconnectBtn);

    mainLayout->addLayout(btnLayout);
}

void DeviceScanPanel::onScanClicked()
{
    if (!m_scanner) return;
    if (m_scanner->isScanning()) {
        m_scanner->stopScan();
    } else {
        m_deviceList->clear();
        m_scanner->startScan();
        m_scanBtn->setText(QStringLiteral("⏹ 停止"));
        m_statusLabel->setText(QStringLiteral("🔵 扫描中..."));
        m_statusLabel->setStyleSheet(
            QString("color: %1; padding: 4px;").arg(Style::kScanningColor));
    }
}

void DeviceScanPanel::onConnectClicked()
{
    auto *item = m_deviceList->currentItem();
    if (!item) {
        QMessageBox::information(this, QStringLiteral("提示"),
                                 QStringLiteral("请先在设备列表中选择一个设备"));
        return;
    }
    // 从显示文本中提取 MAC 地址
    QString displayText = item->text();
    // 格式: "ARM-BLE (D3:52:88:3A:06:27)" 或 纯 MAC
    int start = displayText.indexOf('(');
    int end = displayText.indexOf(')');
    QString address = (start >= 0 && end > start)
        ? displayText.mid(start + 1, end - start - 1)
        : displayText;

    emit connectRequested(address);
}

void DeviceScanPanel::onDisconnectClicked()
{
    emit disconnectRequested();
}

void DeviceScanPanel::onDeviceDiscovered(const armble::BleDeviceInfo &info)
{
    m_deviceList->addItem(info.displayText());
}

void DeviceScanPanel::onScanFinished()
{
    m_scanBtn->setText(QStringLiteral("🔍 扫描"));
    updateButtonStates();
}

void DeviceScanPanel::onScanError(const QString &msg)
{
    m_statusLabel->setText(QStringLiteral("❌ %1").arg(msg));
    m_scanBtn->setText(QStringLiteral("🔍 扫描"));
    updateButtonStates();
}

void DeviceScanPanel::onConnectionStateChanged(int state)
{
    using State = armble::BleConnection::State;
    switch (static_cast<State>(state)) {
    case State::Disconnected:
        m_isConnected = false;
        m_statusLabel->setText(QStringLiteral("⚪ 已断开"));
        m_statusLabel->setStyleSheet(
            QString("color: %1; padding: 4px;").arg(Style::kDisconnectedColor));
        break;
    case State::Ready:
        m_isConnected = true;
        m_statusLabel->setText(QStringLiteral("🟢 已连接"));
        m_statusLabel->setStyleSheet(
            QString("color: %1; padding: 4px; font-weight: bold;").arg(Style::kConnectedColor));
        break;
    case State::Error:
        m_isConnected = false;
        m_statusLabel->setText(QStringLiteral("❌ 连接错误"));
        break;
    default:
        break;
    }
    updateButtonStates();
}

void DeviceScanPanel::updateButtonStates()
{
    const bool scanning = m_scanner && m_scanner->isScanning();
    m_scanBtn->setEnabled(!m_isConnected);
    m_connectBtn->setEnabled(!m_isConnected && !scanning && m_deviceList->currentItem() != nullptr);
    m_disconnectBtn->setEnabled(m_isConnected);
    m_deviceList->setEnabled(!m_isConnected);
}
