#include<iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

class GraphMatrix{
	public:
		int numVertex=0, numEdge=0; //vert->vertice	edge->arista
		int **matrix;			//matriz de adyacencia
		int *mark;				//matriz de marcas de visita
		float *dur;			//guarda duración de cada proceso
		int count;				//contador general
		int count0=0; 			//contador para array de duraciones
		int counter=0;			//contador de ID's
		string *ID;				//matriz de identificaciones
		string line;			//guarda cada linea leída
		vector<string> vect;	//guarda cada linea parseada
		ifstream file;			//variable de archivo
		
		/*-----------------------------CONSTRUCTOR----------------------------------*/
		GraphMatrix(int maxSize, string arch){

			numVertex = maxSize;			//Cantidad de datos
			numEdge=0;						//Contador de aristas
			mark = new int[maxSize];		//Inicializa el array de marcas
			dur = new float[maxSize];		//Inicializa array de duraciones
			ID = new string[maxSize];		//Inicializa array de ID's
			file.open(arch);				//Abre el archivo
			
			for (int i=0; i<maxSize; i++){
				mark[i]=0;					//Setea a Mark como un array de ceros (UN 1 INDICA LA VISITA)
			}			
			matrix = new int*[maxSize];		//Creación de la matriz de adyacencia
			for (int i=0; i<maxSize; i++){
				matrix[i]=new int[maxSize];
			}			
			for(int i=0; i<maxSize; i++){	//Seteo de la matriz en ceros (-1 es cuando i==j)
				for(int j=0; j<maxSize; j++){
					
					if(i==j)	{matrix[i][j]=-1;}
					
					else		{matrix[i][j]=0;}
					
				}
			}			
			vector<string>* aux;
			vector<string>* aux2;
			
			while (file.good()){			//Lectura del archivo. Se sacan los predecesores y se setea la matriz
				count=0;
				string tkn;
				float auxItr;
				getline(file, line, '\n');
				aux = parse(line, ';');
				
				auto itr=aux->end();	//consiguiendo duraciones
				itr--;
				
				if(*itr!="Duration"){			
					auxItr=strtof((*itr).c_str(),0);
					dur[count0]=auxItr;      //convierte el string a flotante y lo mete al array de duracion
					count0++;
				}
				
				for (auto it=aux->begin(); it!=aux->end(); it++){	//consiguiendo valores de adyacencia que estan dados por la columna predecessors en el archivo
					if (count == 0){
						if (*it!="ID"){       						// consigue los ID de cada elemento
							ID[counter]=*it;
							counter++;
						}
					}
					if (count == 2){
						tkn=*it;
						break;
					}
					count++;
				}
				
				if(tkn=="~" || tkn=="Predecessors") {;}		//Si es alguno de esos, se continúa con la lectura
				else{
					
				cout<<"tkn value : "<<tkn<<" - tkn length: "<<tkn.length()<<endl;
					
					if(tkn.length()==1){		//Cuando sólo tiene un pred
						auto itaux=aux->begin();
						matrix[stoi(tkn)-1][stoi(*itaux)-1]=1;
					}
					else{						//Cuando tiene varios pred
						auto itaux=aux->begin();
						aux2 = parse(tkn,',');				// algunos datos en la columna estan divididos por coma esto significa que tiene mas de un pred
						for (auto a=aux2->begin(); a!=aux2->end(); a++){
							matrix[stoi(*a)-1][stoi(*itaux)-1]=1;
						}
					}										
				}				
			}
			file.close();			
		}
		/*----------------------------------FIN DE CONSTRUCTOR----------------------------------*/
		
		
		//GETTERS
		int Vsize()	{return numVertex;}		//numero de vertices
		int Esize()	{return numEdge;}		//numero de aristas
		
		int first(string v){
		int temp;	//Retorna primer vecino
		for (int i=0; i<numVertex; i++){
			if (ID[i]==v){
			temp=i;
			break;
			}
						
			for(int i=0; i<numVertex; i++){
				if (matrix[i][temp]==1)	{return i;} // agregue  "&& matrix[v][i]!=-1" porque si no retornaria la posicion donde esta conectado con el mismo (-1)
			}																  // se le aguego -1 a la posicion v porque si no llama al vertice anterior	y se aplico lo mismo para getNext
			return numVertex;		//Lo retorna si no hay					
		}
		}		
		
		
		int getNext(int v, int w){					//Retorna primer vecino después de W
			for(int i=w+1; i<numVertex; i++){				
				if(matrix[v-1][i]!=0 && matrix[v-1][i]!=-1)		{return i;}	   
			}
			return numVertex;		//Lo retorna si no hay
		}
		
		float * getDur(){  //retorna la matriz de duraciones
			return dur;
		}
		
		float Getdur(string label){  //retorna la duracion de una tarea relacionada a la ID de la tarea
			int indice;
			for (int i=0; i<numVertex; i++){
				if (ID[i]==label){
					indice=i;
					break;
				}
				else{;}
			}
			return dur[indice];
		}
		
		string getID(int indice){  //retorna la ID de una tarea con respecto a la posicion 
				return ID[indice];
			
		}	
		
