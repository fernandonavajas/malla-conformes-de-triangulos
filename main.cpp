#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <math.h>
#include <cmath>
using namespace std;

// Nombre: Fernando Navajas Ortiz
// Asignatura: Computacion paralela y distribuida
// Docente: Sebastian Salazar
// Ayudante: Fernando Pizza Rubilar
// Fecha: 15/12/2018


int contadorMesh=0, contadorNode=0, contadorDeTriangulos=0,numeroDeResultados=0;

float matriz[23][9666]; //[filas][columnas]
float resultado[9][50000]; //[filas][columnas]

void NodesAVector(string  x){

	char *y = new char[x.length() + 1];
	strcpy(y, x.c_str());
	char *numero;
	numero = strtok (y," ");
	int c=0;
	float a[3];
	while (numero != NULL)
	{
		a[c]=atof(numero);
		c++;
		numero = strtok(NULL, " ");// decrese el contador de strtok
	}
	for(int i=0;i<9666;i++){//i columnas
		for(int j=0;j<3;j++){//filas
			if(matriz[0][i]==a[0]){
				matriz[3][i]=a[1];
				matriz[4][i]=a[2];
			}
			if(matriz[1][i]==a[0]){
				matriz[5][i]=a[1];
				matriz[6][i]=a[2];
			}
			if(matriz[2][i]==a[0]){
				matriz[7][i]=a[1];
				matriz[8][i]=a[2];
			}
		}
	}
  delete[] y; // liberar la memoria del "new" creado para el char *y

}
void MeshAVector(string  x){
  char *y = new char[x.length() + 1], *numero;
  strcpy(y, x.c_str());
  numero = strtok (y," ");
  int i=0;
  while (numero != NULL)
  {
	matriz[i][contadorDeTriangulos]=atoi(numero);
    i++;
    numero = strtok(NULL, " ");// decrese el contador de strtok
  }
  delete[] y; // liberar la memoria del "new" creado para el char *y
}

void LeerArchivos(){

	string linea;// variable para capturar por lienas los ficheros de texto

	ifstream fmesh, fnode;// leer los ficheros
    fmesh.open ("espiral.mesh");
    fnode.open ("espiral.node");

    if (fmesh.fail() or fnode.fail()){  // si el arcjhivo no se llama igual o no esta en el directorio del proyecto
        cout<< "El fichero no existe, coloque el archivo 'espiral.mesh' en el directorio del programa"<<endl;
    }
    else{
		while (!fmesh.eof()){ // mientras no llegue al final
                getline(fmesh, linea);// captura linea a linea
                MeshAVector(linea);
                contadorDeTriangulos++;
                contadorMesh++;
		}
        while (!fnode.eof()){ // mientras no llegue al final
                getline(fnode, linea);// captura linea a linea
                NodesAVector(linea);
                contadorNode++;
		}

		cout<<" El archivo mesh posee: "<<contadorMesh<<" registros"<<endl;
		cout<<" El archivo node posee: "<<contadorNode<<" registros"<<endl;
	}

}

void CalcularLargos(){
	for(int i=0;i<9666;i++){ //columnas i

			matriz[9][i] = sqrt(pow((matriz[3][i]-matriz[5][i]),2)+(pow((matriz[4][i]-matriz[6][i]),2)));
			matriz[10][i] = sqrt(pow((matriz[5][i]-matriz[7][i]),2)+(pow((matriz[6][i]-matriz[8][i]),2)));
			matriz[11][i] = sqrt(pow((matriz[3][i]-matriz[7][i]),2)+(pow((matriz[4][i]-matriz[8][i]),2)));
		}

}

void CalcularAngulos(){
	for(int i=0;i<9666;i++){ //columnas i

			double angulo3radianes = acos(((pow(matriz[9][i],2)-pow(matriz[10][i],2)-pow(matriz[11][i],2))/(-2*matriz[10][i]*matriz[11][i])));
			double angulo2radianes = acos(((pow(matriz[11][i],2)-pow(matriz[10][i],2)-pow(matriz[9][i],2))/(-2*matriz[10][i]*matriz[9][i])));
			matriz[14][i] = angulo3radianes*180/3.1416; // angulo 3
			matriz[13][i] = angulo2radianes*180/3.1416; // angulo 2
			matriz[12][i] = (180-(matriz[13][i]+matriz[14][i])); // angulo 1
		}

}

