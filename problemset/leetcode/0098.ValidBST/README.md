# Validate Binary Search Tree

Given the root of a binary tree, determine if it is a valid binary search tree (BST).

A valid BST is defined as follows:

The left subtree of a node contains only nodes with keys strictly less than the node's key.
The right subtree of a node contains only nodes with keys strictly greater than the node's key.
Both the left and right subtrees must also be binary search trees.
 

### Example 1:
![alt text](image.png)
Input: root = [2,1,3]
Output: true


### Example 2:
![alt text](image-1.png)

Input: root = [5,1,4,null,null,3,6]
Output: false
Explanation: The root node's value is 5 but its right child's value is 4.

### Example 3:
![alt text](image-2.png)

Input: root = [5,4,6,null,null,3,7]
Output: false

## Constraints:

* The number of nodes in the tree is in the range [1, 104].
* -2^31 <= Node.val <= 2^31 - 1
 