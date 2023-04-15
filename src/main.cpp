#include <wx/wx.h>
#include <string>
#include <LoginPanel.hpp>
#include <Vault.hpp>

class Vaultware: public wxApp
{
	wxFrame* _mainWin = nullptr;
	LoginPanel* _loginPanel = nullptr;
	Vault _vault;
public:
	virtual bool OnInit() override {
		_mainWin = new wxFrame(nullptr, wxID_ANY, "Vaultware");
		_mainWin->Show();
		if(!_vault.isOpen()){
			wxMessageBox(_vault.getError().what(), wxMessageBoxCaptionStr, 
				wxICON_ERROR);
			_mainWin->Close();
			return false;
		}
		_loginPanel = new LoginPanel(_mainWin);
		if (_loginPanel->ShowModal() == wxID_OK) {
			std::cout << _loginPanel->GetLogin() << std::endl;
			std::cout << _loginPanel->GetPassword() << std::endl;
		}
		else {
			_mainWin->Close();
		}

		_loginPanel->Destroy();

		return true;
	}
};

wxIMPLEMENT_APP(Vaultware);