#pragma once

#include <wx/wx.h>
#include <GeneratePasswordPanel.hpp>
#include <Credential.hpp>

class AddCredentialPanel : public wxDialog {
public:
	AddCredentialPanel(wxFrame* parent);
	wxFrame* _parent;
	Credential GetCredential();
private:
	wxTextCtrl* _nameInput;
	wxTextCtrl* _loginInput;
	wxTextCtrl* _passwordInput;
	wxTextCtrl* _urlInput;
	wxCheckBox* _confirmPasswordInput;

	void OnOpenPasswordGenerator(wxCommandEvent& event);
};