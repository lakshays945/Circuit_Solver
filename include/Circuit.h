#pragma once
#include "Matrix.h"
#include "Element.h"

class Circuit {
public:
	std::vector<std::pair<int,int>> Edges;
	std::vector<std::pair<Element_Type,long double>> Elements;
	std::vector<long double*> CAddress;
	std::vector<long double*> VAddress;
	int TotalElements,TotalNodes;
	Circuit(std::vector<std::pair<int,int>> &edges, std::vector<std::pair<Element_Type,long double>> &elements, std::vector<long double*> cad, std::vector<long double*> vad, int totalNodes){
		Edges = edges;
		Elements = elements;
		TotalElements = Elements.size();
		TotalNodes = totalNodes;
		CAddress = cad;
		VAddress = vad;
	}
	Circuit(){}
	Matrix Solve(){
		int VoltageSources = 0;
		int Wires = 0;
		int Inductors = 0;
		int count = 0;
		for(int i=0; i<Elements.size(); i++){
			if(Elements[i].first == BATTERY) VoltageSources++;
			if(Elements[i].first == WIRE) Wires++;
		}
		std::vector<std::vector<long double>> ConductanceVector(TotalNodes+Wires+VoltageSources-1,std::vector<long double>(TotalNodes+Wires+VoltageSources-1,0));
		std::vector<std::vector<long double>> CapacitenceVector(TotalNodes+Wires+VoltageSources-1,std::vector<long double>(TotalNodes+Wires+VoltageSources-1,0));
		std::vector<std::vector<long double>> SVector;
		for(int i=0; i<TotalNodes+Wires+VoltageSources-1; i++){
			std::vector<long double> temp;
			temp.push_back(0.f);
			SVector.push_back(temp);
		}
		for(int i=0; i<Elements.size(); i++){
			int n1 = Edges[i].first-1;
			int n2 = Edges[i].second-1;
			if(Elements[i].first == WIRE){
				if(n1 >= 0) ConductanceVector[n1][count+TotalNodes-1] += 1;
				if(n2 >= 0) ConductanceVector[n2][count+TotalNodes-1] += -1;
				if(n1 >= 0) ConductanceVector[count+TotalNodes-1][n1] += 1;
				if(n2 >= 0) ConductanceVector[count+TotalNodes-1][n2] += -1;
				count++;
			}
			else if(Elements[i].first == RESISTOR){
				if(n1 >= 0) ConductanceVector[n1][n1] += 1/Elements[i].second;
				if(n2 >= 0) ConductanceVector[n2][n2] += 1/Elements[i].second;
				if(n1 >= 0 && n2 >= 0) ConductanceVector[n1][n2] += -1/Elements[i].second;
				if(n1 >= 0 && n2 >= 0) ConductanceVector[n2][n1] += -1/Elements[i].second;
			}
			else if(Elements[i].first == BATTERY){
				if(n1 >= 0) ConductanceVector[n1][count+TotalNodes-1] += 1;
				if(n2 >= 0) ConductanceVector[n2][count+TotalNodes-1] += -1;
				if(n1 >= 0) ConductanceVector[count+TotalNodes-1][n1] += 1;
				if(n2 >= 0) ConductanceVector[count+TotalNodes-1][n2] += -1;
				SVector[count+TotalNodes-1][0] += Elements[i].second;
				count++;
			}
		}
		SquareMatrix ConductanceMatrix;
		Matrix SMatrix;
		ConductanceMatrix.SetArray(ConductanceVector);
		SMatrix.SetArray(SVector);
		Matrix K = ConductanceMatrix.Solve(SMatrix);
		std::vector<long double> temp(1,0);
		for(int i=0; i<K.Rows; i++){
			temp.push_back(K.Array[i][0]);
		}
		count = 0;
		for(int i=0; i<VAddress.size(); i++){
			*VAddress[i] = (temp[Edges[i].first]-temp[Edges[i].second]);
		}
		for(int i=0; i<CAddress.size(); i++){
			if(Elements[i].first == RESISTOR){
				*CAddress[i] = (*VAddress[i])/Elements[i].second;
			}
			else{
				*CAddress[i] = (temp[count+TotalNodes]);
				count++;
			}
		}
		return K;
	}
};

int BFS(std::vector<std::vector<int>> &nodes, int X, int Y, std::vector<std::vector<bool>> &visited, int &totalNodes, std::vector<std::vector<int>> &nodeIndexArray,const std::vector<std::vector<Element_Type>> &hlines, const std::vector<std::vector<Element_Type>> vlines){
	visited[Y][X] = true;
	nodeIndexArray[Y][X] = totalNodes;
	totalNodes++;
	if(X < nodes[0].size()-1 && nodes[Y][X+1] > 0 && !visited[Y][X+1] && (int)hlines[Y][X] > (int)CURSOR){
		BFS(nodes,X+1,Y,visited,totalNodes,nodeIndexArray,hlines,vlines);
	}
	if(Y < nodes.size()-1 && nodes[Y+1][X] > 0 && !visited[Y+1][X] && (int)vlines[Y][X] > (int)CURSOR){
		BFS(nodes,X,Y+1,visited,totalNodes,nodeIndexArray,hlines,vlines);
	}
	if(X > 0 && nodes[Y][X-1] > 0 && !visited[Y][X-1] && (int)hlines[Y][X-1] > (int)CURSOR){
		BFS(nodes,X-1,Y,visited,totalNodes,nodeIndexArray,hlines,vlines);
	}
	if(Y > 0 && nodes[Y-1][X] > 0 && !visited[Y-1][X] && (int)vlines[Y-1][X] > (int)CURSOR){
		BFS(nodes,X,Y-1,visited,totalNodes,nodeIndexArray,hlines,vlines);
	}
	return totalNodes;
}

