#include "ui.hpp"

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WDateEdit.h>
#include <Wt/WSelectionBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WIntValidator.h>
#include <Wt/WLabel.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTableCell.h>
#include <Wt/WTextArea.h>
#include <Wt/WText.h>

UI::UI()
  : WTable()
{
  create();
}

void UI::create()
{
  Wt::WLabel *label;
  int row = 0;

  // Title
  elementAt(row, 0)->setColumnSpan(3);
  elementAt(row, 0)->setContentAlignment(Wt::AlignmentFlag::Top | Wt::AlignmentFlag::Center);
  elementAt(row, 0)->setPadding(10);
  Wt::WText *title = elementAt(row,0)->addWidget(std::make_unique<Wt::WText>(tr("example.form")));
  title->decorationStyle().font().setSize(Wt::FontSize::XLarge);

  // error messages
  ++row;
  elementAt(row, 0)->setColumnSpan(3);
  feedbackMessages_ = elementAt(row, 0);
  feedbackMessages_->setPadding(5);

  Wt::WCssDecorationStyle& errorStyle = feedbackMessages_->decorationStyle();
  errorStyle.setForegroundColor(Wt::WColor("red"));
  errorStyle.font().setSize(Wt::FontSize::Smaller);
  errorStyle.font().setWeight(Wt::FontWeight::Bold);
  errorStyle.font().setStyle(Wt::FontStyle::Italic);

  // Name
  ++row;
  nameEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WLineEdit>());
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.name")));
  label->setBuddy(nameEdit_);
  nameEdit_->setValidator(std::make_shared<Wt::WValidator>(true));
  nameEdit_->enterPressed().connect(this, &UI::submit);

  // First name
  ++row;
  firstNameEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WLineEdit>());
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.firstname")));
  label->setBuddy(firstNameEdit_);

  // Country
  ++row;
  countryEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WComboBox>());
  countryEdit_->addItem("");
  countryEdit_->addItem("Belgium");
  countryEdit_->addItem("Netherlands");
  countryEdit_->addItem("United Kingdom");
  countryEdit_->addItem("United States");
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.country")));
  label->setBuddy(countryEdit_);
  countryEdit_->setValidator(std::make_shared<Wt::WValidator>(true));
  countryEdit_->changed().connect(this, &UI::countryChanged);

  // City
  ++row;
  cityEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WComboBox>());
  cityEdit_->addItem(tr("example.choosecountry"));
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.city")));
  label->setBuddy(cityEdit_);

  // Birth date
  ++row;
  birthDateEdit_ = elementAt(row, 2)->addWidget(std::make_unique<Wt::WDateEdit>());
  birthDateEdit_->setBottom(Wt::WDate(1900, 1, 1));
  birthDateEdit_->setTop(Wt::WDate::currentDate());
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.birthdate")));
  label->setBuddy(birthDateEdit_);
  birthDateEdit_->setFormat("dd/MM/yyyy");
  birthDateEdit_->validator()->setMandatory(true);

  // Child count
  ++row;
  childCountEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WLineEdit>("0"));
  label = elementAt(row, 0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.childcount")));
  label->setBuddy(childCountEdit_);
  childCountEdit_->setValidator(std::make_shared<Wt::WIntValidator>(0,30));
  childCountEdit_->validator()->setMandatory(true);

  ++row;
  remarksEdit_ = elementAt(row,2)->addWidget(std::make_unique<Wt::WTextArea>());
  remarksEdit_->setColumns(40);
  remarksEdit_->setRows(5);
  label = elementAt(row,0)->addWidget(std::make_unique<Wt::WLabel>(tr("example.remarks")));
  label->setBuddy(remarksEdit_);

  // Submit
  ++row;
  Wt::WPushButton *submit = elementAt(row,0)->addWidget(std::make_unique<Wt::WPushButton>(tr("submit")));
  submit->clicked().connect(this, &UI::submit);
  submit->setMargin(15, Wt::Side::Top);
  elementAt(row, 0)->setColumnSpan(3);
  elementAt(row, 0)->setContentAlignment(Wt::AlignmentFlag::Top | Wt::AlignmentFlag::Center);

  // Set column widths for label and validation icon
  elementAt(2, 0)->resize(Wt::WLength(30, Wt::LengthUnit::FontEx), Wt::WLength::Auto);
  elementAt(2, 1)->resize(20, Wt::WLength::Auto);
}

void UI::countryChanged()
{
  cityEdit_->clear();
  cityEdit_->addItem("");
  cityEdit_->setCurrentIndex(-1);

  switch (countryEdit_->currentIndex()) {
  case 0:
    break;
  case 1:
    cityEdit_->addItem("Antwerp");
    cityEdit_->addItem("Brussels");
    cityEdit_->addItem("Oekene");
    break;
  case 2:
    cityEdit_->addItem("Amsterdam");
    cityEdit_->addItem("Den Haag");
    cityEdit_->addItem("Rotterdam");
    break;
  case 3:
    cityEdit_->addItem("London");
    cityEdit_->addItem("Bristol");
    cityEdit_->addItem("Oxford");
    cityEdit_->addItem("Stonehenge");
    break;
  case 4:
    cityEdit_->addItem("Boston");
    cityEdit_->addItem("Chicago");
    cityEdit_->addItem("Los Angeles");
    cityEdit_->addItem("New York");
    break;
  }
}

bool UI::checkValid(Wt::WFormWidget *edit, const Wt::WString& text)
{
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

bool UI::validate()
{
  feedbackMessages_->clear();
  bool valid = true;

  if (!checkValid(nameEdit_, tr("error.name")))
    valid = false;
  if (!checkValid(countryEdit_, tr("error.country")))
    valid = false;
  if (!checkValid(birthDateEdit_, tr("error.birthdate")))
    valid = false;
  if (!checkValid(childCountEdit_, tr("error.childcount")))
    valid = false;

  return valid;
}

void UI::submit()
{
  if (validate()) {
    // do something useful with the data...
    Wt::WString name = Wt::WString("{1} {2}")
        .arg(firstNameEdit_->text())
        .arg(nameEdit_->text());

    Wt::WString remarks
      = remarksEdit_->text();

    clear();

    elementAt(0,0)->addWidget(std::make_unique<Wt::WText>(
                                Wt::WString("<p>Thank you, {1}, "
                               "for all this precious data.</p>").arg(name)));

    if (!remarks.empty())
      elementAt(0,0)->addWidget(std::make_unique<Wt::WText>(
                                  Wt::WString("<p>You had some remarks. Splendid !</p>")));

    wApp->quit();
  }
}
