#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

/// 
/// Input Format:- 
/// Line 1 -- Number of Points(Lets say N)
/// Following N lines -- x,y of a point
/// Last Line  -- Penalty value
///
/// Complexity:- Time = O(N^2) , Space = = O(N^2)
///

bool comp(pair<double,double> p1,pair<double,double> p2)
{   
    /// 
    /// This Function is used for Sorting the Points based on their x , y values with priority being  given to x and then y 
    /// sort in ascending order of x, y
    ///
    if(p1.first == p2.first)
        return (p1.second < p2.second);
    return (p1.first < p2.first);
}

void process_mem_usage(double& vm_usage, double& resident_set)
{
    /// 
    /// This Function is used to find the VM Usage and Resident Usage of the code for a particular Test case
    ///
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

int main()
{ 
    clock_t start, end;
    start = clock();

    freopen("points.txt", "r", stdin);
    freopen("lines.txt", "w", stdout);

    int n;
    cin>>n;
    vector<pair<double,double> > points(n); // stores all the points in the test case
    for(int i=0;i<n;i++)
    {
        cin>>points[i].first>>points[i].second;    ;
    }
    sort(points.begin(),points.end(),comp); // Sorting all the points

    int c;
    cin>>c; 
    vector<double> opt(n+1,0),prevX(n+1,0); /// opt stores the optimal Value which we want to Minimize , prevX[j] stores the starting point for opt[j] , i.e. it stores i such that we consider (i , i+1 , ... , j) points

    vector<double> x_sum(n+1,0),y_sum(n+1,0),xx_sum(n+1,0),xy_sum(n+1,0),yy_sum(n+1,0); // these stores the prefix sum of x , y , x^2 ,xy , y^2 respectively

    for(int i=1;i<=n;i++)
    {   
        ///
        /// assigning the values of x_sum , y_sum , xx_sum , yy_sum , xy_sum
        /// 

        //complexity = O(N)
        x_sum[i]=x_sum[i-1]+points[i-1].first;
        y_sum[i]=y_sum[i-1]+points[i-1].second;
        xx_sum[i]=xx_sum[i-1]+(points[i-1].first*points[i-1].first);
        yy_sum[i]=yy_sum[i-1]+(points[i-1].second*points[i-1].second);
        xy_sum[i]=xy_sum[i-1]+(points[i-1].first*points[i-1].second);
    }

    vector<vector<double> > a(n+1,vector<double>(n+1,0)),b(n+1,vector<double>(n+1,0)),e(n+1,vector<double>(n+1,0));
    ///
    /// 2d array to store values of a, b , and e (represents error) 
    /// a[i][j] represent the slope value for points (i , i+1, ... , j ) 
    /// b[i][j] represents y - intercept for points (i , i+1, ... , j ) 
    /// e[i][j] represents total error for points (i , i+1, ... , j ) 
    ///

    // Complexity = O(N^2) for below loop
    for(int i=1;i<=n;i++)
    {   
        for(int j=i+1;j<=n;j++)
        {   
            // below variables stores the values requred for points (i , i+1 , ... , j )
            double x_sumTemp=x_sum[j]-x_sum[i-1];
            double y_sumTemp=y_sum[j]-y_sum[i-1];
            double xx_sumTemp=xx_sum[j]-xx_sum[i-1];
            double xy_sumTemp=xy_sum[j]-xy_sum[i-1];
            double yy_sumTemp=yy_sum[j]-yy_sum[i-1];

            double num=j-i+1;
            // calculating the values of a[i][j] , b[i][j] , e[i][j]
            a[i][j]=(num*xy_sumTemp - x_sumTemp*y_sumTemp)/(num*xx_sumTemp - x_sumTemp*x_sumTemp);
            b[i][j]=(y_sumTemp - a[i][j]*x_sumTemp)/num;
            e[i][j]=a[i][j]*a[i][j]*xx_sumTemp + 2*a[i][j]*b[i][j]*x_sumTemp - 2*a[i][j]*xy_sumTemp + num*b[i][j]*b[i][j] - 2*b[i][j]*y_sumTemp + yy_sumTemp;
        }
    }

    // all the prerequisites for Segmented Least Square Algo has been calculated by now , 
    // Their calculation are shown above


    ///
    /// Segmented Least Square Algo Part
    ///
    for(int j=1;j<=n;j++)
    {
        opt[j]=c+e[1][j]; 
        prevX[j]=1;
        for(int i=2;i<j;i++)
        {
            if(opt[j]>e[i][j]+c+opt[i-1]) // if we get smaller value of opt[j] then we will update it to have min value
            {
                opt[j]=e[i][j]+c+opt[i-1];
                prevX[j]=i;
            }
        }
    }

    cout<<"OPT: "<<opt[n]<<endl;
    
    ///
    /// Algorithm to get the optimal Number of Segments
    /// Part 1 - calculating the number of lines
    /// Part 2 - calculating the line end points in that partition
    ///

    // Part 1 - calculating the number of lines
    int current_xIndex=n; // it stores the right side end point index represented by line of the partition that we are creating
    int number_of_lines=0; 
    while(current_xIndex>1)
    {
        int next_xIndex=prevX[current_xIndex];  // it stores the left side end point index represented by line of the partition that we are creating
        current_xIndex=next_xIndex-1;
        number_of_lines++;
    }
    cout<<"\nNumber of lines : "<<number_of_lines<<endl;
    cout<<"The lines are : \n";
    
    // Part 2 - calculating the line end points in that partition
    current_xIndex=n; // it stores the right side end point index represented by line of the partition that we are creating
    while(current_xIndex>1)
    {
        int next_xIndex=prevX[current_xIndex]; // it stores the left side end point index represented by line of the partition that we are creating
        double x1,x2,y1,y2; // will store the end point of line in that partition 
        if(next_xIndex==current_xIndex)  // if index i and j are same
        {
            x1=points[current_xIndex-2].first;
            y1=points[current_xIndex-2].second;
            x2=points[current_xIndex-1].first;
            y2=points[current_xIndex-1].second;
        }
        else // if the points are from index i till j 
        {
            x1=points[next_xIndex-1].first;
            y1=x1*a[next_xIndex][current_xIndex]+b[next_xIndex][current_xIndex];
            x2=points[current_xIndex-1].first;
            y2=x2 * a[next_xIndex][current_xIndex]+b[next_xIndex][current_xIndex];
        }
        cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;
        current_xIndex=next_xIndex-1;
    }

    end = clock(); 
    double cl=double(end-start) / double(CLOCKS_PER_SEC); // calculating the running time of the code
    cout<<"\nTime taken: "<<cl<<" seconds"<<endl;

    double vm_usage,resident_set;
    process_mem_usage(vm_usage, resident_set); // calculating the memory requirements of the code
    cout<<"VM_usage: "<<vm_usage<<" kb \nResident_set: "<<resident_set<<" kb\n";
	return 0;
}