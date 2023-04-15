#pragma once

#include <wx/wx.h>

class LoginPanel : public wxDialog
{
public:
	LoginPanel(wxFrame* parent);

	wxString GetLogin() const;
	wxString GetPassword() const;
private:
	wxTextCtrl* _loginInput;
	wxTextCtrl* _passwordInput;
};