		int getIndice(string label){  //retorna la posicion referente a la ID de una tarea
			int indice;
			for (int i=0; i<numVertex; i++){
				if (ID[i]==label){
					indice=i;
					break;
				}
				else{;}
			}
				return indice;
			
		}	
		//SETEO DE ARISTA
		void setEdge(int v1, int v2){
			if (matrix[v1][v2]==0){
				numEdge++;		//Aumenta el contador de aristas
			}
			else	{cout << "Arista ya existe." << endl;}
			matrix[v1][v2]=1;	//Setea en 1 la intersección de v1 y v2
		}		
		//BORRADO DE ARISTA
		void delEdge(int v1,int v2){
			if (matrix[v1][v2]!=0){
				numEdge--;				//Decrementa el contador de aristas
				matrix[v1][v2]=0;
			}	
			else{
				cout << "Arista inválida." << endl;
			}
		}
		//impresion de mark
		void imprMark(){
			cout << "Mark:" << endl;
			for (int i=0; i<numVertex; i++){
				cout << mark[i] << " ";
			}
			cout << endl;
		}
		//impresion de matriz
		void imprMatrix(){
			cout << "Matrix: " << endl;
			for (int i=0; i<numVertex; i++){
				for (int j=0; j<numVertex; j++){
					cout << matrix[i][j] << ", ";
				}
				cout << endl;
			}
		}
		//impresion de array de duraciones
		void imprDur(){
			cout << "Durations:" << endl;
			for (int i=0; i<numVertex; i++){
				cout << dur[i] << " - ";
			}
			cout << endl;
		}
		//impresion de ID's
		void printID(){
			cout << "ID's: '" << endl;
			for (int i=0; i<numVertex; i++){
				cout << ID[i] << " - ";
			}
		}
		/*---------------------------FUNCIONES EXTRA-------------------------------*/
		vector<string>* parse(string line, char d){					//Tokeniza el string delimitado por 'd'
			vector<string>* parsed = new vector<string>;
			string cell;
			for(int i=0; i<line.length(); i++){
				if(line[i]==d){
					parsed->push_back(cell);
					cell.clear();
				}
				else	{cell+=line[i];}
			}
			parsed->push_back(cell);
			return parsed;						//Devuelve un puntero a un vector de strings
		}
		
		float * dim(){  					// retorna las dimensiones del grafico en funcion de como sean los datos
			float * dim=new float[2];
			string auxID=ID[1];
			int auxCounter=2; 
			float suma=0;
			string * predecesorTemp=new string[numVertex];
							
			suma+=dur[1];
			
			while(true){
				
				int numPred=0;
				
				
				for (int j=0; j<numVertex; j++){
					if (matrix[j][auxCounter]==1){
						predecesorTemp[numPred]=ID[j];
						numPred++;
					}
					else{;}
				}
				
				for (int i=0; i<numPred; i++){
					if (predecesorTemp[i]==auxID){
						suma+=dur[auxCounter];
						auxID=ID[auxCounter];					
						break;
						}
				}
				auxCounter++;	
				if (auxCounter==numVertex){break;}
			}
			dim[0]=suma;   //ancho del grafico
			dim[1]=numVertex;  // altura del grafico
			
			return dim;
			
		}
		
	string * GetPred(string label, int &numPred){ 			//retorna un arreglo con los ID de los predecesores
		int indice;
		string * predecesores=new string[numVertex];
		
		for (int i=0; i<numVertex; i++){
			if (ID[i]==label){
				indice=i;
				break;
			}
			else{;}
		}
		
				
		for (int j=0; j<numVertex; j++){
			if (matrix[j][indice]==1){
				predecesores[numPred]=ID[j];
				numPred++;
			}
			else{;}
		}
		return predecesores;
	}
		
};

