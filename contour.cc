#include<bits/stdc++.h>
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

typedef struct line_segment
{
	interval inter;
	int x;

	bool operator<(const line_segment& e) const
    { 
        if(this->x<e.x)
		{
			return true;
		}
		else if(this->x==e.x)
		{
			if(this->inter.bottom<e.inter.bottom)
			{
				return true;
			}
			else if(this->inter.bottom==e.inter.bottom)
			{
				if(this->inter.top<e.inter.top)
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
        else
        {
            return false;
        }
    }

}line_segment;

typedef struct rectangle
{
    interval x_int,y_int;
    int id;
    
}rectangle;

typedef struct edge
{
    string edgetype;
    interval i;
    int x;
    //rectangle* rect;
    int rect_id;

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

typedef struct ctree
{
    int x;
    string lru;
    ctree* lson;
    ctree* rson;

    ctree(int x, string lru, ctree* lson, ctree* rson)
    {
        this->x=x;
        this->lru=lru;
        this->lson=lson;
        this->rson=rson;
    }
}ctree;

// void print_tree(ctree* tree)
// {
//     if(tree==NULL)
//     {
//         cout<<"NULL"<<endl;
//         return;
//     }
//     cout<<" X is "<<tree->x<<endl;
//     cout<<" LRU is "<<tree->lru<<endl;
// }

typedef struct stripe
{
    interval x_int;
    interval y_int;
    ctree* tree;

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
            else // Can also add the condition for x_int
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

string LRU_TYPES[3] = {"LEFT","RIGHT","UNDEF"};

vector<rectangle> iso_rectangles_input;

typedef struct StripeReturnType
{
    set<pair<interval,int>> L;
    set<pair<interval,int>> R;
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

// set<int> x_proj()
// {
//     set<int> x_coordinates;


//     return x_coordinates;
// }

void inorder(ctree *node)
{
    if (node == NULL)
        return;
    if (node->lru == "LEFT" || node->lru == "RIGHT")
    {
        if (node->lru == "LEFT")
            cout << "Left: ";
        else
            cout << "Right: ";
        cout << " " << node->x << " ";
        return;
    }
    inorder(node->lson);
    inorder(node->rson);
}

void inorder_find(ctree *node, int low_x, int high_x, vector<ctree *> &res)
{
    if (node == NULL)
        return;
    if (node->lru == "LEFT" || node->lru == "RIGHT")
    {
            res.push_back(node);
    }
    if (node->x >= low_x)
        inorder_find(node->lson, low_x, high_x, res);
    if (node->x <= high_x)
        inorder_find(node->rson, low_x, high_x, res);
}

set<pair<interval,int>> set_intersection(set<pair<interval,int>> i1, set<pair<interval,int>> i2)
{
    auto ite = i1.begin();
    auto itr = i2.begin();
    set<pair<interval,int>> i3;
    while(ite!=i1.end() and itr!=i2.end())
    {
        interval a = (*ite).first;
        interval b = (*itr).first;
        int a_id = (*ite).second;
        int b_id = (*itr).second;
        if(a.bottom < b.bottom)
        {
            ite++;
        }
        else if(a.bottom == b.bottom)
        {
            if(a.top == b.top and a_id == b_id)
            {
                i3.insert({a,a_id}); //random number
                ite++;
                itr++;
            }
            else if(a.top == b.top and a_id != b_id)
            {
                if(a_id<b_id)
                {
                    ite++;
                }
                else
                {
                    itr++;
                }
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

set<pair<interval,int>> set_difference(set<pair<interval,int>> i1,set<pair<interval,int>> i2)
{
    auto ite = i1.begin();
    auto itr = i2.begin();
    set<pair<interval,int>> i3;
    while(ite!=i1.end() and itr!=i2.end())
    {
        interval a = (*ite).first;
        interval b = (*itr).first;
        int a_id = (*ite).second;
        int b_id = (*itr).second;
        if(a.bottom < b.bottom)
        {
            i3.insert({a,a_id}); //random number
            ite++;
        }
        else if(a.bottom == b.bottom)
        {
            if(a.top == b.top and a_id == b_id)
            {
                ite++;
                itr++;
            }
            else if(a.top == b.top and a_id != b_id)
            {
                if(a_id<b_id)
                {
                    i3.insert({a,a_id}); //random number
                    ite++;
                }
                else
                {
                    itr++;
                }
            }
            else if(a.top < b.top)
            {
                i3.insert({a,a_id});
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

set<pair<interval,int>> set_union(set<pair<interval,int>> i1,set<pair<interval,int>> i2)
{
    set<pair<interval,int>> i3;
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
        stemp.y_int = (*ite);
        stemp.tree = NULL;
        
        while(itr!= S.end())
        {
            interval c = (*itr).y_int; //S
            if(stemp.y_int.bottom >= c.bottom)
            {
                if(stemp.y_int.top <= c.top)
                {
                    //useful S
                    stemp.tree = (*itr).tree;
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

set<stripe> blacken(set<stripe> S, set<pair<interval,int>> J)
{
    auto itr = S.begin();
    auto ite = J.begin();
    set<stripe> s_final;
    while(itr != S.end())
    {
        stripe stemp = (*itr);
        interval i1 = stemp.y_int;
        while(ite != J.end())
        {
            //i1 should be subset of i2
            interval i2 = (*ite).first;
            if(i1.bottom < i2.bottom)
            {
                break;
            }
            else
            {
                if(i1.top <= i2.top)
                {
                    stemp.tree=NULL;
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
        stemp.y_int = (*ite);
        stemp.tree = NULL;

        stripe val1 = (*itr1);
        stripe val2 = (*itr2);
        
        if(val1.tree!=NULL and val2.tree!=NULL)  // a!=b!=c
        {
            stemp.tree=new ctree(val1.x_int.top,"UNDEF",val1.tree, val2.tree);
        }
        else if(val1.tree!=NULL and val2.tree==NULL)
        {
            stemp.tree=val1.tree;
        }
        else if(val1.tree==NULL and val2.tree!=NULL)
        {
            stemp.tree=val2.tree;
        }
        else
        {
            stemp.tree=NULL;
        }
        itr1++; 
        itr2++;
        S.insert(stemp);
        ++ite;
    }
    return S;
}

SRT stripes(set<edge> VRX, interval x_ext, interval ylimits)
{
    
    if(VRX.size()==1)
    {
        SRT temp1;
        auto it = VRX.begin();
        edge e = *it;
        if(e.edgetype == "LEFT")
        {
            interval l1 = e.i;
            temp1.L.insert({l1,e.rect_id});
        }
        else
        {
            interval r1 = e.i;
            temp1.R.insert({r1,e.rect_id});    
        }

        temp1.P.insert(ylimits.bottom);
        temp1.P.insert(e.i.bottom);
        temp1.P.insert(e.i.top);
        temp1.P.insert(ylimits.top);

        set<interval> part = y_partition(temp1.P);
        auto ite = part.begin();
        while(ite != part.end())
        {
            stripe stemp;
            stemp.x_int = x_ext;
            stemp.y_int = *ite;
            stemp.tree =(ctree*)malloc(sizeof(ctree));
            stemp.tree=NULL;
            if((stemp.y_int.bottom == e.i.bottom) and (stemp.y_int.top == e.i.top))
            {
                if(e.edgetype == "LEFT")
                {
                    stemp.tree=new ctree(e.x,"LEFT",NULL,NULL);
                }
                else if(e.edgetype == "RIGHT")
                {
                    stemp.tree=new ctree(e.x,"RIGHT",NULL,NULL);
                }
            }
            temp1.S.insert(stemp);
            ++ite;
        }
        return temp1;
    }
    else
    {
        int n = VRX.size();
        int xm;
        set<edge> v1,v2;
        auto it = VRX.begin();
        for(int i=0;i<n/2;i++)
        {
            // cout<<"V1: "<<(*it).i.bottom<<" "<<(*it).i.top<<endl;
            v1.insert(*it);
            ++it;
        }
        for(int i=n/2;i<n;i++)
        {
            if(i==n/2)
            {
                xm = (*it).x;
            }
            // cout<<"V2: "<<(*it).i.bottom<<" "<<(*it).i.top<<endl;
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
        set<pair<interval,int>> L1=left.L;
        set<pair<interval,int>> R2=right.R;
        
        set<pair<interval,int>> L,R;
        set<pair<interval,int>> LR;
        
        LR = set_intersection(L1,R2);
        set<pair<interval,int>> l1lr = set_difference(L1,LR);
        set<pair<interval,int>> r2lr = set_difference(R2,LR);
        L = set_union(right.L,l1lr);
        R = set_union(left.R,r2lr);
        // l = (L1-LR) union L2;
        // r = (R2-LR) union R1;
      
        set<int> P;
        set<int> P1= left.P;
        set<int> P2= right.P;
        auto itr=P1.begin();
        while(itr!=P1.end())
        {
            P.insert(*itr);
            itr++;
        }
        itr=P2.begin();
        while(itr!=P2.end())
        {
            P.insert(*itr);
            itr++;
        }


        set<stripe> S_left,S_right;
        interval slefttemp;
        slefttemp.bottom=x_ext.bottom;
        slefttemp.top=xm;
        interval srighttemp;
        srighttemp.bottom=xm;
        srighttemp.top=x_ext.top;

        S_left=copy(left.S,P,slefttemp);
        S_right=copy(right.S,P,srighttemp);

        S_left = blacken(S_left,r2lr);
        S_right = blacken(S_right,l1lr);

        set<stripe> S;
        S = concat(S_left, S_right, P, x_ext);

        SRT temp2;
        temp2.L = L;
        temp2.R = R;
        temp2.P = P;
        temp2.S = S;
        
        return temp2;
    }
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
        // e1.rect=&(*it);
        e1.rect_id = (*it).id;


        edge e2;
        e2.edgetype = "RIGHT";
        e2.i = (*it).y_int;
        e2.x =  (*it).x_int.top;
        e2.rect_id = (*it).id;

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

set<line_segment> contour_pieces(edge h, set<stripe> S)
{
	// TBI
    stripe stemp;
    if(h.edgetype == "BOTTOM")
    {
        for(auto t : S)
        {
            if(t.y_int.top == h.x)
            {
                stemp = t;
                break;
            }
        }
    }
    else
    {
        for(auto t : S)
        {
            if(t.y_int.bottom == h.x)
            {
                stemp = t;
                break;
            }
        }
    }

    int lower_X = h.i.bottom;
    int higher_X = h.i.top;
    vector<ctree *> result;
    set<interval> J;
    cout << "Edge H" << endl;
    cout << h.i.bottom << " " << h.i.top << " " << h.x << endl;
    cout << "InorderSearch" << endl;
    inorder_find(stemp.tree, lower_X, higher_X, result);
    for (auto it : result)
    {
        cout << it->x << " " << it->lru << endl;
    }
    cout << endl;

    bool open = true;
    int px = lower_X;
    for (auto it : result)
    {
        if (it->lru == "LEFT")
        {   
            if (px < it->x)
                J.insert({px, min(higher_X,it->x)});
            open = false;
        }
        else if(it->x>=px)
        {
            px = it->x;
            open = true;
        }
    }

    if (open)
    {
        if (px < higher_X)
        {
            J.insert({px, higher_X});
        }
    }
    set<line_segment> returnVal;
    int y = h.x;
    for (auto it : J)
    {
        line_segment ls;
        ls.inter = it;
        ls.x = y;
        returnVal.insert(ls);
    }
    return returnVal;
}

set<line_segment> contour(set<edge> H, set<stripe> S)   // TBI- nlogn ?
{
	set<line_segment> ans_contour;

	auto it=H.begin();
	while(it!=H.end())
	{
		edge temp=(*it);
        cout<<"The horizontal edge is : "<<temp.x<<" "<<temp.i.bottom<<" "<<temp.i.top<<endl;
		set<line_segment> ltemp=contour_pieces(temp,S);
		auto it1=ltemp.begin();
        while(it1!=ltemp.end())  
        {
            ans_contour.insert(*it1);
            it1++;
        }
		it++;
	}
	return ans_contour;
}

set<interval> merge_intervals(set<interval> s)
{
    set<interval> ans;
    interval itemp;
    auto it = s.begin();
    itemp.bottom = (*it).bottom;
    itemp.top = (*it).top;
    ++it;
    while(it!=s.end())
    {
        interval cur = *it;
        if(cur.bottom <= itemp.top)
        {
            itemp.top = max(cur.top,itemp.top);
        }
        else 
        {
            ans.insert(itemp);
            itemp.bottom = cur.bottom;
            itemp.top = cur.top;
        }
        ++it;
    }
    ans.insert(itemp);
    return ans;
}

void csv_writer(set<line_segment> hrc, set<line_segment> vrc)
{
    std::ofstream myFile("rectangles.csv");
    
    // Send data to the stream
    rectangle frame = findFrame();
    myFile<<"x1"<<","<<"x2"<<","<<"y1"<<","<<"y2"<<"\n";
    myFile<< frame.x_int.bottom<<","<<frame.x_int.top<<","<<frame.y_int.bottom<<","<<frame.y_int.top<<"\n";
    auto iter = iso_rectangles_input.begin();
    while(iter != iso_rectangles_input.end())
    {
        rectangle r = (*iter);
        myFile<< r.x_int.bottom<<","<<r.x_int.top<<","<<r.y_int.bottom<<","<<r.y_int.top<<"\n";
        
        iter++;
    }
    // myFile << "2\n";
    // myFile << "3\n";
    
    // Close the file
    myFile.close();

    std::ofstream myFile2("contours.csv");
    
    // Send data to the stream
    myFile2<<"x1"<<","<<"y1"<<","<<"x2"<<","<<"y2"<<"\n";
    auto itc = hrc.begin();
    while(itc != hrc.end())
    {
        line_segment ls = (*itc);
        myFile2<< ls.inter.bottom<<","<<ls.x<<","<<ls.inter.top<<","<<ls.x<<"\n";
        itc++;
    }
    auto itv = vrc.begin();
    while(itv != vrc.end())
    {
        line_segment ls = (*itv);
        myFile2<< ls.x<<","<<ls.inter.bottom<<","<<ls.x<<","<<ls.inter.top<<"\n";
        itv++;
    }
    // myFile << "2\n";
    // myFile << "3\n";
    
    // Close the file
    myFile2.close();
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
        cin>>x1>>x2>>y1>>y2;
        x.bottom = x1; 
        x.top = x2;
        y.bottom = y1;
        y.top = y2;
        rectangle r;
        r.x_int = x;
        r.y_int = y;
        r.id = i;
        iso_rectangles_input.push_back(r);
    }
    rectangle frame = findFrame();
    // cout<<"Frame determined"<<endl;
    // cout<<"Lower Bound X :"<< frame.x_int.bottom<<endl;
    // cout<<"Higher Bound X :"<< frame.x_int.top<<endl;
    // cout<<"Lower Bound Y :"<< frame.y_int.bottom<<endl;
    // cout<<"Higher Bound Y :"<< frame.y_int.top<<endl;
    // cout<<"--------------------------------------"<<endl;
    
    set<stripe> S = rectangle_DAC(iso_rectangles_input);
   
    int count = 1;
    for (auto s : S)
    {
        cout << "Stripe " << count++ << endl;
        cout << s.x_int.bottom << " " << s.x_int.top << " " << s.y_int.bottom << " " << s.y_int.top << endl;
        inorder(s.tree);
        cout << endl;
    }

    // Ordering for inserting horizontal edges -HRX
    // For (every edge in HRX) and S call contour_pieces
    // Take union of above results
    // Compute Vertical contours from horizontal contours
    // Implement- inorder and contour pieces

    vector<rectangle>::iterator it = iso_rectangles_input.begin();
    set<edge> HRX;
    while(it != iso_rectangles_input.end())
    {
        edge e1;
        e1.edgetype = "BOTTOM";
        e1.i = (*it).x_int;
        e1.x =  (*it).y_int.bottom;
        e1.rect_id=(*it).id;

        edge e2;
        e2.edgetype = "TOP";
        e2.i = (*it).x_int;
        e2.x =  (*it).y_int.top;
        e2.rect_id=(*it).id;

        HRX.insert(e1);
        HRX.insert(e2);
        it++;
    }
    set<line_segment> horizontal_contours_unmerged = contour(HRX,S);
    


    map<int, set<interval>> merged_horizontal_contours;
    for (auto h : horizontal_contours_unmerged)
    {
        interval i1;
        i1.bottom = h.inter.bottom;
        i1.top = h.inter.top;
        merged_horizontal_contours[h.x].insert(i1);
    }

    

    vector<pair<int,int>> vertical_contours_points;

    for(auto h : merged_horizontal_contours)
    {
        set<interval> s1 = merge_intervals(h.second);
        merged_horizontal_contours[h.first] =  s1;
        for(auto siter : s1)
        {
            interval temp = siter;
            pair<int,int> p1;
            p1.first = temp.bottom;
            p1.second = h.first;
            vertical_contours_points.push_back(p1);
            p1.first = temp.top;
            vertical_contours_points.push_back(p1);
        }
    }
    set<line_segment> horizontal_contours;


    
    for (auto h : merged_horizontal_contours)
    {
        for(auto val : h.second)
        {
            line_segment ls;
            ls.inter.bottom = val.bottom;
            ls.inter.top = val.top;
            ls.x = h.first;
            horizontal_contours.insert(ls);
            
        }
    }
    cout << "Horizontal Contours\n";
    for(auto lsi : horizontal_contours)
    {
        cout<<lsi.inter.bottom<<" "<<lsi.x<<"     "<<lsi.inter.top<<" "<<lsi.x<<endl;
    }
    sort(vertical_contours_points.begin(),vertical_contours_points.end());
    
    set<line_segment> vertical_contours;
    auto vit = vertical_contours_points.begin();
    cout<<"Vertical contours\n";
    while(vit!= vertical_contours_points.end())
    {
        pair<int,int> p1;
        p1.first = (*vit).first; 
        p1.second = (*vit).second;
        line_segment l1;
        l1.x = p1.first;
        l1.inter.bottom = p1.second;
        vit++;
        p1.second = (*vit).second;
        l1.inter.top = p1.second;
        vertical_contours.insert(l1);
        vit++;   
        //printing value
        
        cout<<l1.x<<" "<<l1.inter.bottom<<"     "<<l1.x<<" "<<l1.inter.top<<endl;

    }

    csv_writer(horizontal_contours,vertical_contours);


    //send nishit horizontal contours and vertical contours
}