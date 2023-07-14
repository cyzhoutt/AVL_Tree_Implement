#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"

// int AVLNode::getHeight(){
//     return height_;
// }
// int AVLNode::getBalanceFactor(){
//     return balanceFactor_;
// }

// void AVLNode::setHeight(int h){
//     height_ = 100;
// }

// void AVLNode::setBalanceFactor(int bf){
//     balanceFactor_ = bf;
// }

// std::shared_ptr<AVLNode> AVL::getroot(){
// 	return root_;
// }

void AVLNode::updateOneNodeHeight(){
    if(IsLeaf()){
        // node.setHeight(0);
        height_=0;
        // std::cout<<"update is leaf case"<<std::endl;
    }
    else if(HasLeftChild()&&HasRightChild()){
        height_= 1+ std::max(left_->height_, right_->height_);
        // std::cout<<"update both child case height"<<height_<<std::endl;
    }
    else if(HasLeftChild()&&!(HasRightChild())){
        height_= 1+ left_->height_;
        // std::cout<<"update is left child case height"<<height_<<std::endl;
    }
    else if(HasRightChild()&&!(HasLeftChild())){
        height_= 1+ right_->height_;
        // std::cout<<"update is right child case height"<<height_<<std::endl;
    }
// std::cout<<"update oneNodeHeight case"<<height_<<std::endl;
}

void AVLNode::updateOneNodeBalanceFactor(){
    if(IsLeaf()){
        balanceFactor_=0;
    }
    else if(HasLeftChild()&&HasRightChild()){
        balanceFactor_= right_->height_ - left_->height_;
    }
    else if(HasLeftChild()&&!(HasRightChild())){
        balanceFactor_= -1 - left_->height_;
    }
    else if(HasRightChild()&&!(HasLeftChild())){
        balanceFactor_= right_->height_+1;
    }
    // std::cout<<"update oneNodeBalance factor case"<<std::endl;
}

void AVL::updateHeight(std::shared_ptr<AVLNode> node){
    // std::cout<< node->key_ <<std::endl;
    while(node != nullptr){
        node->updateOneNodeHeight();
        // std::cout<<"update height case"<<node->height_<<std::endl;
        node = node->parent_.lock();
    }
    
    
}

void AVL::updateBalanceFactor(std::shared_ptr<AVLNode> node){
     while(node != nullptr){
        node->updateOneNodeBalanceFactor();
        node = node->parent_.lock();
    }
    // std::cout<<"update balance factor case"<<std::endl;
}

void AVL::leftRotation(std::shared_ptr<AVLNode> currentNode){
    std::shared_ptr<AVLNode> b = currentNode->right_;
    std::shared_ptr<AVLNode> tbl = b->left_;
    std::shared_ptr<AVLNode> origParent = currentNode->parent_.lock();
	// std::cout<<"left rotation on "<<currentNode->key_<<std::endl;
	currentNode->right_=nullptr;
    if(origParent==nullptr){
        b->left_ = currentNode;
        if(tbl!=nullptr){
            currentNode->right_ = tbl;
            tbl->parent_ = currentNode;
        }
        b->parent_.reset();
        currentNode->parent_ = b;
        root_ = b;
    }else{
        if(origParent->left_==currentNode){
            origParent->left_ = b;
        }else{
            origParent->right_ = b;
        }
		b->parent_ = origParent;
        b->left_ = currentNode;
		currentNode->parent_ = b;
        if(tbl!=nullptr){
            currentNode->right_ = tbl;
            tbl->parent_ = currentNode;
        }
        // b->parent_ = origParent;
		// currentNode->parent_ = b;
    }
    updateHeight(currentNode);
    updateBalanceFactor(currentNode);    
}


void AVL::rightRotation(std::shared_ptr<AVLNode> currentNode){
    std::shared_ptr<AVLNode> y = currentNode->left_;
    std::shared_ptr<AVLNode> tyr = y->right_;
    std::shared_ptr<AVLNode> origParent = currentNode->parent_.lock();
	// std::cout<<"right rotation on "<<currentNode->key_<<std::endl;
	currentNode->left_=nullptr;
    if(origParent==nullptr){
        y->right_ = currentNode;
        if(tyr!=nullptr){
            currentNode->left_ = tyr;
            tyr->parent_ = currentNode;
        }
        y->parent_.reset();
        currentNode->parent_ = y;
        root_ = y;
    }else{
        if(origParent->left_==currentNode){
            origParent->left_ = y;
        }else{
            origParent->right_ = y;
        }
		y->parent_ = origParent;
        y->right_ = currentNode;
		currentNode->parent_ = y;
        if(tyr!=nullptr){
            currentNode->left_ = tyr;
            tyr->parent_ = currentNode;
        }
    }
    updateHeight(currentNode);
    updateBalanceFactor(currentNode);
}




AVLNode::AVLNode(int key) :
	key_(key),
	parent_(std::weak_ptr<AVLNode>()),
	left_(nullptr),
	right_(nullptr),
    height_ (0),
    balanceFactor_(0) {}
    

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
	key_(key),
	parent_(parent),
	left_(nullptr),
	right_(nullptr),
    height_ (0),
    balanceFactor_(0) {}

bool AVLNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
	return right_ != nullptr;
}

