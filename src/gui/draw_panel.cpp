#include <cmath>
#include "draw_panel.h"
#include "../common/observer.h"
#include "../core/turtle.h"
// #include "controller.h"

wxBEGIN_EVENT_TABLE(DrawPanel, wxPanel)
    EVT_PAINT(DrawPanel::on_paint)
wxEND_EVENT_TABLE()

// DrawPanel::DrawPanel(wxWindow *parent, Model *model, Controller *controller):
DrawPanel::DrawPanel(wxWindow *parent, Model *model)
    : wxPanel(
        parent,
        wxID_ANY,
        wxDefaultPosition,
        wxDefaultSize,
        wxFULL_REPAINT_ON_RESIZE)
    , m_lsys_observer(&DrawPanel::on_lsys_changed, this)
    , m_model(model)
    // , m_controller(controller)
{
    m_model->subject.add_observer(m_lsys_observer);
}

void DrawPanel::on_paint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    // DrawPanel::draw_stuff(dc);
    draw_lsys(dc);
}

void DrawPanel::draw_stuff(wxDC &dc) {
    dc.Clear();
    dc.SetPen(wxPen(wxColour(255, 255, 255, 255)));
    // dc.DrawLine(wxPoint(0, 0), wxPoint(100, 100));

    int n = 3;

    int w, h;
    unsigned  int d, x1, y1, x2, y3, x3;
    this->GetClientSize(&w , &h);

    d = std::min(w, h) * .9;

    y1 = .5*h + .25*d;
    y3 = .5*h - .5*d;
    x1 = .5*w - .25*d*sqrt(3.);
    x2 = .5*w + .25*d*sqrt(3.);
    x3 = .5*w;

    draw_edge(dc, n, x1, y1, x2, y1);
    draw_edge(dc, n, x2, y1, x3, y3);
    draw_edge(dc, n, x3, y3, x1, y1);
}

void DrawPanel::draw_edge(wxDC &dc, unsigned int n, int x1, int y1, int x2, int y2) {
    using namespace std;

    if(n > 0) {
        int x3 , y3 , x4 , y4 , x5 , y5 ;
        x3 = 2.*x1/3. + x2/3.;
        y3 = 2.*y1/3. + y2/3.;

        draw_edge(dc , n-1, x1, y1, x3, y3);
        x4 = x1/3. + 2.*x2/3.;
        y4 = y1/3. + 2.*y2/3.;
        x5 = .5*(x1+x2) - (y2 - y1)*std::sqrt(3.)/6.;
        y5 = .5*(y1+y2) + (x2 - x1)*std::sqrt(3.)/6.;

        draw_edge(dc, n-1,x3, y3, x5, y5) ;
        draw_edge(dc, n-1,x5, y5, x4, y4) ;
        draw_edge(dc, n-1,x4, y4, x2, y2) ;
    } else {
        dc.DrawLine(x1, y1, x2, y2);
    }
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
}

void DrawPanel::on_lsys_changed(int n) {
    Refresh();
}
