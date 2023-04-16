#pragma once

#include <wx/wx.h>
#include <wx/grid.h>
#include <Vault.hpp>

class CredentialsGrid
{
public:
	CredentialsGrid(wxFrame* parent, Vault* vault);

	wxGrid* _grid;
	wxFrame* _parent;
	Vault* _vault;
private:
	int _clickedRow = -1;

	void OnGridCellRightClick(wxGridEvent& event);
	void OnMenuView(wxCommandEvent& event);
	void OnMenuEdit(wxCommandEvent& event);
	void OnMenuDelete(wxCommandEvent& event);
	void OnAddCredential(wxCommandEvent& event);
};