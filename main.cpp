#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstring>
using namespace std;

/* Creado: 22/09/2021
   Referencia: TP K1042
   Autores:
      - Felipe Alejo García
      - Mora Hidalgo
      - Tomas Gomez Taboada
*/

bool LevantoSiNo = false;
int nRegistros = 0;   // <-- cant. de registros cargados en el array
int nTope = 0;        // <-- tamaño del array de Conductores en memoria
int fActual;

struct rConductor{
    int conductorID;
    int fVto;
    bool activo;
    int totInfracciones;
    char eMail[100];
    // string eMail;
};

struct rInfraccion{
    int infraccionID;
    double fechaHora;
    float monto;
    int conductorID;
    int provincia;
};

rConductor arrayConduc[1];

int MostrarMenu() {
    int opcionMenu;
    bool opcionOK = false;

    while (opcionOK == false) {
        system("cls");
        cout << "Menu del programa TP K1042" << endl;
        cout << "--------------------------------------------------------------------------------------------" << endl;
        cout << "1. Levantar los conductores del archivo Conductores.bin \n";
        cout << "2. Cargar un nuevo Conductor \n";
        cout << "3. Desactivar un Conductor existente. \n";
        cout << "4. Listar todos los conductores que tengan al menos una infraccion en una provincia \n";
        cout << "5. Procesar un lote de infracciones. \n";
        cout << "6. Mostrar por pantalla el informe de conductores con detalle de infracciones. \n";
        cout << "7. Mostrar todos los conductores con registro vencido entre fechas HTML \n";
        cout << "8. Mostrar todos los conductores con registro vencido entre fechas CSV. \n";
        cout << "9. Finalizar jornada \n ";
        cout << "10. ingresar una infraccion \n \n";
        cout << "0. Salir del programa \n \n";
        cout << "ingrese su opcion: ";
        cin >> opcionMenu ;

        if (opcionMenu < 0 || opcionMenu > 10) {
            cout << "Opcion incorrecta\n\n" ;
            system("pause");
        } else if (opcionMenu >= 2 && opcionMenu <= 9 && LevantoSiNo == false) {
            cout << "Debe levantar primero el archivo de coductores, utilizando la opcion 1 del menu, para ejecutar esta opcion.\n" ;
            system("pause");
        } else
            opcionOK = true;

    }

    return opcionMenu;
}


void LevantarArchivo(bool MostrarMensajes){
    FILE *f;
    rConductor r;
    f = fopen("D:\\utnTrabajos\\Conductores.bin","rb");

    fseek( f, 0, SEEK_END );
    int tamanoArchivo = ftell(f);

    nRegistros = tamanoArchivo / sizeof(rConductor);
    if (MostrarMensajes)
        cout << "total reg. leidos: " << nRegistros << endl;

    nTope = nRegistros + 10;
    arrayConduc[nTope];

    fseek( f, 0, 0 );
    int i=0;
    while ( fread(&r,sizeof(rConductor),1,f) ) {

        arrayConduc[i].conductorID = r.conductorID;
        arrayConduc[i].fVto = r.fVto;
        arrayConduc[i].activo = r.activo;
        arrayConduc[i].totInfracciones = r.totInfracciones;
        //arrayConduc[i].eMail = r.eMail; no se puede hacer por ser variable array de char
        memcpy(arrayConduc[i].eMail, r.eMail, 100);
        i++;

        if (MostrarMensajes)
            cout << r.conductorID << ", " << r.fVto << ", " << r.activo << ", " << r.totInfracciones << ", " << r.eMail << endl;
    }

    fclose(f);

    if (MostrarMensajes)
        system("pause");

    LevantoSiNo = true;
}

int CargarNuevoConductor(){
    system("cls");

    rConductor r ;

    cout << "ingrese los datos del conductor \n";
    cout << "Conductor ID: ";
    cin >> r.conductorID;
    int i=0;
    bool existe = false;
    while (i  < nRegistros && !existe){
       if( r.conductorID ==  arrayConduc[i].conductorID)
        existe = true;
       i++;
    }

    if (existe){
        cout << "el conductor ingresado ya existe"<< endl;
        system("pause");
        return 0;
    }
    int EvaluarFecha = 1;
    while (EvaluarFecha == 1){
        cout << "fecha de vencimiento(AAAAMMDD): ";
        cin >> r.fVto;
        if (r.fVto < fActual){
            cout << "AVISO: la fecha del registro del conductor esta vencida, se guardara igualmente en memoria, al grabar archivo se eliminara" << endl;
            cout << "En caso de querer guardar registro del mismo, utilizar la opcion 7 u 8 para generar un archivo html o CSV respectivamente." << endl;
            system("pause");
        }
        EvaluarFecha = 0;
    }


    cout << "eMail: ";
    cin >> r.eMail;

    i = nRegistros;
    arrayConduc[i].conductorID = r.conductorID;
    arrayConduc[i].fVto = r.fVto;
    arrayConduc[i].activo = true;
    arrayConduc[i].totInfracciones = 0;
    memcpy(arrayConduc[i].eMail, r.eMail, 100);
    nRegistros++;

    cout << "\n\n>> El conductor se ha ingresado con exito!!"<< endl;
    system("pause");

    return 1;
}


