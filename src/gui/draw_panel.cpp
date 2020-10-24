#include <cmath>
#include "draw_panel.h"
#include "../common/observer.h"
#include "../core/turtle.h"
#include <wx/dcbuffer.h>

wxBEGIN_EVENT_TABLE(DrawPanel, wxPanel)
    EVT_PAINT(DrawPanel::on_paint)
    EVT_LEFT_DOWN(DrawPanel::on_lmb_down)
    EVT_LEFT_UP(DrawPanel::on_lmb_up)
    EVT_MOTION(DrawPanel::on_motion)
    EVT_MOUSEWHEEL(DrawPanel::on_mouse_wheel)
    EVT_ENTER_WINDOW(DrawPanel::on_mouse_enter)
    EVT_LEAVE_WINDOW(DrawPanel::on_mouse_leave)
wxEND_EVENT_TABLE()

DrawPanel::DrawPanel(wxWindow *parent, Model *model)
    : wxPanel(
        parent,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxFULL_REPAINT_ON_RESIZE)
    , m_lsys_observer(&DrawPanel::update_lsys, this)
    , m_model(model)
    , m_drag_origin()
    , m_graph_offset()
    , m_dragging(false)
    , m_zoom(1.)
{
    m_model->change_notifier.add_observer(m_lsys_observer);
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawPanel::~DrawPanel() {
    m_model->change_notifier.remove_observer(m_lsys_observer);
}

void DrawPanel::on_paint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    draw_lsys(dc);
}

void DrawPanel::draw_lsys(wxDC& dc) {
    Turtle turtle;
    int w, h;
    this->GetClientSize(&w , &h);
    int ox = w/2 + m_graph_offset.x;
    int oy = h/2 + m_graph_offset.y;
    double sx = m_zoom;
    double sy = m_zoom;

    auto &alphabet = m_model->lsys.alphabet;
    auto &sequence = m_model->lsys.sequence;

    int dist = alphabet.at("F").data->get_value<int>();
    int angle_plus = alphabet.at("+").data->get_value<int>();
    int angle_minus = alphabet.at("-").data->get_value<int>();

    dc.Clear();

    double scale_factor = 1./std::pow(3., m_model->lsys.level);

    turtle.set_state({0, 0}, 0);

    for (auto &&elem : sequence) {
        if (elem == "F") {
            turtle.forward(dist*scale_factor);
        } else if (elem == "+") {
            turtle.rotate(angle_plus);
        } else if (elem == "-") {
            turtle.rotate(angle_minus);
        }
    }

    auto trace = turtle.trace();
    
    draw_grid(dc, 10, wxColor(50, 50, 50, 255));
    draw_grid(dc, 100, wxColor(64, 64, 64, 255));

    // Axes
    dc.SetPen(wxPen(wxColour(128, 128, 128, 255)));
    dc.DrawLine(0, oy, w, oy);
    dc.DrawLine(ox, 0, ox, h);

    // Shape
    dc.SetPen(wxPen(wxColour(255, 255, 255, 255)));
    for (auto &&seg : trace) {
        dc.DrawLine(
            ox + sx*seg.p0.x,
            oy + sy*seg.p0.y,
            ox + sx*seg.p1.x,
            oy + sy*seg.p1.y
        );
    }
}

void DrawPanel::on_lmb_down(wxMouseEvent& event) {
    m_drag_origin = event.GetPosition();
    m_dragging = true;
}

void DrawPanel::on_lmb_up(wxMouseEvent& event) {
    m_dragging = false;
    Refresh();
}

void DrawPanel::on_motion(wxMouseEvent& event) {
    if (!m_dragging) return;

    wxPoint pos = event.GetPosition();
    m_graph_offset += pos - m_drag_origin;
    m_drag_origin = pos;
    Refresh();
}

void DrawPanel::on_mouse_wheel(wxMouseEvent& event) {
    auto mouse_offset = screen_to_model_coord(event.GetX(), event.GetY());
    double zmulti = .2;
    double factor = 1. + (event.GetWheelRotation() > 0 ? zmulti : -zmulti);
    auto m_zoom_old = m_zoom;
    m_zoom *= factor;
    // m_zoom = std::max(m_zoom, .01);
    m_graph_offset -= mouse_offset*(m_zoom - m_zoom_old);

    Refresh();
}

void DrawPanel::on_mouse_enter(wxMouseEvent& event) {
    SetFocus();
}

void DrawPanel::on_mouse_leave(wxMouseEvent& event) {
    m_dragging = false;
    Refresh();
}

void DrawPanel::update_lsys(int n) {
    Refresh();
}

void DrawPanel::draw_grid(wxDC& dc, int step, wxColor color) {
    int w, h;
    this->GetClientSize(&w , &h);
    int ox = w/2 + m_graph_offset.x;
    int oy = h/2 + m_graph_offset.y;

    dc.SetPen(wxPen(color));
    int x_start = (-w/2 - m_graph_offset.x)/m_zoom;
    int x_end = (w/2 - m_graph_offset.x)/m_zoom;
    int y_start = (-h/2 - m_graph_offset.y)/m_zoom;
    int y_end = (h/2 - m_graph_offset.y)/m_zoom;

    for (int x = 0; x < x_end; x += step) {
        int xx = x*m_zoom + ox;
        dc.DrawLine(xx , 0, xx, h);
    }

    for (int x = 0; x >= x_start; x -= step) {
        int xx = x*m_zoom + ox;
        dc.DrawLine(xx , 0, xx, h);
    }

    for (int y = 0; y < y_end; y += step) {
        int yy = y*m_zoom + oy;
        dc.DrawLine(0, yy, w, yy);
    }

    for (int y = 0; y >= y_start; y -= step) {
        int yy = y*m_zoom + oy;
        dc.DrawLine(0, yy, w, yy);
    }
}

wxPoint DrawPanel::screen_to_model_coord(int x, int y) {
    int w, h;
    this->GetClientSize(&w , &h);
    return wxPoint(
        (x - m_graph_offset.x - w/2)/m_zoom,
        (y - m_graph_offset.y - h/2)/m_zoom
    );
}