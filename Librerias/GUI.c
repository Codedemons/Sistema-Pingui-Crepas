//Libreias a instalar para usos del sistema
#include "GUI.h"
#include "WndPila.h"
#include "api.h"
#include "Conexion_BD.h"
#include "Listview.h"
#include "SQLite\sqlite3.h"

//Librerias por defecto para llamar funciones
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

//definiciones de la ventana principal
#define btn_admin 101
#define btn_vent 102
#define btn_config 103

//Definiciones de la ventana login 
#define  btn_login 20
#define  edit_user 21
#define  edit_key 22

//Definciones de la ventana Procedimientos
#define btn_crepas 104
#define btn_bebidas 105
#define btn_toppings 106
#define btn_empleados 107
#define btn_Ventas 600


// definiciones para el cuadro de dialogo de procedimientos de crepas
#define btn_buscar_crepa  200
#define btn_todo_crepa  198
#define btn_Limpiar_crepa 702
#define btn_Agregar_crepa  201
#define btn_Modificar_crepa  202
#define btn_Eliminar_crepa  199
#define edit_Clave_crepa  203
#define edit_Nombre_crepa  204
#define edit_Tipo_crepa  205
#define edit_Descripcion_crepa  206
#define edit_Precio_crepa  207

// definiciones para el cuadro de dialogo de procedimientos de crepas bebidas
#define btn_buscar_bebidas  210
#define btn_todo_bebidas	208
#define btn_Limpiar_bebidas 701
#define btn_Eliminar_bebidas  209
#define btn_Agregar_bebidas  211
#define btn_Modificar_bebidas  212
#define edit_Clave_bebidas  213
#define edit_Nombre_bebidas  214
#define edit_Descripcion_bebidas  215
#define edit_Precio_bebidas  216

// definiciones para el cuadro de dialogo de procedimientos de crepas empleados
#define btn_buscar_empleados  280
#define btn_todo_empleados 281
#define btn_Limpiar_empleados 282
#define btn_Eliminar_empleados  283
#define btn_Agregar_empleados  284
#define btn_Modificar_empleados  285
#define edit_Usuario_empleados  286
#define edit_Nombre_empleados  287
#define edit_pseudonimo_empleados  288
#define edit_password_empleados  289
#define edit_cargo_empleados  290

// definiciones para el cuadro de dialogo de procedimientos de crepas Toppins
#define btn_buscar_toppins  220
#define btn_todo_toppins 300
#define btn_Limpiar_toppins 700
#define btn_Eliminar_toppins  219
#define btn_Agregar_toppins  221
#define btn_Modificar_toppins  222
#define edit_Clave_toppins  223
#define edit_Nombre_toppins  224
#define edit_Descripcion_toppins  225
#define edit_Precio_toppins  226

//Definiciones para los procedimientos de ventas
#define Combo_Venta_busqueda 400
#define bnt_Venta_Agregar 401
#define bnt_Venta_Compra 402
#define bnt_Venta_Limpiar 403
#define static_Venta_Total 404
#define static_Venta_Cambio 405
#define edit_Venta_Monto 406
#define list_Venta_Producto 407
#define list_Venta_Precio 408

//Titulo de la ventana Principal
const char* AppName = "Pingui-Control";

//muestra el cargo del usuario
char cargo[40] = "";
char curpUsuario[40] = "";

