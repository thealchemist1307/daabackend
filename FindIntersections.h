#ifndef INTER_H
#define INTER_H

#include <bits/stdc++.h>
#include "statusQueue.h"
#include "eventQueue.h"
using namespace std;

class FindIntersections{
    /*!Private members of the class FindIntersections
    * Consists of a Status Queue and an Event Queue
    */
    private:
    event newEvent;
    eventQueue* eq_root = NULL;
    status newStatus = status();
    statusQueue* st_root = NULL;
    public:
    vector<Point> in_points;
    //!A Constructor for class FindIntersections
    /*! It makes sure that the lower and upper pairs are ordered correctly before
    they're inserted into the Event Queue i.e lower points have a teller value of 2 while
    upper points have a teller value of 1.
    */
    FindIntersections(vector<Line> &segments){
        for(auto it = segments.begin(); it!=segments.end(); it++){
            double sx, sy, ex, ey;
            if((*it).upper.y >= (*it).lower.y){
                sx = (*it).upper.x;
                sy = (*it).upper.y;
                ex = (*it).lower.x;
                ey = (*it).lower.y;
            }else{
                sx = (*it).lower.x;
                sy = (*it).lower.y;
                ex = (*it).upper.x;
                ey = (*it).upper.y;
            }
            if((*it).upper.y == (*it).lower.y){
                if((*it).upper.x <= (*it).lower.x){
                    sx = (*it).upper.x;
                    sy = (*it).upper.y;
                    ex = (*it).lower.x;
                    ey = (*it).lower.y;
                }else{
                    sx = (*it).lower.x;
                    sy = (*it).lower.y;
                    ex = (*it).upper.x;
                    ey = (*it).upper.y;
                }
            }
            Point eventPoint;
            Line l1;
            eventPoint.x = sx;
            eventPoint.y = sy;
            l1.upper.x = sx;
            l1.upper.y = sy;
            l1.lower.x = ex;
            l1.lower.y = ey;
            eq_root = newEvent.inserti(eq_root, eventPoint, l1, 1);
            eventPoint.x = ex;
            eventPoint.y = ey;
            eq_root = newEvent.inserti(eq_root, eventPoint, l1, 2);
        }
    }
    //! A helper function to find the minimum of two integer values.
    int min(int a, int b){
        return (a>b)?b:a;
    }
    //! A member function that checks whether a point q lies on the segment between points p and r.
    bool onSegment(Point p, Point q, Point r){
        if(q.x <= max(p.x, r.x) && q.x >=min(p.x, r.x) && q.y <=max(p.y, r.y) && q.y >= min(p.y, r.y)){
            return true;    
        }
        return false;
    }
    //! Checks the relative orientation of three sets of ordered points.
    /*! Checks the orientation of an ordered set of triplets (p, q, r)
    * 0 => p, q and r are collinear
    * 1=> Clockwise orientation
    * 2=> Anti-Clockwise Orientation
    */
    int orientation(Point p, Point q, Point r){
        double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if(val == 0)
            return 0; //collinear
        return (val > 0)?1:2; //clockwise, anti-clockwise
    }
    //! Checks whether two lines l1 and l2 intersect.
    /*! Checks whether the lines l1 and l2 intersect.
    Returns true if they intersect.*/
    bool checkIntersect(Line l1, Line l2){
        //cout<<"CI"<<endl;
        Point p1, q1, p2, q2;
        p1.x = l1.upper.x;
        p1.y = l1.upper.y;
        q1.x = l1.lower.x;
        q1.y = l1.lower.y;
        p2.x = l2.upper.x;
        p2.y = l2.upper.y;
        q2.x = l2.lower.x;
        q2.y = l2.lower.y;
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if(o1 != o2 && o3 != o4)
            return true;
        
        if(o1 == 0 && onSegment(p1, p2, q1))
            return true;
        if(o2 == 0 && onSegment(p1, q2, q1))
            return true;
        if(o3 == 0 && onSegment(p2, p1, q2))
            return true;
        if(o4 == 0 && onSegment(p2, q1, q2))
            return true;
        return false;
    }
    //*! This function returns the point of intersection (if any) of two lines l1 and l2.
    /*! It finds the Intersection Point of two lines l1 and l2. The function does this using the determinant method of finding the intersection of two
    2D lines. (Finds the intersection in O(1))*/
    Point intersectionOf(Line l1, Line l2){
        Point intersect;
        if(checkIntersect(l1, l2) == 0){
            intersect.x = -1;
            intersect.y = -1;
        }else{
            double a1 = l1.lower.y - l1.upper.y;
            double b1 = l1.upper.x - l1.lower.x;
            double c1 = a1*(l1.upper.x) + b1*(l1.upper.y);

            double a2 = l2.lower.y - l2.upper.y;
            double b2 = l2.upper.x - l2.lower.x;
            double c2 = a2*(l2.upper.x) + b2*(l2.upper.y);

            double determinant = a1*b2 - a2*b1;
            intersect.x = (b2*c1 - b1*c2)/determinant;
            intersect.y = (a1*c2 - a2*c1)/determinant;
        }
        return intersect;
    }
    //! A function that is similar to the FineNewEvent(sl, sr, p) function defined in the IEEE Article.
    /*! The function checks whether the lines l1 and l2 intersect below (or on) the sweep line 
    and to the right of the current event point p (dependent on whether or not the intersection
    is not yet present as an event in the Event Queue.*/
    void findNewEvent(Line l1, Line l2, eventQueue* p){
        //cout<<"FNE - 1"<<endl;
        Point newEventPoint = intersectionOf(l1, l2);
        if(newEventPoint.y != - 1){
            if(newEventPoint.y < p->eventPoint.y){
                eq_root = newEvent.inserti(eq_root, newEventPoint, l1, 3);
                eq_root = newEvent.inserti(eq_root, newEventPoint, l2, 3);
            }else if(newEventPoint.y == p->eventPoint.y && newEventPoint.x > p->eventPoint.x){
                eq_root = newEvent.inserti(eq_root, newEventPoint, l1, 3);
                eq_root = newEvent.inserti(eq_root, newEventPoint, l2, 3);
            }
        }
        //cout<<"FNE - END"<<endl;
    }
    //! This helper function checks whether a given line is already present within a vector contatining lines.
    int contains(vector<Line> segments, Line l){
        for(auto it=segments.begin(); it!=segments.end(); it++){
            if((*it).upper.x == l.upper.x && (*it).upper.y == l.upper.y && (*it).lower.x == l.lower.x && (*it).lower.y == l.lower.y){
                return 0;
            }
        }
        return 1;
    }
    //! This helper function helps us with Union operations (which are useful in the handleEventPoint function).
    vector<Line> unionOf(vector<Line> a, vector<Line> b){
        vector<Line> union_vec;
        for(auto it = a.begin(); it!=a.end(); it++)
            union_vec.push_back((*it));
        for(auto it = b.begin(); it!=b.end(); it++){
            if(contains(union_vec, (*it)) == 1)
                union_vec.push_back((*it));
        }
        return union_vec;
    }
    //! A helper function to check whether a given vector is empty.
    bool empty(vector<Line> x){
        if(x.size() == 0)
            return true;
        return false;
    }

