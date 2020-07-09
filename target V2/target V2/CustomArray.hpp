//
//  CustomArray.hpp
//  target V2
//
//  Created by Mumphus on 2019-08-08.
//  Copyright © 2019 mumphus. All rights reserved.
//

#ifndef CustomArray_hpp
#define CustomArray_hpp

#include <stdio.h>

template <class T>
class CustomArray {
public:
    CustomArray(int size){
        this->size = size;
        array = new T*[size];
    }
    
    ~CustomArray(){
        
    }
    
    bool add(T* entry){
        if (entries < size){
            array[entries++] = entry;
            return true;
        }
        return false;
    }
    
    T* get(int index){
        return array[index];
    }
private:
    int size;
    int entries;
    T** array;
protected:
};

#endif /* CustomArray_hpp */
