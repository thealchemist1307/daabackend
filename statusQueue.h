#ifndef STATUSQUEUE_H
#define STATUSQUEUE_H


#include <bits/stdc++.h>
using namespace std;

///Structure defining a point
struct Point{
    ///X-coordinate
    double x;
    ///Y-coordinate
    double y;
    int type;
};

///Structure defining a line
struct Line{
    ///Index of line
    int index;
    ///First point of the line, taken as a point object
    Point upper;
    ///First point of the line, taken as a point object
    Point lower;
};

///Structure for the status queue tree
struct statusQueue{
    ///Each node takes a line object's data
    Line segment;
    ///Left child pointer
    statusQueue* left;
    ///Right child pointer
    statusQueue* right;
    ///Height of the node, for balancing
    int height;
};

///This class is used to manipulate the Status Queue tree
class status{
    
    public:
    int two_insert;
    ///Constructor for the class
    status(){
        two_insert = 1;
    }

    ///Function which returns height of the node, for balancing
    int height(statusQueue* node){
        if(node == NULL)
            return 0;
        return node->height;
    }

    //! Function to create an status queue node for insertion. Takes in the Line segment to be inserted
    statusQueue *createStatusQueueNode(Line segment) {
        statusQueue *newnode = new statusQueue();
        newnode->segment = segment;
        newnode->left = NULL;
        newnode->right = NULL;
        newnode->height = 1;
        return newnode;
    }

    ///Rotate the node toward the right, used in some cases while tree balancing
    statusQueue *rightRotate(statusQueue *y){
        statusQueue *x = y->left;
        statusQueue *z = x->right;
        x->right = y;
        y->left = z;
        if(height(y->left)>(height(y->right)+1))
            y->height=height(y->left);
        else
            y->height=height(y->right)+1;
        if(height(x->left)>(height(x->right)+1))
            x->height=height(x->left);
        else
            x->height=height(x->right)+1;
        return x;
    }

    ///Rotate the node toward the left, used in some cases while tree balancing
    statusQueue *leftRotate(statusQueue *y){
        statusQueue *x = y->right;
        statusQueue *z = x->left;
        x->left = y;
        y->right = z;
        if(height(y->left)>(height(y->right)+1))
            y->height=height(y->left);
        else
            y->height=height(y->right)+1;
        if(height(x->left)>(height(x->right)+1))
            x->height=height(x->left);
        else
            x->height=height(x->right)+1;
        return x;
    }

    ///Used in tree balancing, specifically gets the difference in heights between the left and right children
    int getBalance(statusQueue *N)
    {
        if (N == NULL)
            return 0;
        return height(N->left) - height(N->right);
    }

    ///Used in tree balancing, specifically gets the difference in heights between the left and right children
    int heightDiff(statusQueue* node){
        if(node == NULL)
            return 0;
        return height(node->left) - height(node->right);
    }

    //! Special Comparator for the Status queue tree
    /*! The Status queue does not order the nodes the way a normal tree does
    * It comapres the slope of the two lines to decide which side of the tree the node must enter.
    */
    int compare(Line l1, Line l2, double y){
        int x1 = (y-l1.lower.y)*((l1.lower.x-l1.upper.x)/(l1.lower.y - l1.upper.y))+l1.lower.x;
        int x2 = (y-l2.lower.y)*((l2.lower.x-l2.upper.x)/(l2.lower.y - l2.upper.y))+l2.lower.x;
        if(x1<x2)
            return 1;
        else if(x1>x2)
            return -1;
        else
            return 0;
    }

    /// Returns the X coordinate, for comparison  
    double sloper(Line l1, double y){
        if(l1.lower.y - l1.upper.y != 0)
            return ((y - l1.lower.y)*(l1.lower.x - l1.upper.x)/(l1.lower.y - l1.upper.y)) + l1.lower.x;
        else
            return ((y - l1.lower.y)*(l1.lower.x - l1.upper.x)/(l1.lower.y - l1.upper.y + 1e-15)) + l1.lower.x;
    }

    ///Returns the max between two integers
    int max(int a, int b){
        int maxer = a>b?a:b;
        return maxer;
    }

