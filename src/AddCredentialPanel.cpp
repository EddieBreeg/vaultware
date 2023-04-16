#include <AddCredentialPanel.hpp>

AddCredentialPanel::AddCredentialPanel(wxFrame* parent) : wxDialog(parent, wxID_ANY, "Add credential") {
	_parent = parent;
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
	_nameInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	nameSizer->Add(new wxStaticText(this, wxID_ANY, "Credential name :"), 0, wxRIGHT, 8);
	nameSizer->Add(_loginInput, 1);
	mainSizer->Add(nameSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
	_loginInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	loginSizer->Add(new wxStaticText(this, wxID_ANY, "Credential login :"), 0, wxRIGHT, 8);
	loginSizer->Add(_loginInput, 1);
	mainSizer->Add(loginSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
	_passwordInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PASSWORD);
	passwordSizer->Add(new wxStaticText(this, wxID_ANY, "Credential password :"), 0, wxRIGHT, 8);
	passwordSizer->Add(_passwordInput, 1);
	mainSizer->Add(passwordSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	
	wxBoxSizer* urlSizer = new wxBoxSizer(wxHORIZONTAL);
	_urlInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	urlSizer->Add(new wxStaticText(this, wxID_ANY, "Credential name :"), 0, wxRIGHT, 8);
	urlSizer->Add(_urlInput, 1);
	mainSizer->Add(urlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	wxButton* openPasswordGeneratorButton = new wxButton(this, wxID_ANY, "Open password generator");
	mainSizer->Add(openPasswordGeneratorButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	openPasswordGeneratorButton->Bind(wxEVT_BUTTON, &AddCredentialPanel::OnOpenPasswordGenerator, this);

	wxBoxSizer* appSizer = new wxBoxSizer(wxHORIZONTAL);
	_urlInput = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	appSizer->Add(new wxStaticText(this, wxID_ANY, "Credential app :"), 0, wxRIGHT, 8);
	appSizer->Add(_urlInput, 1);
	mainSizer->Add(appSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	_confirmPasswordInput = new wxCheckBox(this, wxID_ANY, "Confirm password");
	bottomSizer->Add(_confirmPasswordInput, 0);
	bottomSizer->AddStretchSpacer(1);

	wxButton* confirmButton = new wxButton(this, wxID_OK, "Add credential");
	bottomSizer->Add(confirmButton, 0, wxLEFT, 5);
	wxButton* quitButton = new wxButton(this, wxID_CANCEL, "Cancel");
	bottomSizer->Add(quitButton, 0, wxLEFT, 5);
	mainSizer->Add(bottomSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);
	
	_nameInput->SetHint("Enter credential name");
	_loginInput->SetHint("Enter credential login");
	_passwordInput->SetHint("Enter credential password");
	_urlInput->SetHint("Enter credential app");

	SetDefaultItem(confirmButton);
	SetSizer(mainSizer);
}

void AddCredentialPanel::OnOpenPasswordGenerator(wxCommandEvent& event) {
	GeneratePasswordPanel* panel = new GeneratePasswordPanel(_parent);
	panel->Show();
}

Credential AddCredentialPanel::GetCredential() {
	std::string name = _nameInput->GetValue().ToStdString();
	std::string login = _loginInput->GetValue().ToStdString();
	std::string password = _passwordInput->GetValue().ToStdString();
	std::string url = _urlInput->GetValue().ToStdString();
	bool confirm = _confirmPasswordInput->GetValue();

	return Credential(0, name, login, password, url, confirm);
}