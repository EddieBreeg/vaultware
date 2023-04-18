#pragma once

#include <wx/wx.h>
#include <string>
#include <Vault.hpp>

class LoginPanel : public wxDialog
{
public:
	LoginPanel(wxFrame* parent, Vault* vault);

	std::string GetLogin() const;
	std::string GetPassword() const;
private:
	Vault* _vault;
	wxTextCtrl* _loginInput;
	wxTextCtrl* _passwordInput;

	void OnCreateAccount(wxCommandEvent& event);
};