//Muestra el login
LRESULT CALLBACK WndProcPrincipal(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam){
    static HINSTANCE Instancia = NULL;
    char  *admin = "Admin";
    char *empleado = "Empleado";
    switch(Mensaje){
        case WM_CREATE:{
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;
            //Controles de la ventana
            //Labels
            CreateWindowEx(0,"STATIC","Usuario",SS_LEFT|WS_CHILD|WS_VISIBLE,43,53,65,22,Ventana,(HMENU)-1,Instancia,NULL);            
            CreateWindowEx(0,"STATIC","Contraseña",SS_LEFT|WS_CHILD|WS_VISIBLE,43,125,98,22,Ventana,(HMENU)-1,Instancia,NULL);            
			//Cuadros de dialogo
			CreateWindowEx(0,"EDIT","",ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE|WS_BORDER,43,80,200,20,Ventana,(HMENU)edit_user,Instancia,NULL);            			
			CreateWindowEx(0,"EDIT","",ES_AUTOHSCROLL|ES_PASSWORD|ES_LEFT|WS_CHILD|WS_VISIBLE|WS_BORDER,43,150,200,20,Ventana,(HMENU)edit_key,Instancia,NULL);            			
            //Botones
			CreateWindowEx(0,"BUTTON","Ingresar",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,43,199,200,40,Ventana,(HMENU)btn_login,Instancia,NULL);            
            break;
        }
        case WM_COMMAND:{
            switch(LOWORD(wParam)){                
                case btn_login:{
                	//Variables a usar
                	sqlite3 *db;
					sqlite3_stmt *ppStmt;
					char *sql;
					const char* data = "Callback function called";	
					char *zErrMsg = 0, output[300] = "";
					float precioCrepa;
					char precioCrepas[20] = "";
					int rc;		
					rc = sqlite3_open("PinguiBase.db", &db);	
                	char User[40] = "";
                	char key[40] = "";
                	char keys[40] = "";					
					int band = 0;
					Open_BD(rc,db,Ventana);  											 										
					GetDlgItemText(Ventana,edit_user,User,40);								                	
					GetDlgItemText(Ventana,edit_key,key,40);								                	
					//MessageBox(Ventana,User,"Usuario",MB_OK);
					//MessageBox(Ventana,key,"Key",MB_OK);
					sprintf(output, "SELECT * FROM usuario WHERE pseudonimoUsuario LIKE '%s';", User);   					   					
   					sql = output;   					   						
   					//MessageBox(Ventana,sql,"Consulta",MB_OK);
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
					if(rc != SQLITE_OK){
      						MessageBox(Ventana,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(Ventana,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);    					
						//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
    						strncpy(keys,(const char*)sqlite3_column_text(ppStmt,3),40);							
    						strncpy(cargo,(const char*)sqlite3_column_text(ppStmt,4),40);							
    						strncpy(curpUsuario,(const char*)sqlite3_column_text(ppStmt,0),40);							
    					}					
						sqlite3_finalize(ppStmt);											
				   	}   	
   					sqlite3_close(db);  					
                    //MessageBox(Ventana,cargo,"SQL error",MB_ICONERROR);
                    //MessageBox(Ventana,keys,"SQL error",MB_ICONERROR);
					
					if(!strncmp(key,keys,40))
					{
						//MessageBox(Ventana,"Cadena iguales","SQL error",MB_ICONERROR);	
						band = 1;
					}else{
						 //MessageBox(Ventana,"Cadenas Diferentes","SQL error",MB_ICONERROR);
						 band = 0;
					}
					
					if(band == 1){
						if(!strncmp(cargo,admin,40))
							{									
								HWND Wnd = CreateWindowEx(0,"LOGIN","Interfaz",WS_OVERLAPPEDWINDOW,100,100,800,600,Ventana,NULL,Instancia,NULL);
			                    EnableWindow(WndPila_Cima(Pila),FALSE);
            			        WndPila_Insertar(&Pila,Wnd);
                    			ShowWindow(Wnd,SW_NORMAL);
							}else if(!strncmp(cargo,empleado,40)){
								HWND Wnd = CreateWindowEx(0,"VENTAS","Ventas",WS_OVERLAPPEDWINDOW,100,100,680,420,Ventana,NULL,Instancia,NULL);
                    			EnableWindow(WndPila_Cima(Pila),FALSE);
                    			WndPila_Insertar(&Pila,Wnd);
                    			ShowWindow(Wnd,SW_NORMAL);          
								//limpiar los edit					  					    										
								SetDlgItemText(Ventana,edit_user,"");							
								SetDlgItemText(Ventana,edit_key,"");							
							}							
							else{
						 		MessageBox(Ventana,"Error de ingreso","SQL error",MB_ICONERROR);
						 
							}										
					}else{
							MessageBox(Ventana,"Contraseña Incorrecta","Contraseña",MB_ICONERROR);
							//limpiar los edit					  					    										
							SetDlgItemText(Ventana,edit_user,"");							
							SetDlgItemText(Ventana,edit_key,"");
							//limpiar las variables					  					    										
							memset(User,0,40);
							memset(key,0,40);
							memset(keys,0,40);
							memset(cargo,0,40);						
			
					}					
					
					 if(User[0] == '\0' || key[0]== '\0'){
   						MessageBox(Ventana,"No deje los campos vacios","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   //else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);}  							    																					
                    break;
                }
            }
            break;
        }
        case WM_CLOSE:{
            WndPila_Vaciar(&Pila);
            PostQuitMessage(0);
            break;
        }
        default:{
            return DefWindowProc(Ventana,Mensaje,wParam,lParam);
        }
    }
    return 0;
}

//Ventana principal
LRESULT CALLBACK WndProcLogin(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam){
    static HINSTANCE Instancia = NULL;

    switch(Mensaje){
        case WM_CREATE:{
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;
            //Controles de la ventana
            //Botones
            CreateWindowEx(0,"BUTTON","REGISTRAR, CONSULTAR TODO, CONSULTAR, MODIFICAR, ELIMINAR",BS_MULTILINE|WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,75,33,300,200,Ventana,(HMENU)btn_admin,Instancia,NULL);
            CreateWindowEx(0,"BUTTON","VENTA",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,438,33,300,200,Ventana,(HMENU)btn_vent,Instancia,NULL);
            CreateWindowEx(0,"BUTTON","CREDITOS",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,75,312,300,200,Ventana,(HMENU)btn_config,Instancia,NULL);
            break;
        }
        case WM_COMMAND:{
            switch(LOWORD(wParam)){
                case btn_admin:{
                    HWND Wnd = CreateWindowEx(0,"ADMIN","Administracciones",WS_OVERLAPPEDWINDOW,100,100,680,420,Ventana,NULL,Instancia,NULL);
                    EnableWindow(WndPila_Cima(Pila),FALSE);
                    WndPila_Insertar(&Pila,Wnd);
                    ShowWindow(Wnd,SW_NORMAL);
                    break;
                }
                case btn_vent:{
                    HWND Wnd = CreateWindowEx(0,"VENTAS","Ventas",WS_OVERLAPPEDWINDOW,100,100,680,420,Ventana,NULL,Instancia,NULL);
                    EnableWindow(WndPila_Cima(Pila),FALSE);
                    WndPila_Insertar(&Pila,Wnd);
                    ShowWindow(Wnd,SW_NORMAL);
                    break;
                }
				case btn_config:{
                    HWND Wnd = CreateWindowEx(0,"CONFIG","Configuraciones",WS_OVERLAPPEDWINDOW,100,100,680,420,Ventana,NULL,Instancia,NULL);
                    EnableWindow(WndPila_Cima(Pila),FALSE);
                    WndPila_Insertar(&Pila,Wnd);
                    ShowWindow(Wnd,SW_NORMAL);
                    break;
                }
            }
            break;
        }
        case WM_CLOSE:{
            WndPila_Vaciar(&Pila);
            PostQuitMessage(0);
            break;
        }
        default:{
            return DefWindowProc(Ventana,Mensaje,wParam,lParam);
        }
    }
    return 0;
}

//Ventanas de manipulacion Principal
LRESULT CALLBACK WndProcAdmin(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam){
    static HINSTANCE Instancia = NULL;
	switch(Mensaje){
    	case WM_CREATE:{
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;
            CreateWindowEx(0,"BUTTON","Crepas",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,66,33,200,100,Ventana,(HMENU)btn_crepas,Instancia,NULL);
            CreateWindowEx(0,"BUTTON","Bebibas",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,386,33,200,100,Ventana,(HMENU)btn_bebidas,Instancia,NULL);
            CreateWindowEx(0,"BUTTON","Toppings",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,66,252,200,100,Ventana,(HMENU)btn_toppings,Instancia,NULL);
            CreateWindowEx(0,"BUTTON","Ventas",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,386,252,200,100,Ventana,(HMENU)btn_Ventas,Instancia,NULL);			
            CreateWindowEx(0,"BUTTON","Empleado",WS_CHILD|WS_VISIBLE|WS_BORDER|BS_PUSHBUTTON,222,144,200,100,Ventana,(HMENU)btn_empleados,Instancia,NULL);			
			break;
        }
    	case WM_COMMAND:{
    			switch(LOWORD(wParam)){
    				case btn_crepas:{
						DialogBox(Instancia,"DlgCrepas",Ventana,DlgCrepasProc);
					break;
					}
					case btn_bebidas:{
						DialogBox(Instancia,"DlgBebidas",Ventana,DlgBebidasProc);
					break;
					}
					case btn_toppings:{
						DialogBox(Instancia,"DlgToppins",Ventana,DlgToppinsProc);
					break;
					}
					case btn_empleados:{
						DialogBox(Instancia,"DlgEmpleados",Ventana,DlgEmpleadosProc);						
					break;
					}
					case btn_Ventas:{
						DialogBox(Instancia,"DlgVerVentas",Ventana,DlgVentasProc);																		
					break;
					
					
				}
			break;
		}
        case WM_CLOSE:{
            	if(MessageBox(Ventana,"Esta seguro que desea salir","Salir",MB_OKCANCEL)==IDOK){
            		DestroyWindow(WndPila_QuitarCima(&Pila));
            		EnableWindow(WndPila_Cima(Pila),TRUE);
            		SetFocus(WndPila_Cima(Pila));         	
			 	}  
            break;
        }
        default:{
            return DefWindowProc(Ventana,Mensaje,wParam,lParam);
        }
    }
    return 0;
	}
}

//Cuadros de dialogos de la ventana administraccion del boton de crepas
LRESULT CALLBACK DlgCrepasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	//Variable del caudro de dialogo	
	BOOL bSuccess = 0;	
	static HINSTANCE Instancia = NULL;
	//Variables para establecer la comunicacion a la base de datos
	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *sql;
	const char* data = "Callback function called";	
	char *zErrMsg = 0, idCrepa[4] = "",nombreCrepa[40] = "", tipoCrepa[40] = "",descripcionCrepa[75] = "", output[300] = "";
	float precioCrepa;
	char precioCrepas[20] = "";
	int rc;		
	rc = sqlite3_open("PinguiBase.db", &db);	
	switch(mensaje){	
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case btn_buscar_crepa:{				
					Open_BD(rc,db,hwnd);  											 										
					GetDlgItemText(hwnd,edit_Nombre_crepa,nombreCrepa,40);					
   					sprintf(output, "SELECT * FROM crepa WHERE nombreCrepa LIKE '%s';", nombreCrepa);   					   					
   					sql = output;   					   						
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);    					
						//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){
							strncpy(idCrepa,(const char*)sqlite3_column_text(ppStmt,0),4);
    						strncpy(nombreCrepa,(const char*)sqlite3_column_text(ppStmt,1),40);
							strncpy(tipoCrepa,(const char*)sqlite3_column_text(ppStmt,2),40);
							strncpy(descripcionCrepa,(const char*)sqlite3_column_text(ppStmt,3),70);
							strncpy(precioCrepas,(const char*)sqlite3_column_text(ppStmt,4),20);
    					}					
						sqlite3_finalize(ppStmt);					
						SetDlgItemText(hwnd,edit_Clave_crepa,idCrepa);
   					    SetDlgItemText(hwnd,edit_Nombre_crepa,nombreCrepa);
   					    SetDlgItemText(hwnd,edit_Tipo_crepa,tipoCrepa);
   					    SetDlgItemText(hwnd,edit_Descripcion_crepa,descripcionCrepa);
   					    SetDlgItemText(hwnd,edit_Precio_crepa,precioCrepas);
				   	}   	
   					sqlite3_close(db);
   					 if(idCrepa[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   //else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);}  							    						
					break;
				}
				case btn_Agregar_crepa:{					
					Open_BD(rc,db,hwnd); 										
					//obtiene los datos y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_crepa,idCrepa,4);
					GetDlgItemText(hwnd,edit_Nombre_crepa,nombreCrepa,40);
					GetDlgItemText(hwnd,edit_Tipo_crepa,tipoCrepa,40);
					GetDlgItemText(hwnd,edit_Descripcion_crepa,descripcionCrepa,70);
					GetDlgItemText(hwnd,edit_Precio_crepa,precioCrepas,100);
					//conversion de cadena a numero flotantre
					precioCrepa = atof(precioCrepas);					
					//insercion de datos a la bd					
					sprintf(output, "INSERT INTO crepa (idCrepa, nombreCrepa, tipoCrepa, descripcionCrepa, precioCrepa) VALUES ('C-1', '%s', '%s', '%s','%.2f');", nombreCrepa, tipoCrepa, descripcionCrepa, precioCrepa);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Se han guardado Correctamente los datos","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_crepa,"");
   					    SetDlgItemText(hwnd,edit_Nombre_crepa,"");
   					    SetDlgItemText(hwnd,edit_Tipo_crepa,"");
   					    SetDlgItemText(hwnd,edit_Descripcion_crepa,"");
   					    SetDlgItemText(hwnd,edit_Precio_crepa,"");
				   	}					
					break;
				}
				case btn_Modificar_crepa:{
					Open_BD(rc,db,hwnd); 										
					//obtiene los datos y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_crepa,idCrepa,4);
					GetDlgItemText(hwnd,edit_Nombre_crepa,nombreCrepa,40);
					GetDlgItemText(hwnd,edit_Tipo_crepa,tipoCrepa,40);
					GetDlgItemText(hwnd,edit_Descripcion_crepa,descripcionCrepa,70);
					GetDlgItemText(hwnd,edit_Precio_crepa,precioCrepas,100);
					//conversion de cadena a numero flotantre
					precioCrepa = atof(precioCrepas);					
					//insercion de datos a la bd										
					sprintf(output, "UPDATE crepa SET nombreCrepa = '%s', tipoCrepa = '%s', descripcionCrepa = '%s', precioCrepa = '%.2f' WHERE idCrepa = '%s';", nombreCrepa, tipoCrepa, descripcionCrepa, precioCrepa, idCrepa);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Actualizacion de datos realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_crepa,"");
   					    SetDlgItemText(hwnd,edit_Nombre_crepa,"");
   					    SetDlgItemText(hwnd,edit_Tipo_crepa,"");
   					    SetDlgItemText(hwnd,edit_Descripcion_crepa,"");
   					    SetDlgItemText(hwnd,edit_Precio_crepa,"");
				   	}
					break;
					case btn_Eliminar_crepa:{
					Open_BD(rc,db,hwnd);														
					//obtiene los datos  del texbox y los manda a las variables															
					GetDlgItemText(hwnd,edit_Clave_crepa,idCrepa,4);
					GetDlgItemText(hwnd,edit_Nombre_crepa,nombreCrepa,40);
					GetDlgItemText(hwnd,edit_Tipo_crepa,tipoCrepa,40);
					GetDlgItemText(hwnd,edit_Descripcion_crepa,descripcionCrepa,70);
					GetDlgItemText(hwnd,edit_Precio_crepa,precioCrepas,100);
					//conversion de cadena a numero flotantre
					precioCrepa = atof(precioCrepas);					
					//Eliminacion de datos a la bd
					if(idCrepa[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);  											
						sprintf(output, "DELETE FROM crepa WHERE idCrepa = '%s';", idCrepa);
						sql = output;
						rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
							if(rc != SQLITE_OK){
      							MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    						MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
				   			}//limpia los datos de los edit
				   		SetDlgItemText(hwnd,edit_Clave_crepa,"");
   						SetDlgItemText(hwnd,edit_Nombre_crepa,"");
   						SetDlgItemText(hwnd,edit_Tipo_crepa,"");
   						SetDlgItemText(hwnd,edit_Descripcion_crepa,"");
   						SetDlgItemText(hwnd,edit_Precio_crepa,"");
				   		}
				   	sqlite3_close(db);
					break;
				}
				case btn_todo_crepa:{
					DialogBox(Instancia,"DlgVerCrepas",hwnd,DlgVerCrepasProc);					
					break;
				}
				case btn_Limpiar_crepa:{
					//limpiar los cuadro de textos
					SetDlgItemText(hwnd,edit_Clave_crepa,"");
   					SetDlgItemText(hwnd,edit_Nombre_crepa,"");
   					SetDlgItemText(hwnd,edit_Tipo_crepa,"");
   					SetDlgItemText(hwnd,edit_Descripcion_crepa,"");
   					SetDlgItemText(hwnd,edit_Precio_crepa,"");
					//limpiar las variables					  					    										
					memset(idCrepa,0,4);
					memset(nombreCrepa,0,60);
					memset(tipoCrepa,0,60);
					memset(descripcionCrepa,0,75);
					memset(precioCrepas,0,20);
					memset(output,0,300);
					precioCrepa= 0;														
					break;					
				}
				}				
			}
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);			
	 
			return ;
		}
		return bSuccess;
	}
	return bSuccess;
}

