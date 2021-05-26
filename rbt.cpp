/****************************************************
Course		: CO4352 Advanced Algorithms
Name		: YAPA.YMTL
Index no.	: 17_ENG_119
Asgmnt  	: Red Black Tree
****************************************************/

#include <iostream>
#include <vector>
#include <string>

using namespace std;



//Enum that save colors of a node
enum colors { BLACK , RED };

//Structure save data of a node
struct node
{

    int value;

    //If leaf or root nile is true , otherwise fals
    bool NIL;
    colors color;
    
    node* parent;
    node* leftChild;
    node* rightChild;

    node()
    {
        color = BLACK;
        parent = NULL;
        leftChild = NULL;
        rightChild = NULL;
        NIL = true;
        value = 0;
    }

};


class RBT
{
private:
    node* start;
    vector<node> nodes;
    node leaf;

    bool isEmptyTree(node *);//Function to check whether given node is a leaf node(return true), or not (return false)
    bool isNil(node *); //Function to check whether node leaf node 
    void printRBT(string , node* , bool ); //Print tree
    node* minNode(node*);
    node* maxNode(node*);
    node* searchValue(int);

    void LEFT_ROTATION(node*);
    void RIGHT_ROTATION(node*);
    void INSERTION_FIXUP(node*);
    void DELETE_FIXUP(node*);
    void TRANSPLANT_RBT(node*, node* );
    
public:
    RBT();
    void insertion(int);
    void printTree();  
    void printMinMax();
    bool deleteValue(int);

};




int main()
{

    RBT ob;
    /* ob.insertion(10);
    ob.insertion(18);
    ob.insertion(7);
    ob.insertion(15);
    ob.insertion(30);
    ob.insertion(16);
    ob.insertion(25);
    ob.insertion(40);
    ob.insertion(60);
    ob.insertion(2);
    ob.insertion(1);
    ob.insertion(70); */

    ob.insertion(8);
    ob.insertion(5);
    ob.insertion(15);
    ob.insertion(12);
    ob.insertion(19);
    ob.insertion(9);
    ob.insertion(13);
    ob.insertion(23);

    ob.printTree();

    int userInput = 1;
    while(userInput)
    {
        cout<<"\n 1. Insert a value to the tree";
        cout<<"\n 2. Delete a node from the tree";
        cout<<"\n 3. Print the tree ";
        cout<<"\n 4. Print maximum and minimum value of the tree ";
        cout<<"\n 5. Exit(5 or any) " ;
        cout<<"\nEnter Your Choice:_ ";

        cin>>userInput;

        switch (userInput)
        {
            case 1:
            {
                cout<<"\n Enter your value(integer)_ ";
                int n;
                cin>>n;
                ob.insertion(n);
                cout<<endl<<n<<" is entered";
                break;
            }

            case 2:
            {
                cout<<"\n Enter value of the node that you want to delete(integer)_";
                int n;
                cin>>n;
                cout<<endl<< (ob.deleteValue(n)? "Delete sucess": "Delete unsucess")<<endl;
                break;
            }  

            case 3:
            {
                cout<<endl<<endl;
                ob.printTree();
                break;
            }

            case 4:
            {
                cout<<endl<<endl;
                ob.printMinMax();
                break;
            }

            default:
            {
                userInput = 0;
                break;
            }
            
        }
    }
    return 0;
}


RBT::RBT()
{
    //Create root node and attac leaf to children's addresses
    start = new node();
    start->leftChild = &leaf;
    start->rightChild = &leaf;
}

//Chec whether tree is empty
bool RBT::isEmptyTree(node* n)
{
    return (isNil(n->leftChild) && isNil(n->rightChild));
}

//Check given node is a NIL(can be a leaf or start) node or not
bool RBT::isNil(node* n)
{
    return (n->NIL);
}

