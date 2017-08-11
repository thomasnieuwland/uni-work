/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/
#include "branchbound.hpp"
using namespace std;

 BranchBound::BranchBound(){
 }

 BranchBound::~BranchBound(){

 }

 void BranchBound::initialise(char matrixfile[], char cityfile[]){
     matrix.intialise(matrixfile);

     ifstream inf;
     inf.open(cityfile);
     if (!inf.good()){
         cout << "Cities file could not be opened" << endl;
     }

     else{
         string tempstring;
         while(getline(inf,tempstring)){
             citynames.push_back(tempstring);
         }
     }
 }


 void BranchBound::analyse(float upperbound){
     stack<vector <Option> > candidates;
     vector<Option> temp;
     candidates.push(temp);

     while(!candidates.empty()){
         vector<Option> current = candidates.top();
         candidates.pop();

         if (current.size() == 0){ //Finds all possible starting edges
            for(int i = 1; i < matrix.vertices; i++){
                if (matrix.matrix[0][i] <= upperbound){\
                    Option temp;
                    temp.start = 0;
                    temp.finish = i;
                    temp.cost = matrix.matrix[0][i];
                    current.push_back(temp);
                    candidates.push(current);
                    current.clear();
                }
            }
         }

         else if (current.size() == matrix.vertices - 1){
             //Completes the path
             Option otemp;
             otemp.start = current[current.size()-1].finish;
             otemp.finish = 0;
             otemp.cost = matrix.matrix[current[current.size()-1].finish][0];
             current.push_back(otemp);

             vector<Option>::iterator itr;
             float totalcost = 0;
             for(itr = current.begin(); itr != current.end(); itr++){
                 totalcost+= itr->cost;
             }

             if (totalcost <= upperbound){ //Stores current best path
                 upperbound = totalcost;
                 temp.assign(current.begin(),current.end());
             }

         }

         else{
             vector<Option>::iterator itr;
             float totalcost = 0;
             for(itr = current.begin(); itr != current.end(); itr++){
                 totalcost+= itr->cost;
             }

            for(int i = 1; i < matrix.vertices; i++){
                bool flag = true;
                for (int j = 0; j < current.size(); j++){
                    if (i == current[j].start || i == current[j].finish){
                        //If i is already in the path, discard current path
                        flag = false;
                        break;
                    }
                }

                if (totalcost + matrix.matrix[current[current.size()-1].finish][i] <= upperbound && flag){
                    Option temp;
                    temp.start = current[current.size()-1].finish;
                    temp.finish = i;
                    temp.cost = matrix.matrix[current[current.size()-1].finish][i];
                    current.push_back(temp);
                    candidates.push(current);
                    current.pop_back();
                }
            }
         }
     }

     finalroute = temp;
 }

 void BranchBound::results(){
     float totalcost = 0;
     cout << endl << "2. Branch & Bound algorithm: " << endl;
     cout << "Number of Cities: " << matrix.vertices << endl;
     cout << "Tour: " << endl << "\t";

     vector<Option>::iterator itr = finalroute.begin();
     cout << itr->start;

     for(itr = finalroute.begin(); itr != finalroute.end(); itr++){
         cout << " " << itr->finish;
         totalcost+= itr->cost;
     }

     itr = finalroute.begin();
     cout << endl << '\t' << citynames[itr->start] << endl;
     cout.clear();

     for(itr = finalroute.begin(); itr != finalroute.end(); itr++){
         cout << '\t' << citynames[itr->finish] << endl;
     }

     cout << "Total Cost: " << totalcost << endl;
 }
