#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#if DEBUG
#define DEBUG_LOG(x)    std::cout << x
#define WinMain main
#else
#define DEBUG_LOG(x) // do nothing
#endif

class Vaultware: public wxApp
{
private:
    wxFrame *_mainWindow = nullptr;
public:
    Vaultware(/* args */) = default;
    virtual bool OnInit() override {
        DEBUG_LOG("Init\n");
        _mainWindow = new wxFrame(nullptr, wxID_ANY, "Vaultware");
        return _mainWindow->Show();
    }
    ~Vaultware() = default;
};

wxIMPLEMENT_APP(Vaultware);