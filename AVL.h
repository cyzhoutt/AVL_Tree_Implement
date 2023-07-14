#include <memory>
#include <string>

class AVL;

class AVLNode {
 public:
 	AVLNode(int key);
 	AVLNode(int key, std::weak_ptr<AVLNode> parent);
 	bool IsLeaf() const;
 	bool IsMissingChild() const;
 	bool HasLeftChild() const;
 	bool HasRightChild() const;
 	void DeleteChild(std::shared_ptr<AVLNode> v);
 	void ReplaceChild(std::shared_ptr<AVLNode> v, std::shared_ptr<AVLNode> u);
    // int getHeight();
    // int getBalanceFactor();
    void updateOneNodeHeight();
    void updateOneNodeBalanceFactor();
    // void setHeight(int h);
    // void setBalanceFactor(int h);

 private:
  int key_;
  std::weak_ptr<AVLNode> parent_;
  std::shared_ptr<AVLNode> left_;
  std::shared_ptr<AVLNode> right_;
  int height_;
  int balanceFactor_;

  friend AVL;
}; // class AVLNode

class AVL {
 public:
 	AVL();

 	void Insert(int key);
 	bool Delete(int key);
 	bool Find(int key) const;
 	std::string JSON() const;
 	size_t size() const;
 	bool empty() const;
 	int DeleteMin();
	// std::shared_ptr<AVLNode> getroot();

 private:
	void DeleteLeaf(std::shared_ptr<AVLNode> currentNode);
	int DeleteMin(std::shared_ptr<AVLNode> currentNode);
    void updateHeight(std::shared_ptr<AVLNode> node);
    void updateBalanceFactor(std::shared_ptr<AVLNode> node);
    void leftRotation(std::shared_ptr<AVLNode> currentNode);
    void rightRotation(std::shared_ptr<AVLNode> currentNode);

 	std::shared_ptr<AVLNode> root_;
 	size_t size_;
}; // class AVL