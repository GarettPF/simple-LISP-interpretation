#include <fstream>
#include <cstdlib>
using namespace std;

template <class type>
struct binTreeNode {
	type item; //stores the data in the node of the tree
	binTreeNode<type> *left;
	binTreeNode<type> *right;
};

template <class type>
void readLISP(binTreeNode<type> *, ifstream &);

binTreeNode<int> *createNode(ifstream &);

template <class type>
bool evaluate(binTreeNode<type> *, int, int, myStack<type> &);

template <class type>
void destroyTree(binTreeNode<type> *);

////////////////////////////////////////////////////////////////////

/**
 * @brief : reads from a file and builds a tree (preorder)
 * @tparam type : value type of the tree
 * @param r : pointing to some node in the tree
 * @param infile : the file to read from
*/
template <class type>
void readLISP(binTreeNode<type> *r, ifstream &infile) {
	// base case
	if (r != nullptr) {
		char ch = 0;
		string buffer;
		int iter = 0;

		do {
			infile.get(ch);
			if (ch - '0' < 10 && ch - '0' >= 0) {
				// ch must be a number
				buffer += ch;
				iter++;
			} else if (ch == 40 && !iter) {
				// ch is a '(' before a number is found
				ch = 0;
			}
		} while (ch != '(' && ch != ')');

		if (ch == ')') {
			// this means there was no number
			r->item = NULL;
			r->left = r->right = nullptr;
		} else {
			r->item = atoi(buffer.c_str());
			r->left = new binTreeNode<type>;
			r->right = new binTreeNode<type>;
		}

		readLISP(r->left, infile);
		readLISP(r->right, infile);

		// get the closing parenthesis and move up the call stack
		if (ch != ')') infile.get(ch);
	}
}

/**
 * @brief : a preorder traversal to determine if there is a 
 *			path from the root to a leaf whose numbers
 *			equal a target sum.
 * @tparam type : value type of the tree
 * @param r : pointing to some node in the tree
 * @param runningSum : current sum from root to leaf
 * @param targetSum : the sum we are checking for
 * @param path : the call stack of numbers of each leaf to root 
 * @return : true if path found, false otherwise
*/
template <class type>
bool evaluate(binTreeNode<type> *r, int runningSum, int targetSum, myStack<type> &path) {
	bool left_node, right_node;

	if (r != nullptr) {
		// push r->item to stack
		path.push(r->item);

		// base case
		if (runningSum + r->item == targetSum) {
			return true;
		}

		// when running sum > target sum
		if (runningSum + r->item > targetSum) {
			path.pop();
			return false;
		} else {
			// this means running sum + item < target sum so continue
			runningSum += r->item;
		}

		left_node = evaluate(r->left, runningSum, targetSum, path);
		right_node = evaluate(r->right, runningSum, targetSum, path);

		// if a path was found from either children
		if (left_node || right_node) {
			return true;
		} else {
			path.pop();
			return false;
		}

	} else {
		// r was nullptr
		return false;
	}
}

template <class type>
void destroyTree(binTreeNode<type> *r) {
	if (r != nullptr) {
		destroyTree(r->left);
		destroyTree(r->right);
		delete(r);
	}
}