//
// Simulator
//
// 31.08.2012
//

#include "simulator.h"

IMPLEMENT_APP(Simulator);

Window* frame;
wxTimer* cctimer;

bool Simulator::OnInit() {
 frame = new Window(0l, _("Simulator"));
 frame->Show();
 return true;
}

int idTimer = wxNewId();


BEGIN_EVENT_TABLE(Window, wxFrame)
 EVT_PAINT(Window::OnPaint)
// EVT_MENU(mi_quit, Window::OnQuit)
END_EVENT_TABLE()

char SIM_PATH[PATH_MAX];
char SIM_PATH_IO[PATH_MAX];

wxMenu* filemenu;
char* data;
uint32_t* io;
int fd, fd1;
pid_t pid = 0;
int firmware_loaded = 0;

Window::Window(wxFrame* frame, const wxString& title) : wxFrame(frame, -1, title) {
	strcpy(SIM_PATH, "../../software/lcd.sim");
	strcpy(SIM_PATH_IO, "../../software/io.sim");
	
	// load firmware from command line parameter
	if(wxGetApp().argc == 2) {
	  wxString path = wxString(wxGetApp().argv[1]);
	  wxString dir, file;
	  int i;
	  for(i = path.Len() - 1; i >= 0; i--) {
	   if(path[i] == '/') {
	    dir = path.Left(i);
	    file = path.Right(path.Len() - i - 1);
	    DoLoadFirmware(dir, file);
	    break;
	   }
	  }
	}
	
	// init image handlers
	wxInitAllImageHandlers();
	
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	SetMinSize(wxSize(370, 370));

        wxGridSizer* gui_main_sizer;
        gui_main_sizer = new wxGridSizer( 1, 1, 0, 0 );
        
        wxFlexGridSizer* gui_sizer_flex;
        gui_sizer_flex = new wxFlexGridSizer( 2, 1, 0, 0 );
        gui_sizer_flex->AddGrowableCol( 0 );
        gui_sizer_flex->AddGrowableCol( 1 );
        gui_sizer_flex->AddGrowableRow( 0 );
        gui_sizer_flex->AddGrowableRow( 1 );
        gui_sizer_flex->SetFlexibleDirection( wxBOTH );
        gui_sizer_flex->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
        
        wxBoxSizer* bSizer5;
        bSizer5 = new wxBoxSizer( wxHORIZONTAL );
        
        gui_lcd = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
        gui_lcd->SetMinSize( wxSize( 256,128 ) );
        
        bSizer5->Add( gui_lcd, 0, wxALL, 5 );
        
        m_button14 = new wxButton( this, wxID_ANY, wxT("Screenshot"), wxDefaultPosition, wxDefaultSize, 0 );
        bSizer5->Add( m_button14, 0, wxALL, 5 );
        
        gui_sizer_flex->Add( bSizer5, 1, wxEXPAND, 5 );
        
        wxBoxSizer* gui_buttons;
        gui_buttons = new wxBoxSizer( wxVERTICAL );
        
        wxGridSizer* gui_buttons_rotary;
        gui_buttons_rotary = new wxGridSizer( 1, 3, 0, 0 );
        
        m_button2 = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button2->SetMaxSize( wxSize( 60,-1 ) );
        
        gui_buttons_rotary->Add( m_button2, 0, wxALL, 5 );
        
        m_button3 = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button3->SetMaxSize( wxSize( 60,-1 ) );
        
        gui_buttons_rotary->Add( m_button3, 0, wxALL, 5 );
        
        m_button4 = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button4->SetMaxSize( wxSize( 60,-1 ) );
        
        gui_buttons_rotary->Add( m_button4, 0, wxALL, 5 );
        
        gui_buttons->Add( gui_buttons_rotary, 0, 0, 5 );
        
        wxBoxSizer* gui_buttons_function;
        gui_buttons_function = new wxBoxSizer( wxHORIZONTAL );
        
        m_button11 = new wxButton( this, wxID_ANY, wxT("Home"), wxDefaultPosition, wxDefaultSize, 0 );
        gui_buttons_function->Add( m_button11, 0, wxALL, 5 );
        
        m_button10 = new wxButton( this, wxID_ANY, wxT("Play/Stop"), wxDefaultPosition, wxDefaultSize, 0 );
        gui_buttons_function->Add( m_button10, 0, wxALL, 5 );
        
        gui_buttons->Add( gui_buttons_function, 1, 0, 5 );
        
        wxBoxSizer* gui_buttons_nr;
        gui_buttons_nr = new wxBoxSizer( wxHORIZONTAL );
        
        m_button6 = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button6->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr->Add( m_button6, 0, wxALL, 5 );
        
        m_button7 = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button7->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr->Add( m_button7, 0, wxALL, 5 );
        
        m_button8 = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button8->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr->Add( m_button8, 0, wxALL, 5 );
        
        m_button9 = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button9->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr->Add( m_button9, 0, wxALL, 5 );
        
        gui_buttons->Add( gui_buttons_nr, 0, 0, 5 );
        
        wxBoxSizer* gui_buttons_nr1;
        gui_buttons_nr1 = new wxBoxSizer( wxHORIZONTAL );
        
        m_button61 = new wxButton( this, wxID_ANY, wxT("1l"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button61->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr1->Add( m_button61, 0, wxALL, 5 );
        
        m_button71 = new wxButton( this, wxID_ANY, wxT("2l"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button71->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr1->Add( m_button71, 0, wxALL, 5 );
        
        m_button81 = new wxButton( this, wxID_ANY, wxT("3l"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button81->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr1->Add( m_button81, 0, wxALL, 5 );
        
        m_button91 = new wxButton( this, wxID_ANY, wxT("4l"), wxDefaultPosition, wxDefaultSize, 0 );
        m_button91->SetMaxSize( wxSize( 40,-1 ) );
        
        gui_buttons_nr1->Add( m_button91, 0, wxALL, 5 );
        
        gui_buttons->Add( gui_buttons_nr1, 1, wxEXPAND, 5 );
        
        gui_sizer_flex->Add( gui_buttons, 0, 0, 5 );
        
        gui_main_sizer->Add( gui_sizer_flex, 1, wxEXPAND, 5 );
        
        this->SetSizer( gui_main_sizer );
        this->Layout();
        gui_statusbar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
        gui_menubar = new wxMenuBar( 0 );
        m_file = new wxMenu();
        wxMenuItem* mi_load;
        mi_load = new wxMenuItem( m_file, wxID_ANY, wxString( wxT("Load firmware") ) , wxEmptyString, wxITEM_NORMAL );
        m_file->Append( mi_load );
        
        wxMenuItem* m_separator1;
        m_separator1 = m_file->AppendSeparator();
        
        wxMenuItem* mi_quit;
        mi_quit = new wxMenuItem( m_file, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
        m_file->Append( mi_quit );
        
        gui_menubar->Append( m_file, wxT("File") );
        
        m_quest = new wxMenu();
        wxMenuItem* mi_about;
        mi_about = new wxMenuItem( m_quest, wxID_ANY, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
        m_quest->Append( mi_about );
        
        gui_menubar->Append( m_quest, wxT("?") );
        
        this->SetMenuBar( gui_menubar );
        
        
        // Connect Events
        m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::SaveScreenshot ), NULL, this );
        m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryLeft ), NULL, this );
        m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryOk ), NULL, this );
        m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryRight ), NULL, this );
        m_button11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::BtnHome ), NULL, this );
        m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::BtnPlay ), NULL, this );
        m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn1 ), NULL, this );
        m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn2 ), NULL, this );
        m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn3 ), NULL, this );
        m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn4 ), NULL, this );
        m_button61->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn1l ), NULL, this );
        m_button71->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn2l ), NULL, this );
        m_button81->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn3l ), NULL, this );
        m_button91->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn4l ), NULL, this );
        this->Connect( mi_load->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::LoadFirmware ) );
        this->Connect( mi_quit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::OnQuit ) );
        this->Connect( mi_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::OnAbout ) );
	
	cctimer = new wxTimer(this, idTimer);
	cctimer->Start(500);
	Connect(idTimer, wxEVT_TIMER, wxTimerEventHandler(Window::OnTimer)); 
}

