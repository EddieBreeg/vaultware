#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

class GeneratePasswordPanel : public wxDialog
{
public:
	GeneratePasswordPanel(wxFrame* parent);

	int GetLength() const;
	bool GetAllowNumbers() const;
	bool GetAllowSpecialChars() const;
	int GetMinSpecialChars() const;
	void SetGeneratedPassword(std::string);
private:
	wxSpinCtrl* _lengthInput;
	wxSpinCtrl* _minSpecialCharsInput;
	wxCheckBox* _allowNumbers;
	wxCheckBox* _allowSpecialChars;
	wxTextCtrl* _passwordResult;

	void OnSymbolsCheckboxChange(wxCommandEvent& event);
};