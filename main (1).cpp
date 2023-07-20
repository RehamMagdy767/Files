#include <iostream>
#include<fstream>
#include<algorithm>
#include<string>
#include <cstdlib>
#include <vector>
#include<bits/stdc++.h>

using namespace std;
class data
{
public:
    int key;
    int address;
    data ()
    {
        key = 0;
        address = 0;
    }
};
class Node
{
public:
    data * keys;
    int leaf;

    Node ()
    {
        leaf = 0;
        keys = new data[5];
    }
};
Node new_node (fstream & indexfile, int order, int NumberOfRecords)	//return Node
{
    int order1 = order;
    Node n;
    indexfile.seekp (0, ios::end);
    int RRN = indexfile.tellp () / sizeof (n);
    n.leaf = -1;
    for (int i = 0; i < order1; i++)	// initialize the keys in the array
    {
        n.keys[i].key = -1;

        n.keys[i].address = -1;
    }
    if (RRN < NumberOfRecords - 1 )

    {
        n.keys[0].key = RRN + 1;



        return n;
    }
    else
    {
        return n;
    }
}
void print_node_keys (Node node, int order)
{
    cout << "[";
    cout << node.leaf << "   ";
    for (std::size_t i = 0; i < order; i++)

    {
        cout << "|" << node.keys[i].key << "," << node.keys[i].
             address << "|" << "   ";
    };

    cout << "] " << endl;
}
void CreateIndexFileFile (char * FileName, int NumberOfRecords, int m)
{
    fstream Btreefile;
    Btreefile.open (FileName, ios::out);

    for (int i = 0; i < NumberOfRecords; i++)

    {
        Node node = new_node (Btreefile, m, NumberOfRecords);
        Btreefile.write ((char *) &node, sizeof node);

    }
    Btreefile.close ();
}
void DisplayIndexFileContent (char * Filename)
{
    int order = 5;
    Node readnode;
    ifstream file;
    file.open (Filename, ios::in);
    while(file.good())
    {


        file.read ((char *) &readnode, sizeof (Node));
        print_node_keys(readnode,order);
    }
    file.close ();
}
bool compareByLength( data &a,  data &b)
{
    return a.key < b.key;
}