    //! Function to insert a point into the status.
    /*! Takes the line segment, y coordinate in order to calculate the slope of the line for the comparator
        After the Line is inserted, rotations are performed to maintain the balanced property of the Status queue */
    statusQueue* inserti(statusQueue* root, Line l1, double y_coor)
    {
        /*if (root == NULL)
            return createStatusQueueNode(l1);

        //how to insert here?
        if(compare(l1, root->segment, y_coor))
            root->right=inserti(root->right,l1);
        else
            root->left=inserti(root->left,l1);
        root->height =(height(root->left) > height(root->right))? height(root->left) + 1 : height(root->right) + 1 ;

        int diff = heightDiff(root);
        if (diff > 1 && compare(root->segment,l1))
            return rightRotate(root);
        if (diff < -1 && compare(l1,root->segment))
            return leftRotate(root);
        if (diff > 1 && compare(l1,root->segment)){
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (diff < -1 && compare(root->segment,l1)) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;*/
        //cout<<"Entered"<<endl;
        int *jdouble = &two_insert;
        if(root == NULL){
            *jdouble = 1;
            //cout<<"Node Created"<<endl;
            return createStatusQueueNode(l1);
        }
        //double suml1 = ((y_coor - l1.lower.y)*((l1.lower.x - l1.upper.x)/(l1.lower.y - l1.upper.y))) + l1.lower.x;
        //double sumr_segment = ((y_coor - root->segment.lower.y)*((root->segment.lower.x - root->segment.upper.x)/(root->segment.lower.y - root->segment.upper.y))) + root->segment.lower.x;
        //cout<<sloper(l1, y_coor)<<" "<<sloper(root->segment, y_coor)<<endl;
        if(sloper(l1, y_coor) < sloper(root->segment, y_coor)){
            //cout<<"Moving Left"<<endl;
            root->left = inserti(root->left, l1, y_coor);
            if(*jdouble == 1){
                root->right = createStatusQueueNode(root->segment);
                root->segment = l1;
                *jdouble = 0;
            }
        }else if (sloper(l1, y_coor) > sloper(root->segment, y_coor)){
            //cout<<"Moving Right"<<endl;
            root->right = inserti(root->right, l1, y_coor);
            if(*jdouble == 1){
                root->left = createStatusQueueNode(root->segment);
                *jdouble = 0;
            }
        }else{
            //cout<<"Returned"<<endl;
            return root;
        }
        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);
        //cout<<"Started Balancing"<<endl;
        if(balance > 1 && sloper(l1, y_coor) < sloper(root->left->segment, y_coor))
            return rightRotate(root);
        else if(balance < -1 && sloper(l1, y_coor) > sloper(root->right->segment, y_coor))
            return leftRotate(root);
        else if(balance > 1 && sloper(l1, y_coor) > sloper(root->left->segment, y_coor)){
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }else if(balance < -1 && sloper(l1, y_coor) < sloper(root->right->segment, y_coor)){
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        //cout<<"Done Balancing"<<endl;
        return root;
    }

    ///Function that returns the next status queue node
    statusQueue * minValueSegment(statusQueue* segment)
    {
        statusQueue* current = segment;
        while (current->left != NULL)
            current = current->left;
        return current;
    }

    
    ///Function to delete the status queue node from the tree
    statusQueue* deleteSegment(statusQueue* root, Line l1, double y_coor){
        /*if (root == NULL)
            return root;
        if(root->right!=NULL)
            root->right = deleteSegment(root->right);
        else{
            if( (root->left == NULL)||(root->right == NULL)){
                statusQueue *temp = root->left ? root->left : root->right;
                if (temp == NULL){
                    temp = root;
                    root = NULL;
                }
                else
                *root = *temp;
                free(temp);
            }else{
                statusQueue* temp = minValueSegment(root->right);
                root->segment = temp->segment;
                root->right = deleteSegment(root->right);
            }
        }
        if (root == NULL)
            return root;
        root->height = 1 + max(height(root->left),height(root->right));
        int balance = getBalance(root);
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0){
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0){
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;*/
        if(root==NULL)
            return root;
        if(sloper(l1, y_coor) < sloper(root->segment, y_coor)){
            //cout<<"Moving Left"<<endl;
            root->left = deleteSegment(root->left, l1, y_coor);
        }
        else if(sloper(l1, y_coor) > sloper(root->segment, y_coor)){
            //cout<<"Moving Right"<<endl;
            root->right = deleteSegment(root->right, l1, y_coor);
        }
        else{
            //cout<<"Found element to be deleted "<<root->segment.lower.x<<" "<<root->segment.lower.y<<" "<<sloper(l1, y_coor)<<" "<<sloper(root->segment, y_coor)<<endl;
            if((root->left == NULL) || (root->right == NULL)){
                statusQueue* temp = root->left ? root->left : root->right;
                if(temp == NULL){
                    temp = root;
                    root = NULL;
                }else
                    *root = *temp;
                free(temp);
            }else{
                statusQueue* temp = minValueSegment(root->right);
                root->segment = temp->segment;
                //cout<<"Deleted Right"<<endl;
                root->right = deleteSegment(root->right, temp->segment, y_coor);
            }
        }
        if(root == NULL)
            return root;

        root->height = 1 + max(height(root->left), height(root->right));
        int balance = getBalance(root);
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);
        if (balance > 1 && getBalance(root->left) < 0){
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);
        if (balance < -1 && getBalance(root->right) > 0){
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
        return root;
    }

