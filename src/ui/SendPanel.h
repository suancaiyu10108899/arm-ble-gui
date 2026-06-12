#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace armble {
class BleConnection;
}

/**
 * @brief 发送面板 — 手动输入并发送数据
 *
 * 功能：
 * - 文本输入框
 * - 发送按钮（回车也可触发）
 * - 发送历史（在 LogPanel 中显示）
 */
class SendPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SendPanel(QWidget *parent = nullptr);

    /// 依赖注入连接对象
    void setConnection(armble::BleConnection *connection);

signals:
    /// 用户请求发送文本（由 MainWindow 转发到 BleConnection）
    void sendRequested(const QString &text);

private slots:
    void onSendClicked();

private:
    QLineEdit   *m_inputEdit = nullptr;
    QPushButton *m_sendBtn   = nullptr;
    armble::BleConnection *m_connection = nullptr;
};
