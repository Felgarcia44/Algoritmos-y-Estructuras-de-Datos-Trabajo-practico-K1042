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
int fActual;

struct rConductorArchivo{
    int conductorID;
    int fVto;
    bool activo;
    int totInfracciones;
    char eMail[100];
};

struct rConductorNodo{
    int conductorID;
    int fVto;
    bool activo;
    int totInfracciones;
    char eMail[100];
    rConductorNodo *sgte;
    // string eMail;
};

rConductorNodo *listaConduc = NULL;
rConductorNodo *finalConduc = NULL;

struct rInfraccion{
    int infraccionID;
    double fechaHora;
    float monto;
    int conductorID;
    int provincia;
};

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

rConductorNodo *BuscarConductor (int conductorID){
    rConductorNodo *pAux = listaConduc;
    bool encontrado =false;
    while (pAux != NULL && encontrado == false){
        if (pAux->conductorID == conductorID)
            encontrado = true;
        else
            pAux = pAux->sgte;
    }

    return pAux;
}

void LevantarArchivo(bool MostrarMensajes){
    FILE *f;
    rConductorArchivo r;
    rConductorNodo *pNodo;
    f = fopen("D:\\utnTrabajos\\Conductores.bin","rb");

    while ( fread(&r,sizeof(rConductorArchivo),1,f) ) {

        pNodo = new rConductorNodo();
        pNodo->conductorID = r.conductorID;
        pNodo->fVto = r.fVto;
        pNodo->activo = r.activo;
        pNodo->totInfracciones = r.totInfracciones;
        //arrayConduc[i].eMail = r.eMail; no se puede hacer por ser variable array de char
        memcpy(pNodo->eMail, r.eMail, 100);
        pNodo->sgte = NULL;

        if (listaConduc == NULL)
            listaConduc = pNodo;
        else
            finalConduc->sgte = pNodo;

        finalConduc = pNodo;

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

    rConductorArchivo r ;

    cout << "ingrese los datos del conductor \n";
    cout << "Conductor ID: ";
    cin >> r.conductorID;

    rConductorNodo *pNodo = BuscarConductor(r.conductorID);

    if (pNodo != NULL){
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

    pNodo = new rConductorNodo();
    pNodo->conductorID = r.conductorID;
    pNodo->fVto = r.fVto;
    pNodo->activo = true;
    pNodo->totInfracciones = 0;
    //arrayConduc[i].eMail = r.eMail; no se puede hacer por ser variable array de char
    memcpy(pNodo->eMail, r.eMail, 100);
    pNodo->sgte = NULL;

    if (listaConduc == NULL)
        listaConduc = pNodo;
    else
        finalConduc->sgte = pNodo;

    finalConduc = pNodo;


    cout << "\n\n>> El conductor se ha ingresado con exito!!"<< endl;
    system("pause");

    return 1;
}


void DesactivarConductor(){
	int IDbuscado;
	bool encontrado = false;
	cout << "Ingrese el ID del conductor que desea desactivar" << endl;
	cin >> IDbuscado;

    rConductorNodo *pNodo= BuscarConductor(IDbuscado);

    if (pNodo != NULL){
        if (pNodo->activo){
             pNodo->activo = false;
             cout << "el conductor se ha encontrado y ha sido desactivado" << endl;
        }
        else{
            cout << "El conductor ya se encontraba desactivado. No se realizo ningun cambio."<< endl;
        }
    } else {
        cout << "ERROR: no hay ningun conductor que coincida con el ID buscado. " << endl;
    }

	system("pause");

}

bool BuscarInfraccion(int conductorID, int provBuscada) {
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
    struct rListado{
        int conductorID;
        char eMail[100];
        rListado *sgte;
    };

    rListado *pListado = NULL;
    rListado *pUltimo = NULL;
    rListado *pNodo;
    rListado r;
	int numProv;
    rConductorNodo *pAux;
    bool encontrado;

    cout << "Indique el numero de provincia que desea buscar: "<< endl;
	cin >> numProv;

	while(numProv < 0 || numProv > 23){
		cout << "ERROR: numero de provincia incorrecto (debe estar entre 1 y 23, opcion 0 para salir)\n";
		cout << "Vuelva a ingresar el numero. " << endl;
		cin >> numProv;
	}
    if (numProv == 0) //con 0 sale sin buscar
        return;

    FILE * f;
	rInfraccion infraccion;
	f = fopen("D:\\utnTrabajos\\procesados.bin", "r");
    while (fread(&infraccion, sizeof(rInfraccion), 1, f)){

        if(infraccion.provincia == numProv){
            encontrado = false;  // <-- se inicializa acá porqe hay que hacerlo por cada busqueda.

            // Si la lista de "encontrados" tiene algo...
            if (pListado != NULL ){
                pUltimo = pListado;

                // ...busco para ver si ya está el infractor de la infracción recién leída.
                while (pUltimo->conductorID != infraccion.conductorID && pUltimo->sgte != NULL)
                    pUltimo = pUltimo->sgte;

                if (pUltimo->conductorID == infraccion.conductorID)
                    encontrado = true;
            }

           // Si el infracctor de la infracción no existe en la lista...
           if (!encontrado) {
                // busco los datos del conductor
               pAux = BuscarConductor(infraccion.conductorID);

               // creo el nodo y lo valorizo
               pNodo = new rListado();
               pNodo->conductorID = pAux->conductorID;
               memcpy( pNodo->eMail, pAux->eMail, 100);
               pNodo->sgte = NULL;

               // Agrego el nodo a la lista: si está vacía, lo pongo al principio
               if (pListado == NULL)
                pListado = pNodo;
               else
                  // si tiene datos, lo pongo al final.
                pUltimo->sgte = pNodo;
           }

        }

    }
	fclose(f);

// ahora ajustamos esto: La manera de saber si "hay datos en la lista" es verificando pListado
    if(pListado == NULL){
         cout<< "No hay infracctores en dicha provincia" << endl;
         system("pause");
         return;
    }

    // Desde aquí: se imprime primero la cabecera que tendrá el listado
    cout << "Conductores con infraccion en la provincia: " << numProv << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    cout << "Conductor ID     e-mail "  ;
    cout << "------------     ---------------------------------------------";

    // adentro del WHILE: se recorre pListado y se imprime el detalle.

    // No tenés que usar pListado para recorrer pq vas a perder el puntero al primer nodo.
    // Podés reutilizar acá pUltimo para recorrer.
    // luego de imprimir cada conductor podés aprovechar para hacer el free(nodo);

    pUltimo = pListado;
    while (pUltimo != NULL){
        cout << "    " << pUltimo->conductorID << "      " <<  pUltimo->eMail << endl;
        // corro el puntero al nodo recién listado
        pListado = pUltimo;
        // avanzo el puntero al siguente
        pUltimo = pUltimo->sgte;
        // libero la memoria del nodo que ya se imprimió.
        free(pListado);
    }
    // FIN
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
    rConductorNodo *pNodo;

    // Se lee el archivo con nuevas infracciones.
    fNuevas = fopen("D:\\utnTrabajos\\infracciones.bin","rb");

    // Se abre el archivo histórico de infracciones donde se anexarán las nuevas.
    fProce = fopen("D:\\utnTrabajos\\procesados.bin","ab");

    while ( fread(&r, sizeof(rInfraccion), 1, fNuevas) ) {

        CasosLeidos++;
        fwrite(&r, sizeof(rInfraccion), 1, fProce);

        // Verifico si existe el Conductor de la infraccion recien leida.
        pNodo = BuscarConductor(r.conductorID);

        if (pNodo!= NULL)
            pNodo->totInfracciones++;
        else
            CasosNoExiste++;
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

void ImprimirDetalleInfraccciones(int condID){

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

    if (listaConduc == NULL) {
        cout << "Informe de conductores con detalle de infracciones. \n";
        cout << "  No se han cargado los datos aun. Por favor, utilice la opcion 1 del menu. \n";
        system ("pause");
        return;
    }

    cout << "Informe de conductores con detalle de infracciones. \n";
    cout << "--------------------------------------------------------------------------------------------" << endl;
    cout << "ConductorID  F.Vencimiento  Activo  TotInfracciones  eMail \n";
    cout << "-----------  -------------  ------  ---------------  ---------------------------------------" << endl;
    rConductorNodo *pNodo =listaConduc ;

    while (pNodo != NULL){

        if (pNodo->activo == true)
            activoSiNo = "SI";
        else
            activoSiNo = "NO";
        cout << "  " << pNodo->conductorID << "         ";
        cout << pNodo->fVto << "      ";
        cout << activoSiNo << "          ";
        cout << pNodo->totInfracciones << "       ";
        cout << pNodo->eMail << endl;


        ImprimirDetalleInfraccciones(pNodo->conductorID);
        pNodo = pNodo->sgte;
    }

/*
    cout << "  01234         20030117      Si          0001       gar@gmail.com" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
    cout << "                Id Infr: 00000123456  F.Hora: 20211014 17:21  $1005.23  Prov: 1234" << endl;
*/
    system("pause");
}

void RegistoVencidoEntreFechasHTML(rConductorNodo *pLista){
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
    while(pLista !=NULL){
        if((pLista->fVto)< fActual && (pLista->fVto) < fecha2 && (pLista->fVto) > fecha1 ){
            fprintf (pFile, "<tr>\n");
            fprintf (pFile,"<tr><td>%d</td><td>%d</td><td>%s</td>", pLista->conductorID, pLista->fVto, pLista->eMail);
            fprintf (pFile, "<tr>\n");
        }
        pLista = pLista->sgte;
    }
    fprintf (pFile, "</table></body></html>");
    fclose (pFile);
    cout<< "Informe generado con exito.\n";
    system("pause");
}

void RegistoVencidoEntreFechasCSV(rConductorNodo *pLista){
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
    while(pLista != NULL){
        if((pLista->fVto)< fActual && (pLista->fVto) < fecha2 && (pLista->fVto) > fecha1 ){
            fprintf (pFile, "%d,%d,%s\n", pLista->conductorID, pLista->fVto, pLista->eMail);
        }
        pLista = pLista->sgte;
    }
    fclose (pFile);
    cout<< "Informe generado con exito CSV.\n";
    system("pause");
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

    rConductorNodo *pNodo =listaConduc ;
    rConductorArchivo r;

    while (pNodo != NULL){
        r.conductorID = pNodo->conductorID;
        memcpy(r.eMail, pNodo->eMail, 100);
        r.fVto = pNodo->fVto;
        r.totInfracciones = pNodo->totInfracciones;
        r.activo = pNodo->activo;

        if (pNodo->activo == true &&  pNodo->fVto >= fActual){
            fwrite(&r, sizeof(rConductorArchivo),1,f);
            nRegOK++;
        }

        if (pNodo->activo == false)
            nRegInactivos++;

        if (pNodo->fVto < fActual)
            nRegVencidos++;

        pNodo = pNodo->sgte;
    }

    fclose(f);
    cout << "proceso terminado...\n";
    cout << "Cantidad de registros grabados: " << nRegOK << endl;
    cout << "Cantidad de registros vencidos: " << nRegVencidos << endl;
    cout << "Cantidad de registros desactivados: " << nRegInactivos << endl;

    system("pause");
}


void SalirPrograma(){
    rConductorNodo *pNodo = listaConduc;
    rConductorNodo *pAux;

    // Recorro la lista para liberar la mermoria.
    while (pNodo != NULL){
        pAux = pNodo;
        free(pNodo);
        pNodo = pAux->sgte;
    }
}

void RegistrarInfraccion(){
    system("cls");
    rInfraccion r ;

    cout << "ingrese los datos de la infraccion \n";

    cout << "Conductor ID: ";
    cin >> r.conductorID;

    rConductorNodo *pNodo = BuscarConductor(r.conductorID);

    if (pNodo == NULL){
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

    // OJO: las infracciones sólo se acumularán al Procesar el Lote con la opción 5 del menú.
    // pNodo->totInfracciones++;

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

            case 2: CargarNuevoConductor();
                    break;

            case 3: DesactivarConductor();
                    break;

            case 4: ConductoresConInfraccion();
                    break;

            case 5: ProcesarLoteInfraccion();
                    break;

            case 6: InformePantallaInfracciones();
                    break;

            case 7: RegistoVencidoEntreFechasHTML(listaConduc);
                    break;

            case 8: RegistoVencidoEntreFechasCSV(listaConduc);
                    break;

            case 9: FinalizarJornada();
                    SalirPrograma();
                    break;

            case 10: RegistrarInfraccion();
                    break;

            case 0: SalirPrograma();
                    break;

        }
    }

    return 0;
}