// Left rotation around x node
void RBT::LEFT_ROTATION(node* x)
{

    node* y = x->rightChild;
    x->rightChild = y->leftChild;

    if(!isNil(y->leftChild))
        y->leftChild->parent = x;
    
    y->parent = x->parent;

    if(isNil(x->parent))
    {
        start->leftChild = y;
        start->rightChild = y;
    }

    else if(x == x->parent->leftChild)
    {
        x->parent->leftChild = y;
    }

    else
    {
        x->parent->rightChild = y;
    }

    y->leftChild = x;
    x->parent = y;

}

//Right rotation around x node
void RBT::RIGHT_ROTATION(node* x)
{
    node* y = x->leftChild;
    x->leftChild = y->rightChild;

    if(!isNil(y->rightChild))
    {
        y->rightChild->parent = x;
    }

    y->parent = x->parent;

    if(isNil(x->parent))
    {
        start->leftChild = y;
        start->rightChild = y;
    }
    else if(x == x->parent->leftChild)
    {
        x->parent->leftChild = y;
    }
    else 
    {
        x->parent->rightChild = y;
    }

    y->rightChild = x;
    x->parent = y;
}

//Insert new value to tree
void RBT::insertion(int value)
{
    //Create new node for the value and insert all the attributes
    node* temp = new node();
    temp->value = value;
    temp->color = RED;
    temp->NIL = false;
    temp->rightChild = &leaf;
    temp->leftChild = &leaf;
    
    node* y = start;//NIL
    node* x =  start->rightChild; //Root

    //Find the location to store new node
    while(!isNil(x))
    {
        y = x;

        if(value<x->value)
        {
            x = x->leftChild;
        }
        else
        {
            x = x->rightChild;
        }
    }

    //Assign the found parent to new node
    temp->parent = y;

    //If tree is empty , new value assign as root
    if(isNil(y))
    {
        y->rightChild = temp;
    }

    //If new value less than the found parent value
    //Value store parent's left side
    else if (value<y->value)
    {
        y->leftChild = temp;
    }

    //Else value store parent's right side
    else
    {
        y->rightChild = temp;
    }

    //Make sure tree valid all the RBT conditions,
    //If not do the changes that valid all the conditions
    INSERTION_FIXUP(temp);
    
}

void RBT::INSERTION_FIXUP(node* z)
{
    while(z->parent->color == RED)
    {   
        //If parent is left children
        if(z->parent == z->parent->parent->leftChild)
        {
            node* uncle = z->parent->parent->rightChild;

            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            //If uncle is black and z is right child
            else if(z == z->parent->rightChild)
            {
                z = z->parent;
                LEFT_ROTATION(z);
            }

            //Uncle is black and z is left child
            else
            {
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RIGHT_ROTATION(z->parent->parent);
            }
        }
        else
        {
            node* uncle = z->parent->parent->leftChild;

            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else if(z == z->parent->leftChild)
            {
                z = z->parent;
                RIGHT_ROTATION(z);
            }
            else
            {
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LEFT_ROTATION(z->parent->parent);
            }
        }

    }
    start->rightChild->color = BLACK;
}

//Print the tree
void RBT::printRBT(string prefix, node* node, bool isLeft)
{
    if( node != nullptr )
    {
        cout<<prefix;

        cout << (!isLeft ? "+-" : "|_" );

        // print the value of the node
        cout << (node->NIL?"nil":to_string(node->value))<<"("<<(node->color ? "R" : "B" )<<")" << endl;

        // enter the next tree level - left and right branch
        printRBT( prefix + (isLeft ? "|   " : "   "), node->rightChild, true);
        printRBT( prefix + (isLeft ? "|   " : "   "), node->leftChild, false);
    }

}

void RBT::printTree()
{
    printRBT("",start->rightChild,false);
}

void RBT::printMinMax()
{
    cout<<"\nMin = "<<(minNode(start->rightChild))->value;
    cout<<"\nMax = "<<(maxNode(start->rightChild))->value<<endl;
}

node* RBT::minNode(node* x)
{
    //parameter x = starting point need to find min
    while(!isNil(x->leftChild))
        x = x->leftChild;

    return x;
}