void DesactivarConductor(){
	int IDbuscado;
	bool encontrado = false;
	cout << "Ingrese el ID del conductor que desea desactivar" << endl;
	cin >> IDbuscado;
    int i=0;
	while (!encontrado && i < nRegistros){
		if (arrayConduc[i].conductorID == IDbuscado){
            encontrado = true;
            if (arrayConduc[i].activo){
                 cout << "el conductor se ha encontrado y ha sido desactivado" << endl;
                 arrayConduc[i].activo = false;
            }
            else
                cout << "El conductor ya se encontraba desactivado. No se realizo ningun cambio."<< endl;
		}
        i++;
	}

	if (encontrado == false)
		cout << "ERROR: no hay ningún conductor que coincida con el ID buscado. " << endl;

	system("pause");

}

bool BuscarInfraccion(int conductorID, int provBuscada)
{
	FILE * f;
	rInfraccion infraccion;
	f = fopen("D:\\utnTrabajos\\procesados.bin", "r");
    while (fread(&infraccion, sizeof(rInfraccion), 1, f)){
        if(infraccion.conductorID == conductorID && infraccion.provincia == provBuscada)
        return true;
    }
	fclose(f);
	return false;
}

void ConductoresConInfraccion(){
	int numProv;
	bool TenerInfraccion;

	cout << "Indique el numero de provincia que desea buscar: "<< endl;
	cin >> numProv;

	while(numProv < 0 || numProv > 23){
		cout << "ERROR: numero de provincia incorrecto (debe estar entre 1 y 23, opcion 0 para salir)\n";
		cout << "Vuelva a ingresar el numero. " << endl;
		cin >> numProv;
	}

    if (numProv == 0) //con 0 sale sin buscar
        return;

	for (int i=0; i < nRegistros; i++)
	{
		TenerInfraccion = BuscarInfraccion(arrayConduc[i].conductorID, numProv);

		if (TenerInfraccion){
            cout << "Conductor: " << arrayConduc[i].conductorID << "   ";
            cout << "Fecha de vencimiento: " << arrayConduc[i].fVto << "   ";
            if (arrayConduc[i].activo)
               cout << "Estado: activo" << endl;
            else
               cout << "Estado: inactivo" << endl;
             cout << "----------------------------------------------" << endl;
        }
	}
	system("pause");
}

void ProcesarLoteInfraccion(){
    FILE *fNuevas;
    FILE *fProce;
    rInfraccion r;
    int i;
    bool existe;
    int CasosLeidos=0;
    int CasosNoExiste=0;

    // Se lee el archivo con nuevas infracciones.
    fNuevas = fopen("D:\\utnTrabajos\\infracciones.bin","rb");

    // Se abre el archivo histórico de infracciones donde se anexarán las nuevas.
    fProce = fopen("D:\\utnTrabajos\\procesados.bin","ab");

    while ( fread(&r, sizeof(rInfraccion), 1, fNuevas) ) {

        CasosLeidos++;
        fwrite(&r, sizeof(rInfraccion), 1, fProce);

        // Verifico si existe el Conductor de la infraccion recien leida.
        i=0;
        existe = false;
        while (i  < nRegistros && !existe){
           if( r.conductorID ==  arrayConduc[i].conductorID)
            existe = true;

          i++;
        }

        if (!existe){
            CasosNoExiste++;
        }


    }

    fclose(fNuevas);
    fclose(fProce);

    // inicializo el archivo de infracciones (lo vacio)
    fNuevas = fopen("D:\\utnTrabajos\\infracciones.bin","wb");
    fclose(fNuevas);

    cout << "Proceso de Lote terminado\n";
    cout << "Leidos: " << CasosLeidos << "\n";
    cout << "Conductores inexistentes: " << CasosNoExiste << "\n";
    system("pause");

}

