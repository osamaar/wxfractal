#include <wx/wx.h>
#include "../common/observer.h"
#include "../core/model.h"
// #include "controller.h"

class DrawPanel : public wxPanel {
public:
    DrawPanel(wxWindow *parent, Model *model);
    // DrawPanel(wxWindow *parent, Model *model, Controller *controller);

    void on_paint(wxPaintEvent& event);
    void draw_stuff(wxDC &dc);
    void draw_edge(wxDC &dc, unsigned int n, int x1, int y1, int x2, int y2);

    void draw_lsys(wxDC& dc);

    void on_lsys_changed(int n);

    DECLARE_EVENT_TABLE()
private:
    Model *m_model;
    // Controller *m_controller;
    Observer<int> m_lsys_observer;
};