int findParent(fstream& file,Node node,int nodeRRN)
{
    Node parent;

    for(int g=0; g<10; g++)
    {
        file.seekg(g*sizeof(Node),ios::beg);
        file.read((char*)&parent,sizeof(Node));
        for(int i=0; i<5; i++)
        {
            if(  parent.keys[i].address==nodeRRN)
            {

                return g;
            }
        }
    }
}
int split(string FileName,vector<data> vect,int splitnodeRRN,data newnode,bool child) //child 1 -> child //it return RRN
// child = 0 -> is Not child
{

    bool spliting=false; //check if this vector has been splitting before Or Not
    for(int i=0; i<vect.size(); i++)
    {
        if(vect.at(i).address<10)
            spliting=true;
    }
    int searchkey=newnode.key;
    int index_of_node=0;
    vector<data>vector_keys;
    int firstchild_RRN,secChild_RRN,parentRRN;
    data Max_vec1, Max_vec2;
    Node header,splitnode,firstchild,secChiled,parent;
    fstream file;
    file.open (FileName.c_str (), ios::out|ios::in);
    //** Load 4 Nodes From Fill By Seeking To Their Position then Read Them **//

    file.seekg(0,ios::beg);
    file.read((char*)&header,sizeof(Node));

    file.seekg(splitnodeRRN*(sizeof(Node)),ios::beg);
    file.read((char*)&splitnode,sizeof(Node));



    /** Split Vector to 2 parts **/
    std::size_t const half_size = vect.size() / 2;
    std::vector<data> vec1(vect.begin(), vect.begin() + half_size);
    std::vector<data> vec2(vect.begin() + half_size, vect.end());


    /*******************              Is CHILD        ***********************************************/
    if(child==1)//child
    {
        parentRRN=findParent(file,splitnode,splitnodeRRN);
        file.seekg(parentRRN*sizeof(Node),ios::beg);
        file.read((char*)&parent,sizeof(Node));
        secChild_RRN=splitnodeRRN;
        secChiled=splitnode;
        firstchild_RRN=header.keys[0].key;
        file.seekg(secChild_RRN*(sizeof(Node)),ios::beg);
        file.read((char*)&secChiled,sizeof(Node));
        /** Write first vector elements in first child  **/
        for(int i=0; i<vec1.size(); i++)
        {
            firstchild.keys[i].key =vec1.at (i).key ;
            firstchild.keys[i].address= vec1.at (i).address;
        }
        for(int i=3; i<5; i++)
        {
            secChiled.keys[i].key =-1;
            secChiled.keys[i].address= -1;
            firstchild.keys[i].key =-1;
            firstchild.keys[i].address= -1;
        }
        firstchild.leaf=0; //it is leaf now
        file.seekp(firstchild_RRN*(sizeof (Node)),ios::beg);
        file.write((char*)&firstchild,sizeof(Node));
        Max_vec1=*std::max_element(vec1.begin(),vec1.end(),compareByLength); //Get Max Element in vector 1
        Max_vec1.address=firstchild_RRN;
        /** Write second vector elements in sec child  **/
        for(int i=0; i<vec2.size(); i++)
        {
            secChiled.keys[i].key =vec2.at (i).key ;
            secChiled.keys[i].address= vec2.at (i).address;
        }
        secChiled.leaf=0; //it is leaf now
        file.seekp(secChild_RRN*(sizeof (Node)),ios::beg);
        file.write((char*)&secChiled,sizeof(Node));

        //update the parent
        vector_keys.clear();
        for (int i=0; i<5; i++) //load not empty pairs
        {
            if(parent.keys[i].address!=-1)
            {
                data nodeElement;
                nodeElement.key =parent.keys[i].key;
                nodeElement.address =parent.keys[i].address;
                vector_keys.push_back(nodeElement);
                std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
            }
        }
        vector_keys.push_back(Max_vec1);


        std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
        if(vector_keys.size()>5) //5 -> Order
        {







            split(FileName,vector_keys,parentRRN,newnode,false);
            return 0;
        }
        else if(vector_keys.size()<=5) //5 -> Order
        {


            for(int i=0; i<vector_keys.size(); i++)
            {
                parent.keys[i].key =vector_keys.at (i).key ;
                parent.keys[i].address= vector_keys.at (i).address;
            }

            file.seekp(parentRRN*sizeof(Node),ios::beg);
            file.write((char*)&parent,sizeof(Node));
            header.keys[0].key=firstchild_RRN+1;
            file.seekp(0*(sizeof (Node)),ios::beg);
            file.write((char*)&header,sizeof(Node));

            auto it = find_if(vec1.begin(), vec1.end(),[&searchkey](const data& obj)
            {
                return obj.key == searchkey;
            });
            if(it != vec1.end())
                index_of_node=firstchild_RRN;
            else
                index_of_node=secChild_RRN;

            return index_of_node;
        }

    }
    /*******************              NOT CHILD        ***********************************************/
    else if(child==0)
    {
        //first and sec is new nodes
        firstchild_RRN=header.keys[0].key;
        file.seekg(firstchild_RRN*(sizeof(Node)),ios::beg);
        file.read((char*)&firstchild,sizeof(Node));
        secChild_RRN=firstchild_RRN+1;
        file.seekg(secChild_RRN*(sizeof(Node)),ios::beg);
        file.read((char*)&secChiled,sizeof(Node));

        /** Write first vector elements in first child  **/
        for(int i=0; i<vec1.size(); i++)
        {
            firstchild.keys[i].key =vec1.at (i).key ;
            firstchild.keys[i].address= vec1.at (i).address;
        }
        if(spliting)
        {
            firstchild.leaf=1; //it is Not Leaf
        }
        else
        {
            firstchild.leaf=0; //it is leaf now
        }
        file.seekp(firstchild_RRN*(sizeof (Node)),ios::beg);
        file.write((char*)&firstchild,sizeof(Node));
        Max_vec1=*std::max_element(vec1.begin(),vec1.end(),compareByLength); //Get Max Element in vector 1
        Max_vec1.address=firstchild_RRN;

        /** Write secound vector elements in sec child  **/
        for(int i=0; i<vec2.size(); i++)
        {
            secChiled.keys[i].key =vec2.at (i).key ;
            secChiled.keys[i].address= vec2.at (i).address;
        }
        if(spliting)
        {
            secChiled.leaf=1; //it is Not Leaf
        }
        else
        {
            secChiled.leaf=0; //it is leaf now
        }

        file.seekp(secChild_RRN*(sizeof (Node)),ios::beg);
        file.write((char*)&secChiled,sizeof(Node));
        Max_vec2=*std::max_element(vec2.begin(),vec2.end(),compareByLength); //Get Max Element in vector 2
        Max_vec2.address=secChild_RRN;

        splitnode.keys[0].key=Max_vec1.key;
        splitnode.keys[0].address=Max_vec1.address;
        splitnode.keys[1].key=Max_vec2.key;
        splitnode.keys[1].address=Max_vec2.address;
        for(int i=2; i<5; i++) //Update other pairs To be -1
        {
            splitnode.keys[i].key=-1;
            splitnode.keys[i].address=-1;
        }
        splitnode.leaf=1; //it is NOT leaf Now because it has index of firstChild , SecChild
        file.seekp(splitnodeRRN*(sizeof (Node)),ios::beg);
        file.write((char*)&splitnode,sizeof(Node));//write split node in file

        header.keys[0].key=secChild_RRN+1;
        file.seekp(0*(sizeof (Node)),ios::beg);
        file.write((char*)&header,sizeof(Node));
        file.close();
        auto it = find_if(vec1.begin(), vec1.end(), [&searchkey](const data& obj)
        {
            return obj.key == searchkey;
        });
        if(it != vec1.end())
        {
            return(firstchild_RRN);
        }
        else
            return(secChild_RRN);

    }

}