void BusquedaDeLargo(float a){
	float mayor=0;
	int posicion=0;
	for(int i=0;i<9666;i++){ //columnas i
		if((matriz[12][i]<=a) or (matriz[13][i]<=a) or (matriz[14][i]<=a)){
			for(int j=9;j<12;j++){
				if (matriz[j][i]>mayor){
					mayor=matriz[j][i];
					posicion=j;
				}
			}
			if(posicion==9){ // si el lado mas largo esta compuesto por el punto 1 y 2
				matriz[15][i]=matriz[0][i]; // punto 1
				matriz[16][i]=matriz[1][i]; // punto 2
				matriz[20][i]=matriz[2][i]; // punto 3, que seria el contrario******
				matriz[17][i]=((matriz[3][i]+matriz[5][i])/2); // cordenanda x del nuevo punto
				matriz[18][i]=((matriz[4][i]+matriz[6][i])/2); // cordenanda y del nuevo punto
				contadorDeTriangulos++;
				matriz[19][i]=contadorDeTriangulos; // numero asignado del nuevo punto
				matriz[21][i]=matriz[7][i];
				matriz[22][i]=matriz[8][i];

			}
			if(posicion==10){ // si el lado mas largo esta compuesto por el punto 2 y 3
				matriz[15][i]=matriz[1][i]; // punto 2
				matriz[16][i]=matriz[2][i]; // punto 3
				matriz[20][i]=matriz[0][i]; // punto 1, que seria el contrario******
				matriz[17][i]=((matriz[5][i]+matriz[7][i])/2); // cordenanda x del nuevo punto
				matriz[18][i]=((matriz[6][i]+matriz[8][i])/2); // cordenanda y del nuevo punto
				contadorDeTriangulos++;
				matriz[19][i]=contadorDeTriangulos; // numero asignado del nuevo punt
				matriz[21][i]=matriz[3][i];
				matriz[22][i]=matriz[4][i];
			}
			if(posicion==11){ // si el lado mas largo esta compuesto por el punto 1 y 3
				matriz[15][i]=matriz[0][i]; // punto 1
				matriz[16][i]=matriz[2][i]; // punto 3
				matriz[20][i]=matriz[1][i]; // punto 2, que seria el contrario******
				matriz[17][i]=((matriz[3][i]+matriz[7][i])/2); // cordenanda x del nuevo punto
				matriz[18][i]=((matriz[4][i]+matriz[8][i])/2); // cordenanda y del nuevo punto
				contadorDeTriangulos++;
				matriz[19][i]=contadorDeTriangulos; // numero asignado del nuevo punto
				matriz[21][i]=matriz[5][i];
				matriz[22][i]=matriz[6][i];
			}
		}
	}
	//******************** los nodos generados duplicados unirlos en 1 solo ****************************
	for(int i=0;i<9666;i++){
		for(int j=0;j<9666;j++){
			if((matriz[17][i]==matriz[17][j]) and (matriz[18][i]==matriz[18][j]) ){
				// cuando las cordenadas de 2 puntos nuevos sean iguales
				matriz[19][j]=matriz[19][i]; // copiar el numero de nodo para ambos, esto porque son el mismo nodo.
			}
		}
	}
}

