
#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

#include <fstream>
fstream tree;
int m, NumberOfRecords;
class node
{
public :
    char key[5];
    char address[5];
    node()
    {
        strncpy(key, "-1", sizeof(key) - 1);
        strncpy(address, "-1", sizeof(address) - 1);
    }
    node (int key ,int address)

    {
        setKey(key);
        setAddress(address);
    }
    void setKey(int v)
    {
        strncpy(this->key, to_string(v).c_str(), sizeof(key) - 1);
    }

    void setAddress(int p)
    {
        strncpy(this->address, to_string(p).c_str(), sizeof(address) - 1);
    }
    int getAddress()
    {
        return atoi(address);
    }
    int getkey()
    {
        return atoi(key);
    }


};

class block {
public:
    char leaf[5];
    node keys[5];
    block()
    {
        strncpy(leaf, "-1", sizeof(leaf) - 1);
    }
    void setleaf(int leaf)
    {
        strncpy(this->leaf, to_string(leaf).c_str(), sizeof(leaf) - 1);
    }
    int getleaf()
    {
        return atoi(leaf);
    }
};

bool sortn(node &n, node &n1) {
    return n.getkey() < n1.getkey();
}

void DisplayIndexFile(string FileName, int NumberOfRecords, int m) {
    tree.close();
    tree.open(FileName, ios::in);
    block b;
    int rrn = 0;
    while (NumberOfRecords--)
    {
        tree.seekg(rrn * sizeof b, ios::beg);
        tree.read((char *) &b, sizeof b);
        cout << b.getleaf() << " ";
        for (int i = 0; i < m; i++) {
            cout << b.keys[i].getkey() << " " << b.keys[i].getAddress()<< " ";
        }
        cout << endl;
        rrn++;
    }
    tree.close();
}

void CreateIndexFile(string FileName, int NumberOfRecords, int m)
{
    tree.open(FileName, ios::out);
    tree.seekg(0, ios::end);
    int pos=tree.tellg() ;
    if(tree.tellg()<=0)
    {
        for (int i = 0; i < NumberOfRecords; i++)
        {
            block b;
            b.setleaf(-1) ;

            for (int j = 0; j < m; j++)
            {
                if (j == 0 && i < NumberOfRecords - 1)
                {
                    b.keys[j].setKey(i+1) ;
                }
                else
                {
                    b.keys[j].setKey(-1) ;
                }
                b.keys[j].setAddress(-1) ;
            }
            tree.seekg(i* sizeof b, ios::beg);
            tree.write((char *) &b, sizeof b);
            //print_block(FileName,  m) ;
        }
        tree.close();
    }


}
int  num_nodes(int rrn)
 {
    vector<node>vector ;
     block b ;
    tree.seekg(rrn*sizeof b,ios::beg) ;
    tree.read((char *)&b,sizeof b) ;
     for (int i = 0; i < m; i++)
     {
         if (b.keys[i].getkey() == -1)
         {
             break;
         }
         vector.push_back(b.keys[i]);
     }

     if (vector.size()<m)
     {
         return 1;
     }
     else
     {

         return 0;
     }
 }