void ImprimirDetalleInfraccciones(int condID) {

    FILE *f;
    rInfraccion r;
    f = fopen("D:\\utnTrabajos\\procesados.bin","rb");

    while ( fread(&r, sizeof(rInfraccion), 1, f) ) {
        if (r.conductorID == condID){
            std::string strFecha = std::to_string(r.fechaHora);
            std::string strMonto = std::to_string(r.monto);
            // strFecha = strFecha.substr(0, 12);
            strFecha = strFecha.substr(0, 8) + " " + strFecha.substr(8, 2) + ":" + strFecha.substr(10, 2);
            cout << "                Id Infr: " << r.infraccionID << "  F.Hora: " << strFecha << "  $" << strMonto << "  Prov: " << r.provincia << endl;

        }

    }

    fclose(f);

}


void InformePantallaInfracciones(){
    string activoSiNo;
    system("cls");

    if (nRegistros == 0) {

        cout << "Informe de conductores con detalle de infracciones. \n";
        cout << "  No se han cargado los datos aun. Por favor, utilice la opcion 1 del menu. \n";
        system ("pause");
        return;
    }

    cout << "Informe de conductores con detalle de infracciones. \n";
    cout << "--------------------------------------------------------------------------------------------" << endl;
    cout << "ConductorID  F.Vencimiento  Activo  TotInfracciones  eMail \n";
    cout << "-----------  -------------  ------  ---------------  ---------------------------------------" << endl;
    for (int i=0; i< nRegistros; i++){

        if (arrayConduc[i].activo == true)
            activoSiNo = "SI";
        else
            activoSiNo = "NO";
        cout << "  " << arrayConduc[i].conductorID << "         ";
        cout << arrayConduc[i].fVto << "      ";
        cout << activoSiNo << "          ";
        cout << arrayConduc[i].totInfracciones << "       ";
        cout << arrayConduc[i].eMail << endl;


        ImprimirDetalleInfraccciones(arrayConduc[i].conductorID);

    }

/*
    cout << "  01234         20030117      Si          0001       gar@gmail.com" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
*/
    system("pause");
}

void RegistoVencidoEntreFechasHTML(rConductor v[], int tam){
    int fecha1, fecha2;

    cout << "ingrese las fechas entre las cuales quiere un informe de conductores vencidos\n";
    cout << "ingrese primera fecha: \n";
    cin >> fecha1;
    cout << "ingrese segunda fecha: \n";
    cin >> fecha2;

    FILE * pFile;
    pFile = fopen ("D:\\utnTrabajos\\InformeVencidos.html","w");
    if (pFile == NULL){
        cout<< "no se pudo generar el archivo";
        system("pause");
        return ;
    }
    // Cabecera del archivo HTML
    fprintf (pFile,"<html><head>Informe</head><title>Informe HTML</title><body><table>");
    // Encabezados de columnas de la tabla que mostrará la info del conductor.
    fprintf (pFile,"<tr><td>Conductor ID</td><td>F.Vencimiento</td><td>e-mail</td>");
    for (int i=0; i<tam; i++){
        if((v[i].fVto)< fActual && (v[i].fVto) < fecha2 && (v[i].fVto) > fecha1 ){
            fprintf (pFile, "<tr>\n");
            fprintf (pFile,"<tr><td>%d</td><td>%d</td><td>%s</td>", v[i].conductorID, v[i].fVto, v[i].eMail);
            fprintf (pFile, "<tr>\n");
        }
    }
    fprintf (pFile, "</table></body></html>");
    fclose (pFile);
    cout<< "Informe generado con exito.\n";
    system("pause");
}

void RegistoVencidoEntreFechasCSV(rConductor v[], int tam){
    int fecha1, fecha2;

    cout << "ingrese las fechas entre las cuales quiere un informe de conductores vencidos\n";
    cout << "ingrese primera fecha: \n";
    cin >> fecha1;
    cout << "ingrese segunda fecha: \n";
    cin >> fecha2;

    FILE * pFile;
    pFile = fopen ("D:\\utnTrabajos\\InformeVencidos.csv","w");
    if (pFile == NULL){
        cout<< "no se pudo generar el archivo";
        system("pause");
        return ;
    }
    // Encabezados de columnas de la tabla que mostrará la info del conductor.

    fprintf (pFile, "Conductor ID;F.Vencimiento;e-mail\n");
    for (int i=0; i<tam; i++){
           cout<< "Registro:" << i << endl;
        if((v[i].fVto)< fActual && (v[i].fVto) < fecha2 && (v[i].fVto) > fecha1 ){
            fprintf (pFile, "%d,%d,%s\n", v[i].conductorID, v[i].fVto, v[i].eMail);
        cout<< "vencimiento:" << v[i].fVto << endl;
        }
    }
    fclose (pFile);
    cout<< "Informe generado con exito CSV.\n";
    system("pause");
}

int CalcularInfracciones(int condID){

    FILE *f;
    rInfraccion r;
    f = fopen("D:\\utnTrabajos\\procesados.bin","rb");
    int totalInfracciones=0;

    while ( fread(&r, sizeof(rInfraccion), 1, f) ) {
        if (r.conductorID == condID){
           totalInfracciones++;
        }
    }

    fclose(f);
    return totalInfracciones;
}