    //! A function that deals with event points (and enables new event points to be added).
    /*! The function is similar to the HandleEventPoint(p) function defined in the IEEE Article.
    It serves the purpose of adding and removing lines from the status queue as per the rules defined in
    the IEEE Article.*/
    void handleEventPoint(eventQueue* eventer){
        //cout<<"Process-HEP"<<endl;
        vector<Line> all = unionOf(eventer->L, unionOf(eventer->U, eventer->C));
        if(all.size() > 1){
            cout<<"Intersection: "<<eventer->eventPoint.x<<" "<<eventer->eventPoint.y<<endl;
            in_points.push_back(eventer->eventPoint);
        }
        //cout<<"HEP - 4"<<endl;
        vector<Line> deleters = unionOf(eventer->L, eventer->C);
        for(auto it=deleters.begin(); it!=deleters.end(); it++){
            st_root = newStatus.deleteSegment(st_root, (*it), eventer->eventPoint.y);
        }
        //cout<<"HEP - 5"<<endl;
        vector<Line> inserters = unionOf(eventer->U, eventer->C);
        for(auto it=inserters.begin(); it!=inserters.end(); it++)
            st_root = newStatus.inserti(st_root, (*it), eventer->eventPoint.y - 0.1);
        
        //cout<<"HEP - 8"<<endl;
        if(empty(inserters)){
            //cout<<"HEP - 10"<<endl;
            Line l1, l2;
            l1.upper.x = -1;
            l2.upper.x = -1;
            newStatus.getNeighbors(st_root, eventer->eventPoint.x, eventer->eventPoint.y - 0.1, &l1, &l2);
        }else{
            //cout<<"HEP - 11"<<endl;
            Line l1, l2;
            double max = -1.0, min = 1001.0;
            for(auto it = inserters.begin(); it!=inserters.end(); it++){
                double x = newStatus.sloper((*it), eventer->eventPoint.y);
                //cout<<(*it).lower.x<<" "<<(*it).lower.y<<" "<<(*it).upper.x<<" "<<(*it).upper.y<<" "<<eventer->eventPoint.y<<endl;
                //cout<<x<<endl;
                double minX = ((*it).upper.x < (*it).lower.x)?(*it).upper.x:(*it).lower.x;
                double maxX = ((*it).upper.x > (*it).lower.x)?(*it).upper.x:(*it).lower.x;
                if(x < min && x <=maxX && x>=minX){
                    min = x;
                    //cout<<(*it).upper.x<<" "<<(*it).upper.y<<endl;
                    l1 = (*it);
                }
                if(x > max && x <= maxX && x >=minX){
                    max = x;
                    //cout<<(*it).upper.x<<" "<<(*it).upper.y<<endl;
                    l2 = (*it);
                }
            }
            Line l3, l4;
            l3.upper.x = -1;
            l4.upper.x = -1;
            //cout<<"HEP - 11-13"<<endl;
            newStatus.getLeftNeighbor(st_root, l1, eventer->eventPoint.y, &l3);
            //cout<<"HEP 12"<<endl;
            newStatus.getRightNeighbor(st_root, l2, eventer->eventPoint.y, &l4);
            //cout<<"HEP - 13"<<endl;
            if(l3.upper.x != -1 && min!=1001)
                findNewEvent(l3, l1, eventer);
            if(l4.upper.x != -1 && max != - 1)
                findNewEvent(l2, l4, eventer);
            //cout<<"HEP - END"<<endl;
        }
    }

    //!Function that is similar to FindIntersections in the context of the IEEE Document.
    void runAlgorithm(){
        while(eq_root!=NULL){
            eventQueue* pop = newEvent.maxValueEvent(eq_root);
            if(pop != NULL){
                handleEventPoint(pop);
                //newStatus.preOrder(st_root);
                eq_root = newEvent.deleteNode(eq_root, pop->eventPoint.x, pop->eventPoint.y);
            }
        }
        cout<<"Execution Complete"<<endl;
    }
};
#endif