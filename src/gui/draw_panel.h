#include <wx/wx.h>
#include "../common/observer.h"
#include "../core/model.h"

class DrawPanel : public wxPanel {
public:
    DrawPanel(wxWindow *parent, Model *model);
    ~DrawPanel();

    void on_paint(wxPaintEvent& event);
    void draw_lsys(wxDC& dc);
    void on_lmb_down(wxMouseEvent& event);
    void on_lmb_up(wxMouseEvent& event);
    void on_motion(wxMouseEvent& event);
    void on_mouse_wheel(wxMouseEvent& event);
    void on_mouse_enter(wxMouseEvent& event);
    void on_mouse_leave(wxMouseEvent& event);

    bool AcceptsFocus() const {return true;}

    void update_lsys(int n);
    void draw_grid(wxDC& dc, int step, wxColor color);
    wxPoint DrawPanel::screen_to_model_coord(int x, int y);

    DECLARE_EVENT_TABLE()
private:
    Model *m_model;
    Observer<int> m_lsys_observer;
    wxPoint m_drag_origin;
    wxPoint m_graph_offset;
    bool m_dragging;
    double m_zoom;
};

