#ifndef GUI_H
#define GUI_H

#include <windows.h>
#include "WndPila.h"

extern const char* AppName;
extern WndNodo* Pila;

BOOL RegistrarClaseEx(UINT Estilo,HINSTANCE Instancia,LPCSTR NombreClase,LPCSTR NombreMenu,WNDPROC WndProcedimiento,HBRUSH Color);
LRESULT CALLBACK WndProcLogin(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WndProcPrincipal(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WndProcAdmin(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WndProcVent(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WndProcConfig(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK DlgVerToppinsProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgVerBebidasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgEmpleadosProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgVerCrepasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgVentasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DlgVerEmpleadosProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
#endif // GUI_H
