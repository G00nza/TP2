
#ifndef TP2_LINEAR_MAP_HPP
#define TP2_LINEAR_MAP_HPP

#include "linear_map.h"

using namespace std;


template<class K, class S>
linear_map::linear_map(){};

template<class K, class S>
linear_map & linear_map::linear_map(const linear_map<K, S> &other) {
    linear_map res = linear_map();

    auto it = other._elems::begin();
    while (it != other._elems::end()){
        res._elems.push_back(*it);
        ++it;
    }
    //return res;
}

template<class value_type>
pair<iterator, bool> linear_map::insert(const value_type& v){
    bool res = true;
//el cambio hecho aca no tiene sentido OJO en test
    linear_map::iterator it = begin();
    while (it != end()){
        if (*it == v.first){ // eraq *it.first
            *it = v.second;
            res = false;
        }
        ++it;
    }
    if (res){
        _elems.push_back(v);
    }
    return make_pair(it, res);
}


template<class K, class S>
linear_map::iterator linear_map::fast_insert(const value_type& v){
    auto it = end();
    _elems.push_back(v);

    return it;
}

template<class K, class S>
linear_map::size_type linear_map::size() const{
    return _elems.size();
}

template<class K, class S>
S& linear_map::at(const K& key){
   S res;

    auto it = begin();
    while (it != end()){
        if (*it.first == key) {
            res = *it.second;
        }

        ++it;
    }
    return res;
}


template<class K>
linear_map::iterator linear_map::find(const K &k){
    auto res = begin();
    while(res != end() && *res.first != k){
        ++res;
    }
    return res;
}

template<class K>
linear_map::const_iterator linear_map::find(const K &k) const{
    auto res = begin();//Es un const, ojo con el auto
    while(res != end() && *res.first != k){
        ++res;
    }
    return res;
}

linear_map::iterator linear_map::begin(){
    auto res = _elems.begin();
    return res;
}

linear_map::iterator linear_map::end(){
    auto res = _elems.end();
    return res;
}

linear_map::const_iterator linear_map::begin() const{
    auto res = _elems.begin();
    return res;
}

linear_map::const_iterator linear_map::end() const{
    auto res = _elems.end();
    return res;
}

//size_t linear_map::count(const K &k) const {}

template<class K, class S>
const S& linear_map::at(const K& key) const{
    S res;

    auto it = begin();
    while (it != end()){
        if (*it.first == key) {
            res = *it.second;
        }

        ++it;
    }
    return res;
}

template<class K, class S>
size_type linear_map::erase(const K& key){
    auto it = find (key);
    _elems.erase(it);
    return 0; //no se k onda aca guachin
}

template<class K, class S>
linear_map& linear_map::operator=(const linear_map& other){
    linear_map res = linear_map() ;

    auto it = other._elems::begin();
    while (it != other._elems::end()){
        res._elems.push_back(*it);
        ++it;
    }
    return res;
}

template<class K, class S>
bool linear_map::operator==(const linear_map &other) const {
    bool res = true;
    auto itThis = begin();
    while (itThis != end() && res){
        res &= *other.find(*itThis.first) == *itThis;
    }
    auto itOther = other.begin();
    while (itOther != other.end() && res){
        res &= *find(*itOther.first) == *itOther;
    }
    return res;
}


#endif //TP2_LINEAR_MAP_HPP
