#pragma once

#include <QGuiApplication>
#include <QColor>
#include <QStyleHints>
#include <functional>
#include <memory>

namespace Ripes {
namespace Colors {

constexpr QColor BerkeleyBlue = {0x00, 0x32, 0x62};
constexpr QColor FoundersRock = {0x3B, 0x7E, 0xA1};
constexpr QColor CaliforniaGold = {0xFD, 0xB5, 0x15};
constexpr QColor Medalist = {0xC4, 0x82, 0x0E};
constexpr QColor FlatGreen = {0x4c, 0xde, 0x75};

enum SyntaxColor {
  Instruction,
  Register,
  Immediate,
  Label,
  String,
  Comment,
  RegisterValue,
  Highlight,
  InstructionHighlight
};

inline bool isDarkTheme() {
  return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
}

inline QColor getSyntaxColorLight(SyntaxColor c) {
  switch (c) {
      case SyntaxColor::Instruction:
      case SyntaxColor::RegisterValue:
        return QColor("#003262");
      case SyntaxColor::Register:
      case SyntaxColor::String:
        return QColor("#800000");
      case SyntaxColor::Immediate:
        return QColorConstants::DarkGreen;
      case SyntaxColor::Label:
        return QColor("#C4820E");
      case SyntaxColor::Highlight:
        return QColor("#FDB515");
      case SyntaxColor::InstructionHighlight:
        return QColorConstants::Red.lighter(120);
      default:
        return QColor("#3B7EA1");
  }
}

inline QColor getSyntaxColorDark(SyntaxColor c) {
  switch (c) {
      case SyntaxColor::Instruction:
      case SyntaxColor::RegisterValue:
        return QColor("#03A9F4");
      case SyntaxColor::Register:
      case SyntaxColor::String:
        return QColor("#EF5350");
      case SyntaxColor::Immediate:
        return QColorConstants::Green;
      case SyntaxColor::Label:
        return QColor("#eea830");
      case SyntaxColor::Highlight:
        return QColor("#671209");
      case SyntaxColor::InstructionHighlight:
        return QColorConstants::Red.lighter(120);
      default:
        return QColor("#3B7EA1");
  }
}

inline QColor getSyntaxColor(SyntaxColor c) {
  if (isDarkTheme()) {
    return getSyntaxColorDark(c);
  } else {
    return getSyntaxColorLight(c);
  }
}

// Returns a function which returns a lighter color on each instantiation
inline std::function<QColor()> incrementalColorGenerator(const QColor &start,
                                                         unsigned steps) {
  auto color = std::make_shared<QColor>(start);
  return [color = color, steps] {
    const int decRatio = 100 + 80 / steps;
    QColor oldColor = *color;
    *color = color->lighter(decRatio);
    return oldColor;
  };
}

inline std::function<QColor()> incrementalRedGenerator(unsigned steps) {
  return incrementalColorGenerator(QColorConstants::Red.lighter(120), steps);
}

}; // namespace Colors
} // namespace Ripes