//Cuadros de dialogos del caudro de dialogo crepas del boton de crepas
LRESULT CALLBACK DlgVerCrepasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	static HINSTANCE Instancia = NULL;
	static HWND hwndListView;
	BOOL bSuccess = 0;
	switch(mensaje){			
		case WM_ACTIVATE:{
					hwndListView = CreateListView(g_hInst, hwnd);    	    	
			  		InitListView(hwndListView);	
			break;
		}		
		case WM_NOTIFY:
    	 	HandleWM_NOTIFY(lParam,hwnd);
      		//return ListViewNotify(hwnd, lParam);
   
  		 case WM_SIZE:
      		ResizeListView(hwndListView, hwnd);
      		break;

		case WM_COMMAND:{			
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);	
			return ;
		}
		return bSuccess;
	}
	return bSuccess;
}

//Ingresar los datos a las columnas de manera individual
void HandleWM_NOTIFY(LPARAM lParam,	HWND hwnd)
{	
    NMLVDISPINFO* plvdi;	
    switch (((LPNMHDR) lParam)->code)
    {
        case LVN_GETDISPINFO:
            plvdi = (NMLVDISPINFO*)lParam;
            int i=0;
			sqlite3 *db;
			sqlite3_stmt *ppStmt;					
			char *zErrMsg = 0, output[300] = "";					
			char *sql;
			const char* data = "Callback function called";				
			int rc;
			int filas=0;					
					//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,hwnd);
   					sprintf(output, "SELECT COUNT(idCrepa) FROM crepa;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);   					
			rc = sqlite3_open("PinguiBase.db", &db);			   						
            switch (plvdi->item.iSubItem)
            {
                case 0:                					
					Open_BD(rc,db,hwnd);              				  					  					
					sprintf(output, "SELECT * FROM crepa;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgCrepaInfo[i].clave,(const char*)sqlite3_column_text(ppStmt,0),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgCrepaInfo[plvdi->item.iItem].clave;
                    break;                      
                case 1:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM crepa;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgCrepaInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,1),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgCrepaInfo[plvdi->item.iItem].nombre;
                    break;                
                case 2:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM crepa;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgCrepaInfo[i].tipo,(const char*)sqlite3_column_text(ppStmt,2),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgCrepaInfo[plvdi->item.iItem].tipo;
                    break;
                case 3:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM crepa;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgCrepaInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,3),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgCrepaInfo[plvdi->item.iItem].descripcion;
                    break;
                case 4:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM crepa;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgCrepaInfo[i].precio,(const char*)sqlite3_column_text(ppStmt,4),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgCrepaInfo[plvdi->item.iItem].precio;
                    break;
				default:
                    break;
            }
        break;
    }
    return;
}


//Cuadros de dialogos de la ventana administraccion del boton de bebidas
LRESULT CALLBACK DlgBebidasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	//Variable del caudro de dialogo	
	BOOL bSuccess = 0;	
	static HINSTANCE Instancia = NULL;
	//Variables para establecer la comunicacion a la base de datos
	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *sql;
	const char* data = "Callback function called";
	char *zErrMsg = 0, idBebida[4] = "", nombreBebida[40] = "", descripcionBebida[75] = "", output[300] = "";
	float precioBebida;
	char precioBebidas[20] = "";	
	int rc;		
	rc = sqlite3_open("PinguiBase.db", &db);	
	switch(mensaje){	
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case btn_buscar_bebidas:{
					Open_BD(rc,db,hwnd);  											
					GetDlgItemText(hwnd,edit_Nombre_bebidas,nombreBebida,30);					
   					sprintf(output, "SELECT * FROM bebida WHERE nombreBebida LIKE '%s';", nombreBebida);   					   					   					
   					sql = output;   					   						
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_ICONERROR);
      						sqlite3_free(zErrMsg);
   					}else{//el mensaje compureba que si se logro la consulta
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
							strncpy(idBebida,(const char*)sqlite3_column_text(ppStmt,0),15);
    						strncpy(nombreBebida,(const char*)sqlite3_column_text(ppStmt,1),15);
							strncpy(descripcionBebida,(const char*)sqlite3_column_text(ppStmt,2),20);
							strncpy(precioBebidas,(const char*)sqlite3_column_text(ppStmt,3),20);
    					}					
						sqlite3_finalize(ppStmt);											
						SetDlgItemText(hwnd,edit_Clave_bebidas,idBebida);
   					    SetDlgItemText(hwnd,edit_Nombre_bebidas,nombreBebida);
   					    SetDlgItemText(hwnd,edit_Descripcion_bebidas,descripcionBebida);
   					    SetDlgItemText(hwnd,edit_Precio_bebidas,precioBebidas);   					    
				   	}   	
   					sqlite3_close(db);					  					   	
					   if(idBebida[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   //else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);}  							    						
					break;
				}
				case btn_Agregar_bebidas:{					
					Open_BD(rc,db,hwnd);  				
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_bebidas,idBebida,4);
					GetDlgItemText(hwnd,edit_Nombre_bebidas,nombreBebida,25);
					GetDlgItemText(hwnd,edit_Descripcion_bebidas,descripcionBebida,75);
					GetDlgItemText(hwnd,edit_Precio_bebidas,precioBebidas,100);
					//conversion de cadena a numero flotantre
					precioBebida = atof(precioBebidas);					
					//insercion de datos a la bd					
					sprintf(output, "INSERT INTO bebida (idBebida, nombreBebida, descripcionBebida, precioBebida) VALUES ('B-1', '%s', '%s', '%.2f');", nombreBebida, descripcionBebida, precioBebida);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Se han guardado Correctamente los datos","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_bebidas,"");
						SetDlgItemText(hwnd,edit_Nombre_bebidas,"");
						SetDlgItemText(hwnd,edit_Descripcion_bebidas,"");
						SetDlgItemText(hwnd,edit_Precio_bebidas,"");						
				   	}					  
					break;
				}
				case btn_Modificar_bebidas:{						
					Open_BD(rc,db,hwnd);  				
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_bebidas,idBebida,4);
					GetDlgItemText(hwnd,edit_Nombre_bebidas,nombreBebida,25);
					GetDlgItemText(hwnd,edit_Descripcion_bebidas,descripcionBebida,75);
					GetDlgItemText(hwnd,edit_Precio_bebidas,precioBebidas,100);
					//conversion de cadena a numero flotantre
					precioBebida = atof(precioBebidas);					
					//insercion de datos a la bd										
					sprintf(output, "UPDATE bebida SET nombreBebida = '%s', descripcionBebida = '%s', precioBebida = '%.2f' WHERE idBebida = '%s';", nombreBebida, descripcionBebida, precioBebida, idBebida);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Actualizacion de datos realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_bebidas,"");
						SetDlgItemText(hwnd,edit_Nombre_bebidas,"");
						SetDlgItemText(hwnd,edit_Descripcion_bebidas,"");
						SetDlgItemText(hwnd,edit_Precio_bebidas,"");						
				   	}
					break;
				}
				case btn_Eliminar_bebidas:{
					Open_BD(rc,db,hwnd);														
					//obtiene los datos  del texbox y los manda a las variables										
					GetDlgItemText(hwnd,edit_Clave_bebidas,idBebida,4);
					GetDlgItemText(hwnd,edit_Nombre_bebidas,nombreBebida,25);
					GetDlgItemText(hwnd,edit_Descripcion_bebidas,descripcionBebida,75);
					GetDlgItemText(hwnd,edit_Precio_bebidas,precioBebidas,10);
					//conversion de cadena a numero flotantre
					precioBebida = atof(precioBebidas);					
					//Eliminacion de datos a la bd
					if(idBebida[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);  											
						sprintf(output, "DELETE FROM bebida WHERE idBebida = '%s';", idBebida);
						sql = output;
						rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
							if(rc != SQLITE_OK){
      							MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    						MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
				   			}//limpia los datos de los edit
				   		SetDlgItemText(hwnd,edit_Clave_bebidas,"");
						SetDlgItemText(hwnd,edit_Nombre_bebidas,"");
						SetDlgItemText(hwnd,edit_Descripcion_bebidas,"");
						SetDlgItemText(hwnd,edit_Precio_bebidas,"");						
				   		}
				   	sqlite3_close(db);
					break;
				}
				case btn_todo_bebidas:{
					DialogBox(Instancia,"DlgVerToppins",hwnd,DlgVerBebidasProc);					
					break;
				}
				case btn_Limpiar_bebidas:{
					//limpiar los cuadro de textos
					SetDlgItemText(hwnd,edit_Clave_bebidas,"");
					SetDlgItemText(hwnd,edit_Nombre_bebidas,"");
					SetDlgItemText(hwnd,edit_Descripcion_bebidas,"");
					SetDlgItemText(hwnd,edit_Precio_bebidas,"");						
					//limpiar las variables					  					    	
					memset(idBebida,0,4);
					memset(nombreBebida,0,60);
					memset(descripcionBebida,0,75);
					memset(precioBebidas,0,20);
					memset(output,0,300);
					precioBebida = 0;														
					break;					
				}
			}
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);			
			return ;
		}
		return bSuccess;
	}
	return bSuccess;
}


