#include <iostream>
#include "AVL.h"
#include "BST.h"
#include "json.hpp"
#include <stdio.h>
#include <sstream>
#include <fstream>


int main(int argc, char* argv[]){
    std::ifstream file;
    file.open(argv[1]);
    nlohmann::json targetObject;
    if(file.is_open()){
        file >> targetObject;
    }
    file.close();

    AVL avlTree;
    for (auto itr = targetObject.begin(); itr != targetObject.end(); ++itr) {
        if(itr.key()=="metadata"){
            break;
        }
        avlTree.Insert(targetObject[itr.key()]["key"]);
        //  std::cout<<"in commands loop"<<itr.key()<<std::endl;
        // std::cout<<avlTree.JSON();
    }
    
    // int height= avlTree.getroot()->getHeight();
    // targetObject["height"]=height;
    std::cout<<avlTree.JSON();
    
    return 0;
}