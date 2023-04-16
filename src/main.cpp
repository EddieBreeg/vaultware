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
	GeneratePasswordPanel* _generatePasswordPanel = nullptr;
	CredentialsGrid* _grid = nullptr;
	Vault _vault;
public:
	virtual bool OnInit() override {
		_mainWin = new wxFrame(nullptr, wxID_ANY, "Vaultware", wxDefaultPosition, wxSize(420, 300));
		_grid = new CredentialsGrid(_mainWin);

		_mainWin->Show();
		if(!_vault.isOpen()){
			wxMessageBox(_vault.getError().what(), wxMessageBoxCaptionStr, 
				wxICON_ERROR);
			_mainWin->Close();
			return false;
		}
		DEBUG_LOG(generatePassword(RNG::instance(), 10, true, 2) << '\n');
		/*_loginPanel = new LoginPanel(_mainWin);
		if (_loginPanel->ShowModal() == wxID_OK) {
			std::cout << _loginPanel->GetLogin() << std::endl;
			std::cout << _loginPanel->GetPassword() << std::endl;
		}
		else {
			_mainWin->Close();
		}

		_loginPanel->Destroy();*/
		_generatePasswordPanel = new GeneratePasswordPanel(_mainWin);
		_generatePasswordPanel->Show();

		return true;
	}
};

wxIMPLEMENT_APP(Vaultware);