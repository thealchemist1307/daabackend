#include<bits/stdc++.h>
#include <fstream>
using namespace std;


//coord is anyway one integer only

typedef struct point
{
    int x,y;
}point;

typedef struct interval
{
    int bottom,top;

    bool operator<(const interval& e) const
    { 
        if(this->bottom<e.bottom)
        {
            return true;
        }
        else if(this->bottom==e.bottom)
        {
            if(this->top<e.top)
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
}interval;

typedef struct rectangle
{
    interval x_int,y_int;
}rectangle;

typedef struct edge
{
    string edgetype;
    interval i;
    int x;
    //What to do when both are left edges/right edges
    bool operator<(const edge& e) const
    { 
        if(this->x < e.x)
        {
            return true;
        }
        else if(this->x == e.x)
        {
        
            if(this->edgetype == "LEFT" and e.edgetype == "RIGHT")
            {
                return true;
            }
            else if(this->edgetype == "RIGHT" and e.edgetype == "LEFT")
            {
                return false;
            }
            else
            {
                if(this->i.bottom < e.i.bottom)
                {
                    return true;
                }
                else if(this->i.bottom == e.i.bottom)
                {
                    if(this->i.top < e.i.top)
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
        } 
        else
        {
            return false;
        }
    }
}edge;

typedef struct stripe
{
    interval x_int;
    interval y_int;
    int x_union;

    bool operator<(const stripe& e) const
    { 
        if(this->y_int.bottom<e.y_int.bottom)
        {
            return true;
        }
        else if (this->y_int.bottom==e.y_int.bottom)
        {
            if(this->y_int.top<e.y_int.top)
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
}stripe;

string EDGE_TYPES[4] = {"TOP","BOTTOM","LEFT","RIGHT"};

vector<rectangle> iso_rectangles_input;

typedef struct StripeReturnType
{
    set<interval> L;
    set<interval> R;
    set<int> P;
    set<stripe> S;
}SRT;

rectangle findFrame()
{
    rectangle frame;
    int minY=INT_MAX;
    int minX=INT_MAX;
    int maxY = INT_MIN;
    int maxX = INT_MIN;
    for(int i=0;i<iso_rectangles_input.size();i++)
    {
        rectangle r = iso_rectangles_input[i];
        if(r.x_int.bottom <= minX)
        {
            minX = r.x_int.bottom ;
        }
        if(r.x_int.top >= maxX)
        {
            maxX = r.x_int.top;
        }
        if(r.y_int.top >= maxY)
        {
            maxY = r.y_int.top;
        }
        if(r.y_int.bottom <= minY)
        {
            minY = r.y_int.bottom;
        }
    }
    interval x_bound;
    interval y_bound;
    x_bound.bottom = minX-2; x_bound.top = maxX+2;
    y_bound.bottom = minY-2; y_bound.top = maxY+2;
    frame.x_int = x_bound; frame.y_int = y_bound;
    return frame;
}


set<interval> y_partition(set<int> y_coordinates)
{
    priority_queue <int, vector<int>, greater<int>> partition_maker;
    set<interval> plane_partitions;
    set<int>::iterator it = y_coordinates.begin();
    while(it != y_coordinates.end())
    {
        partition_maker.push(*it);
        ++it;
    }
    interval inter;
    int prev = partition_maker.top();
    partition_maker.pop();
    while(!partition_maker.empty())
    {
        inter.bottom = prev;
        inter.top = partition_maker.top();
        prev = partition_maker.top();
        plane_partitions.insert(inter);
        partition_maker.pop();
    }
    return plane_partitions;
}

set<int> y_set()
{
    set<int> y_coordinates;
    for(int i=0;i<iso_rectangles_input.size();i++)
    {
        y_coordinates.insert(iso_rectangles_input[i].y_int.bottom);
        y_coordinates.insert(iso_rectangles_input[i].y_int.top);
    }
    return y_coordinates;
}

set<int> x_proj()
{
    set<int> x_coordinates;


    return x_coordinates;
}

set<interval> set_intersection(set<interval> i1, set<interval> i2)
{
    auto ite = i1.begin();
    auto itr = i2.begin();
    set<interval> i3;
    while(ite!=i1.end() and itr!=i2.end())
    {
        interval a = *ite;
        interval b = *itr;

        if(a.bottom < b.bottom)
        {
            ite++;
        }
        else if(a.bottom == b.bottom)
        {
            if(a.top == b.top)
            {
                i3.insert(a);
                ite++;
                itr++;
            }
            else if(a.top < b.top)
            {
                ite++;
            }
            else
            {
                itr++;
            }
        }
        else
        {
            itr++;
        }
    }
    return i3;
}

set<interval> set_difference(set<interval> i1,set<interval> i2)
{
    auto ite = i1.begin();
    auto itr = i2.begin();
    set<interval> i3;
    while(ite!=i1.end() and itr!=i2.end())
    {
        interval a = *ite;
        interval b = *itr;

        if(a.bottom < b.bottom)
        {
            i3.insert(a);
            ite++;
        }
        else if(a.bottom == b.bottom)
        {
            if(a.top == b.top)
            {
                ite++;
                itr++;
            }
            else if(a.top < b.top)
            {
                i3.insert(a);
                ite++;
            }
            else
            {
                itr++;
            }
        }
        else
        {
            itr++;
        }
    }

    while(ite != i1.end())
    {
        i3.insert((*ite));
        ite++;
    }

    return i3;
}

set<interval> set_union(set<interval> i1,set<interval> i2)
{
    set<interval> i3;
    auto ite = i1.begin();
    auto itr = i2.begin();
    while(ite != i1.end())
    {
        i3.insert(*ite);
        ++ite;
    }
    while(itr != i2.end())
    {
        i3.insert(*itr);
        ++itr;
    }
    return i3;
}

set<stripe> copy(set<stripe> S, set<int> P, interval x_ext)
{
    set<interval> part = y_partition(P);
    auto ite = part.begin();
    auto itr = S.begin();
    set<stripe> s1;
    while(ite != part.end())
    {
        stripe stemp;
        stemp.x_int = x_ext;
        stemp.y_int = *ite;
        stemp.x_union = 0;
        
        while(itr!= S.end())
        {
            interval c = (*itr).y_int; //S
            if(stemp.y_int.bottom >= c.bottom)
            {
                if(stemp.y_int.top <= c.top)
                {
                    //useful S
                    stemp.x_union = (*itr).x_union;
                    //cout<<"HELLO3 "<<stemp.x_union<<endl;;
                    break;
                }
                else
                {
                    itr++;
                }
            }
            else
            {
                break;
            }
        }

        s1.insert(stemp);
        ++ite;
    }

    return s1;

}

set<stripe> blacken(set<stripe> S, set<interval> J)
{
    auto itr = S.begin();
    auto ite = J.begin();
    set<stripe> s_final;
    while(itr != S.end())
    {
        stripe stemp = *itr;
        interval i1 = stemp.y_int;
        while(ite != J.end())
        {
            //i1 should be subset of i2
            interval i2 = (*ite);
            if(i1.bottom < i2.bottom)
            {
                break;
            }
            else if(i1.bottom == i2.bottom)
            {
                if(i1.top <= i2.top)
                {
                    stemp.x_union = stemp.x_int.top - stemp.x_int.bottom;
                    // cout<<"HELLO4 "<<stemp.x_union<<endl;;
                    break;
                }
                else
                {
                    ite++;
                }
            }
            else
            {
                if(i1.top <= i2.top)
                {
                    stemp.x_union = stemp.x_int.top - stemp.x_int.bottom;
                    // cout<<"HELLO5 "<<stemp.x_union<<endl;
                    break;
                }
                else
                {
                    ite++;
                }
            }
        }
        s_final.insert(stemp);
        ++itr;
    }

    return s_final;
}

set<stripe> concat(set<stripe> s_left, set<stripe> s_right, set<int> P, interval x_ext)
{
    set<interval> part = y_partition(P);
    auto ite = part.begin();
    auto itr1 = s_left.begin();
    auto itr2 = s_right.begin();
    set<stripe> S;

    while(ite != part.end())
    {
        stripe stemp;
        stemp.x_int = x_ext;
        stemp.y_int = *ite;
        stemp.x_union = 0;
        int val1 = (*itr1).x_union;
        int val2 = (*itr2).x_union;
        stemp.x_union = val1+val2;
        // cout<<"HELLO6 "<<stemp.x_union<<endl;
        itr1++; 
        itr2++;
        S.insert(stemp);
        ++ite;
    }

    return S;
}

SRT stripes(set<edge> VRX, interval x_ext, interval ylimits)
{
    SRT temp;
    set<interval> l,r;
    set<int> p;
    set<stripe> s;
    
    if(VRX.size()==1)
    {
        auto it = VRX.begin();
        edge e = *it;
        if(e.edgetype == "LEFT")
        {
            interval l1 = e.i;
            l.insert(l1);
        }
        else
        {
            interval r1 = e.i;
            r.insert(r1);    
        }

        p.insert(ylimits.bottom);
        p.insert(e.i.bottom);
        p.insert(e.i.top);
        p.insert(ylimits.top);

        set<interval> part = y_partition(p);
        auto ite = part.begin();
        while(ite != part.end())
        {
            stripe stemp;
            stemp.x_int = x_ext;
            stemp.y_int = *ite;
            stemp.x_union = 0;
            if((stemp.y_int.bottom == e.i.bottom) and (stemp.y_int.top == e.i.top))
            {
                if(e.edgetype == "LEFT")
                {
                    stemp.x_union = x_ext.top - e.x;
                    // cout<<"HELLO1 "<<stemp.x_union<<endl;;
                }
                else
                {
                    stemp.x_union = e.x - x_ext.bottom;
                    // cout<<"HELLO2 "<<stemp.x_union<<endl;;
                }
            }

            s.insert(stemp);
            ++ite;
        }
        // cout<<"EDGE CASE"<<endl;
    }
    else
    {
        int n = VRX.size();
        int xm;
        set<edge> v1,v2;
        auto it = VRX.begin();
        for(int i=0;i<n/2;i++)
        {
            v1.insert(*it);
            ++it;
        }
        for(int i=n/2;i<n;i++)
        {
            if(i==n/2)
            {
                xm = (*it).x;
            }
            v2.insert(*it);
            ++it;
        }
        
        SRT left,right;
        interval left_call,right_call;
        left_call.bottom = x_ext.bottom;
        left_call.top = xm;
        right_call.bottom = xm;
        right_call.top = x_ext.top;

        left = stripes(v1, left_call,ylimits);
        right = stripes(v2,right_call,ylimits);
        
        //Merge step to be writen
        set<interval> L1=left.L;
        set<interval> R2=right.R;
        
        set<interval> LR;
        
        LR = set_intersection(L1,R2);
        set<interval> l1lr = set_difference(L1,LR);
        set<interval> r2lr = set_difference(R2,LR);
        l = set_union(right.L,l1lr);
        r = set_union(left.R,r2lr);
        // l = (L1-LR) union L2;
        // r = (R2-LR) union R1;
      
        

        set<int> P1= left.P;
        set<int> P2= right.P;
        auto itr=P1.begin();
        while(itr!=P1.end())
        {
            p.insert(*itr);
            itr++;
        }
        itr=P2.begin();
        while(itr!=P2.end())
        {
            p.insert(*itr);
            itr++;
        }


        set<stripe> S_left,S_right;
        interval slefttemp;
        slefttemp.bottom=x_ext.bottom;
        slefttemp.top=xm;
        interval srighttemp;
        srighttemp.bottom=xm;
        srighttemp.top=x_ext.top;
        
        // auto itP = p.begin();
        // while(itP != p.end())
        // {
        //     cout<<"P :"<<*itP<<endl;
        //     ++itP;
        // }
        S_left=copy(left.S,p,slefttemp);
        S_right=copy(right.S,p,srighttemp);
        

        S_left = blacken(S_left,r2lr);
        S_right = blacken(S_right,l1lr);
        

        s = concat(S_left, S_right, p, x_ext);
        // cout<<"XM : "<<xm<<endl;
    }
    
    temp.L = l;
    temp.R = r;
    temp.P = p;
    temp.S = s;
    
    return temp;
}

set<stripe> rectangle_DAC(vector<rectangle> rect)
{
    vector<rectangle>::iterator it = rect.begin();
    set<edge> VRX;
    while(it != rect.end())
    {
        edge e1;
        e1.edgetype = "LEFT";
        e1.i = (*it).y_int;
        e1.x =  (*it).x_int.bottom;

        edge e2;
        e2.edgetype = "RIGHT";
        e2.i = (*it).y_int;
        e2.x =  (*it).x_int.top;

        VRX.insert(e1);
        VRX.insert(e2);
        it++;
    }
    set<interval> L,R;
    set<int> P;
    interval ylimits;
    rectangle frame = findFrame();
    ylimits.bottom = frame.y_int.bottom;
    ylimits.top = frame.y_int.top;
    interval xlimits;
    xlimits.bottom = frame.x_int.bottom;
    xlimits.top = frame.x_int.top;
    SRT srt;
    srt = stripes(VRX,xlimits, ylimits);
    return srt.S;
    // set<edge>::iterator ite = VRX.begin();
    // while(ite != VRX.end())
    // {
    //     cout<<(*ite).x<<" "<<(*ite).edgetype<<endl;
    //     ite++;
    // }
}

int measure(set<stripe> S)
{
    int ans = 0;
    auto itr = S.begin();
    while(itr!=S.end())
    {
        stripe s1 = (*itr);
        ans += ((s1.x_union) * (s1.y_int.top - s1.y_int.bottom));
        ++itr;
    }
    return ans;
}

void csv_writer()
{
    std::ofstream myFile("rectangles.csv");
    
    // Send data to the stream
    myFile << "x1"<<","<<"x2"<<","<<"y1"<<","<<"y2"<<","<<"d"<<"\n";
    rectangle frame = findFrame();
    myFile<< frame.x_int.bottom<<","<<frame.x_int.top<<","<<frame.y_int.bottom<<","<<frame.y_int.top<<"\n";
    auto iter = iso_rectangles_input.begin();
    while(iter != iso_rectangles_input.end())
    {
        rectangle r = (*iter);
        myFile<< r.x_int.bottom<<","<<r.x_int.top<<","<<r.y_int.bottom<<","<<r.y_int.top<<","<<0<<"\n";
        iter++;
    }
    // myFile << "2\n";
    // myFile << "3\n";
    
    // Close the file
    myFile.close();
}

int main()
{
    //taking inputs for rectangles
    int n; //number of rectangles
    cin>>n;
    interval x,y;
    int x1,x2,y1,y2;
    for(int i=0;i<n;i++)
    {
        cin>>y1>>y2>>x1>>x2;
        x.bottom = x1; 
        x.top = x2;
        y.bottom = y1;
        y.top = y2;
        rectangle r;
        r.x_int = x;
        r.y_int = y;
        iso_rectangles_input.push_back(r);
    }
    rectangle frame = findFrame();
    // cout<<"Frame determined"<<endl;
    // cout<<"Lower Bound X :"<< frame.x_int.bottom<<endl;
    // cout<<"Higher Bound X :"<< frame.x_int.top<<endl;
    // cout<<"Lower Bound Y :"<< frame.y_int.bottom<<endl;
    // cout<<"Higher Bound Y :"<< frame.y_int.top<<endl;
    // cout<<"--------------------------------------"<<endl;
    
    csv_writer();
    set<stripe> S = rectangle_DAC(iso_rectangles_input);

    // auto itr = S.begin();

    // while(itr!= S.end())
    // {
    //     cout<<(*itr).x_int.bottom<<" "<<(*itr).x_int.top<<endl;
    //     itr++;
    // }

    int measure_val = measure(S);
    cout<<measure_val;
    

}