// Al finalizar el día se reescribe el archivo “Conductores.bin” con los registros de aquellos
// conductores a los que no se le ha vencido su carnet y que estén activos.
void FinalizarJornada(){
    int i;
    FILE *f;
    int nRegOK = 0;
    int nRegInactivos = 0;
    int nRegVencidos = 0;

    f=fopen("D:\\utnTrabajos\\Conductores.bin","wb");
    for (i=0; i < nRegistros; i++) {
        if (arrayConduc[i].activo == true &&  arrayConduc[i].fVto >= fActual){

            arrayConduc[i].totInfracciones = CalcularInfracciones(arrayConduc[i].conductorID);

            fwrite(&arrayConduc[i], sizeof(rConductor),1,f);
            nRegOK++;
        }

        if (arrayConduc[i].activo == false)
            nRegInactivos++;

        if (arrayConduc[i].fVto < fActual)
            nRegVencidos++;
     }
    fclose(f);
    cout << "proceso terminado...\n";
    cout << "Cantidad de registros grabados: " << nRegOK << endl;
    cout << "Cantidad de registros vencidos: " << nRegVencidos << endl;
    cout << "Cantidad de registros desactivados: " << nRegInactivos << endl;

    system("pause");
}

void GrabarConductores(){

    FILE *f;
    f=fopen("D:\\utnTrabajos\\Conductores.bin","wb");
    for (int i = 0 ; i < nRegistros; i++) {
            fwrite(&arrayConduc[i], sizeof(rConductor),1,f);
     }
    fclose(f);
}


void SalirPrograma(){
}

void RegistrarInfraccion(){
    system("cls");
    rInfraccion r ;
    int verificarConductor;
    cout << "ingrese los datos de la infraccion \n";

    cout << "Conductor ID: ";
    cin >> verificarConductor;
    int i=0;
    bool existe = true;
    while (i  < nRegistros && existe == true){
       if( verificarConductor == arrayConduc[i].conductorID){
         existe = false;
         r.conductorID = verificarConductor;
       }
       i++;
    }

    if (existe){
        cout << "El ID del conductor ingresado no existe"<< endl;
        system("pause");
        return ;
    }

    cout << "infraccion ID: ";
    cin >> r.infraccionID;

    cout << "fecha y hora (aaaammddHHMM): ";
    cin >> r.fechaHora;

    cout << "monto: ";
    cin >> r.monto;

    int verificarProvincia;
    bool existeProv = true;
    while (existeProv){
        cout << "provincia ID: ";
        cin >> verificarProvincia;
        if (verificarProvincia < 23 && verificarProvincia > 1 ){
               r.provincia = verificarProvincia;
               existeProv = false;
        }
        else
            cout << "No existe la provincia ingresada. Debe estar entre 1 y 23." << endl;
    }


    FILE *f;
    f=fopen("D:\\utnTrabajos\\infracciones.bin","ab");
    fwrite(&r, sizeof(rInfraccion),1,f);
    fclose(f);

    cout << "infraccion grabada con exito\n";
    system("pause");
}


int main()
{
    int opcionMenu = 1;
    int cantNuevos = 0;
    int a,m,d;

    cout << "Introduzca el anio actual" << endl;
    cin >> a;

    cout << "Introduzca el mes actual" << endl;
    cin >> m;

    cout << "Introduzca el dia de hoy" << endl;
    cin >> d;

    fActual = ((a*10000)+(m*100)+d);


    while (opcionMenu != 0 && opcionMenu != 9){
        opcionMenu = MostrarMenu();

        switch (opcionMenu) {

            case 1: LevantarArchivo(true);
                    break;

            case 2: if (cantNuevos == 10) {
                       GrabarConductores();
                       LevantarArchivo(false);
                       cantNuevos = 0;
                    }

                    cantNuevos += CargarNuevoConductor();
                    break;

            case 3: DesactivarConductor();
                    break;

            case 4: ConductoresConInfraccion();
                    break;

            case 5: ProcesarLoteInfraccion();
                    break;

            case 6: InformePantallaInfracciones();
                    break;

            case 7: RegistoVencidoEntreFechasHTML(arrayConduc, nRegistros);
                    break;

            case 8: RegistoVencidoEntreFechasCSV(arrayConduc, nRegistros);
                    break;

            case 9: FinalizarJornada();
                    break;

            case 10: RegistrarInfraccion();
                    break;

            case 0: if (cantNuevos > 0)
                        SalirPrograma();
                    break;


        }
    }

    return 0;
}
