#include "LogPanel.h"
#include "StyleConstants.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

LogPanel::LogPanel(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    auto *title = new QLabel(QStringLiteral("接收日志"), this);
    title->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(title);

    m_logArea = new QPlainTextEdit(this);
    m_logArea->setReadOnly(true);
    m_logArea->setStyleSheet(Style::kLogAreaStyle);
    m_logArea->setMaximumBlockCount(1000);  // 限制最大行数防止内存泄漏
    layout->addWidget(m_logArea);
}

void LogPanel::appendLog(const QString &text, const QString &color)
{
    // 添加时间戳
    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString html = QStringLiteral(
        "<span style='color:gray;'>[%1]</span> "
        "<span style='color:%2;'>%3</span>")
        .arg(timestamp, color, text.toHtmlEscaped());

    m_logArea->appendHtml(html);

    // 自动滚动到底部
    auto cursor = m_logArea->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_logArea->setTextCursor(cursor);
}

void LogPanel::clearLog()
{
    m_logArea->clear();
}