//Cuadros de dialogos del caudro de dialogo Bebidas del boton de bebidas
LRESULT CALLBACK DlgVerBebidasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	static HINSTANCE Instancia = NULL;
	static HWND hwndListView;
	BOOL bSuccess = 0;	
	switch(mensaje){			
		case WM_ACTIVATE:{
			hwndListView = CreateListView(g_hInst, hwnd);    	    		
			InitListViewBebidas(hwndListView);	
			break;
		}
		case WM_NOTIFY:
    	 	HandleWM_NOTIFY_Bebida(lParam,hwnd);
      		//return ListViewNotify(hwnd, lParam);   
  		 case WM_SIZE:
      		ResizeListView(hwndListView, hwnd);
      		break;
		case WM_COMMAND:{
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);	
			return ;
		}		
		return bSuccess;
	}
	return bSuccess;
}

//Ingresar los datos a las columnas de manera individual
void HandleWM_NOTIFY_Bebida(LPARAM lParam,HWND hwnd)
{	
	//a partir de aqui no modifica interactua con la colocacion
    NMLVDISPINFO* plvdi;
	
    switch (((LPNMHDR) lParam)->code)
    {
        case LVN_GETDISPINFO:
            plvdi = (NMLVDISPINFO*)lParam;			
			int i=0;
			sqlite3 *db;
			sqlite3_stmt *ppStmt;					
			char *zErrMsg = 0, output[300] = "";					
			char *sql;
			const char* data = "Callback function called";				
			int rc;
			int filas=0;				
					//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,hwnd);
   					sprintf(output, "SELECT COUNT(idBebida) FROM bebida;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);   					
			rc = sqlite3_open("PinguiBase.db", &db);			   				
            switch (plvdi->item.iSubItem)
            {
                case 0:                					
  					Open_BD(rc,db,hwnd);              				  								
					sprintf(output, "SELECT * FROM bebida;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgBebidasInfo[i].clave,(const char*)sqlite3_column_text(ppStmt,0),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgBebidasInfo[plvdi->item.iItem].clave;
                    
                    
                    break;
                      
                case 1:
                	Open_BD(rc,db,hwnd);						
					sprintf(output, "SELECT * FROM bebida;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgBebidasInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,1),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgBebidasInfo[plvdi->item.iItem].nombre;
                    break;
                
                case 2:
                	Open_BD(rc,db,hwnd);						
					sprintf(output, "SELECT * FROM bebida;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgBebidasInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,2),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgBebidasInfo[plvdi->item.iItem].descripcion;
                    break;
                //
                case 3:
                	Open_BD(rc,db,hwnd);						
					sprintf(output, "SELECT * FROM bebida;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgBebidasInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,3),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgBebidasInfo[plvdi->item.iItem].descripcion;
                    break;                
				default:
                    break;
            }
        break;
    }
    return;
}


//Cuadros de dialogos de la ventana administraccion del boton de bebidas
LRESULT CALLBACK DlgEmpleadosProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	//Variable del caudro de dialogo	/*
	BOOL bSuccess = 0;	
	static HINSTANCE Instancia = NULL;
	//Variables para establecer la comunicacion a la base de datos
	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *sql;
	const char* data = "Callback function called";	
	char *zErrMsg = 0, curpUsuario[20] = "", nombreUsuario[40] = "", pseudonimoUsuario[40] = "", passwordUsuario[40] = "",cargoUsuario[40] = "", output[300] = "";
	int rc;		
	rc = sqlite3_open("PinguiBase.db", &db);	
	switch(mensaje){	
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case btn_buscar_empleados:{
					Open_BD(rc,db,hwnd);  											
					GetDlgItemText(hwnd,edit_Nombre_empleados,nombreUsuario,40);					
   					sprintf(output, "SELECT * FROM usuario WHERE nombreUsuario LIKE '%s';", nombreUsuario);   					   					   					
   					sql = output;   					   						
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_ICONERROR);
      						sqlite3_free(zErrMsg);
   					}else{//el mensaje compureba que si se logro la consulta
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
							strncpy(curpUsuario,(const char*)sqlite3_column_text(ppStmt,0),20);
    						strncpy(nombreUsuario,(const char*)sqlite3_column_text(ppStmt,1),40);
							strncpy(pseudonimoUsuario,(const char*)sqlite3_column_text(ppStmt,2),40);
							strncpy(passwordUsuario,(const char*)sqlite3_column_text(ppStmt,3),40);
							strncpy(cargoUsuario,(const char*)sqlite3_column_text(ppStmt,4),40);
    					}					
						sqlite3_finalize(ppStmt);											
						SetDlgItemText(hwnd,edit_Usuario_empleados,curpUsuario);
   					    SetDlgItemText(hwnd,edit_Nombre_empleados,nombreUsuario);
   					    SetDlgItemText(hwnd,edit_pseudonimo_empleados,pseudonimoUsuario);
   					    SetDlgItemText(hwnd,edit_password_empleados,passwordUsuario);   					    
   					    SetDlgItemText(hwnd,edit_cargo_empleados,cargoUsuario);   					    
				   	}   	
   					sqlite3_close(db);					  					   	
					   if(curpUsuario[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   //else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);}  							    						
					break;
				}
				case btn_Agregar_empleados:{					
					Open_BD(rc,db,hwnd);  				
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Usuario_empleados,curpUsuario,20);
					GetDlgItemText(hwnd,edit_Nombre_empleados,nombreUsuario,40);
					GetDlgItemText(hwnd,edit_pseudonimo_empleados,pseudonimoUsuario,40);
					GetDlgItemText(hwnd,edit_password_empleados,passwordUsuario,40);						
					GetDlgItemText(hwnd,edit_cargo_empleados,cargoUsuario,40);											
					//insercion de datos a la bd					
					sprintf(output, "INSERT INTO usuario (curpUsuario, nombreUsuario, pseudonimoUsuario, passwordUsuario, cargoUsuario) VALUES ('%s', '%s', '%s', '%s', '%s');", curpUsuario, nombreUsuario, pseudonimoUsuario, passwordUsuario, cargoUsuario);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Se han guardado Correctamente los datos","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Usuario_empleados,"");
   					    SetDlgItemText(hwnd,edit_Nombre_empleados,"");
   					    SetDlgItemText(hwnd,edit_pseudonimo_empleados,"");
   					    SetDlgItemText(hwnd,edit_password_empleados,"");   					    
   					    SetDlgItemText(hwnd,edit_cargo_empleados,"");   					    						
				   	}					  
					break;
				}
				case btn_Modificar_empleados:{						
					Open_BD(rc,db,hwnd);  				
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Usuario_empleados,curpUsuario,20);
					GetDlgItemText(hwnd,edit_Nombre_empleados,nombreUsuario,40);
					GetDlgItemText(hwnd,edit_pseudonimo_empleados,pseudonimoUsuario,40);
					GetDlgItemText(hwnd,edit_password_empleados,passwordUsuario,40);						
					GetDlgItemText(hwnd,edit_cargo_empleados,cargoUsuario,40);											
					//insercion de datos a la bd															
					sprintf(output, "UPDATE usuario SET nombreUsuario = '%s', pseudonimoUsuario = '%s', passwordUsuario = '%s', cargoUsuario = '%s' WHERE curpUsuario = '%s';", nombreUsuario, pseudonimoUsuario, passwordUsuario, cargoUsuario, curpUsuario);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Actualizacion de datos realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Usuario_empleados,"");
   					    SetDlgItemText(hwnd,edit_Nombre_empleados,"");
   					    SetDlgItemText(hwnd,edit_pseudonimo_empleados,"");
   					    SetDlgItemText(hwnd,edit_password_empleados,"");   					    
   					    SetDlgItemText(hwnd,edit_cargo_empleados,"");   					    						
				   	}
					break;
				}
				case btn_Eliminar_empleados:{
					Open_BD(rc,db,hwnd);														
					//obtiene los datos  del texbox y los manda a las variables										
					GetDlgItemText(hwnd,edit_Usuario_empleados,curpUsuario,20);
					GetDlgItemText(hwnd,edit_Nombre_empleados,nombreUsuario,40);
					GetDlgItemText(hwnd,edit_pseudonimo_empleados,pseudonimoUsuario,40);
					GetDlgItemText(hwnd,edit_password_empleados,passwordUsuario,40);						
					GetDlgItemText(hwnd,edit_cargo_empleados,cargoUsuario,40);							
					//Eliminacion de datos a la bd
					if(curpUsuario[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);  											
						sprintf(output, "DELETE FROM usuario WHERE curpUsuario = '%s';", curpUsuario);
						sql = output;
						rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
							if(rc != SQLITE_OK){
      							MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    						MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
				   			}//limpia los datos de los edit
				   		SetDlgItemText(hwnd,edit_Usuario_empleados,"");
   					    SetDlgItemText(hwnd,edit_Nombre_empleados,"");
   					    SetDlgItemText(hwnd,edit_pseudonimo_empleados,"");
   					    SetDlgItemText(hwnd,edit_password_empleados,"");   					    
   					    SetDlgItemText(hwnd,edit_cargo_empleados,"");   					    						
				   		}
				   	sqlite3_close(db);
					break;
				}
				case btn_todo_empleados:{
					DialogBox(Instancia,"DlgVerEmpleados",hwnd,DlgVerEmpleadosProc);					
					break;
				}
				case btn_Limpiar_empleados:{
					//limpiar los cuadro de textos
					SetDlgItemText(hwnd,edit_Usuario_empleados,"");
   				    SetDlgItemText(hwnd,edit_Nombre_empleados,"");
   				    SetDlgItemText(hwnd,edit_pseudonimo_empleados,"");
   				    SetDlgItemText(hwnd,edit_password_empleados,"");   					    
   				    SetDlgItemText(hwnd,edit_cargo_empleados,"");   					    						
					//limpiar las variables					  					    						
					memset(curpUsuario,0,20);
					memset(nombreUsuario,0,40);
					memset(pseudonimoUsuario,0,40);
					memset(passwordUsuario,0,40);
					memset(cargoUsuario,0,40);
					memset(output,0,300);																		
					break;					
				}
			}
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);			
			return ;
		}
		return bSuccess;
	}
	return bSuccess;
}


