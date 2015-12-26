// generated by Fast Light User Interface Designer (fluid) version 1.0107

#include "BrushInterface.h"

void BrushInterface::cb_Constant_i(Fl_Menu_*, void*) {
  myBrush.changedBrush();
}
void BrushInterface::cb_Constant(Fl_Menu_* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_Constant_i(o,v);
}

void BrushInterface::cb_Linear_i(Fl_Menu_*, void*) {
  myBrush.changedBrush();
}
void BrushInterface::cb_Linear(Fl_Menu_* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_Linear_i(o,v);
}

void BrushInterface::cb_Quadratic_i(Fl_Menu_*, void*) {
  myBrush.changedBrush();
}
void BrushInterface::cb_Quadratic(Fl_Menu_* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_Quadratic_i(o,v);
}

void BrushInterface::cb_Gaussian_i(Fl_Menu_*, void*) {
  myBrush.changedBrush();
}
void BrushInterface::cb_Gaussian(Fl_Menu_* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_Gaussian_i(o,v);
}

Fl_Menu_Item BrushInterface::menu_m_iBrushType[] = {
 {"Constant", 0,  (Fl_Callback*)BrushInterface::cb_Constant, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Linear", 0,  (Fl_Callback*)BrushInterface::cb_Linear, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Quadratic", 0,  (Fl_Callback*)BrushInterface::cb_Quadratic, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {"Gaussian", 0,  (Fl_Callback*)BrushInterface::cb_Gaussian, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

void BrushInterface::cb_m_iRadius_i(Fl_Value_Slider*, void*) {
  myBrush.changedBrush( );
}
void BrushInterface::cb_m_iRadius(Fl_Value_Slider* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_m_iRadius_i(o,v);
}

Fl_Menu_Item BrushInterface::menu_m_iToolType[] = {
 {"Brush", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Line", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Circle", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Polygon", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Filter", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

Fl_Menu_Item BrushInterface::menu_m_iFilterType[] = {
 {"Blur", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Sharpen", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {"Edge", 0,  0, 0, 4, FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

void BrushInterface::cb_m_dRed_i(Fl_Value_Slider*, void*) {
  SetPreview();
}
void BrushInterface::cb_m_dRed(Fl_Value_Slider* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_m_dRed_i(o,v);
}

void BrushInterface::cb_m_dGreen_i(Fl_Value_Slider*, void*) {
  SetPreview();
}
void BrushInterface::cb_m_dGreen(Fl_Value_Slider* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_m_dGreen_i(o,v);
}

void BrushInterface::cb_m_dBlue_i(Fl_Value_Slider*, void*) {
  SetPreview();
}
void BrushInterface::cb_m_dBlue(Fl_Value_Slider* o, void* v) {
  ((BrushInterface*)(o->parent()->user_data()))->cb_m_dBlue_i(o,v);
}

Fl_Double_Window* BrushInterface::make_window() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = m_brushWindow = new Fl_Double_Window(480, 220, "Brush UI");
    w = o;
    o->user_data((void*)(this));
    { Fl_Choice* o = m_iBrushType = new Fl_Choice(5, 25, 180, 30, "Brush Type");
      o->down_box(FL_BORDER_BOX);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_m_iBrushType);
    }
    { Fl_Value_Slider* o = m_iRadius = new Fl_Value_Slider(5, 78, 180, 30, "Radius");
      o->type(5);
      o->minimum(1);
      o->maximum(64);
      o->step(1);
      o->value(4);
      o->callback((Fl_Callback*)cb_m_iRadius);
      o->align(FL_ALIGN_TOP_LEFT);
    }
    { Fl_Value_Slider* o = m_dPixelFlow = new Fl_Value_Slider(5, 132, 180, 30, "Pixel flow");
      o->type(5);
      o->value(0.5);
      o->align(FL_ALIGN_TOP_LEFT);
    }
    { Fl_Choice* o = m_iToolType = new Fl_Choice(5, 185, 180, 30, "Tool");
      o->down_box(FL_BORDER_BOX);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_m_iToolType);
    }
    { Fl_Choice* o = m_iFilterType = new Fl_Choice(205, 185, 180, 30, "Filter");
      o->down_box(FL_BORDER_BOX);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_m_iFilterType);
    }
    { Fl_Value_Slider* o = m_dRed = new Fl_Value_Slider(205, 25, 35, 140, "R");
      o->type(4);
      o->box(FL_BORDER_BOX);
      o->value(0.5);
      o->callback((Fl_Callback*)cb_m_dRed);
      o->align(FL_ALIGN_TOP);
    }
    { Fl_Value_Slider* o = m_dGreen = new Fl_Value_Slider(256, 25, 35, 140, "G");
      o->type(4);
      o->box(FL_BORDER_BOX);
      o->value(0.5);
      o->callback((Fl_Callback*)cb_m_dGreen);
      o->align(FL_ALIGN_TOP);
    }
    { Fl_Value_Slider* o = m_dBlue = new Fl_Value_Slider(308, 25, 35, 140, "B");
      o->type(4);
      o->box(FL_BORDER_BOX);
      o->value(0.5);
      o->callback((Fl_Callback*)cb_m_dBlue);
      o->align(FL_ALIGN_TOP);
    }
    { Fl_Box* o = m_boxPreview = new Fl_Box(360, 45, 115, 120, "Preview");
      o->box(FL_DOWN_BOX);
      o->color((Fl_Color)80);
      o->align(FL_ALIGN_TOP);
    }
    o->end();
    o->resizable(o);
  }
  return w;
}

BrushInterface::BrushInterface() {
  m_brushWindow = make_window();
myBrush.setUI( this );
myBrush.changedBrush();
SetPreview();
}

void BrushInterface::SetPreview() {
  Fl_Color c = fl_rgb_color( (unsigned char) (255.0 * m_dRed->value()),
                           (unsigned char) (255.0 * m_dGreen->value()),
                           (unsigned char) (255.0 * m_dBlue->value()) );
m_boxPreview->color( c );
m_boxPreview->redraw();
}

MyBrush::BrushType BrushInterface::getBrushType() const {
  return (MyBrush::BrushType) m_iBrushType->value();
}

MyBrush::FilterType BrushInterface::getFilterType() const {
  return (MyBrush::FilterType) m_iFilterType->value();
}

int BrushInterface::getRadius() const {
  return (int) m_iRadius->value();
}

MyBrush::ToolType BrushInterface::getToolType() const {
  return (MyBrush::ToolType) m_iToolType->value();
}

Color BrushInterface::getColor() const {
  return Color( static_cast<float>(m_dRed->value()),
                static_cast<float>(m_dGreen->value()),
                static_cast<float>(m_dBlue->value()) );
}

float BrushInterface::getFlow() const {
  return (float) m_dPixelFlow->value();
}

void BrushInterface::loadImage( Fl_Image *image ) {
  myBrush.loadImage(image);
}

UIInterface * BrushInterface::getUI() {
  return &myBrush;
}
