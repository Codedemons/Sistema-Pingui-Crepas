
#include "Librerias\GUI.h"

WndNodo* Pila;

int WINAPI WinMain(HINSTANCE hInstanciaAct,HINSTANCE hInstanciaPrev,LPSTR CmdLine,int CmdShow){
    MSG     Mensaje;
    int     Result = 0;
    HBRUSH  ColorDeFondo = NULL;
    HWND    Ventana = NULL;

    WndPila_Crear(&Pila);

    ColorDeFondo = CreateSolidBrush(RGB(0,200,100));	
	
    
	if(!RegistrarClaseEx(CS_DBLCLKS,hInstanciaAct,"PRINCIPAL",NULL,WndProcPrincipal,ColorDeFondo)){
	        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }
	
	if(!RegistrarClaseEx(CS_DBLCLKS,hInstanciaAct,"LOGIN",NULL,WndProcLogin,ColorDeFondo)){
	        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }
    if(!RegistrarClaseEx(CS_DBLCLKS,hInstanciaAct,"ADMIN",NULL,WndProcAdmin,(HBRUSH)GetStockObject(WHITE_BRUSH))){
        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }

    if(!RegistrarClaseEx(CS_DBLCLKS,hInstanciaAct,"VENTAS",NULL,WndProcVent,(HBRUSH)GetStockObject(WHITE_BRUSH))){
        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }

			
	if(!RegistrarClaseEx(CS_DBLCLKS,hInstanciaAct,"CONFIG",NULL,WndProcConfig,(HBRUSH)GetStockObject(WHITE_BRUSH))){
        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }

    Ventana = CreateWindowEx(0,"PRINCIPAL",AppName,WS_OVERLAPPEDWINDOW,100,100,300,350,HWND_DESKTOP,NULL,hInstanciaAct,NULL);
//    Ventana = CreateWindowEx(0,"PRINCIPAL",AppName,WS_OVERLAPPEDWINDOW,40,50,800,600,HWND_DESKTOP,NULL,hInstanciaAct,NULL);
    if(Ventana == NULL){
        MessageBox(NULL,"No se Pudo Iniciar la Aplicación","Error",MB_ICONERROR|MB_OK);
        goto fin;
    }
    
    
    WndPila_Insertar(&Pila,Ventana);
    ShowWindow(Ventana,CmdShow);
    UpdateWindow(Ventana);

    while((Result = GetMessage(&Mensaje,NULL,0,0)) !=0 ){
        if(Result < 0){
            ///MANEJAR ERROR
        }else{
            TranslateMessage(&Mensaje);
            DispatchMessage(&Mensaje);
        }
    }

    fin:
    DeleteObject(ColorDeFondo);
    return Mensaje.wParam;
}