node* RBT::maxNode(node* x)
{
    //parameter x = starting point need to find max
    while(!isNil(x->rightChild))
        x = x->rightChild;

    return x;
}

node* RBT::searchValue(int val)
{
    node* x = start->rightChild;

    while(!isEmptyTree(x))
    {
        if(x->value == val)
            return x;

        x = val < x->value ? x->leftChild : x->rightChild;

    }
    
    // If val root node return address of the root, no value equal to val (return null)
    return x->value == val ? x : NULL;
}

bool RBT::deleteValue(int val)
{
    if(node* z = searchValue(val))
    {
        node* y = z;
        node* x;

        colors yColor = y->color;
        // z (node going to delete)

        //z has only right child
        if(isNil(z->leftChild))
        {
            x = z->rightChild;
            TRANSPLANT_RBT(z,z->rightChild);
        }

        //z has only left child
        else if(isNil(z->rightChild))
        {
            x = z->leftChild;
            TRANSPLANT_RBT(z,z->leftChild);
        }

        //z has both child
        else
        {
            y = minNode(z->rightChild);
            yColor = y->color;
            x = y->rightChild;

            if(y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                TRANSPLANT_RBT(y,y->rightChild);
                y->rightChild = z->rightChild;
                y->rightChild->parent = y;
            }

            TRANSPLANT_RBT(z,y);
            y->leftChild = z->leftChild;
            y->leftChild->parent = y;
            y->color = z->color;

        }

        if(yColor == BLACK)
            DELETE_FIXUP(x);

        return true;
    }

    //If given value(val) not in the tree, return false
    return false;
}

void RBT::TRANSPLANT_RBT(node* u, node* v)
{
    // U and V are nodes that need to exchange

    //If u is root node
    if(isNil(u->parent))
        start->rightChild = v;

    else if (u == u->parent->leftChild)
        u->parent->leftChild = v;
    
    else
        u->parent->rightChild = v;

    if(!isNil(v))
        v->parent = u->parent;
        
}

void RBT::DELETE_FIXUP(node* x)
{
    while( x != start->rightChild && x->color ==  BLACK && !isNil(x))
    {
        node* sibling;

        // If x is left child of the parent
        if(x == x->parent->leftChild)
        {
            sibling = x->parent->rightChild;

            if(sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                LEFT_ROTATION(x->parent);
                sibling = x->parent->rightChild;
            }
            colors cl1 = sibling->leftChild->color, cl2 = sibling->rightChild->color;
            if(sibling->leftChild->color ==  BLACK && sibling->rightChild->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }

            else if(sibling->rightChild->color == BLACK)
            {
                sibling->leftChild->color = BLACK;
                sibling->color = RED;
                RIGHT_ROTATION(sibling);
                sibling = x->parent->rightChild;
            }

            sibling->color = x->parent->color;
            x->parent->color = BLACK;
            sibling->rightChild->color = BLACK;
            LEFT_ROTATION(x->parent);
            x = start->rightChild;
        }
        
        // x is right child
        else
        {
            sibling = x->parent->leftChild;

            if(sibling->color == RED)
            {
                sibling->color = BLACK;
                x->parent->color = RED;
                RIGHT_ROTATION(x->parent);
                sibling = x->parent->leftChild;
            }

            if(sibling->rightChild->color ==  BLACK && sibling->leftChild->color == BLACK)
            {
                sibling->color = RED;
                x = x->parent;
            }

            else if(sibling->leftChild->color == BLACK)
            {
                sibling->rightChild->color = BLACK;
                sibling->color = RED;
                LEFT_ROTATION(sibling);
                sibling = x->parent->leftChild;
            }

            sibling->color = x->parent->color;
            x->parent->color = BLACK;
            sibling->leftChild->color = BLACK;
            RIGHT_ROTATION(x->parent);
            x = start->leftChild;
        }
    }

    x->color = BLACK;
}