//Cuadros de dialogos del caudro de dialogo Empleados del boton de Empleados
LRESULT CALLBACK DlgVerEmpleadosProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	static HINSTANCE Instancia = NULL;
	static HWND hwndListView;
	BOOL bSuccess = 0;	
	switch(mensaje){			
		case WM_ACTIVATE:{
			hwndListView = CreateListView(g_hInst, hwnd);    	    		
			InitListViewEmpleados(hwndListView);	
			break;
		}
		case WM_NOTIFY:
    	 	HandleWM_NOTIFY_Empleados(lParam,hwnd);
      		//return ListViewNotify(hwnd, lParam);   
  		 case WM_SIZE:
      		ResizeListView(hwndListView, hwnd);
      		break;
		case WM_COMMAND:{
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);	
			return ;
		}		
		return bSuccess;
	}
	return bSuccess;
}

//Ingresar los datos a las columnas de manera individual
void HandleWM_NOTIFY_Empleados(LPARAM lParam,HWND hwnd)
{	
	NMLVDISPINFO* plvdi;	
    switch (((LPNMHDR) lParam)->code)
    {
        case LVN_GETDISPINFO:
            plvdi = (NMLVDISPINFO*)lParam;
            int i=0;
			sqlite3 *db;
			sqlite3_stmt *ppStmt;					
			char *zErrMsg = 0, output[300] = "";					
			char *sql;
			const char* data = "Callback function called";				
			int rc;
			int filas=0;					
					//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,hwnd);
   					sprintf(output, "SELECT COUNT(curpUsuario) FROM usuario;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);   					
			rc = sqlite3_open("PinguiBase.db", &db);			   						
            switch (plvdi->item.iSubItem)
            {
                case 0:                					
					Open_BD(rc,db,hwnd);              				  					  					
					sprintf(output, "SELECT * FROM usuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgEmpleadosInfo[i].curpUsuario,(const char*)sqlite3_column_text(ppStmt,0),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgEmpleadosInfo[plvdi->item.iItem].curpUsuario;
                    break;                      
                case 1:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM usuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgEmpleadosInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,1),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgEmpleadosInfo[plvdi->item.iItem].nombre;
                    break;                
                case 2:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM usuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgEmpleadosInfo[i].pseudonimo,(const char*)sqlite3_column_text(ppStmt,2),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgEmpleadosInfo[plvdi->item.iItem].pseudonimo;
                    break;
                case 3:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM usuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgEmpleadosInfo[i].password,(const char*)sqlite3_column_text(ppStmt,3),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgEmpleadosInfo[plvdi->item.iItem].password;
                    break;
                case 4:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM usuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgEmpleadosInfo[i].cargo,(const char*)sqlite3_column_text(ppStmt,4),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgEmpleadosInfo[plvdi->item.iItem].cargo;
                    break;
				default:
                    break;
            }
        break;
    }
    return;
}

//Cuadros de dialogos de la ventana administraccion del boton de toppins
LRESULT CALLBACK DlgToppinsProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	//Variable del caudro de dialogo	
	BOOL bSuccess = 0;	
	static HINSTANCE Instancia = NULL;
	//Variables para establecer la comunicacion a la base de datos
	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *sql;
	const char* data = "Callback function called";
	char *zErrMsg = 0, idTopping[4] = "", nombreTopping[60] = "", descripcionTopping[75] = "", output[300] = "";
	float precioTopping = 0;	
	char precioToppings[20] ="";
	int rc;		
	rc = sqlite3_open("PinguiBase.db", &db);	
	switch(mensaje){	
		case WM_COMMAND:{
			switch(LOWORD(wParam)){
				case btn_buscar_toppins:{										
					Open_BD(rc,db,hwnd);  											
					GetDlgItemText(hwnd,edit_Nombre_toppins,nombreTopping,30);					
   					sprintf(output, "SELECT * FROM topping WHERE nombreTopping LIKE '%s';", nombreTopping);   					   					
   					sql = output;   					   						
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_ICONERROR);
      						sqlite3_free(zErrMsg);
   					}else{//el mensaje compureba que si se logro la consulta
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
							strncpy(idTopping,(const char*)sqlite3_column_text(ppStmt,0),15);
    						strncpy(nombreTopping,(const char*)sqlite3_column_text(ppStmt,1),15);
							strncpy(descripcionTopping,(const char*)sqlite3_column_text(ppStmt,2),20);
							strncpy(precioToppings,(const char*)sqlite3_column_text(ppStmt,3),20);
    					}					
						sqlite3_finalize(ppStmt);											
						SetDlgItemText(hwnd,edit_Clave_toppins,idTopping);
   					    SetDlgItemText(hwnd,edit_Nombre_toppins,nombreTopping);
   					    SetDlgItemText(hwnd,edit_Descripcion_toppins,descripcionTopping);
   					    SetDlgItemText(hwnd,edit_Precio_toppins,precioToppings);   					    
				   	}   	
   					sqlite3_close(db);					  					   	
					   if(idTopping[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   //else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);}  					
					break;
				}
				case btn_Agregar_toppins:{
					Open_BD(rc,db,hwnd);																															
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_toppins,idTopping,4);
					GetDlgItemText(hwnd,edit_Nombre_toppins,nombreTopping,30);
					GetDlgItemText(hwnd,edit_Descripcion_toppins,descripcionTopping,75);
					GetDlgItemText(hwnd,edit_Precio_toppins,precioToppings,100);
					//conversion de cadena a numero flotantre
					precioTopping = atof(precioToppings);					
					//insercion de datos a la bd					
					sprintf(output, "INSERT INTO topping (idTopping, nombreTopping, descripcionTopping, precioTopping) VALUES ('T-1', '%s', '%s', '%.2f');", nombreTopping, descripcionTopping, precioTopping);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_ICONERROR);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Se han guardado Correctamente los datos","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_toppins,"");
						SetDlgItemText(hwnd,edit_Nombre_toppins,"");
						SetDlgItemText(hwnd,edit_Descripcion_toppins,"");
						SetDlgItemText(hwnd,edit_Precio_toppins,"");						
				   	}					
					break;
				}
				case btn_Modificar_toppins:{
					Open_BD(rc,db,hwnd);																																													
					//obtiene los datos  del texbox y los manda a las variables					
					GetDlgItemText(hwnd,edit_Clave_toppins,idTopping,4);
					GetDlgItemText(hwnd,edit_Nombre_toppins,nombreTopping,60);
					GetDlgItemText(hwnd,edit_Descripcion_toppins,descripcionTopping,75);
					GetDlgItemText(hwnd,edit_Precio_toppins,precioToppings,100);
					//conversion de cadena a numero flotantre
					precioTopping = atof(precioToppings);					
					//Actualizar la base de datos de los toppins
					sprintf(output, "UPDATE topping SET nombreTopping = '%s', descripcionTopping = '%s', precioTopping = '%.2f' WHERE idTopping = '%s';", nombreTopping, descripcionTopping, precioTopping, idTopping);
					sql = output;		
					rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					MessageBox(hwnd,"Actualizacion de datos realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					SetDlgItemText(hwnd,edit_Clave_toppins,"");
						SetDlgItemText(hwnd,edit_Nombre_toppins,"");
						SetDlgItemText(hwnd,edit_Descripcion_toppins,"");
						SetDlgItemText(hwnd,edit_Precio_toppins,"");						
				   	}
				   	sqlite3_close(db);
					break;
				}
				case btn_Eliminar_toppins:{
					Open_BD(rc,db,hwnd);														
					//obtiene los datos  del texbox y los manda a las variables										
					GetDlgItemText(hwnd,edit_Clave_toppins,idTopping,4);					
					GetDlgItemText(hwnd,edit_Nombre_toppins,nombreTopping,30);
					GetDlgItemText(hwnd,edit_Descripcion_toppins,descripcionTopping,75);
					GetDlgItemText(hwnd,edit_Precio_toppins,precioToppings,100);
					//conversion de cadena a numero flotantre
					precioTopping = atof(precioToppings);					
					//Eliminacion de datos a la bd
					if(idTopping[0] == '\0'){
   						MessageBox(hwnd,"No existe","SQL error",MB_ICONERROR);
					   }//Comprueba si no hay nada en el las claves del texto
					   else{//MessageBox(hwnd,"existe","SQL error",MB_ICONERROR);  											
						sprintf(output, "DELETE FROM topping WHERE idTopping = '%s';", idTopping);
						sql = output;
						rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
							if(rc != SQLITE_OK){
      							MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    						MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
				   			}//limpia los datos de los edit
				   		SetDlgItemText(hwnd,edit_Clave_toppins,"");
						SetDlgItemText(hwnd,edit_Nombre_toppins,"");
						SetDlgItemText(hwnd,edit_Descripcion_toppins,"");
						SetDlgItemText(hwnd,edit_Precio_toppins,"");						
				   		}
				   	sqlite3_close(db);
					break;
				}
				case btn_todo_toppins:{
					DialogBox(Instancia,"DlgVerToppins",hwnd,DlgVerToppinsProc);					
					break;
				}
				case btn_Limpiar_toppins:{
					//limpiar los cuadro de textos
					SetDlgItemText(hwnd,edit_Clave_toppins,"");
					SetDlgItemText(hwnd,edit_Nombre_toppins,"");
					SetDlgItemText(hwnd,edit_Descripcion_toppins,"");
					SetDlgItemText(hwnd,edit_Precio_toppins,"");						
					//limpiar las variables					  
					memset(idTopping,0,4);
					memset(nombreTopping,0,60);
					memset(descripcionTopping,0,75);
					memset(precioToppings,0,20);
					memset(output,0,300);
					precioTopping = 0;														
					break;					
				}		
								
			}
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);
			return;
		}
		return bSuccess;
	}
	return bSuccess;
}

