#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>

// a simple solution to the Level-Ancestor Problem is implemented
// based on the article in the link https://arxiv.org/pdf/1903.01387.pdf
// Time limit is 1 second for a test case.

using namespace std;

int root;

class Node{
public:
	int ID;
	int label;
	int level;
	bool visited = false;
	vector<Node*> children;

	Node(int id){
		this->ID = id;
	}

	void addChild(Node* child){
		this->children.push_back(child);
	}

	void assignLabel(int label){
		this->label = label;
	}

	void assignLevel(int level) {
		this->level = level;
	}

	void visit(){
		this->visited = true;
	}
};

int main(int argc, char const *argv[])
{
	string infile_name = argv[1];
	string outfile_name = argv[2];

	ifstream infile;
	infile.open(infile_name);
	ofstream outfile;
	outfile.open(outfile_name);

	int numOfNodes;
	infile >> numOfNodes;

	vector<Node> nodes;

	for(int i = 0; i < numOfNodes; i++){
		Node temp = Node(i);
		nodes.push_back(temp);
	}
	for(int i = 0; i < numOfNodes; i++){
		int parent;
		infile >> parent;
		if(parent == -1){
			root = i;
			nodes[root].assignLevel(0);
		}else{
			nodes[parent].addChild(&nodes[i]);
	 	}
	}

	vector<int> preorder;
	stack<Node*> stack;
	unordered_map<int, Node*> hash;
	vector<vector<int>> levels(numOfNodes);

	stack.push(&nodes[root]);
	nodes[root].visit();

	nodes[root].assignLabel(1);
	levels[0].push_back(1);
	hash.insert(make_pair(1, &nodes[root]));
	int label = 2;

	// doing preorder traversal in order to assign labels and create hash table
	while(!stack.empty()) {

		int visited = 0;

		if(stack.top()->children.size() == 0) { // leaf
			stack.pop();
		}else{
			Node* parent = stack.top();

			for(int i = 0; i < parent->children.size(); i++){
				if(!parent->children[i]->visited){
					visited = 1;
					stack.push(parent->children[i]);
					parent->children[i]->visit();
					parent->children[i]->assignLabel(label);
					parent->children[i]->assignLevel(parent->level + 1);
					// appending label to the dynamic array associated with the level of node
					levels[parent->children[i]->level].push_back(label); 
					// inserting a pointer to node in the hash table with label as key
					hash.insert(make_pair(label, parent->children[i])); 
					label++;
					break;
				}
			}
			if(visited == 0){
				stack.pop();
			}
		}
	}

	int numOfQueries;
	infile >> numOfQueries;

	while(numOfQueries != 0){
		int node, ancestor;
		infile >> node >> ancestor;
		int label = nodes[node].label;
		int depth = nodes[node].level - ancestor;
		// looking up the largest number smaller than the label associated with the node
		if(depth >= 0){ 
			int first = 0;
			int last = levels[depth].size() - 1;
			int find = 0;
			while(first <= last){
				int mid = (first + last)/2;
				if(levels[depth][mid] >= label){
					last = mid - 1;
				}else{
					find = mid;
					first = mid + 1;
				}
			}
			outfile << hash.at(levels[depth][find])->ID << endl;
		}else{
			outfile << -1 << endl;
		}
		numOfQueries--;
	}

	infile.close();
	outfile.close();

	return 0;
}