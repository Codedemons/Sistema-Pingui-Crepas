#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <tchar.h>
#include <wchar.h>
#include <windowsx.h>
#include <commctrl.h>

#include "SQLite\sqlite3.h"

#ifndef _UNICODE
#define _UNICODE
#endif 

#define ID_LISTVIEW 800
#define ITEM_COUNT 10

HINSTANCE   g_hInst; //instancia para el listview
//Procedemientos necesarios para el lisview

/******************************************************************************
   CreateListView
******************************************************************************/
HWND CreateListView(HINSTANCE hInstance, HWND hwndParent)
{
DWORD       dwStyle;
HWND        hwndListView;
BOOL        bSuccess = TRUE;
dwStyle =   WS_TABSTOP | 
            WS_CHILD | 
            WS_BORDER | 
            WS_VISIBLE |
            LVS_AUTOARRANGE |
            LVS_REPORT | 
            LVS_OWNERDATA;            
hwndListView = CreateWindowEx(   WS_EX_CLIENTEDGE,          // ex style
                                 WC_LISTVIEW,               // class name - defined in commctrl.h
                                 TEXT(""),                        // dummy text
                                 dwStyle,                   // style
                                 0,                         // x position
                                 0,                         // y position
                                 0,                         // width
                                 0,                         // height
                                 hwndParent,                // parent
                                 (HMENU)ID_LISTVIEW,        // ID
                                 g_hInst,                   // instance
                                 NULL);                     // no extra data

if(!hwndListView)
   return NULL;
return hwndListView;
}

/******************************************************************************
   ResizeListView
******************************************************************************/

void ResizeListView(HWND hwndListView, HWND hwndParent)
{
RECT  rc;
GetClientRect(hwndParent, &rc);
MoveWindow( hwndListView, 
            rc.left,
            rc.top,
            rc.right - rc.left,
            rc.bottom - rc.top,
            TRUE);
//only call this if we want the LVS_NOSCROLL style
//PositionHeader(hwndListView);
}

/******************************************************************************
   InsertListViewItems
******************************************************************************/
BOOL InsertListViewItems(HWND hwndListView)
{
//empty the list
ListView_DeleteAllItems(hwndListView);
//set the number of items in the list
ListView_SetItemCount(hwndListView, ITEM_COUNT);
   LVITEM lvI;
    // Initialize LVITEM members that are common to all items.
    lvI.pszText   = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
    lvI.mask      = LVIF_TEXT | LVIF_IMAGE |LVIF_STATE;
    lvI.stateMask = 0;
    lvI.iSubItem  = 0;
    lvI.state     = 0;
    // Initialize LVITEM members that are different for each item.
    int index;
    //for ( index = 0; index < cItems; index++)
    for ( index = 0; index < ITEM_COUNT; index++)
    {
        lvI.iItem  = index;        
        // Insert items into the list.
        if (ListView_InsertItem(hwndListView, &lvI) == -1)
            return FALSE;
    }
    return TRUE;
return TRUE;
}

/**************************************************************************
   ListViewNotify()
**************************************************************************/

LRESULT ListViewNotify(HWND hWnd, LPARAM lParam)
{
LPNMHDR  lpnmh = (LPNMHDR) lParam;
HWND   hwndListView = GetDlgItem(hWnd, ID_LISTVIEW);
switch(lpnmh->code)
   {
   case LVN_GETDISPINFO:
      {
      LV_DISPINFO *lpdi = (LV_DISPINFO *)lParam;
      TCHAR szString[MAX_PATH];
      if(lpdi->item.iSubItem)
         {
         if(lpdi->item.mask & LVIF_TEXT)
            {
            }
         }
      else
         {
         if(lpdi->item.mask & LVIF_TEXT)
            {
            }
         }
      }
      return 0;
   case LVN_ODCACHEHINT:
      {
      LPNMLVCACHEHINT   lpCacheHint = (LPNMLVCACHEHINT)lParam;
      /*
      This sample doesn't use this notification, but this is sent when the 
      ListView is about to ask for a range of items. On this notification, 
      you should load the specified items into your local cache. It is still 
      possible to get an LVN_GETDISPINFO for an item that has not been cached, 
      therefore, your application must take into account the chance of this 
      occurring.
      */
      }
      return 0;
   case LVN_ODFINDITEM:
      {
      LPNMLVFINDITEM lpFindItem = (LPNMLVFINDITEM)lParam;
      /*
      This sample doesn't use this notification, but this is sent when the 
      ListView needs a particular item. Return -1 if the item is not found.
      */
		  }
      return 0;
   }
return 0;
}

/******************************************************************************
   ListView para los toppings
******************************************************************************/

/******************************************************************************
   InitListView Toppins
******************************************************************************/