int InsertNewRecordAtIndex (char * FileName, int Key, int Address)
{
    vector<data> vector_keys; //Vector to load node's keys in it
    int numberofelements=0,order=5,splitRRN;
    int arr_RRN[10]= {1,2,3,4,5,6,7,8,9,10}; //store my paths in it
    int indexNode=0;
    data newelemnt;
    newelemnt.key=Key;
    newelemnt.address=Address;

    Node header,nextempty,root,current; //Header -> Header Node , inNode -> Node where we will insert our new key;
    fstream file;
    file.open (FileName, ios::in|ios::out);
    file.seekg(0,ios::beg); // Read Pointer at the beg of file
    file.read((char*)&header,sizeof(Node)); //load the header node to Know RRN
    int RRN=header.keys[0].key; //Get the next free node
    file.seekg(RRN*(sizeof (Node)),ios::beg); //Read pointer go to the record which header point to it
    file.read((char*)&nextempty,sizeof(Node));
    /* get root node */
    file.seekg(1*(sizeof (Node)),ios::beg);
    file.read((char*)&root,sizeof(Node));

    if(root.leaf==-1) //base case
    {

        RRN=root.keys[0].key; //Get RRN Which Root Point to
        header.keys[0].key=RRN;
        root.keys[0].key=newelemnt.key;
        root.keys[0].address=newelemnt.address;
        root.leaf=0;
        file.seekp(1*(sizeof (Node)),ios::beg);
        file.write((char*)&root,sizeof(Node));

        file.seekp(0,ios::beg);
        file.write((char*)&header,sizeof(Node));

        file.close();
        return (header.keys[0].key-1) ;
    }

    else if(root.leaf==0)
    {
        for (int i=0; i<order; i++) //load not empty pairs
        {
            if(root.keys[i].address!=-1)
            {
                data nodeElement;
                nodeElement.key =root.keys[i].key;
                nodeElement.address =root.keys[i].address;
                vector_keys.push_back(nodeElement);
                std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
            }
        }
        vector_keys.push_back(newelemnt); //add new element to vector
        std::sort(vector_keys.begin(), vector_keys.end(), compareByLength); //sorting all elements on vector

        numberofelements=vector_keys.size(); //get the current number of elements that vector hold

        if(numberofelements>order)
        {
            ///we should apply the split here
            indexNode=split(FileName,vector_keys,1,newelemnt,false);

            file.close();

            return indexNode; //not correct yet,, Waiting to use search here
        }
        else  if (numberofelements<=order)
        {
            for(int i=0; i<vector_keys.size(); i++)
            {
                root.keys[i].key =vector_keys.at (i).key ;
                root.keys[i].address= vector_keys.at (i).address;
            }
            root.leaf=0; //it is leaf now
            file.seekp(1*(sizeof (Node)),ios::beg);
            file.write((char*)&root,sizeof(Node));
            file.seekp(0,ios::beg);
            if(numberofelements==order)
            {
                header.keys[0].key=RRN+1;
            }
            header.keys[0].key=RRN;
            file.write((char*)&header,sizeof(Node));
            file.close();
            return RRN-1;
        }

    }
    else if(root.leaf==1)
    {
        for(int i=0; i<5; i++)
        {

            if(root.keys[i].key>=newelemnt.key)
            {
                arr_RRN[i]=root.keys[i].address;//RRN=2
                file.seekg(arr_RRN[i]*sizeof(Node),ios::beg);
                file.read((char*)&current,sizeof(Node));

                for (int i=0; i<order; i++) //load not empty pairs
                {
                    if(current.keys[i].address!=-1)
                    {
                        data nodeElement;
                        nodeElement.key =current.keys[i].key;
                        nodeElement.address =current.keys[i].address;
                        vector_keys.push_back(nodeElement);
                        std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
                    }
                }
                vector_keys.push_back(newelemnt); //add new element to vector
                std::sort(vector_keys.begin(), vector_keys.end(), compareByLength); //sorting all elements on vector

                numberofelements=vector_keys.size(); //get the current number of elements that vector hold

                if(numberofelements>order)
                {
                    ///we should apply the split here

                    indexNode=split(FileName,vector_keys,arr_RRN[i],newelemnt,true); //index node hold the RRN where the new element added

                    file.close();
                    return indexNode; ///not correct yet,, Waiting to use search here
                }
                else  if (numberofelements<=order)
                {
                    for(int i=0; i<vector_keys.size(); i++)
                    {
                        current.keys[i].key =vector_keys.at (i).key ;
                        current.keys[i].address= vector_keys.at (i).address;
                    }

                    file.seekp(arr_RRN[i]*(sizeof (Node)),ios::beg);
                    file.write((char*)&current,sizeof(Node));
                    file.seekp(0,ios::beg);
                    if(numberofelements==order)
                    {
                        header.keys[0].key=RRN+1;
                    }
                    header.keys[0].key=RRN;
                    file.write((char*)&header,sizeof(Node));
                    file.close();
                    return arr_RRN[i];
                }


            }
        }
        for(int i=0; i<5; i++) //get Root's element to know which is the last element
        {
            if(root.keys[i].address!=-1) //load all non empty keys on root
            {
                data nodeElement;
                nodeElement.key =root.keys[i].key;
                nodeElement.address =root.keys[i].address;
                vector_keys.push_back(nodeElement);
                std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
            }
        }
        data last_Element;
        last_Element.key=vector_keys.back().key; // hold the last one
        last_Element.address=vector_keys.back().address;
        vector_keys.clear(); //delete all elements to hold new elements

        file.seekg(last_Element.address*sizeof(Node),ios::beg); //seeking to this given address
        file.read((char*)&current,sizeof(Node)); //Read the node which i will insert into
        for(int i=0; i<5; i++)
        {
            if(current.keys[i].address!=-1)
            {
                data nodeElement;
                nodeElement.key =current.keys[i].key;
                nodeElement.address =current.keys[i].address;
                vector_keys.push_back(nodeElement);
                std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
            }

        }
        vector_keys.push_back(newelemnt);
        std::sort(vector_keys.begin(), vector_keys.end(), compareByLength);
        numberofelements=vector_keys.size(); //get the current number of elements that vector hold
        if(numberofelements>order)
        {
            ///we should apply the split here
            indexNode=split(FileName,vector_keys,last_Element.address,newelemnt,true); //index node hold the RRN where the new element added
            Node parent;
            int parent_RRN;
            parent_RRN=findParent(file,current,last_Element.address);
            file.seekg(parent_RRN*sizeof(Node),ios::beg);
            file.read((char*)&parent,sizeof(Node));
            for(int i=0; i<5; i++)
            {
                if( parent.keys[i].address==last_Element.address
                        && parent.keys[i].key<newelemnt.key )
                {
                    parent.keys[i].key=newelemnt.key;
                    break;
                }
            }
            file.close();
            return indexNode; ///not correct yet,, Waiting to use search here
        }
        else  if (numberofelements<=order) //there is place
        {
            for(int i=0; i<vector_keys.size(); i++)
            {
                current.keys[i].key =vector_keys.at (i).key ;
                current.keys[i].address= vector_keys.at (i).address;
            }
            current.leaf=0;//it is leaf now
            file.seekp(last_Element.address*sizeof(Node),ios::beg);
            file.write((char*)&current,sizeof(Node));

            Node parent;
            int parent_RRN;
            parent_RRN=findParent(file,current,last_Element.address);
            file.seekg(parent_RRN*sizeof(Node),ios::beg);
            file.read((char*)&parent,sizeof(Node));
            for(int i=0; i<5; i++)
            {
                if( parent.keys[i].address==last_Element.address
                        && parent.keys[i].key<newelemnt.key )
                {
                    parent.keys[i].key=newelemnt.key;
                    break;
                }
            }
            file.seekp(parent_RRN*sizeof(Node),ios::beg);
            file.write((char*)&parent,sizeof(Node));
            file.seekp(0,ios::beg);
            if(numberofelements==order)
            {
                header.keys[0].key=RRN+1;
            }
            header.keys[0].key=RRN;
            file.write((char*)&header,sizeof(Node));
            file.close();
            return last_Element.address;
        }
    }
}


