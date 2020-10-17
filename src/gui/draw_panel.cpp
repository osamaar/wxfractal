#include <cmath>
#include "draw_panel.h"
#include "../common/observer.h"
#include "../core/turtle.h"

wxBEGIN_EVENT_TABLE(DrawPanel, wxPanel)
    EVT_PAINT(DrawPanel::on_paint)
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
{
    m_model->change_notifier.add_observer(m_lsys_observer);
}

DrawPanel::~DrawPanel() {
    m_model->change_notifier.remove_observer(m_lsys_observer);
}

void DrawPanel::on_paint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    // DrawPanel::draw_stuff(dc);
    draw_lsys(dc);
}

void DrawPanel::draw_lsys(wxDC& dc) {
    Turtle turtle;
    int w, h;
    this->GetClientSize(&w , &h);

    auto &alphabet = m_model->lsys.alphabet;
    auto &sequence = m_model->lsys.sequence;

    int dist = alphabet.at("F").data->get_value<int>();
    int angle_plus = alphabet.at("+").data->get_value<int>();
    int angle_minus = alphabet.at("-").data->get_value<int>();

    dc.Clear();
    dc.SetPen(wxPen(wxColour(255, 255, 255, 255)));

    double scale_factor = 1./std::pow(3., m_model->lsys.level);

    turtle.set_state({w/2. - dist/2., h/2. + dist*.5/2.}, 0);

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
    
    for (auto &&seg : trace) {
        dc.DrawLine(seg.p0.x, seg.p0.y, seg.p1.x, seg.p1.y);
    }

    dc.SetTextForeground(wxColour(255, 255, 255, 255));
    wxFont ft;
    ft.SetPointSize(15);
    dc.SetFont(ft);
    auto str = m_model->lsys.as_string();
    dc.DrawText(str, wxPoint(20, 20));
    str = std::to_string(m_model->lsys.sequence.size());
    dc.DrawText(str, wxPoint(20, 40));
}

void DrawPanel::update_lsys(int n) {
    Refresh();
}