void GeneracionDeMatrizTemporal(){
	float temporal[10][20000];
	int a=0; // auxiliar
	// ************ grabar en la matriz temporal los triangulos separados
	for(int i=0;i<9666;i++){ //columnas i
		if(matriz[19][i]!=0){ // triangulos afectador por la condicion inicial
			temporal[0][a]=matriz[19][i];// el nuevo nodo
			temporal[1][a]=matriz[20][i];// el nodo contrario al nuevo nodo
			temporal[2][a]=matriz[15][i];// el nodo restante 1 de 2
			temporal[3][a]=matriz[17][i];// cordenada x del nodo nuevo
			temporal[4][a]=matriz[18][i];// cordenada y del nodo nuevo
			temporal[5][a]=matriz[21][i];// cordenada x del nodo contrario
			temporal[6][a]=matriz[22][i];// cordenada y del nodo contrario
			if(matriz[15][i]==matriz[0][i]){
				temporal[7][a]=matriz[3][i];
				temporal[8][a]=matriz[4][i];
			}
			if(matriz[15][i]==matriz[1][i]){
				temporal[7][a]=matriz[5][i];
				temporal[8][a]=matriz[6][i];
			}
			if(matriz[15][i]==matriz[2][i]){
				temporal[7][a]=matriz[7][i];
				temporal[8][a]=matriz[8][i];
			}
			a++;// incrementamos en contador para grabar el nodo restante
			temporal[0][a]=matriz[19][i];// el nuevo nodo
			temporal[1][a]=matriz[20][i];// el nodo que une el triangulo separado
			temporal[2][a]=matriz[16][i];// el nodo restante 2 de 2
			temporal[3][a]=matriz[17][i];// cordenada x del nodo nuevo
			temporal[4][a]=matriz[18][i];// cordenada y del nodo nuevo
			temporal[5][a]=matriz[21][i];// cordenada x del nodo contrario
			temporal[6][a]=matriz[22][i];// cordenada y del nodo contrario
			if(matriz[16][i]==matriz[0][i]){// si
				temporal[7][a]=matriz[3][i];// que guarde la cordenada x1
				temporal[8][a]=matriz[4][i];// que guarde la cordenada y1
			}
			if(matriz[16][i]==matriz[1][i]){
				temporal[7][a]=matriz[5][i];// que guarde la cordenada x2
				temporal[8][a]=matriz[6][i];// que guarde la cordenada y2
			}
			if(matriz[16][i]==matriz[2][i]){
				temporal[7][a]=matriz[7][i];// que guarde la cordenada x3
				temporal[8][a]=matriz[8][i];// que guarde la cordenada y3
			}
			a++;

		}
		if(matriz[19][i]==0){ // los triangulos no afectados por la condicion inicial
			temporal[0][a]=matriz[0][i];// el nodo 1
			temporal[1][a]=matriz[1][i];// el nodo 2
			temporal[2][a]=matriz[2][i];// el nodo 3
			temporal[3][a]=matriz[3][i];// cordenada x1
			temporal[4][a]=matriz[4][i];// cordenada y1
			temporal[5][a]=matriz[5][i];// cordenada x2
			temporal[6][a]=matriz[6][i];// cordenada y2
			temporal[7][a]=matriz[7][i];// cordenada x3
			temporal[8][a]=matriz[8][i];// cordenada y3
			a++;

		}
	}

	//******************** los nodos generados duplicados unirlos en 1 solo ****************************
	for(int i=0;i<a;i++){
		for(int j=0;j<a;j++){
			if((temporal[3][i]==temporal[3][j]) and (temporal[4][i]==temporal[4][j]) ){
				// cuando las cordenadas de 2 puntos nuevos sean iguales
				temporal[0][j]=temporal[0][i]; // copiar el numero de nodo para ambos, esto porque son el mismo nodo.
			}
		}
	}
	//cout<<"a :"<<a<<endl;
	// **************** Si el triangulo adyacente a la arista biseccionada necesita correcion ********
	int h=0;
	a=a-1;
	for(int i=0;i<9666;i++){
		//cout<<h<<" "<<endl;
		for(int j=0;j<a;j++){
			if((matriz[15][i]==temporal[0][j]) and (matriz[16][i]==temporal[1][j]) and (temporal[9][j]==0)){// p1=nodo[0] y p2=nodo[1]
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[1][j];
					resultado[2][h]=temporal[2][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[6][j];
					resultado[7][h]=temporal[7][j];
					resultado[8][h]=temporal[8][j];
					h++;
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[0][j];
					resultado[2][h]=temporal[2][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[4][j];
					resultado[7][h]=temporal[7][j];
					resultado[8][h]=temporal[8][j];
					h++;
					temporal[9][j]=1;
					//cout<<" for 1 "<<resultado[0][h]<<" ";
			}
			if((matriz[15][i]==temporal[0][j]) and (matriz[16][i]==temporal[2][j])and (temporal[9][j]==0)){// p1=nodo[0] y p2=nodo[2]
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[1][j];
				resultado[2][h]=temporal[2][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[6][j];
				resultado[7][h]=temporal[7][j];
				resultado[8][h]=temporal[8][j];
				//cout<<" for 2 "<<resultado[0][0]<<" ";
				h++;
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[1][j];
				resultado[2][h]=temporal[0][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[6][j];
				resultado[7][h]=temporal[3][j];
				resultado[8][h]=temporal[4][j];
				h++;
				temporal[9][j]=1;
			}
			if((matriz[16][i]==temporal[0][j]) and (matriz[15][i]==temporal[1][j])and (temporal[9][j]==0)){// p2=nodo[0] y p1=nodo[1]
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[1][j];
					resultado[2][h]=temporal[2][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[6][j];
					resultado[7][h]=temporal[7][j];
					resultado[8][h]=temporal[8][j];
            //cout<<" for 3 "<<resultado[0][0]<<" ";
					h++;
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[0][j];
					resultado[2][h]=temporal[2][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[4][j];
					resultado[7][h]=temporal[7][j];
					resultado[8][h]=temporal[8][j];
					h++;
					temporal[9][j]=1;
			}
			if((matriz[16][i]==temporal[0][j]) and (matriz[15][i]==temporal[2][j])and (temporal[9][j]==0)){// p2=nodo[0] y p1=nodo[2]
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[1][j];
					resultado[2][h]=temporal[2][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[6][j];
					resultado[7][h]=temporal[7][j];
					resultado[8][h]=temporal[8][j];
					//cout<<" for 4 "<<resultado[0][0]<<" ";
					h++;
					resultado[0][h]=matriz[19][i];
					resultado[1][h]=temporal[1][j];
					resultado[2][h]=temporal[0][i];
					resultado[3][h]=matriz[17][i];
					resultado[4][h]=matriz[18][i];
					resultado[5][h]=temporal[5][j];// creará el triangulo 1 de 2
					resultado[6][h]=temporal[6][j];
					resultado[7][h]=temporal[3][j];
					resultado[8][h]=temporal[4][j];
					h++;
					temporal[9][j]=1;
				}
			if((matriz[15][i]==temporal[1][j]) and (matriz[16][i]==temporal[2][j])and (temporal[9][j]==0)){// p1=nodo[1] y p2=nodo[2]
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[0][j];
				resultado[2][h]=temporal[1][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[4][j];
				resultado[7][h]=temporal[5][j];
				resultado[8][h]=temporal[6][j];
				//cout<<" for 5 "<<resultado[0][0]<<" ";
				h++;
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[0][j];
				resultado[2][h]=temporal[2][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[4][j];
				resultado[7][h]=temporal[7][j];
				resultado[8][h]=temporal[8][j];
				h++;
				temporal[9][j]=1;
			}
			if((matriz[16][i]==temporal[1][j]) and (matriz[15][i]==temporal[2][j])and (temporal[9][j]==0)){// p2=nodo[1] y p1=nodo[2]
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[0][j];
				resultado[2][h]=temporal[1][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[4][j];
				resultado[7][h]=temporal[5][j];
				resultado[8][h]=temporal[6][j];
				//cout<<" for 6 "<<resultado[0][0]<<" ";
				h++;
				resultado[0][h]=matriz[19][i];
				resultado[1][h]=temporal[0][j];
				resultado[2][h]=temporal[2][i];
				resultado[3][h]=matriz[17][i];
				resultado[4][h]=matriz[18][i];
				resultado[5][h]=temporal[3][j];// creará el triangulo 1 de 2
				resultado[6][h]=temporal[4][j];
				resultado[7][h]=temporal[7][j];
				resultado[8][h]=temporal[8][j];
				h++;
				temporal[9][j]=1;
			}
		}

	}
	//cout<<h<<" triangulos separados"<<endl;
	for(int i=0;i<a;i++){
		if(temporal[9][i]==0){
			resultado[0][h]=temporal[0][i];
			resultado[1][h]=temporal[1][i];
			resultado[2][h]=temporal[2][i];
			resultado[3][h]=temporal[3][i];
			resultado[4][h]=temporal[4][i];
			resultado[5][h]=temporal[5][i];// creará el triangulo 1 de 2
			resultado[6][h]=temporal[6][i];
			resultado[7][h]=temporal[7][i];
			resultado[8][h]=temporal[8][i];
			h++;
		}
	}

	cout<<h<<" total = separados + no afectados"<<endl;
	for(int i=0;i<10;i++){
			cout<<" P1: "<<resultado[0][i]<<" ("<<resultado[3][i]<<" , "<<resultado[4][i]<<") \t";
			cout<<" P2: "<<resultado[1][i]<<" ("<<resultado[5][i]<<" , "<<resultado[6][i]<<") \t";
			cout<<" P3: "<<resultado[2][i]<<" ("<<resultado[7][i]<<" , "<<resultado[8][i]<<") "<<endl;
	}
    numeroDeResultados=h-1;
	cout<<endl;
}

void EscribirArchivo(){
    ofstream ofs ("resultado.mesh");

    for(int i=0;i<numeroDeResultados;i++){
        ofs<<resultado[0][i]<<" "<<resultado[1][i]<<" "<<resultado[2][i]<<" \n";
    }
    ofs.close();


    ofstream ofs1 ("resultado.node");

    for(int i=0;i<numeroDeResultados;i++){
        for(int j=0;j<3;j++){
            if(j==0){
                ofs1<<resultado[0][i]<<" "<<resultado[3][i]<<" "<<resultado[4][i]<<" \n";
            }
            if(j==1){
                ofs1<<resultado[1][i]<<" "<<resultado[5][i]<<" "<<resultado[6][i]<<" \n";
            }
            if(j==2){
                ofs1<<resultado[2][i]<<" "<<resultado[7][i]<<" "<<resultado[8][i]<<" \n";
            }

        }

    }
    ofs1.close();

}
int main(){
	float solicitudDeAngulo=0;
	cout<<" Proyecto ayudantia Computacion paralela "<<endl;
	cout<<endl;
	cout<<" Ingrese el angulo de tope para la ejecucion de la biseccion(0,180): ";
	cin>>solicitudDeAngulo;
	cout<<endl;
	LeerArchivos();
	CalcularLargos();
	CalcularAngulos();
	BusquedaDeLargo(solicitudDeAngulo);
	GeneracionDeMatrizTemporal();

    EscribirArchivo();

return 0 ;

}


