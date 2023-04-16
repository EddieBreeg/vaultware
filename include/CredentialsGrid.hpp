#pragma once

#include <wx/wx.h>
#include <wx/grid.h>

class CredentialsGrid
{
public:
	CredentialsGrid(wxFrame* parent);

	wxGrid* _grid;
	wxFrame* _parent;
private:
	int _clickedRow = -1;

	void OnGridCellRightClick(wxGridEvent& event);
	void OnMenuView(wxCommandEvent& event);
	void OnMenuEdit(wxCommandEvent& event);
	void OnMenuDelete(wxCommandEvent& event);
	void OnAddCredential(wxCommandEvent& event);
};