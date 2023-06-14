#ifndef CONEXIONBD_H
#define CONEXIONBD_H

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "SQLite\sqlite3.h"


void Open_BD(int rc, sqlite3 *db, HWND hwnd){    
    if(rc){
		MessageBox(hwnd,sqlite3_errmsg(db),"No se pudo abrir la base de datos",MB_ICONERROR);
   	}else{//muestra si la conezion es exitosa
     	//MessageBox(hwnd,"Conexion Exitosa","Base de datos abierta exitosamente",MB_OK);
   	}
}

static int callbackInsert(void *NotUsed, int argc, char **argv, char **azColName) {
   	int i;
   	for(i = 0; i<argc; i++) {
     	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   	}
   	printf("\n");
   	return 0;
}


/*

static int callbackOther(void *data, int argc, char **argv, char **azColName){
    int i;
    for(i = 0; i<argc; i++){
        printf("\t%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void abrirBaseDatosCarrito(int rc, sqlite3 *db){
    if(rc){
        fprintf(stderr, "\n\t[ERROR: NO SE PUDO ABRIR LA BASE DE DATOS]: %s\n", sqlite3_errmsg(db));
    }else{
        //fprintf(stderr, "\n\t[BASE DE DATOS ABIERTA CORRECTAMENTE]\n\n");
    }
}
void ejecutarConsulta(int rc, char *zErrMsg){
    if(rc != SQLITE_OK){
        fprintf(stderr, "\n\tSQL ERROR: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "\n\t[OPERACION REALIZADA EXITOSAMENTE]\n");
    }
}

void ejecutarConsultaCarrito(int rc, char *zErrMsg){
    if(rc != SQLITE_OK){
        fprintf(stderr, "\n\tSQL ERROR: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        //fprintf(stdout, "\n\t[OPERACION REALIZADA EXITOSAMENTE]\n");
    }
}
*/

#endif // CONEXIONBD_H