int main(){
	int num=0;
	string name,counter, ID;
	
	float *dur;
	
	
	cout << "Nombre del archivo (sin extension): "; cin >> name;
	
	ifstream file(name+".csv");
	while(getline(file,counter,'\n')){
		num++;
	}
	file.close();
	num--;
	
	GraphMatrix graph(num, name+".csv");
	
	graph.imprMark();
	graph.imprMatrix();
	graph.imprDur();
	graph.printID();
	cout<<"\n";
	
	float * dimention=graph.dim();

	
	

	
	ofstream myfile ("GGrantt.html");  //Construccion del SVG
	if (myfile.is_open())  {    
		
		myfile << "<!DOCTYPE html>"<<endl;   
		
		myfile << "	<html>"<<endl; 
		myfile <<"		<head>";
		myfile <<"		<title>grafico Grantt</title>"<<endl;
		myfile <<"		<style>";
		myfile <<"				body {background-color: white;}"<<endl;
		myfile <<"				h2{color: black;position: relative;left: 310px}"<<endl;
		myfile <<"				svg  {float: left;position: relative;left: 100px;}"<<endl;
		myfile <<"		</style>"<<endl;
		myfile <<"		</head>"<<endl;
	
		myfile <<"		<body>"<<endl; 
		myfile <<"				<h2>"; 
		myfile <<"					<font face=''Arial'>"<<endl; 
		myfile <<"						Grafico de Grantt"<<endl; 
		myfile <<"					</font>		</h2>"<<endl; 
		
		myfile <<"					<svg width='1100' height='901'>"<<endl;

		myfile <<"					<line x1='2' x2='"<<dimention[0]*30+(graph.Vsize()*5)<<"' y1='20' y2='20' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		
		myfile <<"					<line x1='2' x2='2' y1='2' y2='"<<dimention[1]*30<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;	
		myfile <<"					<line x1='20' x2='20' y1='20' y2='"<<dimention[1]*30<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		myfile <<"					<line x1='"<<dimention[0]*30+(graph.Vsize()*5)<<"' x2='"<<dimention[0]*30+(graph.Vsize()*5)<<"' y1='20' y2='"<<dimention[1]*30<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		
		myfile <<"					<line x1='2' x2='"<<dimention[0]*30+(graph.Vsize()*5)<<"' y1='2' y2='2' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		myfile <<"					<line x1='20' x2='20' y1='2' y2='20' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		myfile <<"					<line x1='"<<dimention[0]*30+(graph.Vsize()*5)<<"' x2='"<<dimention[0]*30+(graph.Vsize()*5)<<"' y1='2' y2='20' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		
		myfile <<"					<line x1='2' x2='"<<dimention[0]*30+(graph.Vsize()*5)<<"' y1='"<<dimention[1]*30<<"' y2='"<<dimention[1]*30<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		myfile <<"					<text font-size='10' font-family='Arial' x='8' y='15' >T</text>/"<<endl;
	
	
		int temp1=52;
		float temp2=27;
		int i=1;
		while (true){
			
		myfile <<"					<line x1='"<<temp1<<"' x2='"<<temp1<<"' y1='2' y2='20' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		myfile <<"					<text font-size='8' font-family='Arial' x='"<<temp2<<"' y='15' >Dia "<<i<<"</text>/"<<endl;
			temp1+=30;
			temp2+=29.9;
			i++;
			if (temp1>dimention[0]*30+(graph.Vsize()*5)){
				break;
			}		
	}
	
		temp1=40;
		temp2=60;
		
		while (true){
			
			myfile <<"					<line x1='2' x2='20' y1='"<<temp1<<"' y2='"<<temp1<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
			myfile <<"					<line x1='2' x2='20' y1='"<<temp2<<"' y2='"<<temp2<<"' stroke='black' style='stroke:rgb(0,0,0);stroke-width:2' />"<<endl;
		
			temp1+=30;
			temp2+=30;
			
			i++;
			if ((temp1)>(dimention[1]*30)){
				break;
			}		
		}
		
		myfile <<"					<text font-size='8' font-family='Arial' x='8' y='"<<31<<"' >"<<graph.getID(0)<<"</text>/"<<endl;
	
		myfile <<"					<text font-size='8' font-family='Arial' x='8' y='"<<31<<"' >"<<graph.getID(0)<<"</text>/"<<endl;
		int temp3= 52;
		for (int j=1; j<graph.Vsize();j++){
		
		
		myfile <<"					<text font-size='8' font-family='Arial' x='8' y='"<<temp3<<"' >"<<graph.getID(j)<<"</text>/"<<endl;
		temp3+=30;
		}
	
		myfile <<"/>"<<endl;
		
		string IDfirst=graph.getID(0);
		float h=40;
		string * predecesores;
		string tempID;
		dur=graph.getDur();
		int * sumatorias=new int[graph.Vsize()];
		
		for (int i=1; i<graph.Vsize(); i++){
		
			int numPred=0;
			ID=graph.getID(i);
			predecesores=graph.GetPred(ID, numPred);
			
				
				if (predecesores[0]==IDfirst){
					
					sumatorias[i-1]=(dur[i]*30)+24;			
					myfile <<"	<rect x='22' y='"<<h<<"' width='"<<dur[i]*30<<"' height='20' style='stroke-width:2; stroke:crimson; fill:crimson;'/>"<<endl;
					
					
				}
				else if(numPred==1){
					
					myfile <<"	<rect x='"<<sumatorias[graph.getIndice(predecesores[0])-1]<<"' y='"<<h<<"' width='"<<dur[i]*30<<"' height='20' style='stroke-width:2; stroke:crimson; fill:crimson;'/>"<<endl;
					sumatorias[i-1]=sumatorias[graph.getIndice(predecesores[0])-1]+dur[i]*30+2;
				}
				else if(numPred>1){
					
					myfile <<"	<rect x='"<<sumatorias[graph.getIndice(predecesores[1])-1]<<"' y='"<<h<<"' width='"<<dur[i]*30<<"' height='20' style='stroke-width:2; stroke:crimson; fill:crimson;'/>"<<endl;
					sumatorias[i-1]=sumatorias[graph.getIndice(predecesores[1])-1]+(dur[i]*30)+2;
					
				}
			
		
			h+=30;
		
		}
		myfile <<"				</svg>"<<endl;
		myfile <<"</body>"<<endl;
		myfile <<"</html>"<<endl;
	}	
}

	

