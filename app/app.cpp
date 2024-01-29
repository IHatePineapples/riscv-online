#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>
#include <Wt/WTextEdit.h>

#include "emulator.hpp"

using namespace emulator;

class UI : public Wt::WTable {
public:
  /*!\brief Instantiate a new form.
   */
  UI(void (*)(UI *));
  // void (*runf)(UI*);
  void (*on_run_)(UI *);

private:
  /*!\brief The user selected a new country: adjust the cities combo box.
   */
  void countryChanged();

  /*!\brief Submit the form.
   */
  void submit();

  void create();

  Wt::WContainerWidget *feedbackMessages_;

  Wt::WTextArea *assemblyEdit_;

  /*!\brief Add a validation feedback for a field
   */
  void addValidationStatus(int row, Wt::WFormWidget *field);

  /*!\brief Validate the form, and return whether succesfull.
   */
  bool validate();

  /*!\brief Validate a single form field.
   *
   * Checks the given field, and appends the given text to the error
   * messages on problems.
   */
  bool checkValid(Wt::WFormWidget *edit, const Wt::WString &text);
};

UI::UI(void (*f)(UI *)) : on_run_(f), WTable() { create(); }

void UI::create() {
  Wt::WLabel *label;
  int row = 0;

  // Title
  elementAt(row, 0)->setColumnSpan(3);
  elementAt(row, 0)->setContentAlignment(Wt::AlignmentFlag::Top |
                                         Wt::AlignmentFlag::Center);
  elementAt(row, 0)->setPadding(10);
  Wt::WText *title = elementAt(row, 0)->addWidget(
      std::make_unique<Wt::WText>("RISC-V Online"));
  title->decorationStyle().font().setSize(Wt::FontSize::XLarge);

  // error messages
  ++row;
  elementAt(row, 0)->setColumnSpan(3);
  feedbackMessages_ = elementAt(row, 0);
  feedbackMessages_->setPadding(5);

  Wt::WCssDecorationStyle &errorStyle = feedbackMessages_->decorationStyle();
  errorStyle.setForegroundColor(Wt::WColor("red"));
  errorStyle.font().setSize(Wt::FontSize::Smaller);
  errorStyle.font().setWeight(Wt::FontWeight::Bold);
  errorStyle.font().setStyle(Wt::FontStyle::Italic);

  ++row;
  assemblyEdit_ =
      elementAt(row, 2)->addWidget(std::make_unique<Wt::WTextArea>());
  assemblyEdit_->setColumns(40);
  assemblyEdit_->setRows(5);
  label = elementAt(row, 0)->addWidget(
      std::make_unique<Wt::WLabel>("Assembly"));
  label->setBuddy(assemblyEdit_);

  // Submit
  ++row;
  Wt::WPushButton *submit = elementAt(row, 0)->addWidget(
      std::make_unique<Wt::WPushButton>("submit"));
  submit->clicked().connect(this, &UI::submit);
  submit->setMargin(15, Wt::Side::Top);
  elementAt(row, 0)->setColumnSpan(3);
  elementAt(row, 0)->setContentAlignment(Wt::AlignmentFlag::Top |
                                         Wt::AlignmentFlag::Center);

  // Set column widths for label and validation icon
  elementAt(2, 0)->resize(Wt::WLength(30, Wt::LengthUnit::FontEx),
                          Wt::WLength::Auto);
  elementAt(2, 1)->resize(20, Wt::WLength::Auto);
}

bool UI::checkValid(Wt::WFormWidget *edit, const Wt::WString &text) {
  if (edit->validate() != Wt::ValidationState::Valid) {
    feedbackMessages_->addWidget(std::make_unique<Wt::WText>(text));
    feedbackMessages_->addWidget(std::make_unique<Wt::WBreak>());
    edit->label()->decorationStyle().setForegroundColor(Wt::WColor("red"));
    edit->setStyleClass("Wt-invalid");
    return false;
  } else {
    edit->label()->decorationStyle().setForegroundColor(Wt::WColor());
    edit->setStyleClass("");

    return true;
  }
}

bool UI::validate() {
  feedbackMessages_->clear();
  bool valid = true;

  return valid;
}

void UI::submit() {
  if (validate()) {
    // do something useful with the data...
    // on_run(this);

    Wt::WString remarks = assemblyEdit_->text();
    on_run_(this);

    if (!remarks.empty()) {
      elementAt(0, 0)->addWidget(std::make_unique<Wt::WText>(
          Wt::WString("<p>You had some remarks. Splendid !</p>")));
    }

    // wApp->quit();
  }
}

class RiscvOnline : public Wt::WContainerWidget {
public:
  RiscvOnline();
  emulator::Emulator emu;

private:
  Wt::WTextArea *programEdit_;
};

static void UIrun(UI *ui){
    // emu.run();
    // update UI
    /* ui.update()  */
};

RiscvOnline::RiscvOnline() : WContainerWidget() {
  UI *ui = this->addWidget(std::make_unique<UI>(&UIrun));
  ui->setMargin(20);
}

std::unique_ptr<Wt::WApplication>
createApplication(const Wt::WEnvironment &env) {
  std::unique_ptr<Wt::WApplication> app =
      std::make_unique<Wt::WApplication>(env);
  app->messageResourceBundle().use(Wt::WApplication::appRoot() + "app");
  app->setTitle("Risc-V OL");

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
