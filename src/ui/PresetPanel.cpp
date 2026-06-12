#include "PresetPanel.h"
#include "StyleConstants.h"

#include <QVBoxLayout>
#include <QLabel>

PresetPanel::PresetPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    auto *title = new QLabel(QStringLiteral("预设指令"), this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);

    m_btn0 = new QPushButton(QStringLiteral("舵机 0°"), this);
    m_btn0->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_btn0, &QPushButton::clicked, this, [this]() {
        emit presetCommand(QStringLiteral("0"));
    });
    layout->addWidget(m_btn0);

    m_btn90 = new QPushButton(QStringLiteral("舵机 90°"), this);
    m_btn90->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_btn90, &QPushButton::clicked, this, [this]() {
        emit presetCommand(QStringLiteral("90"));
    });
    layout->addWidget(m_btn90);

    m_btn180 = new QPushButton(QStringLiteral("舵机 180°"), this);
    m_btn180->setStyleSheet(Style::kPresetButtonStyle);
    connect(m_btn180, &QPushButton::clicked, this, [this]() {
        emit presetCommand(QStringLiteral("180"));
    });
    layout->addWidget(m_btn180);

    m_btnStop = new QPushButton(QStringLiteral("⛔ 紧急停止"), this);
    m_btnStop->setStyleSheet(
        "QPushButton { padding: 10px 20px; font-size: 14px; "
        "background-color: #c62828; color: white; font-weight: bold; }");
    connect(m_btnStop, &QPushButton::clicked, this, [this]() {
        emit presetCommand(QStringLiteral("STOP"));
    });
    layout->addWidget(m_btnStop);

    layout->addStretch();
}
