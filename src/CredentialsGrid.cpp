#include <CredentialsGrid.hpp>
#include <iostream>
#include <string>
#include <AddCredentialPanel.hpp>

CredentialsGrid::CredentialsGrid(wxFrame* parent, Vault* vault) {
    _vault = vault;
    _parent = parent;
    _grid = new wxGrid(_parent, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addCredentialButton = new wxButton(_parent, wxID_ANY, "Add credential");
    addCredentialButton->Bind(wxEVT_BUTTON, &CredentialsGrid::OnAddCredential, this);

    _grid->CreateGrid(0, 2);
    _grid->SetRowLabelSize(0);
    _grid->SetColLabelValue(0, "Name");
    _grid->SetColLabelValue(1, "URL");
    _grid->SetColSize(0, 200);
    _grid->SetColSize(1, 200);

    int size = _vault->end() - _vault->begin();
    _grid->AppendRows(size);
    for (auto it = _vault->begin(); it != _vault->end(); ++it) {
        int index = it - _vault->begin();
        _grid->SetCellValue(index, 0, (*_vault)[index].getName());
        _grid->SetCellValue(index, 1, (*_vault)[index].getUrl());
    }

    _grid->SetGridLineColour(wxColour(200, 200, 200));
    _grid->EnableGridLines(true);

    _grid->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &CredentialsGrid::OnGridCellRightClick, this);

    sizer->Add(addCredentialButton, 0, wxEXPAND, 30);
    sizer->Add(_grid, 1, wxCENTER, 5);
    _parent->SetSizer(sizer);
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

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* nameInput = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    nameSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential name :"), 0, wxRIGHT, 8);
    nameSizer->Add(nameInput, 1);
    mainSizer->Add(nameSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* loginSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* loginInput = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    loginSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential login :"), 0, wxRIGHT, 8);
    loginSizer->Add(loginInput, 1);
    mainSizer->Add(loginSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* passwordInput = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
    passwordSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential password :"), 0, wxRIGHT, 8);
    passwordSizer->Add(passwordInput, 1);
    mainSizer->Add(passwordSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* urlSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* urlInput = new wxTextCtrl(dialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    urlSizer->Add(new wxStaticText(dialog, wxID_ANY, "Credential URL :"), 0, wxRIGHT, 8);
    urlSizer->Add(urlInput, 1);
    mainSizer->Add(urlSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxButton* ok = new wxButton(dialog, wxID_CANCEL, "OK");
    mainSizer->Add(ok, 0, wxCenter, 1);
 
    dialog->SetDefaultItem(ok);
    dialog->SetSizer(mainSizer);
}

void CredentialsGrid::OnMenuEdit(wxCommandEvent& event) {
    std::cout << "Edit !" << _clickedRow << std::endl;
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
        _vault->deleteCredential(_clickedRow);
    }

    deleteMenu->Destroy();

    _grid->ClearGrid();
    _grid->ForceRefresh();
    _grid->AppendRows(_vault->end() - _vault->begin());
    for (auto it = _vault->begin(); it != _vault->end(); ++it) {
        int index = it - _vault->begin();
        _grid->SetCellValue(index, 0, (*_vault)[index].getName());
        _grid->SetCellValue(index, 1, (*_vault)[index].getUrl());
    }
    _grid->ForceRefresh();
}

void CredentialsGrid::OnAddCredential(wxCommandEvent& event) {
    AddCredentialPanel* panel = new AddCredentialPanel(_parent);
    if (panel->ShowModal() == wxID_OK) {
        _vault->addCredential(panel->GetCredential());
    }
    panel->Destroy();
    _grid->ClearGrid();
    _grid->ForceRefresh();
    _grid->AppendRows(_vault->end() - _vault->begin());
    for (auto it = _vault->begin(); it != _vault->end(); ++it) {
        int index = it - _vault->begin();
        _grid->SetCellValue(index, 0, (*_vault)[index].getName());
        _grid->SetCellValue(index, 1, (*_vault)[index].getUrl());
    }
    _grid->ForceRefresh();
}