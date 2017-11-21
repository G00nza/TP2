#ifndef TP2_STRING_MAP_HPP
#define TP2_STRING_MAP_HPP

#include "string_map.h"
#include "utils.h"
#include "string_map_iterators.h"
#include <utility>

using namespace std;

template<typename T>
string_map<T>::string_map() {
//    Nodo *nuevo = new Nodo;
//    _raiz = nuevo;
    _raiz = new Nodo("");
    _raiz->padre = _end;
    _tamano = 0;
}

template<typename T>
string_map<T>::~string_map() {
    // ver que pasa con el map vacio
    while (_tamano > 0) {
        this->erase(begin());
    }
    delete (_raiz);
    _raiz = NULL;
}

template<typename T>
string_map<T>::string_map(const string_map<T> &other) : string_map() {
    const_iterator it = other.begin();
    while (this->size() != other.size()) {
        this->insert(*it);
        ++it;
    }
    //this->insert(*it);
}


template<typename T>
pair<typename string_map<T>::iterator, bool>
string_map<T>::insert(const string_map<T>::value_type &v) {
    string recorrido = v.first;
    unsigned long iteraciones = recorrido.size();
    bool pertenece = false;
    Nodo *actual = _raiz;//actual->_camino
    string auxNombre = "";
    int insertAt = 0;
    for (int i = 0; i < iteraciones; ++i) {
        for (int j = 0; j < actual->_hijos.size() && !pertenece; ++j) {
            if ((*(actual->_hijos[j]->_camino))[i] == recorrido[i]){
                actual = actual->_hijos[j];
                pertenece = true;
            } else if ((*(actual->_hijos[j]->_camino))[i] < recorrido[i]){
                insertAt = j + 1;
            }
        }
        auxNombre += recorrido[i];

        if (!pertenece){
            Nodo *nuevo = new Nodo(auxNombre);
            auto it = actual->_hijos.begin();
            actual->_hijos.insert(it + insertAt, nuevo);
            nuevo->padre = actual;
            actual = nuevo;
        }
        insertAt = 0; //hago reset de insertAt
        pertenece = false;// hago reset de pertenece
    }
    if (!actual->_definido){
        pertenece = true;
        _tamano++;
        actual->_definido = true;
    }
    (actual->v->second) = v.second;
    string_map<T>::iterator it(actual);
    return make_pair(it, pertenece);
}

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &other) {
    //this->~string_map();
    this->clear();
    const_iterator it = other.begin();
    while (this->size() != other.size()) {
        this->insert(*it);
        ++it;
    }
    return *this;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::size() const {
    return _tamano;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::find(const key_type &k) {
    Nodo *actual = _raiz;
    auto it = end();
    for (int i = 0; i < k.size(); ++i) {
        int j = 0;
        while (j < actual->_hijos.size()) {
            if (k[i] == actual->_hijos[j]->_camino->back()) {
                actual = actual->_hijos[j];
                break;
            }
            j++;
        }
        if (i != k.size()-1 && j == actual->padre->_hijos.size()) {
            return it;
        }
    }
    it._nodo = actual;
    return it;
}

template<typename T>
typename string_map<T>::const_iterator
string_map<T>::find(const key_type &k) const {
    Nodo *actual = _raiz;
    auto it = end();
    for (int i = 0; i < k.size(); ++i) {
        int j = 0;
        while (j < actual->_hijos.size()) {
            if (k[i] == actual->_hijos[j]->_camino->back()) {
                actual = actual->_hijos[j];
                break;
            }
            j++;
        }
        if (i != k.size()-1 && actual != _raiz && j == actual->padre->_hijos.size()) {
            return it;
        }
    }
    it._nodo = actual;
    return it;
}


template<typename T>
bool string_map<T>::count(const key_type &key) const {
    return find(key) != end() &&
            ((find(key)._nodo)->_definido && *((find(key)._nodo)->_camino) == key);
}

template<typename T>
bool string_map<T>::operator==(const string_map<T> &other) const {
    bool res = true;
    auto it = begin();
    while ( it != end() && res) {
        res &= other.find(it->first) != other.end() && other.at(it->first) == it->second;
        ++it;
    }
    return res;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const key_type &key) {
    string camino = key;
    unsigned long iteraciones = camino.size();
    Nodo *actual = _raiz;
    int pos = 0;
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_hijos.size(); ++i) {
            if (camino[0] == actual->_hijos[i]->_camino[0]) {////////// <========== HAY UN ERROR ACA <===========
                pos = i;
            }
        }
        actual = actual->_hijos[pos];
        camino.erase(0);
    }
    if (actual->_hijos.size() == 0 && actual != _raiz) {
        //nunca puedo borrar la raiz
        Nodo *borrar = actual;
        actual = actual->padre;
        actual->_hijos.erase(actual->_hijos.begin() + borrar->posEnPadre());
        delete borrar;

        while (!actual->_definido && actual->_hijos.size() == 0 && actual != _raiz) {
            Nodo *borrar = actual;
            actual = actual->padre;
            actual->_hijos.erase(actual->_hijos.begin() + borrar->posEnPadre());
            delete borrar;
        }

    } else{
        actual->_definido = false;
        //delete (actual->_obtener);//ver que los iteradores no rompan agregando pre
        delete (actual->_obtener);
        actual->_obtener = nullptr;
    }
    if (key == "") {
        actual->_definido = false;
        //delete (actual->_obtener);
        delete (actual->v);//ver que los iteradores no rompan agregando pre
        actual->_obtener = nullptr;
    }
    _tamano--;
//    return _tamano;
    return 1;
}


template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const {
    return ((find(key)._nodo)->v->second);
}

template<typename T>
bool string_map<T>::empty() const {
    return _tamano == 0;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) {
    return ((find(key)._nodo)->v->second);
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    string_map<T>::iterator it(_raiz);
    if (_tamano > 0) {
        Nodo *actual = _raiz;
        while (actual->_hijos.size() > 0 && !actual->_definido) {
            actual = actual->_hijos[0];
        }
        it._nodo = actual;
    }
    return it;
}

template < typename T >
typename string_map<T>::iterator string_map<T>::end() {
    auto it (_end);
    return it;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const {
    string_map<T>::const_iterator it(_raiz);
    if (_tamano > 0) {
        Nodo *actual = _raiz;
        while (actual->_hijos.size() > 0 && !actual->_definido) {
            actual = actual->_hijos[0];
        }
        it._nodo = actual;
    }
    return it;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const {
    auto it (_end);
    return it;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::operator[](const key_type &key) {
    if (!count(key)){
        insert(make_pair(key, mapped_type()));
    }
    return at(key);
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(iterator pos) {
    string sacar = *(pos._nodo->_camino);
    ++pos;
    erase(sacar);
    return pos;
}

template<typename T>
void string_map<T>::clear(){
    while (_tamano > 0) {
        erase(begin());
    }
}





/*
template < typename T >
std::ostream &operator<<(std::ostream &os, const string_map<T>& m) {
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


#endif //TP2_STRING_MAP_HPP
