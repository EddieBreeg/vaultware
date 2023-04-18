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
		_grid = new CredentialsGrid(_mainWin, &_vault);

		if (!_vault.isOpen()) {
			wxMessageBox(_vault.getError().what(), wxMessageBoxCaptionStr,
				wxICON_ERROR);
			_mainWin->Close();
			return false;
		}
		_loginPanel = new LoginPanel(_mainWin, &_vault);
login:	if (_loginPanel->ShowModal() == wxID_OK) {
			std::string email(_loginPanel->GetLogin()), pwd(_loginPanel->GetPassword());
			bool loginSuccessful;
			try
			{
				loginSuccessful = _vault.login(email, pwd);
			}
			catch(const SQLite3::error_code& e)
			{
				wxMessageBox(e.what(), wxMessageBoxCaptionStr, wxOK | wxICON_ERROR);
				return false;
			}
			if (loginSuccessful){
				_grid->RefreshGrid();
				_mainWin->Show();
				_loginPanel->Destroy();
			}else {
				wxDialog* errorPassword = new wxDialog(_loginPanel, wxID_ANY, "Wrong credentials");
				wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
				wxStaticText* msg = new wxStaticText(errorPassword, wxID_ANY, "The login or the password is not correct");
				mainSizer->Add(msg, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
				wxButton* close = new wxButton(errorPassword, wxID_OK, "Close");
				mainSizer->Add(close, 0, wxALIGN_CENTER);
				errorPassword->SetSizerAndFit(mainSizer);
				errorPassword->ShowModal();
				goto login;
			}
		}

		return true;
	}
	virtual int OnExit() override {
		try
		{
			_vault.saveVault();
			return 0;
		}
		catch(const SQLite3::error_code& err)
		{
			wxMessageBox(err.what(), wxMessageBoxCaptionStr, wxOK | wxICON_ERROR, _mainWin);
			return 1;
		}
	}
};

wxIMPLEMENT_APP(Vaultware);