vector <int> v ;
vector <int> address;
void search (Node node, int order)
{
    for (std::size_t i = 0; i < order; i++)
    {
        if(node.keys[i].key !=-1 && node.keys[i].address!=-1  )
        {
            v.push_back(node.keys[i].key);
            address.push_back(node.keys[i].address);
        }
    };
}
int SearchARecord2 (string Filename, int RecordID)
{
    int order = 5;
    string flag;
    Node readnode;
    ifstream file;
    file.open (Filename.c_str (), ios::in);
    while(file.good())
    {
        search(readnode,order);
        file.read ((char *) &readnode, sizeof (Node));
    }
    cout<<endl;
    auto it = find(v.begin(), v.end(), RecordID);
    if (it != v.end())
    {
        int index = it - v.begin();
        int x = address.at(index);
        return x;
    }
    else
    {
        return -1;
    }

    file.close ();
}
int searchHelpler(fstream &file,Node node,int key)
{
    Node current;
    int nextRRN;

    if(node.leaf==0) //only root is in file
    {
        for(int i=0; i<5; i++)
        {

            if(node.keys[i].key==key)
                return node.keys[i].address;
            else
                break;
        }

    }
    else if(node.leaf==1)
    {
        for(int i=0; i<5; i++) //go through root elements
        {
            nextRRN=node.keys[i].address;
            file.seekg(nextRRN*sizeof(Node),ios::beg); // Read Pointer at root node
            file.read((char*)&current,sizeof(Node));
            if(current.leaf==0)
            {
                for(int i=0; i<5; i++)
                {
                    if(current.keys[i].key==key)
                    {
                        file.close();
                        return current.keys[i].address;
                    }

                }
            }
            else if(current.leaf==1)
            {
                searchHelpler(file,current,key);
            }
        }
        file.close();
        return -1;
    }

}
int SearchaRecord3 (string filename, int RecordID)
{
    Node root,current;
    int nextRRN;
    fstream file;
    file.open(filename.c_str(),ios::out|ios::in);
    file.seekg(1*sizeof(Node),ios::beg); // Read Pointer at root node
    file.read((char*)&root,sizeof(Node)); //load the root
    if(root.leaf==-1)
    {
        file.close();
        return 0;
    }
    else if(root.leaf==0) //only root is in file
    {
        for(int i=0; i<5; i++)
        {
            if(root.keys[i].key==RecordID)
                return root.keys[i].address;
        }
    }
    else if(root.leaf==1)
    {
        return searchHelpler(file,root,RecordID);
    }

}

