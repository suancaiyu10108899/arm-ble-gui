#pragma once

#include <QWidget>
#include <QPlainTextEdit>

/**
 * @brief 接收日志面板
 *
 * 显示所有来自 BLE 设备的数据（RX 特征通知）。
 * 只读、自动滚动、等宽字体。
 */
class LogPanel : public QWidget
{
    Q_OBJECT

public:
    explicit LogPanel(QWidget *parent = nullptr);

    /// 追加一条日志
    void appendLog(const QString &text, const QString &color = "#333");

    /// 清空日志
    void clearLog();

private:
    QPlainTextEdit *m_logArea = nullptr;
};
