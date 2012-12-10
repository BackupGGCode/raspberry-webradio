//
// Simulator Header
//
// 31.08.2012
//

#include <wx/wx.h>
#include <wx/process.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>

#define C_STR(str) const_cast<char*>((const char*)(str).mb_str())


class Window: public wxFrame {
 public:
  Window(wxFrame *frame, const wxString& title);
  Window();
  ~Window();
  
protected:
  wxStaticBitmap* gui_lcd;
  wxButton* m_button14;
  wxButton* m_button2;
  wxButton* m_button3;
  wxButton* m_button4;
  wxButton* m_button11;
  wxButton* m_button10;
  wxButton* m_button6;
  wxButton* m_button7;
  wxButton* m_button8;
  wxButton* m_button9;
  wxButton* m_button61;
  wxButton* m_button71;
  wxButton* m_button81;
  wxButton* m_button91;
  wxStatusBar* gui_statusbar;
  wxMenuBar* gui_menubar;
  wxMenu* m_file;
  wxMenu* m_quest;

 private:
  uint8_t getPixelFromData(uint8_t x, uint8_t y, unsigned char* data);
  void RotaryLeft(wxCommandEvent& event);
  void RotaryRight(wxCommandEvent& event);
  void RotaryOk(wxCommandEvent& event);
  void BtnHome(wxCommandEvent& event);
  void BtnPlay(wxCommandEvent& event);
  void Btn1(wxCommandEvent& event);
  void Btn2(wxCommandEvent& event);
  void Btn3(wxCommandEvent& event);
  void Btn4(wxCommandEvent& event);
  void Btn1l(wxCommandEvent& event);
  void Btn2l(wxCommandEvent& event);
  void Btn3l(wxCommandEvent& event);
  void Btn4l(wxCommandEvent& event);
  void SaveScreenshot(wxCommandEvent& event);
  void LoadFirmware(wxCommandEvent& event);
  void DoLoadFirmware(wxString path, wxString name);
  void DoUnloadFirmware();
  void DrawDisplay(wxDC &dc, int scale, int off_x, int off_y);
  void OnAbout(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnTimer(wxTimerEvent& event);
  void OnPaint(wxPaintEvent& event);
  DECLARE_EVENT_TABLE()
};

class Simulator : public wxApp {
 public:
  virtual bool OnInit();
};

