#include "main_gui.h"
#include "main_frame.h"

bool MainGUI::OnInit() {
    MainFrame *frame = new MainFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}