#pragma once

#include <QString>

/// 全局 UI 样式常量（延续 MindPath 的 StyleConstants 习惯）
namespace Style {

// ── 颜色 ──
inline const QString kPanelBackground  = QStringLiteral("#f5f5f5");
inline const QString kConnectedColor   = QStringLiteral("#2e7d32");  // 绿色-已连接
inline const QString kDisconnectedColor = QStringLiteral("#c62828"); // 红色-已断开
inline const QString kScanningColor    = QStringLiteral("#1565c0");  // 蓝色-扫描中

// ── 面板通用样式 ──
inline const QString kPanelFrameStyle =
    "QFrame { background: #ffffff; border: 1px solid #ddd; border-radius: 4px; }";

// ── 按钮预设大小 ──
inline const QString kPresetButtonStyle =
    "QPushButton { padding: 10px 20px; font-size: 14px; }";

// ── 日志区域 ──
inline const QString kLogAreaStyle =
    "QPlainTextEdit { font-family: 'Consolas', 'Courier New', monospace; font-size: 12px; }";

// ── 界面尺寸 ──
inline constexpr int kWindowWidth  = 900;
inline constexpr int kWindowHeight = 650;
inline constexpr int kLeftPanelWidth = 300;
inline constexpr int kRightPanelWidth = 560;

} // namespace Style