void Window::OnQuit(wxCommandEvent& event) {
 DoUnloadFirmware();
 
 Close();
}

uint8_t Window::getPixelFromData(uint8_t x, uint8_t y, unsigned char* data) {
 return (data[(x & 127) + (((y & 63) >> 3) << 7)]) & (1 << (y & 7));
}

void Window::OnTimer(wxTimerEvent& event) {
 Refresh();
}

void Window::SaveScreenshot(wxCommandEvent& event) {
 int scale = 2;
 wxBitmap* lcd = new wxBitmap((scale * 128), (scale * 64));
 wxMemoryDC dc;
 dc.SelectObject(*lcd);
 DrawDisplay(dc, 2, 0, 0);
 dc.SelectObject(wxNullBitmap);
 lcd->SaveFile( _T("lcd.png"), wxBITMAP_TYPE_PNG, (wxPalette*)NULL);  
}

void Window::DrawDisplay(wxDC &dc, int scale, int off_x, int off_y) {
 int x, y;

 // Set line color to black, fill color to white
 dc.SetPen(wxPen(*wxBLACK, 1, wxSOLID));
 dc.SetBrush(wxBrush(wxColour(0, 0, 128), wxSOLID));
 dc.DrawRectangle(off_x - 4, off_y - 4, (128 * scale) + 8, (64 * scale) + 8);
  
 dc.SetBrush(wxBrush(*wxWHITE, wxSOLID));
 dc.SetPen(wxPen(*wxWHITE, 1, wxSOLID));
 
 if(firmware_loaded) {
  for(x = 0; x < 128; x++) {
    for(y = 0; y < 64; y++) {
      if(getPixelFromData(x, y, (unsigned char*)data)) {
        dc.DrawRectangle(off_x + x*scale, off_y + y*scale, scale, scale);
      }
    }
  }
 }
}

