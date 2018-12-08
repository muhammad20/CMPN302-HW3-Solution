#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

class StopWatch
{
  private:
    int millis;

  public:
    void start()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        millis = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    }

    int get_elapsed_ms()
    {
        timeval tp;
        gettimeofday(&tp, nullptr);
        return tp.tv_sec * 1000 + tp.tv_usec / 1000 - millis;
    }
};

vector<string> splitString(string stringToSplit, char delimiter)
{
    vector<string> splittedString;
    stringstream ss(stringToSplit);
    string item;
    while (getline(ss, item, delimiter))
    {
        splittedString.push_back(item);
    }
    return splittedString;
}

class FileHandler
{
  public:
    /// array for binary tree in preorder traversal with nulls marked as -1
    /// returns a pointer to 4 arrays:
    /// 1- first binary tree preorder traversal
    /// 2- length of the first array
    /// 3- second binary tree preorder traversal
    /// 4- length of the second array
    int **readArrayFromFile(string filename)
    {
        ifstream infile(filename, ios::in);
        string line;

        //first get the length of the first binary tree
        infile >> line;
        int bt1_length = stoi(line);
        bt1_length;

        //initialize the array, then populate it
        int *bt1 = new int[bt1_length];
        string num;
        int i;
        for (i = 0; i < bt1_length; i++)
        {
            infile >> num;
            bt1[i] = stoi(num);
        }

        //get the length of the second binary tree
        infile >> line;
        int bt2_length = stoi(line);
        bt2_length;
        int *bt2 = new int[bt2_length];
        for (i = 0; i < bt2_length; i++)
        {
            infile >> num;
            bt2[i] = stoi(num);
        }

        int **data = new int *[4];
        data[0] = bt1;
        data[1] = &bt1_length;
        data[2] = bt2;
        data[3] = &bt2_length;

        return data;
    }

    void write_running_time_to_file(string filename, int running_time)
    {
        ofstream outfile(filename, ios::out);
        outfile << "running time in ms: " << running_time;
        outfile.close();
    }

    void write_commonNodes_to_file(string filename, vector<int> commonNodes)
    {
        ofstream outfile(filename, ios::out);
        outfile << commonNodes.size() << endl;
        for (int i = 0; i < commonNodes.size(); i++)
        {
            outfile << commonNodes[i] << " ";
        }
    }
};

class Node
{
    friend class BST;

  private:
    int data;
    Node *left;
    Node *right;

  public:
    Node(int data)
    {
        left = nullptr;
        right = nullptr;
        this->data = data;
    }
};

class BST
{
  private:
    Node *root;
    Node *insertNode(Node *root, int data)
    {
        if (!root)
        {
            return new Node(data);
        }
        if (data > root->data)
        {
            root->right = insertNode(root->right, data);
        }
        else
        {
            root->left = insertNode(root->left, data);
        }
    }

    void inOrderTraversal(Node *root)
    {
        if (!root)
        {
            return;
        }
        inOrderTraversal(root->left);
        cout << root->data << " ";
        inOrderTraversal(root->right);
    }

  public:
    void insertNode(int data)
    {
        root = insertNode(root, data);
    }

    void inOrderTraversal()
    {
        inOrderTraversal(root);
    }

    void initializeTreeFromArray(int *arr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i] != -1)
            {
                root = insertNode(root, arr[i]);
            }
        }
    }

    //get common nodes between the calling tree and the tree passed as a paramter to this function
    vector<int> getCommonNodes(BST *tree)
    {
        vector<int> commonNodes;
        if (tree->root == nullptr || this->root == nullptr)
        {
            return commonNodes;
        }
        stack<Node *> t1_stack, t2_stack;
        int data1, data2;
        Node *root1 = this->root;
        Node *root2 = tree->root;
        while (true)
        {
            while (root1)
            {
                t1_stack.push(root1);
                root1 = root1->left;
            }

            while (root2)
            {
                t2_stack.push(root2);
                root2 = root2->left;
            }

            //break if either of the stacks is empty (reached the root of the smaller tree)
            if (t1_stack.empty() || t2_stack.empty())
                break;

            data1 = t1_stack.top()->data;
            data2 = t2_stack.top()->data;

            if (data1 == data2)
            {
                commonNodes.push_back(data1);
                root1 = t1_stack.top()->right;
                root2 = t2_stack.top()->right;
                t1_stack.pop();
                t2_stack.pop();
            }
            else if (data1 < data2)
            {
                root1 = t1_stack.top()->right;
                t1_stack.pop();
            }
            else
            {
                root2 = t2_stack.top()->right;
                t2_stack.pop();
            }
        }
        return commonNodes;
    }
};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        cout << "error! invalid arguments" << endl;
        exit(1);
    }

    string infile = argv[1];
    string outfile = argv[2];
    string runningTimeFile = argv[3];

    FileHandler *fileHandler = new FileHandler();
    StopWatch *stopWatch = new StopWatch();
    int **data = fileHandler->readArrayFromFile(infile);

    int bst1_length = *data[1];
    int bst2_length = *data[3];

    stopWatch->start();
    BST *tree = new BST();
    BST *tree2 = new BST();
    tree->initializeTreeFromArray(data[2], bst2_length);
    tree2->initializeTreeFromArray(data[0], bst1_length);

    vector<int> commonNodes = tree->getCommonNodes(tree2);
    int running_time = stopWatch->get_elapsed_ms();
    fileHandler->write_commonNodes_to_file(outfile, commonNodes);
    fileHandler->write_running_time_to_file(runningTimeFile, running_time);
    delete fileHandler;
    delete stopWatch;
    /// ------------ for testing and debugging
    // int **data = fileHandler->readArrayFromFile("/home/engineer/Desktop/CCE-C/Senior-1/Fall18/CMPN302/Assignments/hw_3/p1/" + infile);
    // int bst1_length = *data[1];
    // int bst2_length = *data[3];
    // for (int i = 0; i < bst1_length; i++)
    // {
    //     cout << data[0][i] << " ";
    // }
    // cout << endl;
    // // for (int i = 0; i < bst2_length; i++)
    // // {
    // //     cout << data[2][i] << " ";
    // // }
    // cout << endl
    //      << endl;
    // BST *tree = new BST();
    // BST *tree2 = new BST();
    // tree->initializeTreeFromArray(data[2], bst2_length);
    // tree2->initializeTreeFromArray(data[0], bst1_length);

    // vector<int> commonNodes = tree2->getCommonNodes(tree);
    //     fileHandler->write_commonNodes_to_file(outfile, commonNodes);

    return 0;
}