//Cuadros de dialogos del caudro de dialogo toppins del boton de ver toppins

LRESULT CALLBACK DlgVerToppinsProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	static HINSTANCE Instancia = NULL;
	static HWND hwndListView;
	BOOL bSuccess = 0;
	switch(mensaje){	
		case WM_ACTIVATE:{
			hwndListView = CreateListView(g_hInst, hwnd);    	    		
      		InitListViewToppins(hwndListView);						      		
			break;
		}
		case WM_NOTIFY:
    	 	HandleWM_NOTIFY_Toppins(lParam,hwnd);
      		//return ListViewNotify(hwnd, lParam);
  		 case WM_SIZE:
		    ResizeListView(hwndListView, hwnd);
      		break;
		case WM_COMMAND:{			
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);	
			return ;
		}		
		return bSuccess;
	}
	return bSuccess;
}
//Ingresar los datos a las columnas de manera individual
void HandleWM_NOTIFY_Toppins(LPARAM lParam,HWND hwnd)
{	
	//a partir de aqui no modifica interactua con la colocacion
    NMLVDISPINFO* plvdi;	
    switch (((LPNMHDR) lParam)->code)
    {
        case LVN_GETDISPINFO:
            plvdi = (NMLVDISPINFO*)lParam;
			int i=0;
			sqlite3 *db;
			sqlite3_stmt *ppStmt;					
			char *zErrMsg = 0, output[300] = "";					
			char *sql;
			const char* data = "Callback function called";				
			int rc;
			int filas=0;					
					//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,hwnd);
   					sprintf(output, "SELECT COUNT(idTopping) FROM topping;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);
   					
			rc = sqlite3_open("PinguiBase.db", &db);			   			
            switch (plvdi->item.iSubItem)
            {
                case 0:
					Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "SELECT * FROM topping;");
   					sql = output;
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   						}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
							for(i=0; i<filas; i++){
								if(SQLITE_ROW ==sqlite3_step(ppStmt)){
		   							strncpy(rgToppinsInfo[i].clave,(const char*)sqlite3_column_text(ppStmt,0),20);
    							}
							}
						sqlite3_finalize(ppStmt);			
				   		}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgToppinsInfo[plvdi->item.iItem].clave;
                    break;                      
                case 1:
                	Open_BD(rc,db,hwnd);              				  											
					sprintf(output, "SELECT * FROM topping;");
   					sql = output;
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   						}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
							for(i=0; i<filas; i++){
								if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   								strncpy(rgToppinsInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,1),20);
    							}
							}
						sqlite3_finalize(ppStmt);			
				   		}	   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgToppinsInfo[plvdi->item.iItem].nombre;
                    break;                
                case 2:
                	Open_BD(rc,db,hwnd);              				  										
					sprintf(output, "SELECT * FROM topping;");
   					sql = output;
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   						}else{
    						//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    						//busca y copia los datos que se encuentran en la base de datos
							for(i=0; i<filas; i++){
								if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   								strncpy(rgToppinsInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,2),20);
    							}
							}
						sqlite3_finalize(ppStmt);			
				   		}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgToppinsInfo[plvdi->item.iItem].descripcion;
                    break;
                case 3:
                	Open_BD(rc,db,hwnd);              				  											
					sprintf(output, "SELECT * FROM topping;");
   					sql = output;
					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
						if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   						}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
							for(i=0; i<filas; i++){
								if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   								strncpy(rgToppinsInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,3),20);
    							}
							}
						sqlite3_finalize(ppStmt);			
				   		}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgToppinsInfo[plvdi->item.iItem].descripcion;
                    break;             
				default:
                    break;
            }
        break;
    }
    return;
}

