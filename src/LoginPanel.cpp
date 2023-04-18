#include <LoginPanel.hpp>

LoginPanel::LoginPanel(wxFrame* parent, Vault* vault) : wxDialog(parent, wxID_ANY, "Login") {
	_vault = vault;
	_loginInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	_loginInput->SetHint("Enter login");
	_passwordInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD);
	_passwordInput->SetHint("Enter password");

	wxButton* loginButton = new wxButton(this, wxID_OK, "Login");
	wxButton* quitButton = new wxButton(this, wxID_CANCEL, "Quit");
	wxButton* signupButton = new wxButton(this, wxID_ANY, "Create an account");
	signupButton->Bind(wxEVT_BUTTON, &LoginPanel::OnCreateAccount, this);
	SetDefaultItem(loginButton);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
	loginSizer->Add(new wxStaticText(this, wxID_ANY, "Login :"), 0, wxALL, 5);
	loginSizer->Add(_loginInput, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	passwordSizer->Add(new wxStaticText(this, wxID_ANY, "Password :"), 0, wxALL, 5);
	passwordSizer->Add(_passwordInput, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(loginButton, 0, wxALL, 5);
	buttonSizer->Add(quitButton, 0, wxALL, 5);

	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Vaultware login"), 0, wxALIGN_CENTER | wxALL, 20);
	mainSizer->Add(loginSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 20);
	mainSizer->Add(passwordSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 20);
	mainSizer->AddStretchSpacer();
	mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 20);
	mainSizer->Add(signupButton, 0, wxALIGN_CENTER | wxALL, 10);

	SetSizerAndFit(mainSizer);
}

std::string LoginPanel::GetLogin() const {
	return _loginInput->GetValue().ToStdString();
}

std::string LoginPanel::GetPassword() const {
	return _passwordInput->GetValue().ToStdString();
}

void LoginPanel::OnCreateAccount(wxCommandEvent& event) {
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Create an account");
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	
	wxTextCtrl* login = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	wxTextCtrl* password = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD);
	wxTextCtrl* confirmPassword = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
	mainSizer->Add(new wxStaticText(dialog, wxID_ANY, "Login :"), 0, wxALL, 5);
	mainSizer->Add(login, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(new wxStaticText(dialog, wxID_ANY, "Password :"), 0, wxALL, 5);
	mainSizer->Add(password, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(new wxStaticText(dialog, wxID_ANY, "Confirm password :"), 0, wxALL, 5);
	mainSizer->Add(confirmPassword, 0, wxEXPAND | wxALL, 5);
	login->SetHint("Enter login");
	password->SetHint("Enter password");
	confirmPassword->SetHint("Enter password again");

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* ok = new wxButton(dialog, wxID_OK, "Create account");
	wxButton* cancel = new wxButton(dialog, wxID_CANCEL, "Cancel");
	dialog->SetDefaultItem(ok);
	buttonSizer->Add(ok, 0, wxALL, 5);
	buttonSizer->Add(cancel, 0, wxALL, 5);

	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
	dialog->SetSizerAndFit(mainSizer);

	if (dialog->ShowModal() == wxID_OK) {
		std::string loginStr = login->GetValue().ToStdString();
		std::string passwordStr = password->GetValue().ToStdString();
		std::string confirmPasswordStr = confirmPassword->GetValue().ToStdString();
		if (!passwordStr.compare(confirmPasswordStr)) {
			if (!loginStr.compare("") || !passwordStr.compare("")) {
				wxDialog* errorPassword = new wxDialog(dialog, wxID_ANY, "Wrong credentials");
				wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
				wxStaticText* msg = new wxStaticText(errorPassword, wxID_ANY, "The login and the password must not be empty");
				mainSizer->Add(msg, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
				wxButton* close = new wxButton(errorPassword, wxID_OK, "Close");
				mainSizer->Add(close, 0, wxALIGN_CENTER);
				errorPassword->SetSizerAndFit(mainSizer);
				errorPassword->ShowModal();
			}
			else {
				_vault->createUser(loginStr, passwordStr);
				dialog->Destroy();
			}
		}
		else {
			wxDialog* errorPassword = new wxDialog(dialog, wxID_ANY, "Wrong password");
			wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
			wxStaticText* msg = new wxStaticText(errorPassword, wxID_ANY, "The passwords you supplied do not match");
			mainSizer->Add(msg, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
			wxButton* close = new wxButton(errorPassword, wxID_OK, "Close");
			mainSizer->Add(close, 0, wxALIGN_CENTER);
			errorPassword->SetSizerAndFit(mainSizer);
			errorPassword->ShowModal();
		}
	}
	dialog->Destroy();
}