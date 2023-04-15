#include <GeneratePasswordPanel.hpp>

GeneratePasswordPanel::GeneratePasswordPanel(wxFrame* parent) : wxDialog(parent, wxID_ANY, "Password generator") {
    _allowNumbers = new wxCheckBox(this, wxID_ANY, "0-9", wxDefaultPosition, wxDefaultSize, 0);
    _allowSpecialChars = new wxCheckBox(this, wxID_ANY, "!@#$%^&*", wxDefaultPosition, wxDefaultSize, 0);
    _allowNumbers->SetValue(true);
    _allowSpecialChars->SetValue(true);
    _allowSpecialChars->Bind(wxEVT_CHECKBOX, &GeneratePasswordPanel::OnSymbolsCheckboxChange, this);

	_lengthInput = new wxSpinCtrl(this, wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 100, 10);
	_minSpecialCharsInput = new wxSpinCtrl(this, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 5, 100, 5);

    _passwordResult = new wxTextCtrl(this, wxID_ANY, "blablabla", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

	wxButton* generateButton = new wxButton(this, wxID_OK, "Generate");
	wxButton* quitButton = new wxButton(this, wxID_CANCEL, "Quit");
	SetDefaultItem(generateButton);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(_allowNumbers, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);
    mainSizer->Add(_allowSpecialChars, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 10);

    wxFlexGridSizer* spinnerSizer = new wxFlexGridSizer(2, wxSize(5, 5));
    spinnerSizer->Add(new wxStaticText(this, wxID_ANY, "Password length :"), 0, wxALL, 5);
    spinnerSizer->Add(_lengthInput, 0, wxALL | wxEXPAND, 5);
    spinnerSizer->Add(new wxStaticText(this, wxID_ANY, "Min special chars :"), 0, wxALL, 5);
    spinnerSizer->Add(_minSpecialCharsInput, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(spinnerSizer, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(generateButton, 0, wxALL, 5);
    buttonSizer->Add(quitButton, 0, wxALL, 5);

    mainSizer->Add(new wxStaticText(this, wxID_ANY, "Generated password :"), 0, wxLEFT, 5);
    mainSizer->Add(_passwordResult, 0, wxALL | wxEXPAND, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 20);

    SetSizer(mainSizer);
    SetAutoLayout(true);
    mainSizer->Fit(this);
}

int GeneratePasswordPanel::GetLength() const {
	return _lengthInput->GetValue();
}

bool GeneratePasswordPanel::GetAllowNumbers() const {
    return _allowNumbers->GetValue();
}

bool GeneratePasswordPanel::GetAllowSpecialChars() const {
    return _allowSpecialChars->GetValue();
}


int GeneratePasswordPanel::GetMinSpecialChars() const {
	return _minSpecialCharsInput->GetValue();
}

void GeneratePasswordPanel::SetGeneratedPassword(std::string password) {
    _passwordResult->SetValue(password);
}

void GeneratePasswordPanel::OnSymbolsCheckboxChange(wxCommandEvent& event) {
    if (event.IsChecked())
        _minSpecialCharsInput->Enable(true);
    else
        _minSpecialCharsInput->Enable(false);
}