BOOL InitListViewToppins(HWND hwndListView)
{
LV_COLUMN   lvColumn;
int         i;
TCHAR       szString[4][20] = {TEXT("Clave"), TEXT("Nombre"), TEXT("Descripcion"), TEXT("Precio")};
//empty the list
ListView_DeleteAllItems(hwndListView);
//initialize the columns
lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvColumn.fmt = LVCFMT_LEFT;
lvColumn.cx = 120;
for(i = 0; i < 4; i++)
   {
   lvColumn.pszText = szString[i];
   ListView_InsertColumn(hwndListView, i, &lvColumn);
   }
InsertListViewItems(hwndListView);
return TRUE;
}

//Estrutura para guardar los datos
 struct TOPPINS
        {
            char clave[4];
            char nombre[40];
            char descripcion[75];
            char precio[20];
        }rgToppinsInfo[100];
        
/******************************************************************************
   InitListView Bebidas
******************************************************************************/
BOOL InitListViewBebidas(HWND hwndListView)
{
LV_COLUMN   lvColumn;
int         i;
TCHAR       szString[4][20] = {TEXT("Clave"), TEXT("Nombre"), TEXT("Descripcion"), TEXT("Precio")};
//empty the list
ListView_DeleteAllItems(hwndListView);
//initialize the columns
lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvColumn.fmt = LVCFMT_LEFT;
lvColumn.cx = 120;
for(i = 0; i < 4; i++)
   {
   lvColumn.pszText = szString[i];
   ListView_InsertColumn(hwndListView, i, &lvColumn);
   }
InsertListViewItems(hwndListView);
return TRUE;
}

 struct BEBIDAS
        {
            char clave[10];
            char nombre[20];
            char descripcion[40];
            char precio[20];
        }rgBebidasInfo[100];

/******************************************************************************
   InitListView Crepas
******************************************************************************/

BOOL InitListView(HWND hwndListView)
{
LV_COLUMN   lvColumn;
int         i;
TCHAR       szString[5][20] = {TEXT("Clave"), TEXT("Nombre"), TEXT("Tipo"), TEXT("Descripcion"), TEXT("Precio")};

//empty the list
ListView_DeleteAllItems(hwndListView);

//initialize the columns
lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvColumn.fmt = LVCFMT_LEFT;
lvColumn.cx = 120;
for(i = 0; i < 5; i++)
   {
   lvColumn.pszText = szString[i];
   ListView_InsertColumn(hwndListView, i, &lvColumn);
   }

InsertListViewItems(hwndListView);

return TRUE;
}

 struct CREPAS
        {
            char clave[10];
            char nombre[20];
            char tipo[20];
            char descripcion[40];
            char precio[20];
        }rgCrepaInfo[100];
        

/******************************************************************************
   InitListView Ventas
******************************************************************************/

BOOL InitListViewVentas(HWND hwndListView)
{
LV_COLUMN   lvColumn;
int         i;
TCHAR       szString[5][20] = {TEXT("Clave"), TEXT("Producto"), TEXT("Fecha"), TEXT("Hora"), TEXT("Usuario")};
//TCHAR       szString[5][20] = {TEXT("Clave"), TEXT("Nombre"), TEXT("Precio"), TEXT("Fecha"), TEXT("Hora")};
//empty the list
ListView_DeleteAllItems(hwndListView);

//initialize the columns
lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvColumn.fmt = LVCFMT_LEFT;
lvColumn.cx = 120;
for(i = 0; i < 5; i++)
   {
   lvColumn.pszText = szString[i];
   ListView_InsertColumn(hwndListView, i, &lvColumn);
   }

InsertListViewItems(hwndListView);

return TRUE;
}

 struct VENTAS
        {
            char clave[10];
            char nombre[20];
            char descripcion[40];
            char precio[20];
            char usuario[20];
        }rgVentaslistInfo[100];


/******************************************************************************
   InitListView Empleados
******************************************************************************/

BOOL InitListViewEmpleados(HWND hwndListView)
{
LV_COLUMN   lvColumn;
int         i;
TCHAR       szString[5][20] = {TEXT("Clave"), TEXT("Nombre"), TEXT("Pseudonimo"), TEXT("Password"), TEXT("Cargo")};

//empty the list
ListView_DeleteAllItems(hwndListView);

//initialize the columns
lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
lvColumn.fmt = LVCFMT_LEFT;
lvColumn.cx = 120;
for(i = 0; i < 5; i++)
   {
   lvColumn.pszText = szString[i];
   ListView_InsertColumn(hwndListView, i, &lvColumn);
   }

InsertListViewItems(hwndListView);

return TRUE;
}

 struct EMPLEADOS
        {
            char curpUsuario[10];
            char nombre[40];
            char pseudonimo[40];
            char password[40];
            char cargo[40];
        }rgEmpleadosInfo[100];
        


#endif // CONEXIONBD_H
