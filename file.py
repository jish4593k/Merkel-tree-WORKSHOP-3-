# Building the Merkle Tree:

# To create a Merk le Tree, star t by initializing an instance of the Merkle Tree class.
# Utilize the build Tree From List method to construct the tree from a list of transactions (provided as strings). The neces sary number of  odes for the tr ee is automatically de termi ned.

# Calculating the  Mer kle Root:

# After constructing the tree, use the calculate MerkleRoot method to compute the Merkle root. This process involves recursively hashing the data of child no des.

# Obtaining the Merkle Root:

# Retrieve the Merkle root using the getMerkleRoot method. It returns the computed Merkle root as a hexadecimal string.

# Verifying Transactions:

# To verify if a list of transactions matches the Merkle root, employ the verifyTransactions method.
# This method creates a new Merkle Tree from a list of transactions marked for verification, calculates its Merkle root, and compares it to the original Merkle root.




class Node:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None

    def is_full(self):
        return self.left and self.right

    def is_leaf(self):
        return not (self.left or self.right)

class MerkleTree:
    def __init__(self):
        self.root = None
        self._merkle_root = ''

    def calculate_hash(self, data):
   
        return hashlib.sha256(data.encode()).hexdigest()

    def build_tree_from_list(self, transactions):
        def no_of_nodes_required(arr):
            x = len(arr)
            return 2 * x - 1

        def build_tree(arr, i, n):
            if i < n:
                temp = Node(str(arr[i]))
                root = temp
                root.left = build_tree(arr, 2 * i + 1, n)
                root.right = build_tree(arr, 2 * i + 2, n)
                return root

        def add_leaf_data(arr, node):
            if not node:
                return
            add_leaf_data(arr, node.left)
            if node.is_leaf():
                node.data = self.calculate_hash(arr.pop())
            else:
                node.data = ''
            add_leaf_data(arr, node.right)

        nodes_required = no_of_nodes_required(transactions)
        node_arr = [num for num in range(1, nodes_required + 1)]
        self.root = build_tree(node_arr, 0, nodes_required)
        add_leaf_data(transactions, self.root)

    def inorder_traversal(self, node):
        if not node:
            return
        self.inorder_traversal(node.left)
        print(node.data)
        self.inorder_traversal(node.right)

    def calculate_merkle_root(self):
        def merkle_hash(node):
            if node.is_leaf():
                return node
            left = merkle_hash(node.left).data
            right = merkle_hash(node.right).data
            node.data = self.calculate_hash(left + right)
            return node

        merkle_root_node = merkle_hash(self.root)
        self._merkle_root = merkle_root_node.data
        return self._merkle_root

    def get_merkle_root(self):
        return self._merkle_root

    def verify_transactions(self, arr1):
        hash1 = self.get_merkle_root()
        new_tree = MerkleTree()
        new_tree.build_tree_from_list(arr1)
        new_tree.calculate_merkle_root()
        hash2 = new_tree.get_merkle_root()
        if hash1 == hash2:
            print("Transactions verified successfully")
            return True
        else:
            print("Transactions have been tampered")
            return False




        