void Window::OnPaint(wxPaintEvent& event) {
 wxPaintDC dc(this);
 DrawDisplay(dc, 2, 8, 8); 
}

void Window::RotaryLeft(wxCommandEvent& event) {
  *io = *io - 0x010000;
}

void Window::RotaryRight(wxCommandEvent& event) {
  *io = *io + 0x010000;
}

void Window::RotaryOk(wxCommandEvent& event) {
  *io = *io | 1; 
}

void Window::BtnHome(wxCommandEvent& event) {
  *io = *io | (1 << 1);
}

void Window::BtnPlay(wxCommandEvent& event) {
  *io = *io | (1 << 2);
}

void Window::Btn1(wxCommandEvent& event) {
  *io = *io | (1 << 3);
}

void Window::Btn2(wxCommandEvent& event) {
  *io = *io | (1 << 4);
}

void Window::Btn3(wxCommandEvent& event) {
  *io = *io | (1 << 5);
}

void Window::Btn4(wxCommandEvent& event) {
  *io = *io | (1 << 6);
}

void Window::Btn1l(wxCommandEvent& event) {
  *io = *io | (1 << 11);
}

void Window::Btn2l(wxCommandEvent& event) {
  *io = *io | (1 << 12);
}

void Window::Btn3l(wxCommandEvent& event) {
  *io = *io | (1 << 13);
}

void Window::Btn4l(wxCommandEvent& event) {
  *io = *io | (1 << 14);
}

