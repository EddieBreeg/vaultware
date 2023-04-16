#include <wx/wx.h>
#include <string>
#include <LoginPanel.hpp>
#include <GeneratePasswordPanel.hpp>
#include <Vault.hpp>
#include <PasswordGenerator.hpp>
#include <Log.h>
#include <RNG.hpp>

class Vaultware: public wxApp
{
	wxFrame* _mainWin = nullptr;
	LoginPanel* _loginPanel = nullptr;
	GeneratePasswordPanel* _generatePasswordPanel = nullptr;
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
			std::string email(_loginPanel->GetLogin()), pwd(_loginPanel->GetPassword());
			DEBUG_LOG(_vault.login(email, pwd) << '\n');
		}
		else {
			_mainWin->Close();
		}

		_loginPanel->Destroy();
		_generatePasswordPanel = new GeneratePasswordPanel(_mainWin);
		_generatePasswordPanel->Show();

		return true;
	}
};

wxIMPLEMENT_APP(Vaultware);