std::vector<Circuit> ExtractCircuits(const std::vector<std::vector<Element_Type>> hlines,const std::vector<std::vector<long double>> hvalues,
									 const std::vector<std::vector<Element_Type>> vlines,const std::vector<std::vector<long double>> vvalues,
									 long double ** hcurrent, long double ** vcurrents, long double ** hvoltages, long double ** vvoltages)
{
	std::vector<Circuit> IndependentCircuits;
	int H = vlines.size();
	int W = hlines[0].size();
	if(H+1 != hlines.size() || W+1 != vlines[0].size() || hvalues[0].size() != W || hvalues.size() != H+1 || vvalues.size() != H || vvalues[0].size() != W+1){
		std::cout<<"Not Valid Data!!!!\n";
		return IndependentCircuits;
	}
	std::vector<bool> tempB(W+1,0);
	std::vector<int> tempI(W+1,0);
	std::vector<int> tempX(W+1,-1);
	std::vector<std::vector<bool>> Visited(std::vector<std::vector<bool>>(H+1,tempB));
	std::vector<std::vector<int>> NodeArray(std::vector<std::vector<int>>(H+1,tempI));
	for(int i=0; i<H+1; i++){
		for(int j=0; j<W+1; j++){
			NodeArray[i][j] = 0;
		}
	}
	//Nodes due to Hlines
	for(int i=0; i<H+1; i++){
		for(int j=0; j<W; j++){
			if((int)hlines[i][j] > (int)CURSOR){
				NodeArray[i][j] = 1;
				NodeArray[i][j+1] = 1;
			}
		}
	}
	//Nodes due to Vlines
	for(int j=0; j<W+1; j++){
		for(int i=0; i<H; i++){
			if((int)vlines[i][j] > (int)CURSOR){
				NodeArray[i][j] = 1;
				NodeArray[i+1][j] = 1;
			}
		}
	}
	for(int i=0; i<H+1; i++){
		for(int j=0; j<W+1; j++){
			if(NodeArray[i][j] == 1 && !Visited[i][j]){
				std::vector<std::vector<int>> NodeIndexArray(std::vector<std::vector<int>>(H+1,tempX));
				int TotalNodes = 0;
				TotalNodes = BFS(NodeArray,j,i,Visited,TotalNodes,NodeIndexArray,hlines,vlines);
				std::vector <std::pair<int,int>> Edges;
				std::vector <std::pair<Element_Type,long double>> Elements;
				std::vector <long double*> Cad;
				std::vector <long double*> Vad;
				for(int i=0; i<H+1; i++){
					for(int j=0; j<W+1; j++){
						if(NodeArray[i][j] > 0){
							std::pair<int,int> p;
							if(j<W && NodeIndexArray[i][j+1] >= 0){
								p.first = NodeIndexArray[i][j];
								p.second = NodeIndexArray[i][j+1];
								if((int)hlines[i][j] > (int)CURSOR){
									Elements.push_back(std::make_pair(hlines[i][j],hvalues[i][j]));
									Edges.push_back(p);
									Cad.push_back(&hcurrent[i][j]);
									Vad.push_back(&hvoltages[i][j]);
								}
							}
							if(i<H && NodeIndexArray[i+1][j] >= 0){
								p.first = NodeIndexArray[i][j];
								p.second = NodeIndexArray[i+1][j];
								if((int)vlines[i][j] > (int)CURSOR){
									Elements.push_back(std::make_pair(vlines[i][j],vvalues[i][j]));
									Edges.push_back(p);
									Cad.push_back(&vcurrents[i][j]);
									Vad.push_back(&vvoltages[i][j]);
								}
							}
						}
					}
				}
				IndependentCircuits.push_back(Circuit(Edges,Elements,Cad,Vad,TotalNodes));
			}
		}
	}
	return IndependentCircuits;
}

void StartSolving(Element_Type ** htypes, Element_Type ** vtypes, long double ** vvalues, long double ** hvalues, 
				  long double ** hcurrents, long double ** vcurrents, long double ** hvoltages, long double ** vvoltages, int size){
	
	for(int i=0; i<=size; i++){
            for(int j=0; j<=size; j++){
                if(j<size){
                    hcurrents[i][j] = 0;
                    hvoltages[i][j] = 0;
                }
                if(i<size){
                    vcurrents[i][j] = 0;
                    vvoltages[i][j] = 0;
                }
            }
        }

    std::vector<std::vector<Element_Type>> HType(size+1,std::vector<Element_Type>(size,EMPTY));
    std::vector<std::vector<Element_Type>> VType(size,std::vector<Element_Type>(size+1,EMPTY));
    std::vector<std::vector<long double>> HValues(size+1,std::vector<long double>(size,0.f));
    std::vector<std::vector<long double>> VValues(size,std::vector<long double>(size+1,0.f));
    for(int i=0; i<=size; i++){
        for(int j=0; j<=size; j++){
            if(i<size) VType[i][j] = vtypes[i][j];
            if(j<size) HType[i][j] = htypes[i][j];
            if(i<size) VValues[i][j] = vvalues[i][j];
            if(j<size) HValues[i][j] = hvalues[i][j];
        }
    }
    std::vector<Circuit> CircuitsArray = ExtractCircuits(HType,HValues,VType,VValues,hcurrents,vcurrents,hvoltages,vvoltages);
    for(int i=0; i<CircuitsArray.size(); i++){
		Matrix Ans = CircuitsArray[i].Solve();
	}
}