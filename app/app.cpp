#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>
#include <Wt/WTextEdit.h>

#include "ui.hpp"
class RiscvOnline : public Wt::WContainerWidget {
public:
  RiscvOnline();

private:
  Wt::WTextArea *programEdit_;
  Wt::WText *greeting_;
};

RiscvOnline::RiscvOnline() : WContainerWidget() {
  UI* ui = this->addWidget(std::make_unique<UI>());
  ui->setMargin(20);
}

// int main(int argc, char **argv)
// {
//     return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
//       return std::make_unique<RiscvOnline>(env);
//     });
// }

std::unique_ptr<Wt::WApplication>
createApplication(const Wt::WEnvironment &env) {
  std::unique_ptr<Wt::WApplication> app =
      std::make_unique<Wt::WApplication>(env);
  app->messageResourceBundle().use(Wt::WApplication::appRoot() + "app");
  app->setTitle("R-V Online");

  app->root()->addWidget(std::make_unique<RiscvOnline>());

  Wt::WCssDecorationStyle langStyle;
  langStyle.font().setSize(Wt::FontSize::Smaller);
  langStyle.setCursor(Wt::Cursor::PointingHand);
  langStyle.setForegroundColor(Wt::WColor("blue"));
  langStyle.setTextDecoration(Wt::TextDecoration::Underline);
  app->styleSheet().addRule(".lang", langStyle);

  langStyle.setCursor(Wt::Cursor::Arrow);
  langStyle.font().setWeight(Wt::FontWeight::Bold);
  app->styleSheet().addRule(".langcurrent", langStyle);

  return app;
}

int main(int argc, char **argv) { return WRun(argc, argv, &createApplication); }
