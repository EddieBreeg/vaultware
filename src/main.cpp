#include <wx/wx.h>

class Vaultware: public wxApp
{
	wxFrame* _mainWin = nullptr;
public:
	Vaultware() = default;
	~Vaultware() = default;
	virtual bool OnInit() override {
		_mainWin = new wxFrame(nullptr, wxID_ANY, "Vaultware");
		return _mainWin->Show();
	}

};


wxIMPLEMENT_APP(Vaultware);