void AVLNode::DeleteChild(std::shared_ptr<AVLNode> v) {
	if (left_ == v) {
		left_ = nullptr;
	} else if (right_ == v) {
		right_ = nullptr;
	} else {
		std::cerr << "AVLNode::DeleteChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

void AVLNode::ReplaceChild(std::shared_ptr<AVLNode> v, std::shared_ptr<AVLNode> u) {
	if (left_ == u || right_ == u) {
		std::cerr << "AVLNode::ReplaceChild Error: child passed as replacement\n";
	}
	if (left_ == v) {
		left_ = u;
		u->parent_ = v->parent_;
	} else if (right_ == v) {
		right_ = u;
		u->parent_ = v->parent_;
	} else {
		std::cerr << "AVLNode::ReplaceChild Error: non-child passed as argument\n";
		exit(EXIT_FAILURE);
	}
}

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = std::make_shared<AVLNode>(key);
		size_++;
		return;
	}
	std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	if (key < lastNode->key_) {
		lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
	} else {
		lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
	}
    updateHeight(lastNode);
    updateBalanceFactor(lastNode);
	// std::cout<<"outside"<<lastNode->key_<<std::endl;
	while(lastNode != nullptr&&lastNode->parent_.lock()!=nullptr){
		// std::cout<<"in loop"<<std::endl;
		if(lastNode->balanceFactor_==-1 && lastNode->parent_.lock()->balanceFactor_==-2){
			// std::cout<<"before right rotation"<<lastNode->key_<<std::endl;
			rightRotation(lastNode->parent_.lock());
			// std::cout<<"after right rotation"<<lastNode->key_<<std::endl;
		}
		if(lastNode->balanceFactor_==1 && lastNode->parent_.lock()->balanceFactor_==2){
			leftRotation(lastNode->parent_.lock());
		}
		if(lastNode->balanceFactor_==-1 && lastNode->parent_.lock()->balanceFactor_==2){
			rightRotation(lastNode);
			// std::cout<<"right left rotation"<<std::endl;
			//  std::cout<<lastNode->parent_.lock()->parent_.lock()->key_<<std::endl;
			leftRotation(lastNode->parent_.lock()->parent_.lock());
		}
		if(lastNode->balanceFactor_==1 && lastNode->parent_.lock()->balanceFactor_==-2){
			leftRotation(lastNode);
			rightRotation(lastNode->parent_.lock()->parent_.lock());
		}else{
			// std::cout<<"break at "<<lastNode->key_<<std::endl;
			lastNode=lastNode->parent_.lock();
		}
		// std::cout<<"before update"<<lastNode->key_<<std::endl;
		// //  std::cout<<"update parents error"<<std::endl;
		// lastNode = lastNode->parent_.lock();
		// std::cout<<"after update"<<lastNode->key_<<std::endl;
	}
	
	size_++;
//     std::cout<<size_<<std::endl;
}

bool AVL::Delete(int key) {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			if (currentNode->IsLeaf()) {
				DeleteLeaf(currentNode);
			} else if (currentNode->left_ == nullptr) {
				assert(currentNode->right_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->right_);
				size_--; assert(size_ >= 0);
			} else if (currentNode->right_ == nullptr) {
				assert(currentNode->left_ != nullptr);
				std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
				parent->ReplaceChild(currentNode, currentNode->left_);
				size_--; assert(size_ >= 0);
			} else {
				currentNode->key_ = DeleteMin(currentNode);
			}
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

int AVL::DeleteMin() {
	return DeleteMin(root_);
}


void AVL::DeleteLeaf(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> parent = currentNode->parent_.lock();
	if (parent == nullptr) {
		// Delete root
		root_ = nullptr;
		size_--; assert(size_ == 0);
	} else {
		if (parent->right_ == currentNode) {
			parent->right_ = nullptr;
		} else if (parent->left_ == currentNode) {
			parent->left_ = nullptr;
		} else {
			std::cerr << "AVL::DeleteLeaf Error: inconsistent state\n";
		}
		size_--; assert(size_ >= 0);
	}
}

int AVL::DeleteMin(std::shared_ptr<AVLNode> currentNode) {
	std::shared_ptr<AVLNode> lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = currentNode->left_;
	}
	int result = lastNode->key_;
	std::shared_ptr<AVLNode> parent = lastNode->parent_.lock();
	if (parent == nullptr) {
		// lastNode is root
		if (lastNode->right_ != nullptr) {
			root_ = lastNode->right_;
			lastNode->right_->parent_.reset();
		} else {
			root_ = nullptr;
		}
	} else {
		// lastNode under the root
		if (lastNode->right_ != nullptr) {
			parent->left_ = lastNode->right_;
			lastNode->right_->parent_ = parent;
		} else {
			parent->left_ = nullptr;
		}
  }
	size_--; assert(size_ >= 0);
	return result;
}

size_t AVL::size() const {
	return size_;
}

bool AVL::empty() const {
	return size_ == 0;
}

bool AVL::Find(int key) const {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ == key) {
			return true;
		}
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	return false;
}

std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			// std::cout<<v->key_<<std::endl;
			nodes.pop();
			std::string key = std::to_string(v->key_);
            result[key]["height"]= v->height_;
            result[key]["balance factor"]= v->balanceFactor_;
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	result["size"] = size_;
	result["height"] = root_->height_;
	return result.dump(2) + "\n";
}
