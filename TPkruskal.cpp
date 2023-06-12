#include <iostream>     // Librer�a con las funciones estandar de I/O
#include <map>
#include <utility>
#include <vector>
#include <string>
using namespace std;


typedef pair <pair<char, char>, int> arista;
// Tipo de dato pair que conforma una arista, donde pair<char,char> vertices
// representa los y el int el costo entre ellos.

typedef struct combina_encuentra{
            map <char, pair<char, char>> nombres; // map<key=NombreV, pair<nombredelConjunto, Vertice Siguiente>>
            map <char, pair<char, char>>::iterator ite;
            map <char, pair<int, char>> encabezados; // map<key=NombreV, pair<costo, primer elemento>>
        }conjunto_CE;



class grafo{

    vector <char> V;            // vector con todos los vertices ingresados.
    vector <arista> E;          // vector con las aristas creadas.
    conjunto_CE CE;             // Estructura para el analisis de adyacencias
    vector <arista> min_tree;   // Arbol de minima expansion.

    public :
    grafo(){}                   // constructor
    ~grafo(){}                  // destructor
    void insertar_vertice(const char &);                            // inserta los vertices en el vector V
    void insertar_arista(const char &, const char &, const int &);  // inserta las aristas en el vector E
    void insertar_arista();                                         // inserta las aristas en el vector E pero pide el ingreso por teclado
    void inicial ( const char &, const char &);                     // inicializa a las estructuras de conjunto COMBINA - ENCUENTRA
    int calc_min(void);                                             // busca el minimo costo
    void combina ( const char &, const char &);                     // Combina las aristas que se encuentran formando el arbol
    char encuentra ( const char &) ;                                // Encuentra los vertices dentro del conjunto COMBINA - ENCUENTRA
    void kruskal(void);                                             // Algoritmo generador del arbol recubridor minimo
    arista sacar_minimo(void);                                      // extrae la arista de menos costo y la elimina del vector E
    friend ostream & operator <<(ostream &, grafo);                 // Sobrercarga de la salida �aestndar para mostrar al grafo
};

ostream& operator <<(ostream& os, grafo G){
	os <<"El grafo es: \n";
	for(auto x: G.min_tree)
		os <<"("<< x.first.first << " - " << x.first.second <<")"<< ": costo = " << x.second << endl;

	return os;
}

void grafo::insertar_vertice(const char &v){
    V.push_back(v);
}

void grafo::insertar_arista( const char &v1 , const char &v2 , const int &c){
    arista ar1;
    ar1.first= make_pair(v1, v2);
    ar1.second= c;
    E.push_back(ar1);
}

void grafo:: insertar_arista(void){

    char c1;
    char c2;
    int cost;
    cout<<"Ingrese el vertice inicial:"<<endl;
    cin>>c1;
    cout<< "Ingrese el vertice final:"<<endl;
    cin>>c2;
    cout<<"Ingrese el costo:"<<endl;
    cin>>cost;

    arista ar1;
    ar1.first = make_pair(c1,c2);
    ar1.second= cost;
    E.push_back(ar1);
}


void grafo:: inicial(const char &nom, const char &v){
    pair<char, char> par1;
    par1.first = v+1;
    par1.second = '0';
    CE.nombres.insert(pair<char,pair<char,char>>(nom+1, par1));

    pair<int, char> par2;
    par2.first = 1;
    par2.second = v+1;
    CE.encabezados.insert(pair<char,pair<int,char>>(nom+1, par2));
}

void grafo::combina(const char &v1, const char &v2){
    char i;

    if(CE.encabezados[v1].first > CE.encabezados[v2].first){

        i = CE.encabezados[v2].second;

        while(CE.nombres[i].second != '0'){
            CE.nombres[i].first = v1;
            i = CE.nombres[i].second;
        }

        CE.nombres[i].first = v1;
        CE.nombres[i].second = CE.encabezados[v1].second;
        CE.encabezados[v1].second = CE.encabezados[v2].second;
        CE.encabezados[v1].first = CE.encabezados[v1].first + CE.encabezados[v2].first;

    }else{

        i = CE.encabezados[v1].second;
        while(CE.nombres[i].second != '0'){
            CE.nombres[i].first = v2;
            i = CE.nombres[i].second;
        }

        CE.nombres[i].first = v2;
        CE.nombres[i].second = CE.encabezados[v2].second;
        CE.encabezados[v2].second = CE.encabezados[v1].second;
        CE.encabezados[v2].first = CE.encabezados[v2].first + CE.encabezados[v1].first;
    }
}

char grafo::encuentra(const char &v){
    return CE.nombres[v].first;
}

arista grafo::sacar_minimo(void){
    int auxC = E[0].second;
    arista auxA = E[0];
    int auxi = 0;
    for(int i = 0; i<E.size(); i++){
        if(E[i].second < auxC){
            auxC = E[i].second;
            auxA = E[i];
            auxi = i;
        }
    }
    E.erase(E.begin()+auxi);
    return auxA;
}

int grafo::calc_min(void){
    int costomin = 0;
    for(int i = 0; i<min_tree.size(); i++){
        costomin += min_tree[i].second;
    }
    return costomin;
}

void grafo::kruskal(void){

    int comp_n = 5;
    char comp_u;
    char comp_v;

    arista ari; // estructura tipo arista que permite evaluar si la arista actual se agrega al �rbol o se elimina.

    // bucle empleado para inicializar los conjuntos.
    char v = '0';
    for(int i = 0; i < 5; i++){
        inicial(v, v);
        v++;
    }

    while(comp_n > 1){
        ari = sacar_minimo(); // almacenamos en ari la arista a evaluar.
        comp_u = encuentra(ari.first.first+1);
        comp_v = encuentra(ari.first.second+1);

        if(comp_u != comp_v){
            combina(comp_u, comp_v);
            comp_n = comp_n - 1;
            min_tree.push_back(ari);
        }
    }
    cout<<"El costo del arbol recubridor minimo es: "<<calc_min()<<endl;
}


int main(void){

    grafo g;

    int M_Costos[5][5];
    int num=0;

    cout<<"****************************************************"<<endl;
    cout<<"** Trabajo Practico Kruskal: Algoritmo de Kruskal **"<<endl;
    cout<<"**                   Avila,Ivan                   **"<<endl;
    cout<<"****************************************************"<<endl;
    // Se utilizan bucles anidados para completar la matriz y almacenar los datos de los v�rtices ingresados por el usuario.
    char auxi= '0';
    char auxj= '0';
    for (int i=0;i<=4;i++){
        auxj = auxi + 1;
        for (int j=i+1;j<=4;j++)
        {
            g.insertar_vertice(auxi);
            g.insertar_vertice(auxj);
            cout<<"Ingrese el costo de la arista entre los vertices "<<auxi<<" y "<<auxj<<" :"<<endl;
            cin>>M_Costos[i][j];
            g.insertar_arista(auxi, auxj, M_Costos[i][j]); // Guarda las aristas en el Conjunto E
            auxj++;
        }
        auxi++;
    }
    //g.ordena_E();
    g.kruskal();// llamada a la funci�n Kruskal para realizar el algoritmo de dicho nombre.
    cout<<g<<endl;

    return 0;
}

