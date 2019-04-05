#include <iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
using namespace std;
double calculate_cost(double** D,int** state,int p,int k,int t,int c);
void choose_random_State(int** state,int p,int k);
double choose_best_neighbour(double** D,int** state, int N,int p,int k, int c);

int main(int argc, char* argv[])
{
    time_t start_t, end_t;
    char* infilename = argv[1], *outfilename = argv[2];

    ifstream in;
    ofstream out;
    in.open(infilename);
    out.open(outfilename);
    start_t = time(NULL);
    bool continue_loop = true;
    int k,p,t,C,N,i,j,no_ofrows;
    double l_time;
    in>>l_time>>k>>p>>t>>C;
    no_ofrows = p*t;
    double time_limit = l_time*60-1;
    N= k*p*t;
    double** D = new double*[N];
    for(i=0;i<N;i++)
        D[i]= new double[N];
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
            in>>D[i][j];
    }
    int** scheduler = new int*[no_ofrows];
    for(i=0;i<no_ofrows;i++)
        scheduler[i]= new int[k];
    choose_random_State(scheduler,no_ofrows,k);
    double cost=0.0,temp_cost=0.0;
    cost = calculate_cost(D,scheduler,p,k,t,C);
    int outputs[no_ofrows][k];
    ////////////////////////////
    for(i=0;i<no_ofrows;i++)
        for(j=0;j<k;j++)
            outputs[i][j] = scheduler[i][j];
    //////////////////////////////

    while(continue_loop)
    {
        temp_cost = choose_best_neighbour(D,scheduler,p,k,t,C);
        if(temp_cost>cost)
        {
            cost = temp_cost;
            for(i=0;i<no_ofrows;i++)
                for(j=0;j<k;j++)
                    outputs[i][j] = scheduler[i][j];
        }
        end_t = time(NULL);
        if(difftime(end_t, start_t) > time_limit)
            continue_loop = false;
    }
int row_no=0;
i=0;
for(int x=0;x<t;x++)
{
    row_no+=p;
    while(i<row_no-1)
    {
        for(j=0;j<k;j++)
            out<<outputs[i][j]<<" ";
        out<<"| ";
        i++;
    }
    for(j=0;j<k;j++)
            out<<outputs[row_no-1][j]<<" ";
    out<<endl;
    i++;
}

  for(i=0;i<no_ofrows;i++)
  {
      delete scheduler[i];
  }
    for(i=0;i<N;i++)
        delete D[i];
  delete scheduler;
  delete D;
 return 0;
}

double choose_best_neighbour(double** D,int** state,int p,int k, int t,int c)
{
    int n=p*k;
    int no_of_rows = p*t;
    int row1=rand()%no_of_rows,col1=rand()%k,row2=rand()%no_of_rows,col2=rand()%k,trow1,trow2,tcol1,tcol2;

    double cost,tempcost;
    int temp=state[row1][col1];
    state[row1][col1] = state[row2][col2];
    state[row2][col2]=temp;

    cost = calculate_cost(D,state,p,k,t,c);

    temp=state[row1][col1];
    state[row1][col1] = state[row2][col2];
    state[row2][col2]=temp;

    for(int i=0;i<100;i++)
    {
        trow1=rand()%no_of_rows,tcol1=rand()%k,trow2=rand()%no_of_rows,tcol2=rand()%k;
        temp=state[trow1][tcol1];
        state[trow1][tcol1] = state[trow2][tcol2];
        state[trow2][tcol2]=temp;

        tempcost = calculate_cost(D,state,p,k,t,c);
        if(tempcost>cost)
        {
            cost = tempcost;
            row1= trow1;
            row2 = trow2;
            col1 = tcol1;
            col2 = tcol2;
        }

        temp=state[trow1][tcol1];
        state[trow1][tcol1] = state[trow2][tcol2];
        state[trow2][tcol2]=temp;
    }
    temp=state[row1][col1];
    //cout<<state[no_of_rows-1][k-1]<<"\t"<<state[0][0]<<endl;
    state[row1][col1] = state[row2][col2];
    state[row2][col2]=temp;
    //cout<<state[row1][col1]<<"\t"<<state[row2][col2]<<endl;
    return cost;
}


void choose_random_State(int** state, int p, int k)
{
    int n= p*k,i,j,row1,col1,row2,col2,r,temp,sum=-1;
    for(i=0;i<p;i++)
        for(j=0;j<k;j++)
            state[i][j] = ++sum;
    n = n<100?n:100;
    for(i=0;i<n;i++)
    {
        row1 = rand()%p;
        col1 = rand()%k;
        row2 = rand()%p;
        col2 = rand()%k;
        temp = state[row1][col1];
        state[row1][col1] = state[row2][col2];
        state[row2][col2] = temp;
    }
}


double calculate_cost(double** D,int** const state,int p,int k,int t,int c)
{
    int i=0,j,x,y,element;
    double sum = 0.0,diff=0.0,row_limit=0;
    for(int time_slot=0;time_slot<t;time_slot++)
    {
        row_limit+=p;
        while(i<row_limit)
        {
            for(j=0;j<k;j++)
            {
                element = state[i][j];
                for(x=j+1;x<k;x++)
                {
                    sum+=(1-D[element][state[i][x]]);
                }
                for(x=i+1;x<row_limit;x++)
                {
                    for(y=0;y<k;y++)
                    {
                        diff+=D[element][state[x][y]];
                    }
                }
            }
            i++;
        }
    }
    sum+=(c*diff);
    return sum;
}
