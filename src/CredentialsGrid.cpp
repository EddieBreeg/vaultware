#include <CredentialsGrid.hpp>
#include <iostream>
#include <string>
#include <AddCredentialPanel.hpp>

CredentialsGrid::CredentialsGrid(wxFrame* parent, Vault* vault) {
    _vault = vault;
    _parent = parent;
    _grid = new wxGrid(_parent, wxID_ANY);
    _grid->EnableCellEditControl(false);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addCredentialButton = new wxButton(_parent, wxID_ANY, "Add credential");
    addCredentialButton->Bind(wxEVT_BUTTON, &CredentialsGrid::OnAddCredential, this);

    _grid->CreateGrid(0, 2);
    _grid->SetRowLabelSize(0);
    _grid->SetColLabelValue(0, "Name");
    _grid->SetColLabelValue(1, "URL");
    _grid->SetColSize(0, 200);
    _grid->SetColSize(1, 200);
    
    _grid->SetGridLineColour(wxColour(200, 200, 200));
    _grid->EnableGridLines(true);

    _grid->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &CredentialsGrid::OnGridCellRightClick, this);

    sizer->Add(addCredentialButton, 0, wxEXPAND, 30);
    sizer->Add(_grid, 1, wxCENTER, 5);
    _parent->SetSizer(sizer);
}

void CredentialsGrid::RefreshGrid() {
    for (int i = _grid->GetNumberRows() - 1; i >= 0; i--) {
        _grid->DeleteRows(i);
    }
    _grid->AppendRows(_vault->end() - _vault->begin());
    for (auto it = _vault->begin(); it != _vault->end(); ++it) {
        int index = it - _vault->begin();
        _grid->SetCellValue(index, 0, (*_vault)[index].getName());
        _grid->SetCellValue(index, 1, (*_vault)[index].getUrl());
    }
    _grid->ForceRefresh();
}

void CredentialsGrid::OnGridCellRightClick(wxGridEvent& event) {
    wxMenu* menu = new wxMenu();
    menu->Append(wxID_ANY, "View");
    menu->Append(wxID_ANY, "Edit");
    menu->Append(wxID_ANY, "Delete");

    _grid->Bind(wxEVT_MENU, &CredentialsGrid::OnMenuView, this, menu->FindItemByPosition(0)->GetId());
    _grid->Bind(wxEVT_MENU, &CredentialsGrid::OnMenuEdit, this, menu->FindItemByPosition(1)->GetId());
    _grid->Bind(wxEVT_MENU, &CredentialsGrid::OnMenuDelete, this, menu->FindItemByPosition(2)->GetId());

    _clickedRow = event.GetRow();

    _grid->PopupMenu(menu, event.GetPosition());
}