//Cuadros de dialogos de la ventana administraccion del boton de ventas
LRESULT CALLBACK DlgVentasProc(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam){
	static HINSTANCE Instancia = NULL;
	static HWND hwndListView;
	BOOL bSuccess = 0;
	
	switch(mensaje){	
		case WM_ACTIVATE:{
			hwndListView = CreateListView(g_hInst, hwnd);    	    		
			InitListViewVentas(hwndListView);	
			break;
		}
		case WM_NOTIFY:
    	 	HandleWM_NOTIFY_Ventas(lParam,hwnd);
      		//return ListViewNotify(hwnd, lParam);
  		 case WM_SIZE:
      		ResizeListView(hwndListView, hwnd);
      		break;
		case WM_COMMAND:{
			bSuccess = 1;
			return bSuccess;
		}
		case WM_CLOSE:{
			bSuccess = 1;
			EndDialog(hwnd,bSuccess);	
			return ;
		}		
		return bSuccess;
	}
	return bSuccess;
}

//Ingresar los datos a las columnas de manera individual
void HandleWM_NOTIFY_Ventas(LPARAM lParam,HWND hwnd)
{	
	//a partir de aqui no modifica interactua con la colocacion
    NMLVDISPINFO* plvdi;
	
    switch (((LPNMHDR) lParam)->code)
    {
        case LVN_GETDISPINFO:
            plvdi = (NMLVDISPINFO*)lParam;            	
			int i=0;
			sqlite3 *db;
			sqlite3_stmt *ppStmt;					
			char *zErrMsg = 0, output[300] = "";					
			char *sql;
			const char* data = "Callback function called";				
			int rc;
			int filas=0;					
					//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,hwnd);
   					sprintf(output, "SELECT COUNT(venta.idProducto) FROM venta inner join producto on venta.idProducto = producto.idProducto;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);   					
			rc = sqlite3_open("PinguiBase.db", &db);			   					
            switch (plvdi->item.iSubItem)
            {
                case 0:                					
  					Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "select * from venta inner join producto on venta.idProducto = producto.idProducto;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgVentaslistInfo[i].clave,(const char*)sqlite3_column_text(ppStmt,0),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgVentaslistInfo[plvdi->item.iItem].clave;
                    break;
                      
                case 1:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "select * from venta inner join producto on venta.idProducto = producto.idProducto;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgVentaslistInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,4),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgVentaslistInfo[plvdi->item.iItem].nombre;
                    break;                
                case 2:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "select * from venta inner join producto on venta.idProducto = producto.idProducto;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgVentaslistInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,1),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgVentaslistInfo[plvdi->item.iItem].descripcion;
                    break;
                case 3:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "select * from venta inner join producto on venta.idProducto = producto.idProducto;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgVentaslistInfo[i].descripcion,(const char*)sqlite3_column_text(ppStmt,2),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgVentaslistInfo[plvdi->item.iItem].descripcion;
                    break;
				case 4:
                	Open_BD(rc,db,hwnd);              				  					
					sprintf(output, "select * from venta inner join usuario on venta.curpUsuario = usuario.curpUsuario;");
   					sql = output;
					   rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
					if(rc != SQLITE_OK){
      						MessageBox(hwnd,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
	   							strncpy(rgVentaslistInfo[i].usuario,(const char*)sqlite3_column_text(ppStmt,5),20);
    						}
						}
						sqlite3_finalize(ppStmt);			
				   	}   	
   					sqlite3_close(db);				
                    plvdi->item.pszText = rgVentaslistInfo[plvdi->item.iItem].usuario;
                    break;	          
				default:
                    break;
            }
        break;
    }
    return;
}


//Ventas de Procedimientos de ventas

struct ComboVentas
        {
            char nombre[40];
        }rgVentasComboInfo[100];

struct ListVentas
        {
            char nombre[40];
        }rgVentasLisInfo[100];

struct Claves
        {
            char clave[40];
        }rgVentasClaveInfo[100];


struct Precios
        {
            int monto;
        }rgVentasmontoInfo[100];


//Ventas de Procedimientos de ventas
//Controles de listbox
HWND Controles;
HWND Controle;
HWND EditMonto;
int value=0;

