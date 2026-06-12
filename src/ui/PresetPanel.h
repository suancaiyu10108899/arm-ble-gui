#pragma once

#include <QWidget>
#include <QPushButton>

/**
 * @brief 预设指令面板
 *
 * 提供一键发送预定义指令的按钮：
 * - 舵机0° / 90° / 180°（占位指令，遥控器协议到手后修改）
 * - 紧急停止
 */
class PresetPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PresetPanel(QWidget *parent = nullptr);

signals:
    /// 用户点击预设按钮，发出指令文本
    void presetCommand(const QString &command);

private:
    QPushButton *m_btn0    = nullptr;
    QPushButton *m_btn90   = nullptr;
    QPushButton *m_btn180  = nullptr;
    QPushButton *m_btnStop = nullptr;
};