int check(block Root, int RecordId, int rrn)
{
    if (Root.getleaf()== 1)
    {
        int rrn_parent=rrn ;
        for (int i = 0; i < m; i++)
        {

            if (RecordId < Root.keys[i].getkey())
            {


                block next_block;
                rrn = Root.keys[i].getAddress();
                tree.seekg(Root.keys[i].getAddress() * sizeof(next_block), ios::beg);
                tree.read((char*)&next_block, sizeof(next_block));
                if(num_nodes(rrn_parent)==1)
                {
                    Root.keys[i].setKey(RecordId);
                    tree.seekg(rrn* sizeof(Root), ios::beg);
                    tree.write((char*)&Root, sizeof(Root));
                }
                check(next_block, RecordId, rrn);
                break;
            }
            else
            {
                if (i == Root.keys[i].getkey()!= -1 && Root.keys[i + 1].getkey() == -1)
                {


                    rrn = Root.keys[i].getAddress();
                    //Root.keys[i].setAddress(rrn) ;
                    block next_block;
                    tree.seekg(Root.keys[i].getAddress() * sizeof(next_block), ios::beg);
                    tree.read((char*)&next_block, sizeof(next_block));
                    //طلما وصلنا لنقطة دى معناها اننا لازم نغير ال root

                    if(num_nodes(rrn)==1)
                    {
                        Root.keys[i].setKey(RecordId);
                        tree.seekg(rrn_parent* sizeof(Root), ios::beg);
                        tree.write((char*)&Root, sizeof(Root));
                    }

                    check(next_block, RecordId, rrn);

                    break;
                }
            }
        }
    }
    else  //كده وصلنا اننا في الريكورد الى هنقرا منه
    {

        return rrn;
    }
}
int  splitroot(string FileName, vector<node> v, int root_RRN, int n) {

    tree.open(FileName, ios::in | ios::out);
    block header;
    tree.seekg(0, ios::beg);
    tree.read((char*)&header, sizeof(header));
    int empty_record = header.keys[0].getkey();
    int end = 0;
    if(empty_record==-1)
    {
        tree.close() ;
        return -1 ;
    }
    block firstchild;
    block secondchild;
    tree.seekg(empty_record * sizeof(firstchild), ios::beg);
    tree.read((char*)&firstchild, sizeof(firstchild));

    for (int i = 0; i < (v.size()) / 2; i++) {
        firstchild.keys[i].setKey(v[i].getkey());
        firstchild.keys[i].setAddress(v[i].getAddress());
    }
    for (int i = v.size() / 2; i < m; i++) {
        firstchild.keys[i].setKey(-1);
        firstchild.keys[i].setAddress(-1);
    }


    //نبدا فى ثاني نقطه
    tree.seekg((empty_record + 1) * sizeof(secondchild), ios::beg);
    tree.read((char*)&secondchild, sizeof secondchild);
    header.keys[0].setKey(secondchild.keys[0].getkey()); //هيبقي فيه المكان الي كان بيشاور عليه ثاني نقطه
    tree.seekg(0, ios::beg);
    tree.write((char*)&header, sizeof header);
    for (int j = 0, i = (v.size() / 2); i < v.size(); i++, j++) {
        secondchild.keys[j].setKey(v[i].getkey());
        secondchild.keys[j].setAddress(v[i].getAddress());;
    }

    if (n == 0)
    {
        firstchild.setleaf(0);

        secondchild.setleaf(0);

    }
    else
    {
        firstchild.setleaf(1);
        secondchild.setleaf (1);
    }
    tree.seekg(empty_record * sizeof(firstchild), ios::beg);
    tree.write((char*)&firstchild, sizeof firstchild);
    tree.seekg((empty_record + 1) * sizeof(secondchild), ios::beg);
    tree.write((char*)&secondchild, sizeof secondchild);

    node maxv1 = *max_element(v.begin(), v.begin() + v.size() / 2, sortn);
    maxv1.setAddress(empty_record);
    node maxv2 = *max_element(v.begin() + v.size() / 2, v.end(), sortn);
    maxv2.setAddress(empty_record + 1);


    block root;
    tree.seekg(root_RRN * sizeof(root), ios::beg);
    tree.read((char *) &root, sizeof root);
    root.keys[0].setKey(maxv1.getkey());
    root.keys[0].setAddress(maxv1.getAddress());
    root.keys[1].setKey(maxv2.getkey());
    root.keys[1].setAddress(maxv2.getAddress());
    for (int i = 2; i < m; i++)
    {
        root.keys[i].setKey(-1);
        root.keys[i].setAddress(-1);
    }
    root.setleaf(1);
    tree.seekg(1 * sizeof(root), ios::beg);
    tree.write((char *) &root, sizeof root);
    tree.close();
    return 0 ;

}
int  split(string FileName, vector<node> v, int root_RRN) {
    tree.open(FileName, ios::in | ios::out);
    block header;
    tree.seekg(0, ios::beg);
    tree.read((char *) &header, sizeof header);
    int available=header.keys[0].getkey() ;

    if (available == -1)
    {
        tree.close();
        return -1 ;
    }

    block firstblock;
    block secblock;

    tree.seekg(root_RRN * sizeof(firstblock), ios::beg);
    tree.read((char*)&firstblock, sizeof firstblock);
    int end;
    for (int i = 0; i < (v.size()) / 2; i++)
    {
        firstblock.keys[i].setKey(v[i].getkey());
        firstblock.keys[i].setAddress(v[i].getAddress());
        end = i;
    }
    for (int i = end + 1; i < m; i++)
    {
        firstblock.keys[i].setKey(-1);
        firstblock.keys[i].setAddress(-1);
    }
    firstblock.setleaf(0);
    tree.seekg(root_RRN * sizeof(firstblock), ios::beg);
    tree.write((char*)&firstblock, sizeof firstblock);
    node maxv1 = *max_element(v.begin(), v.begin() + v.size() / 2, sortn);
    maxv1.setAddress(root_RRN);

    tree.seekg(0, ios::beg);
    tree.read((char*)&header, sizeof firstblock);
    int sec_rrn = header.keys[0].getkey();
    tree.seekg(sec_rrn * sizeof(firstblock), ios::beg);
    tree.read((char*)&secblock, sizeof firstblock);
    header.keys[0].setKey(secblock.keys[0].getkey());
    tree.seekg(0 * sizeof(firstblock), ios::beg);
    tree.write((char*)&header, sizeof header);
    tree.seekg(sec_rrn * sizeof(firstblock), ios::beg);
    tree.read((char*)&secblock, sizeof firstblock);
    for (int j = 0, i = (v.size() / 2); i < v.size(); i++, j++)
    {
        secblock.keys[j].setKey(v[i].getkey());
        secblock.keys[j].setAddress(v[i].getAddress());
    }
    node maxv2 = *max_element(v.begin() + v.size() / 2, v.end(), sortn);
    maxv2.setAddress(sec_rrn);
    secblock.setleaf(0);
    tree.seekg(sec_rrn * sizeof(firstblock), ios::beg);
    tree.write((char*)&secblock, sizeof firstblock);
    block root;
    tree.seekg(1 * sizeof(root), ios::beg);
    tree.read((char*)&root, sizeof root);
    vector<node> curr_root;
    for (int i = 0; i < m; i++) {
        if (root.keys[i].getAddress() != root_RRN && root.keys[i].getAddress() != -1) {
            curr_root.push_back(root.keys[i]);
        }
    }
    curr_root.push_back(maxv1);
    curr_root.push_back(maxv2);
    sort(curr_root.begin(), curr_root.end(), sortn);


    if (curr_root.size() > m)
    {
        tree.close();
        if( splitroot(FileName, curr_root, 1, 1)==-1)
             return -1 ;
        else
            return 0 ;
    }
    else
    {
        for (int i = 0; i < curr_root.size(); i++)
        {
            root.keys[i].setKey(curr_root[i].getkey());
            root.keys[i].setAddress(curr_root[i].getAddress());
        }
        tree.seekg(1 * sizeof(root), ios::beg);
        tree.write((char*)&root, sizeof root);
        tree.close();
        return 0 ;
    }


}

 void insert(string FileName, int RecordId, int reference)        //المطلوب int بس لسه معرفش هعمل بيها ايه
{

    tree.open(FileName, ios::in | ios::out);
    vector<node> current_vector;
    current_vector.clear();
    block header, Root;
    tree.seekg(0, ios::beg);
    tree.read((char *)&header, sizeof header);
    //int empty_record = header.keys[0].key;   //عرفت هروح لاني ريكررد
    tree.seekg(1 *sizeof(header), ios::beg);//واقففه عند ريكورد الفاضي
    tree.read((char *)&Root, sizeof Root);  //هقرا االريكورد

    if (Root.getleaf() == -1)
    {
        Root.setleaf(0);
        //cout << Root.keys[0].getkey()<<endl;
        header.keys[0].setKey(Root.keys[0].getkey());
        Root.keys[0].setKey(RecordId);
        Root.keys[0].setAddress(reference) ;
        for(int i=1 ;i<5 ;i++)
        {
            Root.keys[i].setKey(-1);
            Root.keys[i].setAddress(-1) ;
        }
        tree.seekg(1 *sizeof(header),ios::beg);
        tree.write((char *) &Root, sizeof Root);
        tree.seekg(0, ios::beg);
        tree.write((char *) &header, sizeof header);
        tree.close();
    }

    else if (Root.getleaf() == 0)       //كده معناهااااا انى هدخل حاجات فى root
    {
        bool flag = false;
        //بشوف هل في مكان ادخل ولا لا

        for (int i = 0; i < m; i++)
        {
            if (Root.keys[i].getkey() == -1)
            {
                Root.keys[i].setKey(RecordId);
                Root.keys[i].setAddress(reference) ;
                current_vector.push_back(Root.keys[i]);
                flag = true;
                break;
            }
            current_vector.push_back(Root.keys[i]);
        }
        // كده في مكان ادخل فيه وخلاص ضيفته في قيكتور

        if (flag == true)
        {
            sort(current_vector.begin(), current_vector.end(), sortn);
            for (int i = 0; i < current_vector.size(); i++)
            {
                Root.keys[i].setKey( current_vector[i].getkey());
                Root.keys[i].setAddress( current_vector[i].getAddress());
            }
            tree.seekg(1 * sizeof(Root), ios::beg);
            tree.write((char *) &Root, sizeof Root);
            tree.close();
        }

        else if (flag == false)            // ,لسه مضفتهاش في الفيكتور ومعناها ان مفيش مكان خالص
        {
            // محتاجة اقسمهم ل نقطتين

            node lastnode;
            lastnode.setKey(RecordId);
            lastnode.setAddress(reference);
            current_vector.push_back(lastnode);
            sort(current_vector.begin(), current_vector.end(), sortn);
            tree.close();
            splitroot(FileName, current_vector, 1, 0);                                 //محتاج الفيكتور كله علشان ده الي هقسمه

        }


    }

    else if (Root.getleaf() == 1)   //الروت كده اتقسم قبل كده
    {
        int RRN, lastindex;
        for (int i = 0; i < m; i++)
        {
            if (RecordId < Root.keys[i].getkey())
            {
                lastindex = i;
                block next_block;
                int rrn = Root.keys[i].getAddress();
                tree.seekg(rrn* sizeof(next_block), ios::beg);
                tree.read((char*)&next_block, sizeof(next_block));
                RRN = check(next_block, RecordId, rrn);
                break;
            }
            else if (Root.keys[i].getkey() != -1 && Root.keys[i + 1].getkey() == -1)
            {
                lastindex = i;
                block next_block;
                int rrn = Root.keys[i].getAddress();
                tree.seekg(rrn * sizeof(next_block), ios::beg);
                tree.read((char*)&next_block, sizeof(next_block));
                RRN = check(next_block, RecordId, rrn);

            }
        }

        block next_block;
        tree.seekg(RRN * sizeof next_block, ios::beg);
        tree.read((char*)&next_block, sizeof next_block);

        for (int i = 0; i < m; i++)
        {
            if (next_block.keys[i].getkey() != -1)
                current_vector.push_back(next_block.keys[i]);
            else
                break;
        }
        node lastnode;
        lastnode.setKey(RecordId);
        lastnode.setAddress(reference);
        current_vector.push_back(lastnode);
        sort(current_vector.begin(), current_vector.end(), sortn);
        if (current_vector.size() > m )
        {
            tree.close();
            if(split(FileName, current_vector, RRN) )
             {
                 cout<<"we dont add new node"<<endl ;

             }

        }
        else
        {
            for (int i = 0; i < current_vector.size(); i++)
            {
                next_block.keys[i].setKey(current_vector[i].getkey());
                next_block.keys[i].setAddress(current_vector[i].getAddress());
            }
            tree.seekg(RRN * sizeof next_block, ios::beg);
            tree.write((char*)&next_block, sizeof next_block);

            if (lastnode.getkey() > Root.keys[lastindex].getkey())
            {
                Root.keys[lastindex].setKey(lastnode.getkey());
                // Root.keys[lastindex].address =  ;
            }
            tree.seekg(1 * sizeof Root, ios::beg);
            tree.write((char*)&Root, sizeof Root);

            tree.close();
        }
    }





}

