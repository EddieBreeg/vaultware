#pragma once

#include <wx/wx.h>
#include <GeneratePasswordPanel.hpp>

class AddCredentialPanel : public wxDialog {
public:
	AddCredentialPanel(wxFrame* parent);
	wxFrame* _parent;
private:
	wxTextCtrl* _nameInput;
	wxTextCtrl* _loginInput;
	wxTextCtrl* _passwordInput;
	wxTextCtrl* _appInput;
	wxCheckBox* _confirmPasswordInput;

	void OnOpenPasswordGenerator(wxCommandEvent& event);
};