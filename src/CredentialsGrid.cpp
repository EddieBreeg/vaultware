#include <CredentialsGrid.hpp>
#include <iostream>
#include <string>
#include <AddCredentialPanel.hpp>

CredentialsGrid::CredentialsGrid(wxFrame* parent, Vault* vault) {
    _vault = vault;
    _parent = parent;
    _grid = new wxGrid(parent, wxID_ANY);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxButton* addCredentialButton = new wxButton(parent, wxID_ANY, "Add credential");
    addCredentialButton->Bind(wxEVT_BUTTON, &CredentialsGrid::OnAddCredential, this);

    _grid->CreateGrid(0, 2);
    _grid->SetRowLabelSize(0);
    _grid->SetColLabelValue(0, "Name");
    _grid->SetColLabelValue(1, "URL");
    _grid->SetColSize(0, 200);
    _grid->SetColSize(1, 200);

    // _grid->AppendRows(5);
    // for (int i = 0; i < 5; i++) {
    //     _grid->SetCellValue(i, 0, "Name");
    //     _grid->SetCellValue(i, 1, "URL");
    // }

    _grid->SetGridLineColour(wxColour(200, 200, 200));
    _grid->EnableGridLines(true);

    _grid->Bind(wxEVT_GRID_CELL_RIGHT_CLICK, &CredentialsGrid::OnGridCellRightClick, this);

    sizer->Add(addCredentialButton, 0, wxEXPAND, 30);
    sizer->Add(_grid, 1, wxCENTER, 5);
    parent->SetSizer(sizer);
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
    std::cout << "View !" << _clickedRow << std::endl;
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
        std::cout << "suppr !" << std::endl;
    }
    else {
        std::cout << "non" << std::endl;
    }

    deleteMenu->Destroy();
}

void CredentialsGrid::OnAddCredential(wxCommandEvent& event) {
    AddCredentialPanel* panel = new AddCredentialPanel(_parent);
    if (panel->ShowModal() == wxID_OK) {
        _vault->addCredential(panel->GetCredential());
    }
    panel->Destroy();
}