void CredentialsGrid::OnMenuView(wxCommandEvent& event) {
    const Credential& cred = (*_vault)[_clickedRow];
    wxDialog* dialog = new wxDialog(_parent, wxID_ANY, "View credential");

    if (cred.getConfirmPassword()) {
        wxDialog* getPassword = new wxDialog(_parent, wxID_ANY, "Confirm password");

        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        wxStaticText* label = new wxStaticText(getPassword, wxID_ANY, "You have to enter your password again to view this credential");
        wxTextCtrl* passwordField = new wxTextCtrl(getPassword, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD|wxTE_PROCESS_ENTER);
        wxButton* okButton = new wxButton(getPassword, wxID_OK, "OK");
        wxButton* cancelButton = new wxButton(getPassword, wxID_CANCEL, "Cancel");
        mainSizer->Add(label, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
        mainSizer->Add(passwordField, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(okButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);
        mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);

        getPassword->SetDefaultItem(okButton);
        getPassword->SetSizerAndFit(mainSizer);
        
        if (getPassword->ShowModal() == wxID_OK) {
            if (!_vault->checkPassword(passwordField->GetValue().ToStdString())) {
                dialog->Destroy();
                wxDialog* errorPassword = new wxDialog(_parent, wxID_ANY, "Wrong password");
                wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
                wxStaticText* msg = new wxStaticText(errorPassword, wxID_ANY, "The password is not correct");
                mainSizer->Add(msg, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
                wxButton* close = new wxButton(errorPassword, wxID_OK, "Close");
                mainSizer->Add(close, 0, wxALIGN_CENTER);
                errorPassword->SetSizerAndFit(mainSizer);
                errorPassword->ShowModal();
                return;
            }
        }
        getPassword->Destroy();
    }

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* nameInput = new wxTextCtrl(dialog, wxID_ANY, cred.getName(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    nameSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential name :"), 0, wxRIGHT, 8);
    nameSizer->Add(nameInput, 1);
    mainSizer->Add(nameSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* loginInput = new wxTextCtrl(dialog, wxID_ANY, cred.getLogin(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    loginSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential login :"), 0, wxRIGHT, 8);
    loginSizer->Add(loginInput, 1);
    mainSizer->Add(loginSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* passwordInput = new wxTextCtrl(dialog, wxID_ANY, cred.getPassword(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    passwordSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential password :"), 0, wxRIGHT, 8);
    passwordSizer->Add(passwordInput, 1);
    mainSizer->Add(passwordSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* urlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* urlInput = new wxTextCtrl(dialog, wxID_ANY, cred.getUrl(), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    urlSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential URL :"), 0, wxRIGHT, 8);
    urlSizer->Add(urlInput, 1);
    mainSizer->Add(urlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxButton* ok = new wxButton(dialog, wxID_CANCEL, "OK");
    mainSizer->Add(ok, 0, wxCENTER, 1);
 
    dialog->SetDefaultItem(ok);
    dialog->SetSizer(mainSizer);
    dialog->Show();
}

void CredentialsGrid::OnMenuEdit(wxCommandEvent& event) {
    Credential cred = (*_vault)[_clickedRow];

    wxDialog* editMenu = new wxDialog(_parent, wxID_ANY, "Edit credential");

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* nameInput = new wxTextCtrl(editMenu, wxID_ANY, cred.getName(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    nameSizer->Add(new wxStaticText(editMenu, wxID_ANY, "Credential name :"), 0, wxRIGHT, 8);
    nameSizer->Add(nameInput, 1);
    mainSizer->Add(nameSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* loginInput = new wxTextCtrl(editMenu, wxID_ANY, cred.getLogin(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    loginSizer->Add(new wxStaticText(editMenu, wxID_ANY, "Credential login :"), 0, wxRIGHT, 8);
    loginSizer->Add(loginInput, 1);
    mainSizer->Add(loginSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* passwordInput = new wxTextCtrl(editMenu, wxID_ANY, cred.getPassword(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
    passwordSizer->Add(new wxStaticText(editMenu, wxID_ANY, "Credential password :"), 0, wxRIGHT, 8);
    passwordSizer->Add(passwordInput, 1);
    mainSizer->Add(passwordSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxButton* openPasswordGeneratorButton = new wxButton(editMenu, wxID_ANY, "Open password generator");
    mainSizer->Add(openPasswordGeneratorButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    openPasswordGeneratorButton->Bind(wxEVT_BUTTON, &CredentialsGrid::OnOpenPasswordGenerator, this);

    wxBoxSizer* urlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* urlInput = new wxTextCtrl(editMenu, wxID_ANY, cred.getUrl(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    urlSizer->Add(new wxStaticText(editMenu, wxID_ANY, "Credential URL :"), 0, wxRIGHT, 8);
    urlSizer->Add(urlInput, 1);
    mainSizer->Add(urlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxCheckBox* confirmPasswordInput = new wxCheckBox(editMenu, wxID_ANY, "Confirm password");
    confirmPasswordInput->SetValue(cred.getConfirmPassword());
    bottomSizer->Add(confirmPasswordInput, 0);
    bottomSizer->AddStretchSpacer(1);

    wxButton* confirmButton = new wxButton(editMenu, wxID_OK, "Save");
    bottomSizer->Add(confirmButton, 0, wxLEFT, 5);
    wxButton* quitButton = new wxButton(editMenu, wxID_CANCEL, "Cancel");
    bottomSizer->Add(quitButton, 0, wxLEFT, 5);
    mainSizer->Add(bottomSizer, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    nameInput->SetHint("Enter credential name");
    loginInput->SetHint("Enter credential login");
    passwordInput->SetHint("Enter credential password");
    urlInput->SetHint("Enter credential URL");

    editMenu->SetDefaultItem(confirmButton);
    editMenu->SetSizer(mainSizer);

    if (editMenu->ShowModal() == wxID_OK) {
        std::string name = nameInput->GetValue().ToStdString();
        std::string login = loginInput->GetValue().ToStdString();
        std::string password = passwordInput->GetValue().ToStdString();
        std::string url = urlInput->GetValue().ToStdString();
        bool confirm = confirmPasswordInput->GetValue();
        cred.setName(name);
        cred.setLogin(login);
        cred.setPassword(password);
        cred.setUrl(url);
        cred.setConfirmPassword(confirm);
        try {
            _vault->deleteCredential(_clickedRow);
            _vault->addCredential(std::move(cred));
        }
        catch (const SQLite3::error_code& e){
            wxMessageBox(e.what(), wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
        }
    }
    editMenu->Destroy();
    RefreshGrid();
}

void CredentialsGrid::OnMenuDelete(wxCommandEvent& event) {
    wxDialog* deleteMenu = new wxDialog(_parent, wxID_ANY, "Delete confirm");
    wxButton* ok = new wxButton(deleteMenu, wxID_OK, "Yes");
    wxButton* cancel = new wxButton(deleteMenu, wxID_CANCEL, "Cancel");

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(new wxStaticText(deleteMenu, wxID_ANY, "Are you sure you want to delete this credential ?"), 0, wxALIGN_CENTER | wxALL, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(ok, 0, wxALIGN_CENTER | wxALL, 5);
    buttonSizer->Add(cancel, 0, wxALIGN_CENTER | wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM | wxTOP, 10);

    deleteMenu->SetSizer(sizer);
    deleteMenu->Layout();

    if (deleteMenu->ShowModal() == wxID_OK) {
        try
        {
            _vault->deleteCredential(_clickedRow);
        }
        catch(const SQLite3::error_code& e)
        {
            wxMessageBox(e.what(), wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
        }
        RefreshGrid();
    }
    deleteMenu->Destroy();
}

void CredentialsGrid::OnAddCredential(wxCommandEvent& event) {
    AddCredentialPanel* panel = new AddCredentialPanel(_parent);
    if (panel->ShowModal() == wxID_OK) {
        try
        {
            _vault->addCredential(panel->GetCredential());
        }
        catch(const SQLite3::error_code& e)
        {
            wxMessageBox(e.what(), wxMessageBoxCaptionStr, wxICON_ERROR | wxOK);
        }
        RefreshGrid();
    }
    panel->Destroy();
}

void CredentialsGrid::OnOpenPasswordGenerator(wxCommandEvent& event) {
    GeneratePasswordPanel* panel = new GeneratePasswordPanel(_parent);
    panel->Show();
}