#pragma once

#include <wx/wx.h>
#include <string>

class LoginPanel : public wxDialog
{
public:
	LoginPanel(wxFrame* parent);

	std::string GetLogin() const;
	std::string GetPassword() const;
private:
	wxTextCtrl* _loginInput;
	wxTextCtrl* _passwordInput;
};