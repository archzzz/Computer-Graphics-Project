// generated by Fast Light User Interface Designer (fluid) version 1.0107

#ifndef IntersectionInterface_h
#define IntersectionInterface_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "IntersectionUI.h"
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include "../shapes/ShapesUI.h"
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Value_Input.H>

class IntersectionInterface {
public:
  Fl_Double_Window* make_window();
  Fl_Double_Window *m_intersectionWindow;
  Fl_Choice *m_iShapeType;
  static Fl_Menu_Item menu_m_iShapeType[];
private:
  void cb_Sphere_i(Fl_Menu_*, void*);
  static void cb_Sphere(Fl_Menu_*, void*);
  void cb_Cone_i(Fl_Menu_*, void*);
  static void cb_Cone(Fl_Menu_*, void*);
  void cb_Cylinder_i(Fl_Menu_*, void*);
  static void cb_Cylinder(Fl_Menu_*, void*);
  void cb_Cube_i(Fl_Menu_*, void*);
  static void cb_Cube(Fl_Menu_*, void*);
  void cb_Bunny_i(Fl_Menu_*, void*);
  static void cb_Bunny(Fl_Menu_*, void*);
public:
  Fl_Value_Slider *m_dXAt;
private:
  void cb_m_dXAt_i(Fl_Value_Slider*, void*);
  static void cb_m_dXAt(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dYAt;
private:
  void cb_m_dYAt_i(Fl_Value_Slider*, void*);
  static void cb_m_dYAt(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dZAt;
private:
  void cb_m_dZAt_i(Fl_Value_Slider*, void*);
  static void cb_m_dZAt(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dTheta;
private:
  void cb_m_dTheta_i(Fl_Value_Slider*, void*);
  static void cb_m_dTheta(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dPhi;
private:
  void cb_m_dPhi_i(Fl_Value_Slider*, void*);
  static void cb_m_dPhi(Fl_Value_Slider*, void*);
  void cb_Write_i(Fl_Button*, void*);
  static void cb_Write(Fl_Button*, void*);
public:
  Fl_Value_Slider *m_dXRot;
private:
  void cb_m_dXRot_i(Fl_Value_Slider*, void*);
  static void cb_m_dXRot(Fl_Value_Slider*, void*);
public:
  Fl_Value_Slider *m_dYRot;
private:
  void cb_m_dYRot_i(Fl_Value_Slider*, void*);
  static void cb_m_dYRot(Fl_Value_Slider*, void*);
public:
  Fl_Check_Button *m_bGrid;
private:
  void cb_m_bGrid_i(Fl_Check_Button*, void*);
  static void cb_m_bGrid(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *m_bRay;
private:
  void cb_m_bRay_i(Fl_Check_Button*, void*);
  static void cb_m_bRay(Fl_Check_Button*, void*);
public:
  Fl_Check_Button *m_bRayShadow;
private:
  void cb_m_bRayShadow_i(Fl_Check_Button*, void*);
  static void cb_m_bRayShadow(Fl_Check_Button*, void*);
public:
  Fl_Value_Input *m_iSeed;
  IntersectionInterface();
private:
  IntersectionUI intersectionUI;
public:
  UIInterface * getUI();
  double getTheta() const;
  double getPhi() const;
  double getXRot() const;
  double getYRot() const;
};
#endif
