#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/clipbrd.h>

class GeneratePasswordPanel : public wxDialog
{
public:
	GeneratePasswordPanel(wxFrame* parent);

	int GetLength() const;
	bool GetAllowNumbers() const;
	bool GetAllowSpecialChars() const;
	int GetMinSpecialChars() const;
private:
	wxSpinCtrl* _lengthInput;
	wxSpinCtrl* _minSpecialCharsInput;
	wxCheckBox* _allowNumbers;
	wxCheckBox* _allowSpecialChars;
	wxTextCtrl* _passwordResult;

	void OnGenerate(wxCommandEvent& event);
	void OnSymbolsCheckboxChange(wxCommandEvent& event);
	void OnCopyClicked(wxCommandEvent& event);
};