///@file
///@brief This file contains a complete working implementation of the segmented least squares algorithm
#include<bits/stdc++.h>
using namespace std;
#define UNDEF DBL_MAX
///Brief description of point class
///
///The point structure contains two values of datatype double, reresenting the x and y coordinates of the point in 2D space
typedef struct point
{
	double x;
	double y;

	// bool operator<(const point& e) const
	// {
	// 	if(this->x<e.x)
	// 	{
	// 		return true;
	// 	}
	// 	else if(this->x==e.x)
	// 	{
	// 		if(this->y<e.y)
	// 		{
	// 			return true;
	// 		}
	// 		else
	// 		{
	// 			return false;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		return false;
	// 	}
	// }

}point;

///Brief description of points class
///
///The point structure contains two values of datatype double, representing the slope and intercept of the best fit line
typedef struct line
{
	double slope;
	double intercept;

}line;

///Global variable to store the range of points that together comprise a segment
vector<pair<int,int>> least_square_segments;

/*! Custom comparator for the points class to facilitate easy sorting operations using STL Sort
    \param p1 a point structure consisting of two double datatype values representing the x and y values of the point in 2D space
    \param p2 a point structure consisting of two double datatype values representing the x and y values of the point in 2D space
    \return a boolean value - true if p1 is supposed to be before p2 in the ordering
*/  
bool compare_points(point p1,point p2)
{
	if(p1.x<p2.x)
	{
		return true;
	}
	else if(p1.x==p2.x)
	{
		if(p1.y<p2.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*! Computes the summation of x coordinates and y coordinates for a set of points
    \param points vector<point> a set of points represented by their x and y coordinates in 2D space
    \return a pair of double datatype values representing the sum of x coordinates and the sum of y coordinates
*/  
pair<double,double> find_cumulative_sum(vector<point> points)
{
    pair<double,double> cumulative_sum;
    double x_sum=0;
    double y_sum=0;
    for(auto i : points)
    {
        x_sum += i.x;
        y_sum += i.y;
    }
    cumulative_sum.first = x_sum;
    cumulative_sum.second = y_sum;
    return cumulative_sum;
}

/*! Computes the summation of squares of x-coordinates and summation of product of x and y coordinates for a set of points
    \param points vector<point> a set of points represented by their x and y coordinates in 2D space
    \return a pair of double datatype values - the first value representing the sum of x coordinates squared, and the second representing the sum of individual products of x and y coordinates
*/  
pair<double,double> find_cumulative_product(vector<point> points)
{

    double xi2=0;
    double xiyi=0;
    for(auto i : points)
    {
        xi2 += i.x * i.x;
        xiyi += i.x*i.y;
    }
    pair<double,double> cumulative_product;
    cumulative_product.first = xi2;
    cumulative_product.second = xiyi;
    return cumulative_product;
} 

/*! Computes the best fit line for a set of points
    \param pointSet vector<point> a set of points represented by their x and y coordinates in 2D space
    \return a line represented by its slope and intercept
*/
line FindBestFit(vector<point> pointSet)
{
    pair<double,double> cumulative_sum = find_cumulative_sum(pointSet);
    pair<double,double> cumulative_product = find_cumulative_product(pointSet);
    double xi_sum = cumulative_sum.first;
    double yi_sum = cumulative_sum.second;
    double xi2 = cumulative_product.first;
    double xiyi = cumulative_product.second;
    
    int n = pointSet.size();
    line best_fit_line;
    double a,b;
    double a_denom;
    double a_numerator = (n*xiyi) - (xi_sum*yi_sum);
    if(n*xi2 == (xi_sum*xi_sum))
    {
        //Perpendicular line, infinite slope
        best_fit_line.slope = UNDEF;
        best_fit_line.intercept = UNDEF;
    }
    else
    {
        a_denom = n*xi2 - (xi_sum*xi_sum);
        a = a_numerator/a_denom;
        b = (yi_sum - a*xi_sum)/n;
        best_fit_line.slope = a;
        best_fit_line.intercept = b;
        
    }
    return best_fit_line;
}

/*! Computes the sum of squares error for a subset of the point set
    \param pointSet vector<point> a set of points represented by their x and y coordinates in 2D space
    \param i integer indicating the first point from where  the sum of squares error should be computed
    \param j integer indicating the last point up until which the sum of squares error should be computed
    \return a value of datatype double which is the computed sum of squares error
*/
double calculateErrorValues(vector<point> pointSet, int i, int j)
{
    double error = 0;
    vector<point> pointSubset;
    for(int k=i;k<=j;k++)
    {
        pointSubset.push_back(pointSet[k]);
    }
    line best_fit = FindBestFit(pointSubset);

    if(best_fit.intercept == UNDEF and best_fit.slope == UNDEF)
    {
        error=0;

    }
    
    for(auto k : pointSubset)
    {
        double temp = k.y - k.x*best_fit.slope - best_fit.intercept;
        double val = temp*temp;
        error += val;
    }
    return error;
}
/*! Computes the OPT array which contains the minimum error value for multiple number of points
    \param n an integer representing the number of points for the OPT array needs to be computed 
    \param e a vector<vector<double>> containing the computed precalculated sum of squares error for various ranges of points 
    \param c a value of double datatype representing the penalty value
    \return a vector of datatype double, representing the OPT array
*/
vector<double> segmentedLeastSquares(int n, vector<vector<double>> e, double c)
{
    vector<double> opt(n+1,0);
    for(int j=1;j<=n;j++)
    {
        double min_val = INT_MAX;
        for(int i=1;i<=j;i++)
        {
            min_val = min(min_val, opt[i-1] + c + e[i][j]);
        }
        opt[j] = min_val;
    }
    return opt;
}
/*! Finds the segments of continguous points that have a single best fit line for them
    \param j an integer representing the position of the point which signifies the end of the segment
    \param e a vector<vector<double>> containing the computed precalculated sum of squares error for various ranges of points 
    \param c a value of double datatype representing the penalty value
    \return a vector of datatype double, representing the OPT array
*/
void findSegments(int j, vector<vector<double>> e,double c, vector<double> opt)
{
    if(j==0)
    {
        return;
    }
    double min_val = INT_MAX;
    int opt_i = 1;
    for(int i=1;i<=j;i++)
    {
        double val = e[i][j] + c + opt[i-1];
        if(val<min_val)
        {
            opt_i = i;
            min_val = val;
        }
    }
    pair<double,double> seg;
    seg.first = opt_i;
    seg.second = j;
    least_square_segments.push_back(seg);
    findSegments(opt_i-1,e,c,opt);
}

/*! Function that writes the set of points, and computed coordinates for the lines representing the segments to a CSV file for visualization
    \param pointSet  vector<point> consisting of points in 2D space 
    \param segments a vector<vector<double>> containing the computed sum of squares error
    \return a vector of datatype double, representing the OPT array
*/
void csv_writer(vector<point> pointSet, vector<pair<int,int>> segments)
{
    std::ofstream myFile("points.csv");
    
    myFile <<"x"<<","<<"y"<<"\n";
    for(auto it : pointSet)
    {
        myFile<<it.x<<", "<<it.y<<"\n";
    }
    
    myFile.close();

    std::ofstream myFile2("segments.csv");
    myFile << "x1"<<","<<"x2"<<","<<"y1"<<","<<"y2"<<","<<"d"<<"\n";
    // while(itc != hrc.end())
    // {
    //     line_segment ls = (*itc);
    //     myFile2<< ls.inter.bottom<<", "<<ls.x<<", "<<ls.inter.top<<", "<<ls.x<<"\n";
    //     itc++;
    // }

    
    for(auto seg : segments)
    {
        bool single_point = false;
        vector<point> pointSubset;
        for(int i = seg.first-1;i<=seg.second-1;i++)
        {
            pointSubset.push_back(pointSet[i]);
        }
        line l1 = FindBestFit(pointSubset);
        if(seg.first == seg.second)
        {
            float pointx1 = pointSubset[0].x;
            float pointy1 = pointSubset[0].y;
            float pointx2 = pointSubset[pointSubset.size()-1].x;
            float pointy2 = pointy1;
            myFile2<<pointx1<<", "<<pointy1<<", "<<pointx2<<", "<<pointy2<<"\n";    
        }
        else
        {
            if(l1.slope == UNDEF and l1.intercept == UNDEF)
            {
                float pointx1 = pointSubset[0].x;
                float pointy1 = pointSubset[0].y;
                float pointx2 = pointx1;
                float pointy2 = pointSubset[pointSubset.size()-1].y;
                myFile2<<pointx1<<", "<<pointy1<<", "<<pointx2<<", "<<pointy2<<"\n";
            }
            else
            {
                float pointx1 = pointSubset[0].x;
                float pointy1 = l1.slope*pointx1 + l1.intercept;
                float pointx2 = pointSubset[pointSubset.size()-1].x;
                float pointy2 = l1.slope*pointx2 + l1.intercept;
                myFile2<<pointx1<<", "<<pointy1<<", "<<pointx2<<", "<<pointy2<<"\n";
            }
            
        }
        
    }

    myFile2.close();
}

int main()
{
    int n;
    cin>>n;
    vector<point> pointSet;
    vector<double> OPT(n+1,0);
    OPT[0] = 0;
    double x,y;
    for(int i=0;i<n;i++)
    {
        cin>>x>>y; 
        point p; 
        p.x = x;
        p.y = y;
        pointSet.push_back(p);
    }
    double c;
    cin>>c;
    
    sort(pointSet.begin(),pointSet.end(),compare_points);
    
    
    vector<vector<double>> e(n+1);
    
    for(int i=0;i<n+1;i++)
    {
        for(int j=0;j<n+1;j++)
        {
            e[i].push_back((double)0);
        }
    }
    
    //precalculate error values
    for(int j=1;j<=pointSet.size();j++)
    {
        for(int i=1;i<j;i++)
        {
            e[i][j] = calculateErrorValues(pointSet,i-1,j-1);
            // cout<<e[i][j]<<" ";
        }
    }
    OPT = segmentedLeastSquares(pointSet.size(),e,c);
    cout<<"Segmented least square error is : "<<OPT[pointSet.size()];
    findSegments(pointSet.size(),e,c,OPT);
    // cout<<OPT[pointSet.size()];
    // for(auto i : least_square_segments)
    // {
        
    // }
    csv_writer(pointSet,least_square_segments);
}