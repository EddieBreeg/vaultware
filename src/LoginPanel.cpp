#include <LoginPanel.hpp>

LoginPanel::LoginPanel(wxFrame* parent) : wxDialog(parent, wxID_ANY, "Login") {
	_loginInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	_loginInput->SetHint("Enter login");
	_passwordInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD);
	_passwordInput->SetHint("Enter password");

	wxButton* loginButton = new wxButton(this, wxID_OK, "Login");
	wxButton* quitButton = new wxButton(this, wxID_CANCEL, "Quit");
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

	SetSizerAndFit(mainSizer);
}

std::string LoginPanel::GetLogin() const {
	return _loginInput->GetValue().ToStdString();
}

std::string LoginPanel::GetPassword() const {
	return _passwordInput->GetValue().ToStdString();
}