    ///Preorder Traversal of the tree, used for debugging mainly
    void preOrder(statusQueue* root){
        if(root!=NULL){
            cout<<root->segment.upper.x<<" "<<root->segment.upper.y<<" "<<root->segment.lower.x<<" "<<root->segment.lower.y<<" "<<root->height<<endl;
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    ///Function that returns the left neighbor of a node, very crucial in the algorithm to determine the node in the status queue.
    void getLeftNeighbor(statusQueue* node, Line l1, double y_coor, Line* lastRight){
        //cout<<"Entered Left Neighbor"<<node->height<<endl;
        //cout<<l1.upper.x<<" "<<l1.upper.y<<endl;
        if(node->height == 1){
            if(lastRight->upper.x == -1){
                if(sloper(node->segment, y_coor-0.1) < sloper(l1, y_coor-0.1))
                    *lastRight = node->segment;
            }
            return ;
        }
        

        if((sloper(l1, y_coor-0.1) - 0.1) < sloper(node->segment, y_coor-0.1))
            getLeftNeighbor(node->left, l1, y_coor, lastRight);
        else if((sloper(l1, y_coor-0.1) - 0.1) > sloper(node->segment, y_coor-0.1)){
            *lastRight = node->segment;
            getLeftNeighbor(node->right, l1, y_coor, lastRight);
        }
    }

    ///Function that returns the right neighbor of a node, very crucial in the algorithm to determine the node in the status queue.
    void getRightNeighbor(statusQueue* node, Line l1, double y_coor, Line* lastLeft){
        //cout<<"Entered Right Neighbor"<<node->height<<endl;
        //cout<<l1.upper.x<<" "<<l1.upper.y<<endl;
        if(node->height == 1){
            //cout<<"Found"<<endl;
            if(lastLeft->upper.x == -1){
                if(sloper(node->segment, y_coor-0.1) > sloper(l1, y_coor-0.1))
                    *lastLeft = node->segment;
            }
            return ;
        }
        if((sloper(l1, y_coor-0.1)+0.1) < sloper(node->segment, y_coor-0.1)){
            *lastLeft = node->segment;
            getRightNeighbor(node->left, l1, y_coor, lastLeft);
        }else if((sloper(l1, y_coor-0.1)+0.1) > sloper(node->segment, y_coor-0.1))
            getRightNeighbor(node->right, l1, y_coor, lastLeft);
        //cout<<"Exit Right Neighbor"<<endl;
    }
    
    ///Function that returns both the neighbors of a node, given the point (taken in x_coor and y_coor)
    void getNeighbors(statusQueue* node, double x_coor, double y_coor, Line* lastRight, Line* lastLeft){
        if(node->height == 1){
            if(lastRight->upper.x == -1){
                if(sloper(node->segment, y_coor-0.1) <= x_coor)
                    *lastRight = node->segment;
            }
            if(lastLeft->upper.x == -1){
                if(sloper(node->segment, y_coor-0.1) > x_coor)
                    *lastLeft = node->segment;
            }
            return ;
        }
        if(x_coor < sloper(node->segment, y_coor-0.1)){
            *lastLeft = node->segment;
            getNeighbors(node->left, x_coor, y_coor, lastRight, lastLeft);
        }else if(x_coor > sloper(node->segment, y_coor-0.1)){
            *lastRight = node->segment;
            if(node->right == NULL)
                return ;
            getNeighbors(node->right, x_coor, y_coor, lastRight, lastLeft);
        }
    }
};

#endif
