#include "SendPanel.h"
#include "BleConnection.h"
#include "StyleConstants.h"

#include <QHBoxLayout>

SendPanel::SendPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);

    m_inputEdit = new QLineEdit(this);
    m_inputEdit->setPlaceholderText(QStringLiteral("输入要发送的数据..."));
    m_inputEdit->setStyleSheet("padding: 6px; font-size: 13px;");
    // 回车发送
    connect(m_inputEdit, &QLineEdit::returnPressed,
            this, &SendPanel::onSendClicked);
    layout->addWidget(m_inputEdit, 1);

    m_sendBtn = new QPushButton(QStringLiteral("发送"), this);
    m_sendBtn->setStyleSheet(Style::kPresetButtonStyle);
    m_sendBtn->setEnabled(false);
    connect(m_sendBtn, &QPushButton::clicked,
            this, &SendPanel::onSendClicked);
    layout->addWidget(m_sendBtn);
}

void SendPanel::setConnection(armble::BleConnection *connection)
{
    m_connection = connection;
    m_sendBtn->setEnabled(m_connection != nullptr);
    if (m_connection) {
        connect(m_connection, &armble::BleConnection::stateChanged,
                this, [this](int state) {
                    using State = armble::BleConnection::State;
                    bool ready = (static_cast<State>(state) == State::Ready);
                    m_sendBtn->setEnabled(ready);
                });
    }
}

void SendPanel::onSendClicked()
{
    const QString text = m_inputEdit->text().trimmed();
    if (text.isEmpty()) return;

    emit sendRequested(text);
    m_inputEdit->clear();
}
