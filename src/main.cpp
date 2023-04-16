#include <wx/wx.h>
#include <string>
#include <LoginPanel.hpp>
#include <GeneratePasswordPanel.hpp>
#include <Vault.hpp>
#include <PasswordGenerator.hpp>
#include <Log.h>
#include <RNG.hpp>
#include <CredentialsGrid.hpp>

class Vaultware: public wxApp
{
	wxFrame* _mainWin = nullptr;
	LoginPanel* _loginPanel = nullptr;
	CredentialsGrid* _grid = nullptr;
	Vault _vault;
public:
	virtual bool OnInit() override {
		_mainWin = new wxFrame(nullptr, wxID_ANY, "Vaultware", wxDefaultPosition, wxSize(420, 300));

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
			if (_vault.login(email, pwd)) {
				_loginPanel->Destroy();
				_grid = new CredentialsGrid(_mainWin, &_vault);
			}
			else {
				_mainWin->Close();
			}
		}
		else {
			_mainWin->Close();
		}

		if(!_vault.isOpen()){
			wxMessageBox(_vault.getError().what(), wxMessageBoxCaptionStr, 
				wxICON_ERROR);
			_mainWin->Close();
			return false;
		}

		return true;
	}
};

wxIMPLEMENT_APP(Vaultware);