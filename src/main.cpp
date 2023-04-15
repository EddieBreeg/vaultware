#include <wx/wx.h>
#include <string>
#include <LoginPanel.hpp>
#include <SQLite3/SQLite3.hpp>

#if DEBUG
#include <iostream>
#define DEBUG_LOG(x)	std::cout << x
#else
#define DEBUG_LOG(x)
#endif

class Vaultware: public wxApp
{
	wxFrame* _mainWin = nullptr;
	LoginPanel* _loginPanel = nullptr;
	
public:
	virtual bool OnInit() override {
		DEBUG_LOG((int)SQLite3::SQLite3Error::Retry);
		_mainWin = new wxFrame(nullptr, wxID_ANY, "Vaultware");

		_loginPanel = new LoginPanel(_mainWin);
		_mainWin->Show();
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