void Window::DoLoadFirmware(wxString path, wxString name) {
    if(pid != 0) {
     DoUnloadFirmware();
    }
    
    wxString image = path;
    
    
    // generate all file paths
    char* input_argv[2];
    char firmware[PATH_MAX];
    char real_path[PATH_MAX];
    strcpy(firmware, C_STR(image));
    realpath(firmware, real_path);
    strcpy(firmware, real_path);
    strcat(firmware, "/");
    strcat(firmware, C_STR(name));
    strcpy(SIM_PATH, real_path);
    strcat(SIM_PATH, "/lcd.sim");
    strcpy(SIM_PATH_IO, real_path);
    strcat(SIM_PATH_IO, "/io.sim");

    // fork and execute firmware
    input_argv[0] = firmware;
    input_argv[1] = NULL;
    printf("[ Load ] %s\n", input_argv[0]);
    
    pid = fork();
    if(pid == 0) { // child
      chdir(real_path);
      
      execvp(input_argv[0], input_argv);
      printf("[ Load ] Firmware unloaded!\r\n");
      exit(0);
    } else {
      // mmap lcd file
      fd = open(SIM_PATH, O_RDWR | O_CREAT | O_TRUNC, 0777);
      if(fd == -1) {
	perror("[ Load ] Error opening file 'lcd.sim' for writing");
	return;
      }
      printf("[ Load ] LCD opened\n");
    
      write(fd, "\0", 128*8); // write 4 0-byte to file
      data = (char*)mmap(0, 128*8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if(data == MAP_FAILED) {
	perror("[ Load ] Error mapping file 'lcd.sim'");
	return;
      }
      printf("[ Load ] LCD mapped\n");
    
      // mmap io file
      fd1 = open(SIM_PATH_IO, O_RDWR | O_CREAT | O_TRUNC, 0777);
      if(fd == -1) {
	perror("[ Load ] Error opening file 'io.sim' for writing");
	return;
      }
      printf("[ Load ] I/O opened\n");
    
      write(fd1, "\0\0\0\0", 4); // write 4 0-byte to file
      io = (uint32_t*)mmap(0, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
      if(io == MAP_FAILED) {
	perror("[ Load ] Error mapping file 'io.sim'");
	return;
      }  
      printf("[ Load ] I/O mapped\n");

      firmware_loaded = 1;
    }

}

void Window::DoUnloadFirmware() {
  if(!firmware_loaded) return;
  
  printf("[ Load ] Unloading firmware...\r\n");
  if(pid != 0) kill(pid, 2);
  pid = 0;
  
  munmap(data, 128*8);
  close(fd);
  munmap(io, 4);
  close(fd1);
  firmware_loaded = 0;
}


void Window::LoadFirmware(wxCommandEvent& event) {
  wxFileDialog* openFileDialog = new wxFileDialog(this, _("Select firmware"), _(""), _(""),  _("*"), wxOPEN | wxFILE_MUST_EXIST, wxDefaultPosition);

  if(openFileDialog->ShowModal() == wxID_OK) {
    wxString dir = openFileDialog->GetDirectory();
    wxString name = openFileDialog->GetFilename();
    delete openFileDialog;
    DoUnloadFirmware();
    DoLoadFirmware(dir, name);
  }
}

void Window::OnAbout(wxCommandEvent& event) {
  wxMessageBox(_("Raspberry Pi Webradio Simulator v1.0\r\nCopyright (C) 2012 by Michael Schwarz"), _("About"), wxOK);
}


Window::~Window()
{
        // Disconnect Events
        m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::SaveScreenshot ), NULL, this );
        m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryLeft ), NULL, this );
        m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryOk ), NULL, this );
        m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::RotaryRight ), NULL, this );
        m_button11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::BtnHome ), NULL, this );
        m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::BtnPlay ), NULL, this );
        m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn1 ), NULL, this );
        m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn2 ), NULL, this );
        m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn3 ), NULL, this );
        m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn4 ), NULL, this );
        m_button61->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn1l ), NULL, this );
        m_button71->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn2l ), NULL, this );
        m_button81->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn3l ), NULL, this );
        m_button91->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Window::Btn4l ), NULL, this );
        this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::LoadFirmware ) );
        this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::OnQuit ) );
        this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Window::OnAbout ) );
}