int SearchARecord (char * Filename, int RecordID)
{
    fstream file;
    file.open (Filename, ios::in | ios::out);
    file.seekg (0, ios::beg);
    Node node,root;
    data element;
    int flag ;
    file.seekg(1*(sizeof (Node)),ios::beg);
    file.read((char*)&root,sizeof(Node));

    if(root.leaf==-1)
    {
        return 0;
    }
    if(root.leaf==1)
    {
        for (int g = 2; g < 10; g++)
        {
            file.seekg(g*sizeof(Node),ios::beg);
            file.read ((char *) &node, sizeof (Node));

            for (int i = 0; i < 5; i++)
            {
                element.key = node.keys[i].key;
                element.address = node.keys[i].address;
                if (element.address != -1)
                {
                    if (node.keys[i].key == RecordID)
                    {
                        cout << "records\nnode key is : " << node.keys[i].key <<" and it's address is : " <<node.keys[i].address << endl;
                        //flag =node.keys[i].address;
                        return node.keys[i].address;
                    }
                }
            }
        }

    }
    if(root.leaf == 0)
    {
       file.seekg(1*sizeof(Node),ios::beg);
            file.read ((char *) &node, sizeof (Node));

            for (int i = 0; i < 5; i++)
            {
                element.key = node.keys[i].key;
                element.address = node.keys[i].address;
                if (element.address != -1)
                {
                    if (node.keys[i].key == RecordID)
                    {
                        cout << "node key is : " << node.keys[i].key <<" and it's address is : " <<node.keys[i].address << endl;
                        //flag =node.keys[i].address;
                        return node.keys[i].address;
                    }
                }
            }
    }

    return -1;

}

