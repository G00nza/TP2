//
// Created by gsaenz on 12/10/17.
//

#ifndef TP2_STRING_MAP_HPP
#define TP2_STRING_MAP_HPP

#include "string_map.h"
#include "utils.h"
#include "string_map_iterators.h"
#include <utility>

using namespace std;

template<typename T>
string_map<T>::string_map() {
    Nodo *nuevo = new Nodo;
    _raiz = nuevo;
    _tamano = 0;
}

template<typename T>
string_map<T>::~string_map() {
    // ver que pasa con el map vacio
    while (_tamano > 0) {
        erase(begin());
    }
    _raiz = NULL;
}

template<typename T>
string_map<T>::string_map(const string_map<T> &other) : string_map() {
    *this = other;
}


template<typename T>
pair<typename string_map<T>::iterator, bool>
string_map<T>::insert(const string_map<T>::value_type &v) {
    string camino = v.first;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo *actual = _raiz;
    int pos = 0;
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves.at(i)) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece) {
            actual->_claves.push_back(camino[0]);
            Nodo *nuevo = new Nodo;
            actual->_hijos.push_back(nuevo);
            nuevo->padre = actual;

            actual = nuevo;
        } else {
            actual = actual->_hijos[pos];
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
    actual->_camino = v.first;
    string_map<T>::iterator it(actual);

    return make_pair(it, pertenece);
}


/*
template < typename T >
typename string_map<T>::iterator string_map<T>::fast_insert(const string_map<T>::value_type& v) {
    auto it = insert(v).first;
    return it;
}*/

template<typename T>
string_map<T> &string_map<T>::operator=(const string_map<T> &other) {
    this->~string_map();
    // COSAS MUY COMPLICADAS, COPIAR TODOS LOS NODOS Y SUB NODOS
    // HACER UNA SUB RUTINA QUE DADO UN NODO SE COPIEN TODOS LOS
    // SUB NODOS
    return *this;
}

template<typename T>
typename string_map<T>::size_type string_map<T>::size() const {
    return _tamano;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::find(const key_type &k) {
    string camino = k;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo *actual = _raiz;
    int pos;
    auto it = end();
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves[i]) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece) {
            return it;
        } else {
            actual = actual->_hijos[pos];
        }
        camino.erase(0);
    }
    it.it = actual;
    return it;
}

template<typename T>
typename string_map<T>::const_iterator
string_map<T>::find(const key_type &k) const {
    string camino = k;
    unsigned long iteraciones = camino.size();
    bool pertenece = false;
    Nodo *actual = _raiz;
    int pos;
    auto it = end();
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves[i]) {
                pertenece = true;
                pos = i;
            }
        }
        if (!pertenece) {
            return it;
        } else {
            actual = actual->_hijos[pos];
        }
        camino.erase(0);
    }
    it.it = actual;
    return it;
}

//si aparece => 1
//si no  => 0
template<typename T>
size_t string_map<T>::count(const key_type &key) const {
    return 0;
    //implementar
    //return find(key) != end();
}

template<typename T>
bool string_map<T>::operator==(const string_map<T> &other) const {
    Nodo *actualThis = _raiz;
    Nodo *actualOther = other._raiz;
    return igualDeNodo(*actualThis, *actualOther);
}

template<typename T>
typename string_map<T>::size_type string_map<T>::erase(const key_type &key) {
    string camino = key;
    unsigned long iteraciones = camino.size();
    Nodo *actual = _raiz;
    int pos = 0;
    vector<int> posicions;
    for (int j = 0; j < iteraciones; ++j) {
        for (int i = 0; i < actual->_claves.size(); ++i) {
            if (camino[0] == actual->_claves[i]) {
                pos = i;
            }
        }
        actual = actual->_hijos[pos];
        camino.erase(0);
        posicions.push_back(pos);
    }
    if (actual->_hijos.size() > 0) {
        //sacar letra en padre
        while (actual != _raiz) {
            if (actual->padre->_hijos.size() == 1) {
                Nodo *aux = actual;
                actual->padre->_hijos.erase(actual->padre->_hijos.begin());
                actual->padre->_claves.erase(actual->padre->_claves.begin());
                posicions.pop_back();
                actual = actual->padre;
                delete aux;
            } else {
                break;
            }
        }
        if (actual != _raiz) {
            actual->padre->_hijos.erase(actual->padre->_hijos.begin() + posicions.back());
            actual->padre->_claves.erase(actual->padre->_claves.begin() + posicions.back());
            delete actual;
        } else {
            if (_raiz->_obtener != NULL) {
                _raiz->_hijos.erase(_raiz->_hijos.begin() + posicions.back());
                _raiz->_hijos.erase(_raiz->_hijos.begin() + posicions.back());
            } else {
                _raiz = new Nodo;
                delete actual;
            }
        }
    } else {
        actual->_obtener = NULL;
    }

    return _tamano--;
}

template<typename T>
const typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) const {
    return find(key);
}

template<typename T>
bool string_map<T>::empty() const {
    return _tamano == 0;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::at(const key_type &key) {
    return *find(key)->second;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    string_map<T>::iterator it;
    it._nodo = _raiz;
    if ((it->first) != NULL) {}
    if (_tamano > 0) {
        Nodo *actual = _raiz;
        while (actual->_claves.size() > 0) {
            actual = actual->_hijos[0];
        }
        it._nodo = actual;
    }
    return it;
}

//hacer que el iterador end sea el ultimo
/*
template < typename T >
typename string_map<T>::iterator string_map<T>::end() {
    auto it = NULL;
    return it;
}*/

template<typename T>
typename string_map<T>::const_iterator string_map<T>::begin() const {
    auto it = _raiz->_obtener;
    if (_tamano > 0) {
        Nodo *actual = _raiz;
        while (actual->_claves.size() > 0) {
            actual = actual->_hijos[0];
        }
        it = actual->_obtener;
    }
    return it;
}

template<typename T>
typename string_map<T>::const_iterator string_map<T>::end() const {
    auto it = NULL;
    return it;
}

template<typename T>
int string_map<T>::posicion(vector<char> v, char c) const {
    int pos = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (c == v[i])
            pos = i;
    }
    return pos;
}

template<typename T>
bool string_map<T>::igualDeNodo(string_map<T>::Nodo &n1, string_map<T>::Nodo &n2) const {
    bool res;
    res = n1._obtener == n2._obtener;
    res &= seteq(n1._claves, n2._claves);
    if (res) {
        for (int i = 0; i < n1._claves.size() && res; ++i) {
            Nodo *n1aux = n1._hijos[i];
            Nodo *n2aux = n2._hijos[posicion(n1._claves, n1._claves[i])];
            res = igualDeNodo(*n1aux, *n2aux);
        }
    }

    return res;
}

template<typename T>
typename string_map<T>::mapped_type &string_map<T>::operator[](const key_type &key) {
    mapped_type nada;
    //auto res = ;
    return insert(make_pair(key, nada)).first->second;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::erase(string_map<T>::iterator pos) {
    string sacar = pos._nodo->_camino;
    erase(sacar);
    return begin();
}

/*void string_map::clear() {
    this->~string_map();
    _raiz = string_map()._raiz;
}*/





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