int DeleteARecord (string FileName, int RecordID)
{
    fstream file;
    file.open (FileName.c_str (), ios::in | ios::out);
    file.seekg (0, ios::beg);
    block n;
    node element;
    vector <node> deleteElement ;
    vector <int> TheKeys;
    int RRN;
    for (int g = 0; g < 10; g++)
    {
        file.seekg (g * sizeof (block), ios::beg);
        file.read ((char *) &n, sizeof (block));
        for (int i = 0; i < 5; i++)
        {
            element.setKey(n.keys[i].getkey())  ;
            element.setAddress(n.keys[i].getAddress())  ;
            if (n.keys[i].getkey() == RecordID)
            {
                for(int i =0 ; i<5 ; i++)
                {
                    node nodeElement;
                    nodeElement.setKey(n.keys[i].getkey());
                    nodeElement.setAddress(n.keys[i].getAddress());

                    if(nodeElement.getAddress() !=-1)
                    {
                        deleteElement.push_back(nodeElement);
                        TheKeys.push_back(nodeElement.getkey());
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
                std::sort(deleteElement.begin(), deleteElement.end(), sortn);
                int VecSize = deleteElement.size();
                for(int i=0; i<5; i++)
                {
                    if(VecSize !=0)
                    {
                        n.keys[i].setKey(deleteElement.at (i).getkey());
                        n.keys[i].setAddress(deleteElement.at (i).getAddress());
                        VecSize --;
                    }
                    else
                    {
                        n.keys[i].setKey(-1);
                        n.keys[i].setAddress(-1);

                    }
                }
                file.seekp(RRN*(sizeof (block)),ios::beg);
                file.write((char*)&n,sizeof(block));

                return element.getAddress();
            }
        }
        RRN++;
    }
    return -1;
}

int main()
{
    string FileName;

    FileName = "tree.txt";
    NumberOfRecords=10 ,m=5;

    CreateIndexFile(FileName, NumberOfRecords, m);

    insert(FileName, 3, 12);
    insert(FileName, 7, 24);

    insert(FileName, 10, 48);

    insert(FileName, 24, 60);
    insert(FileName, 19, 84);

    insert(FileName, 14, 72);

    insert(FileName, 19, 84);

    insert(FileName, 30, 96);

    insert(FileName, 15, 108);

    insert(FileName, 1, 120);

    insert(FileName, 5, 132);

    insert(FileName, 2, 144);
    insert(FileName, 8, 156);
    insert(FileName, 9, 168);

    insert(FileName, 6, 180);

    insert(FileName, 11, 192);

    insert(FileName, 12, 204);

    insert(FileName, 17, 216);
    insert(FileName, 18, 228);

    //insert(FileName, 34, 230);

    insert(FileName, 32, 240);
    //insert(FileName, 40, 24);

    DisplayIndexFile(FileName, NumberOfRecords, m);
    cout<<"deleting"<<endl;
    cout<< DeleteARecord(FileName,10);
    cout<<"after delete"<<endl;
    cout<<endl;
    cout<<endl;

    cout<<endl;


    DisplayIndexFile(FileName, NumberOfRecords, m);




    return 0;
}
