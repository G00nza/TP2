//
// Created by gsaenz on 11/10/17.
//

#ifndef TP2_TRIE_HPP
#define TP2_TRIE_HPP
#include "trie.h"
#include <utility>

using namespace std;

template<class K, class S>
Trie<K, S>::Trie(){
    Nodo* nuevo = new Nodo;
    _raiz = nuevo;
    _tamano = 0;
}

Trie::~Trie() {
    while (_tamano > 0){
        erase(begin());
    }
    _raiz = NULL;
}

template<class K, class S>
Trie<K, S>::Trie(const Trie<K, S>& other) : Trie(){
    *this = other;
}



template<class K, class S>
pair<typename Trie<K, S>::iterator, bool>
Trie<K, S>::insert(const Trie<K, S>::value_type& v) {
    string camino = v.first;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo* actual = _raiz;
    int pos;
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves.[i]) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece) {
            actual->_claves.push_back(camino.[0]);
            Nodo *nuevo = new Nodo;
            actual->_hijos.push_back(nuevo);
            nuevo->padre = actual;
            actual = nuevo;
        } else {
            actual = actual->_hijos.[pos];
        }
        camino.erase(0);
    }
    if (actual->_obtener != NULL) {
        pertenece = false;
    } else {
        pertenece = true;
        _tamano++;
    }
    actual->_obtener = v.second;
    auto it = actual->_obtener;
    return make_pair(it, pertenece);
}

template<class K, class S>
typename Trie<K, S>::iterator Trie<K, S>::fast_insert(const Trie<K, S>::value_type& v) {
    auto it = insert(v).first;
    return it;
}

template<class K, class S>
Trie<K, S>& Trie<K, S>::operator=(const Trie<K, S>& other) {
    this->~Trie();
    // COSAS MUY COMPLICADAS, COPIAR TODOS LOS NODOS Y SUB NODOS
    // HACER UNA SUB RUTINA QUE DADO UN NODO SE COPIEN TODOS LOS
    // SUB NODOS
    return *this;
}

template<class K, class S>
typename Trie<K, S>::size_type Trie<K, S>::size() const {
    return _tamano;
}

template<class K, class S>
typename Trie<K, S>::iterator Trie<K, S>::find(const K &k) {
    string camino = k;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo* actual = _raiz;
    int pos;
    auto it = end();
    for (int j = 0; j < iteraciones ; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves.[i]) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece){
            return it;
        } else{
            actual = actual->_hijos.[pos];
        }
        camino.erase(0);
    }
    it = actual->_obtener;
    return it;
}

template<class K, class S>
typename Trie<K, S>::const_iterator
Trie<K, S>::find(const K &k) const {
    string camino = k;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo* actual = _raiz;
    int pos;
    auto it = end();
    for (int j = 0; j < iteraciones ; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves.[i]) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece){
            return it;
        } else{
            actual = actual->_hijos.[pos];
        }
        camino.erase(0);
    }
    it = actual->_obtener;
    return it;
}

template<class K, class S>
size_t Trie<K, S>::count(const K &k) const{
    return find(k) != end();
}
/*
template<class K, class S>
bool Trie<K, S>::operator==(const Trie<K, S> &other) const {
    for (auto v : other) {
        if (not count(v.first) or at(v.first) != v.second) {
            return false;
        }
    }
    return true;
}*/

template<class K, class S>
typename Trie<K, S>::size_type Trie<K, S>::erase(const K& key) {
    string camino = key;
    unsigned long iteraciones = camino.size();
    Nodo* actual = _raiz;
    int pos;
    vector <int> posicions;
    for (int j = 0; j < iteraciones ; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves.[i]) {
                pos = i;
            }
        }
        actual = actual->_hijos.[pos];
        camino.erase(0);
        posicions.push_back(pos);
    }

    if (actual->_hijos.size() > 0){
        //sacar letra en padre
        while (actual != _raiz){
            if (actual->padre->_hijos.size() == 1){
                Nodo* aux = actual;
                actual->padre->_hijos.erase(actual->padre->_hijos.begin());
                actual->padre->_claves.erase(actual->padre->_claves.begin());
                posicions.pop_back();
                actual = actual->padre;
                delete aux;
            } else {
                break;
            }
        }
    }
    if (actual != _raiz) {
        actual->padre->_hijos.erase(actual->padre->_hijos.begin() + posicions.back());
        actual->padre->_claves.erase(actual->padre->_claves.begin() + posicions.back());
    } else {
        _raiz = NULL;
    }
    delete actual;
    return _tamano--;
}

template<class K, class S>
const S& Trie<K, S>::at(const K& key) const {
    return find(key)->second;
}

template<class K, class S>
bool Trie::empty() const {
    return _tamano == 0;
}

template<class K, class S>
S& Trie<K, S>::at(const K& key) {
    return find(key)->second;
}

template<class K, class S>
typename Trie<K, S>::iterator Trie<K, S>::begin() {
    auto it = _raiz->_obtener;
    if (_tamano > 0) {
        Nodo* actual = _raiz;
        while (actual->_claves.size() > 0){
            actual = actual->_hijos.[0];
        }
        it = actual->_obtener;
    }
    return it;
}

template<class K, class S>
typename Trie<K, S>::iterator Trie<K, S>::end() {
    auto it = NULL;
    return it;
}

template<class K, class S>
typename Trie<K, S>::const_iterator Trie<K, S>::begin() const {
    auto it = _raiz->_obtener;
    if (_tamano > 0) {
        Nodo* actual = _raiz;
        while (actual->_claves.size() > 0){
            actual = actual->_hijos.[0];
        }
        it = actual->_obtener;
    }
    return it;
}

template<class K, class S>
typename Trie<K, S>::const_iterator Trie<K, S>::end() const {
    auto it = NULL;
    return it;
}



/*
template<class K, class S>
std::ostream &operator<<(std::ostream &os, const Trie<K, S>& m) {
    os << "{ ";
    int count = m.size();
    for (auto kv : m) {
        count--;
        os << kv.first << ": " << kv.second;
        if (count) {
            os << ", ";
        }
    }
    os << " }";
    return os;
}
*/


#endif //TP2_TRIE_HPP
