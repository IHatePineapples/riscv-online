// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bv, Herent, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#pragma once

#include <Wt/WTable.h>


namespace Wt {
  class WContainerWidget;
  class WText;
  class WTextArea;
  class WLineEdit;
  class WComboBox;
  class WFormWidget;
  class WDateEdit;
}

/**
 * @addtogroup formexample
 */
/*@{*/

/*!\brief A simple Form.
 *
 * Shows how a simple form can made, with an emphasis on how
 * to handle validation.
 */
class UI : public Wt::WTable
{
public:
  /*!\brief Instantiate a new form.
   */
  UI();

private:
  /*!\brief The user selected a new country: adjust the cities combo box.
   */
  void countryChanged();

  /*!\brief Submit the form.
   */
  void submit();

  void create();

  Wt::WContainerWidget *feedbackMessages_;

  Wt::WLineEdit *nameEdit_;
  Wt::WLineEdit *firstNameEdit_;

  Wt::WComboBox *countryEdit_;
  Wt::WComboBox *cityEdit_;

  Wt::WDateEdit *birthDateEdit_;
  Wt::WLineEdit *childCountEdit_;
  Wt::WLineEdit *weightEdit_;

  Wt::WTextArea *remarksEdit_;

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
  bool checkValid(Wt::WFormWidget *edit, const Wt::WString& text);
};

/*@}*/