int DeleteRecordFromIndex (char *FileName, int RecordID)
{
    fstream file;
    file.open (FileName, ios::in | ios::out);
    file.seekg (0, ios::beg);
    Node node;
    data element;
    vector <data> deleteElement ;
    vector <int> TheKeys;
    int RRN;
    for (int g = 0; g < 10; g++)
    {
        file.seekg (g * sizeof (Node), ios::beg);
        file.read ((char *) &node, sizeof (Node));
        for (int i = 0; i < 5; i++)
        {
            element.key = node.keys[i].key;
            element.address = node.keys[i].address;
            if (node.keys[i].key == RecordID)
            {
                for(int i =0 ; i<5 ; i++)
                {
                    data nodeElement;
                    nodeElement.key =node.keys[i].key;
                    nodeElement.address =node.keys[i].address;
                    if(nodeElement.address !=-1)
                    {
                        deleteElement.push_back(nodeElement);
                        TheKeys.push_back(nodeElement.key);
                    }
                }
                auto it = find(TheKeys.begin(), TheKeys.end(), RecordID);
                int index;
                if (it != TheKeys.end())
                {
                    index = it - TheKeys.begin();
                }
                int currentposition=file.tellp();
                deleteElement.erase(deleteElement.begin() + index);
                std::sort(deleteElement.begin(), deleteElement.end(), compareByLength);
                int VecSize = deleteElement.size();
                for(int i=0; i<5; i++)
                {
                    if(VecSize !=0)
                    {
                        node.keys[i].key =deleteElement.at (i).key ;
                        node.keys[i].address= deleteElement.at (i).address;
                        VecSize --;
                    }
                    else
                    {
                        node.keys[i].key = -1;
                        node.keys[i].address = -1;
                    }
                }
                file.seekp(RRN*(sizeof (Node)),ios::beg);
                file.write((char*)&node,sizeof(Node));

                return element.address;
            }
        }
        RRN++;
    }
    return -1;
}

int main ()
{
  char * filename = "btree.txt";

    CreateIndexFileFile (filename, 10,5);

    data newelement;
    int key;

    int choice;
    do
    {
        cout<<endl;

        cout<<"1- Insert"<<endl;
        cout<<"2- Display All Nodes"<<endl;
        cout<<"3- Search "<<endl;
        cout<<"4- Delete "<<endl;
        cout<<"5- Exit "<<endl;

        cin>>choice;
        switch(choice)
        {
        case 1:
            cout<<"Enter Key : ";
            cin>>newelement.key;
            cout<<"Enter Address : ";
            cin>>newelement.address;
            cout<<"Your Key With Value : "<<newelement.key<<"  Is Inserted In RRN = "<<InsertNewRecordAtIndex(filename,newelement.key,newelement.address)<<endl;
            break;
        case 2:
            cout<<" *********************************** "<<endl;
            DisplayIndexFileContent (filename);
            break;
        case 3:
        {
            cout<<"Enter The Key : ";
            cin>>key;
            int Search =SearchARecord(filename,key);
            if(Search == -1)
            {
                cout<<Search << endl <<"This key is not found"<<endl;
            }
            else if(Search==0)
            {
                cout<<"The File Is Empty .. "<<endl;
            }

            break;

        }
        case 4:
        {
            cout<<"Enter The Key To Be Deleted : ";
            cin>>key;            int del = DeleteRecordFromIndex(filename,key);
            if(del == -1)
            {
                cout <<del << endl;
                cout<<"This key is not found \n";
            }
            else
            {
                cout << "\nthe key " << key << " at address = "<< del <<" deleted successfully "<<endl;
            }
            break;
        }
        default :
            exit(0);
        }
    }
    while(choice=5);
    return 0;

}



