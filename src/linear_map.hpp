
#ifndef TP2_LINEAR_MAP_HPP
#define TP2_LINEAR_MAP_HPP

#include "linear_map.h"

using namespace std;


template<class K, class S>
linear_map<K,S>::linear_map(){};

template<class K, class S>
linear_map<K,S>::linear_map(const linear_map<K, S> &other) {
    auto it = other.begin();
    while (it != other.end()){
        this->_elems.push_back(*it);
        ++it;
    }
    //return this;
}
/*
template<class K, class S>
pair<iterator, bool> linear_map<K,S>::insert(const value_type& v){
    bool res = true;
    linear_map::iterator it = begin();
    while (it != end()){
        if ((*it).first == v.first){
            (*it).second = v.second;
            res = false;
        }
        ++it;
    }
    if (res){
        _elems.push_back(v);
    }
    pair<iterator, bool> parRes = make_pair(it,res);
    return parRes;
}*/


template<class K, class S>
typename linear_map<K,S>::iterator linear_map<K,S>::fast_insert(const value_type& v){
    auto it = end();
    _elems.push_back(v);

    return it;
}

template<class K, class S>
typename linear_map<K,S>::size_type linear_map<K,S>::size() const{
    return _elems.size();
}

template<class K, class S>
S& linear_map<K,S>::at(const K& key){
   //S res;

    auto it = begin();
    while (it != end()){
        if ((*it).first == key) {
            //res = (*it).second;
            break;
        }

        ++it;
    }
    return (*it).second;
}


template<class K, class S>
typename linear_map<K,S>::iterator linear_map<K,S>::find(const K &k){
    auto res = begin();
    while(res != end() && (*res).first != k){
        ++res;
    }
    return res;
}

template<class K, class S>
typename linear_map<K,S>::const_iterator linear_map<K,S>::find(const K &k) const{
    auto res = begin();//Es un const, ojo con el auto
    while(res != end() && (*res).first != k){
        ++res;
    }
    return res;
}

template<class K, class S>
typename linear_map<K,S>::iterator linear_map<K,S>::begin(){
    auto res = _elems.begin();
    return res;
}

template<class K, class S>
typename linear_map<K,S>::iterator linear_map<K,S>::end(){
    auto res = _elems.end();
    return res;
}

template<class K, class S>
typename linear_map<K,S>::const_iterator linear_map<K,S>::begin() const{
    auto res = _elems.begin();
    return res;
}

template<class K, class S>
typename linear_map<K,S>::const_iterator linear_map<K,S>::end() const{
    auto res = _elems.end();
    return res;
}

//size_t linear_map<K,S>::count(const K &k) const {}

template<class K, class S>
const S& linear_map<K,S>::at(const K& key) const{
    auto it = begin();
    while (it != end()){
        if ((*it).first == key) {
            //res = (*it).second;
            break;
        }

        ++it;
    }
    return (*it).second;
}

template<class K, class S>
typename linear_map<K,S>::size_type linear_map<K,S>::erase(const K& key){
    auto it = find (key);
    _elems.erase(it);
    return 0; //no se k onda aca guachin
}

template<class K, class S>
linear_map<K,S>& linear_map<K,S>::operator=(const linear_map<K,S>& other){
    auto it = other.begin();
    while (it != other.end()){
        _elems.push_back(*it);
        ++it;
    }
}

template<class K, class S>
bool linear_map<K,S>::operator==(const linear_map<K,S> &other) const {
    bool res = true;
    auto itThis = begin();
    while (itThis != end() && res){
        res &= *other.find((*itThis).first) == *itThis;
    }
    auto itOther = other.begin();
    while (itOther != other.end() && res){
        res &= *find((*itOther).first) == *itOther;
    }
    return res;
}


#endif //TP2_LINEAR_MAP_HPP
