#include <wx/wx.h>
#include "../common/observer.h"
#include "../core/model.h"

class DrawPanel : public wxPanel {
public:
    DrawPanel(wxWindow *parent, Model *model);
    ~DrawPanel();

    void on_paint(wxPaintEvent& event);
    void draw_lsys(wxDC& dc);
    void update_lsys(int n);

    DECLARE_EVENT_TABLE()
private:
    Model *m_model;
    Observer<int> m_lsys_observer;
};