LRESULT CALLBACK WndProcVent(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam){
    static HINSTANCE Instancia = NULL;
    int monto;
    int resul = 0;
    int  valuar = 0;
	//Variables para establecer la comunicacion a la base de datos
	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *sql;
	char *zErrMsg = 0, output[300] = "";
	const char* data = "Callback function called";
	int rc;		
	rc = sqlite3_open("PinguiBase.db", &db);	
	int filas = 0; 
			//obtener la cantidad de itemas que hay	
  					rc = sqlite3_open("PinguiBase.db", &db);   					
   					Open_BD(rc, db,Ventana);
   					sprintf(output, "SELECT COUNT(idProducto) FROM producto;");   					
   					sql = output;
   					rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
   						if(rc != SQLITE_OK){
      						//MessageBox(hwnd,"Consulta se ha eliminadoa exitosamente","Ejecutado Correctamente",MB_OK);
   						}else{    					
						   if(SQLITE_ROW ==sqlite3_step(ppStmt)){
						   	filas = sqlite3_column_int(ppStmt,0);
						   }	    													
				   		}	
						   sqlite3_finalize(ppStmt);
   					sqlite3_close(db);   					
			rc = sqlite3_open("PinguiBase.db", &db);			   			
	//variables
	int i = 0;
	    
	switch(Mensaje){
    	case WM_CREATE:{
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;     		
			 
            CreateWindow("COMBOBOX","",CBS_DROPDOWNLIST |WS_CHILD|WS_VISIBLE,67,73,300,500,Ventana,(HMENU)Combo_Venta_busqueda,NULL,NULL);			    	   
				Open_BD(rc,db,Ventana);     		
				sprintf(output, "SELECT * FROM producto;");
   				sql = output;
			    rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);
			  		if(rc != SQLITE_OK){
      					MessageBox(Ventana,zErrMsg,"SQL error",MB_OK);
      					sqlite3_free(zErrMsg);
   					}else{
    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						for(i=0; i<filas; i++){
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){
								//rgVentasComboInfo
	   							//strncpy(operaciones[i],(const char*)sqlite3_column_text(ppStmt,2),20);
	   							strncpy(rgVentasComboInfo[i].nombre,(const char*)sqlite3_column_text(ppStmt,1),20);	   							
	   							SendDlgItemMessage(Ventana,Combo_Venta_busqueda,CB_ADDSTRING,0,(LPARAM)rgVentasComboInfo[i].nombre);
                                SendDlgItemMessage(Ventana,Combo_Venta_busqueda,CB_SETCURSEL,(WPARAM)0,(LPARAM)0);					                 
    						}
						}
						sqlite3_finalize(ppStmt);			
					   	}   	
	   					sqlite3_close(db);						   			
            //botones
			CreateWindowEx(0,"BUTTON","Agregar",WS_VISIBLE|WS_CHILD,404,70,189,36,Ventana,(HMENU)bnt_Venta_Agregar,Instancia,NULL);
			CreateWindowEx(0,"BUTTON","Realizar Compra",WS_VISIBLE|WS_CHILD,404,192,189,36,Ventana,(HMENU)bnt_Venta_Compra,Instancia,NULL);
			CreateWindowEx(0,"BUTTON","Limpiar",WS_VISIBLE|WS_CHILD,404,240,189,36,Ventana,(HMENU)bnt_Venta_Limpiar,Instancia,NULL);
			//labels			
			CreateWindowEx(0,"STATIC","Monto",SS_CENTER|WS_CHILD|WS_VISIBLE,404,117,189,36,Ventana,(HMENU)-1,Instancia,NULL);
			CreateWindowEx(0,"STATIC","Total",SS_CENTER|WS_CHILD|WS_VISIBLE,404,284,189,36,Ventana,(HMENU)-2,Instancia,NULL);
			CreateWindowEx(0,"STATIC","0",SS_CENTER|WS_CHILD|WS_VISIBLE,404,310,189,30,Ventana,(HMENU)static_Venta_Total,Instancia,NULL);
			CreateWindowEx(0,"STATIC","Cambio",SS_CENTER|WS_CHILD|WS_VISIBLE,404,339,189,36,Ventana,(HMENU)-3,Instancia,NULL);
			CreateWindowEx(0,"STATIC","0",SS_CENTER|WS_CHILD|WS_VISIBLE,404,357,189,36,Ventana,(HMENU)static_Venta_Cambio,Instancia,NULL);
			CreateWindowEx(0,"STATIC","Producto",SS_CENTER|WS_CHILD|WS_VISIBLE,67,137,127,22,Ventana,(HMENU)-4,Instancia,NULL);
			CreateWindowEx(0,"STATIC","Precio",SS_CENTER|WS_CHILD|WS_VISIBLE,194,137,127,22,Ventana,(HMENU)-5,Instancia,NULL);
			//TEXBOX			
		 	EditMonto = CreateWindowEx(0,"EDIT","0",ES_AUTOHSCROLL|ES_NUMBER|ES_LEFT|WS_CHILD|WS_VISIBLE|WS_BORDER,404,152,189,25,Ventana,(HMENU)edit_Venta_Monto,Instancia,NULL);
			//lisbox
			Controles = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD|WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL, 67, 160, 127, 218, Ventana, (HMENU)list_Venta_Producto, Instancia, NULL);			
			Controle = CreateWindowEx(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD|WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL, 194, 160, 127, 218, Ventana, (HMENU)list_Venta_Precio, Instancia, NULL);			
            break;
        }
    	case WM_COMMAND:{
    			switch(LOWORD(wParam)){
    				case bnt_Venta_Agregar:{
						char Contenido[256];												
						char Precio[10];												
						int total;												
						GetDlgItemText(Ventana,Combo_Venta_busqueda,Contenido,256);						
						SendMessage(Controles, LB_ADDSTRING, 0, (LPARAM)Contenido);								
						Open_BD(rc,db,Ventana);     				
   						sprintf(output, "SELECT * FROM producto WHERE nombreProducto = '%s' limit 1;", Contenido);   					
   						sql = output;   					   						
						rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
							if(rc != SQLITE_OK){
      							MessageBox(Ventana,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    					//MessageBox(Ventana,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
						if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
    					//	strncpy(Precio,(const char*)sqlite3_column_text(ppStmt,2),15);													
    						total = sqlite3_column_int(ppStmt,2);													
    					}    					
    					itoa(total,Precio,10);
						SendMessage(Controle, LB_ADDSTRING, 0, (LPARAM)Precio);									
						value++;																								
							for(i=value-1; i<value; i++){							
								rgVentasmontoInfo[i].monto = total;   									   							                     						
							}
						int suma = 0;
						int num = 0;
							for (i=0; i<value; i++){
								int num =rgVentasmontoInfo[i].monto;
								suma += num;
								SetDlgItemInt(Ventana,static_Venta_Total,suma,0);
							}												
						sqlite3_finalize(ppStmt);				
				   	}   					   	
   					sqlite3_close(db);   					
   						//MessageBox(Ventana,Contenido,"Base de datos abierta exitosamente",MB_OK);	
   						int i;
   							for(i=value; i<value + 1; i++){							
							//rgVentasComboInfo	   							
	   							strncpy(rgVentasLisInfo[i].nombre,Contenido,20);	   							
	   						}
                //            SetDlgItemInt(Ventana,static_Venta_Cambio,value,0);        											
					break;
					}
					case bnt_Venta_Compra:{						
						int mont;
						char buffer[256]={0};
						char buffer2[256]={0};
						int convert =0;
						int convert2 =0;
						int resul;
						GetDlgItemText(Ventana,edit_Venta_Monto,buffer,100);
						GetDlgItemText(Ventana,static_Venta_Total,buffer2,100);
						convert =atoi(buffer);
						convert2 =atoi(buffer2);
						resul = convert -convert2;						
						SetDlgItemInt(Ventana,static_Venta_Cambio,resul,0);												
						int j;																																			
  						Open_BD(rc,db,Ventana);						
						for(j= 1; j < value+1;j++){
						//MessageBox(Ventana,rgVentasLisInfo[j].nombre,"Ejecutado Correctamente",MB_OK);	
						//rgVentasClaveInfo[100];
							sprintf(output, "SELECT * FROM producto WHERE nombreProducto = '%s' limit 1;", rgVentasLisInfo[j].nombre);   					
   							sql = output;   					   						
							rc = sqlite3_prepare_v2(db, sql, -1, &ppStmt, NULL);					   
							if(rc != SQLITE_OK){
      							MessageBox(Ventana,zErrMsg,"SQL error",MB_OK);
      							sqlite3_free(zErrMsg);
   							}else{
    					//MessageBox(Ventana,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
    					//busca y copia los datos que se encuentran en la base de datos
							if(SQLITE_ROW ==sqlite3_step(ppStmt)){							
    							strncpy(rgVentasClaveInfo[j].clave,(const char*)sqlite3_column_text(ppStmt,0),15);													
    						}    					
							sqlite3_finalize(ppStmt);				
				   		}   	
				   	
						}									
					
	  					Open_BD(rc,db,Ventana);											
						for(j= 1; j < value+1;j++){
						//MessageBox(Ventana,rgVentasClaveInfo[j].clave,"Ejecutado Correctamente",MB_OK);	
						sprintf(output, "INSERT INTO venta (idProducto, fechaVenta, horaVenta, curpUsuario) VALUES ('%s', DATE('now', '-1 days'), TIME('now', '-6 hours'),'%s');", rgVentasClaveInfo[j].clave, curpUsuario);						
						sql = output;
						rc = sqlite3_exec(db, sql, callbackInsert, 0, &zErrMsg);
						if(rc != SQLITE_OK){
      						MessageBox(Ventana,zErrMsg,"SQL error",MB_OK);
      						sqlite3_free(zErrMsg);
   						}	else{
	    					//MessageBox(hwnd,"Consulta realizada exitosamente","Ejecutado Correctamente",MB_OK);
					   	}
					   	
    					}
						sqlite3_close(db);   					    						
						MessageBox(Ventana,"Se ha registrado Correctamente","Ejecutado Correctamente",MB_OK);											
						SetDlgItemInt(Ventana,edit_Venta_Monto,0,0);
						SendMessage(EditMonto, EM_GETMODIFY, 0, 0);						
						SendMessage(Controles, LB_RESETCONTENT, 0, 0);						
						SendMessage(Controle, LB_RESETCONTENT, 0, 0);
						SetDlgItemInt(Ventana,static_Venta_Total,0,0);
						SetDlgItemInt(Ventana,static_Venta_Cambio,0,0);											
						
   					
					break;
					}
					case bnt_Venta_Limpiar:{
						//Lipiadores de Controles
						SetDlgItemInt(Ventana,edit_Venta_Monto,0,0);
						SendMessage(EditMonto, EM_GETMODIFY, 0, 0);						
						SendMessage(Controles, LB_RESETCONTENT, 0, 0);						
						SendMessage(Controle, LB_RESETCONTENT, 0, 0);
						SetDlgItemInt(Ventana,static_Venta_Total,0,0);
						SetDlgItemInt(Ventana,static_Venta_Cambio,0,0);											
						
					break;
					}
				}
			break;
		}
        case WM_CLOSE:{
            DestroyWindow(WndPila_QuitarCima(&Pila));
            EnableWindow(WndPila_Cima(Pila),TRUE);
            SetFocus(WndPila_Cima(Pila));
            break;
        }
        default:{
            return DefWindowProc(Ventana,Mensaje,wParam,lParam);
        }
    }
    return 0;
}

// Venata de Creditos
LRESULT CALLBACK WndProcConfig(HWND Ventana,UINT Mensaje,WPARAM wParam,LPARAM lParam){
    static HINSTANCE Instancia = NULL;
	switch(Mensaje){
    	case WM_CREATE:{
            Instancia = ((LPCREATESTRUCT)lParam)->hInstance;
                CreateWindowEx(0,"STATIC","Sistema desarrollado por alumnos de licenciatura en",SS_CENTER|WS_CHILD|WS_VISIBLE,80,49,440,53,Ventana,(HMENU)-1,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Ingeneria en Sistemas Computacionales",SS_CENTER|WS_CHILD|WS_VISIBLE,80,69,440,53,Ventana,(HMENU)-2,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","En la materia de Analisis de Sistemas 1",SS_CENTER|WS_CHILD|WS_VISIBLE,80,151,440,53,Ventana,(HMENU)-3,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Integrantes",SS_CENTER|WS_CHILD|WS_VISIBLE,80,178,440,53,Ventana,(HMENU)-4,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Ramon Jesus Puch Magaña",SS_CENTER|WS_CHILD|WS_VISIBLE,80,210,440,53,Ventana,(HMENU)-5,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Jose del Carmen Campan Lopez",SS_CENTER|WS_CHILD|WS_VISIBLE,80,239,440,53,Ventana,(HMENU)-6,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Angel Gustavo Graniel Torrez",SS_CENTER|WS_CHILD|WS_VISIBLE,80,266,440,53,Ventana,(HMENU)-7,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Sheyla  Patricia Perez",SS_CENTER|WS_CHILD|WS_VISIBLE,80,290,440,53,Ventana,(HMENU)-8,Instancia,NULL);
            	CreateWindowEx(0,"STATIC","Gracias por hacer uso de nuestro sistema.",SS_CENTER|WS_CHILD|WS_VISIBLE,80,317,440,53,Ventana,(HMENU)-7,Instancia,NULL);
            break;
        }    	
        case WM_CLOSE:{
            DestroyWindow(WndPila_QuitarCima(&Pila));
            EnableWindow(WndPila_Cima(Pila),TRUE);
            SetFocus(WndPila_Cima(Pila));
            break;
        }
        default:{
            return DefWindowProc(Ventana,Mensaje,wParam,lParam);
        }
    }
    return 0;
}

BOOL RegistrarClaseEx(UINT Estilo,HINSTANCE Instancia,LPCSTR NombreClase,LPCSTR NombreMenu,WNDPROC WndProcedimiento,HBRUSH Color){
    WNDCLASSEX wc;

    wc.style = Estilo;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = Instancia;
    wc.lpszClassName = NombreClase;
    wc.lpszMenuName = NombreMenu;
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
    wc.lpfnWndProc = WndProcedimiento;
    wc.hbrBackground = Color;

    if(RegisterClassEx(&wc)){
        return  TRUE;
    }
    return FALSE;
}
