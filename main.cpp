#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QMessageBox>
#include <QResource>
#include <QTimer>
#include <iostream>
#include <QPalette>
#include <QStyleFactory>

#include "src/cli/clioptions.h"
#include "src/cli/clirunner.h"
#include "src/mainwindow.h"
#include "colors.h"

using namespace std;

void initParser(QCommandLineParser &parser, Ripes::CLIModeOptions &options) {
  QString helpText =
      "The command line interface allows for assembling/compiling/executing a "
      "\n"
      "program on an arbitrary processor model and subsequent reporting of \n"
      "execution telemetry.\nCommand line mode is enabled when the '--mode "
      "cli' argument is provided.";

  helpText.prepend("Ripes command line interface.\n");
  parser.setApplicationDescription(helpText);
  QCommandLineOption modeOption("mode", "Ripes mode [gui, cli]", "mode", "gui");
  parser.addOption(modeOption);
  Ripes::addCLIOptions(parser, options);
}

enum CommandLineParseResult {
  CommandLineError,
  CommandLineHelpRequested,
  CommandLineGUI,
  CommandLineCLI
};

CommandLineParseResult parseCommandLine(QCommandLineParser &parser,
                                        QString &errorMessage) {
  const QCommandLineOption helpOption = parser.addHelpOption();
  if (!parser.parse(QCoreApplication::arguments())) {
    errorMessage = parser.errorText();
    return CommandLineError;
  }

  if (parser.isSet(helpOption))
    return CommandLineHelpRequested;

  if (parser.value("mode") == "gui")
    return CommandLineGUI;
  else if (parser.value("mode") == "cli")
    return CommandLineCLI;
  else {
    errorMessage = "Invalid mode: " + parser.value("mode");
    return CommandLineError;
  }
}

void printPaletteColors(const QPalette& palette) {
    QList<QPalette::ColorRole> roles = {
        QPalette::Window,
        QPalette::WindowText,
        QPalette::Base,
        QPalette::AlternateBase,
        QPalette::ToolTipBase,
        QPalette::ToolTipText,
        QPalette::Text,
        QPalette::Button,
        QPalette::ButtonText,
        QPalette::BrightText,
        QPalette::Highlight,
        QPalette::HighlightedText,
        QPalette::Link,
        QPalette::LinkVisited,
        QPalette::PlaceholderText
    };

    QStringList roleNames = {
        "Window",
        "WindowText",
        "Base",
        "AlternateBase",
        "ToolTipBase",
        "ToolTipText",
        "Text",
        "Button",
        "ButtonText",
        "BrightText",
        "Highlight",
        "HighlightedText",
        "Link",
        "LinkVisited",
        "PlaceholderText"
    };

    for (int i = 0; i < roles.size(); ++i) {
        QColor color = palette.color(QPalette::Active, roles[i]);
        qDebug() << roleNames[i] << ": " << color.name();
    }
}

void setForceDarkTheme(QApplication& app) {
  printPaletteColors(app.palette());

  QPalette darkPalette;

  darkPalette.setColor(QPalette::Window, QColor("#202326"));
  darkPalette.setColor(QPalette::WindowText, QColor("#fcfcfc"));
  darkPalette.setColor(QPalette::Base, QColor("#141618"));
  darkPalette.setColor(QPalette::AlternateBase, QColor("#1d1f22"));
  darkPalette.setColor(QPalette::ToolTipBase, QColor("#292c30"));
  darkPalette.setColor(QPalette::ToolTipText, QColor("#fcfcfc"));
  darkPalette.setColor(QPalette::Text, QColor("#fcfcfc"));
  darkPalette.setColor(QPalette::Button, QColor("#292c30"));
  darkPalette.setColor(QPalette::ButtonText, QColor("#fcfcfc"));
  darkPalette.setColor(QPalette::BrightText, QColor("#ffffff"));
  darkPalette.setColor(QPalette::Highlight, QColor("#3daee9"));
  darkPalette.setColor(QPalette::HighlightedText, QColor("#fcfcfc"));
  darkPalette.setColor(QPalette::Link, QColor("#1d99f3"));
  darkPalette.setColor(QPalette::LinkVisited, QColor("#9b59b6"));
  darkPalette.setColor(QPalette::PlaceholderText, QColor("#a1a9b1"));

  app.setStyle(QStyleFactory::create("Fusion"));
  app.setPalette(darkPalette);
}

int guiMode(QApplication &app) {
  Ripes::MainWindow m;

#ifdef Q_OS_WASM
  // In the WASM build, we'll just want a full-screen application that can't be
  // dragged or resized by the user.
  m.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
#endif

  // The following sequence of events manages to successfully start the
  // application as maximized, with the processor at a reasonable size. This has
  // been found to be specially a problem on windows.
  m.resize(800, 600);
  m.showMaximized();
  m.setWindowState(Qt::WindowMaximized);
  QTimer::singleShot(100, &m, [&m] { m.fitToView(); });

#ifdef Q_OS_WINDOWS
  if (QSysInfo::productVersion() == "10" && Ripes::Colors::isDarkTheme()) setForceDarkTheme(app);
#endif

  return app.exec();
}

int CLIMode(QCommandLineParser &parser, Ripes::CLIModeOptions &options) {
  QString err;
  if (!Ripes::parseCLIOptions(parser, err, options)) {
    std::cerr << "ERROR: " << err.toStdString() << std::endl;
    parser.showHelp();
    return 0;
  }
  return Ripes::CLIRunner(options).run();
}

int main(int argc, char **argv) {
  Q_INIT_RESOURCE(icons);
  Q_INIT_RESOURCE(examples);
  Q_INIT_RESOURCE(layouts);
  Q_INIT_RESOURCE(fonts);

  QApplication app(argc, argv);
  QCoreApplication::setApplicationName("Ripes");

  QCommandLineParser parser;
  Ripes::CLIModeOptions options;
  initParser(parser, options);
  QString err;
  switch (parseCommandLine(parser, err)) {
  case CommandLineError:
    std::cerr << "ERROR: " << err.toStdString() << std::endl;
    parser.showHelp();
    return 0;
  case CommandLineHelpRequested:
    parser.showHelp();
    return 0;
  case CommandLineGUI:
    return guiMode(app);
  case CommandLineCLI:
    return CLIMode(parser, options);
  }
}
