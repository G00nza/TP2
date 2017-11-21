#ifndef TP2_STRING_MAP_ITERATORS_H
#define TP2_STRING_MAP_ITERATORS_H

#include "string_map.h"
#include "string_map.hpp"
#include <iostream>

using namespace std;


template<class T>
class string_map<T>::const_iterator {
public:
    using value_type = string_map<T>::value_type;
    using reference = value_type&;
    using pointer = value_type*;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::const_iterator&);

    const_iterator(const typename string_map<T>::Nodo*);

    /**
     * @brief Conversión desde iterator
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(1)}
     */
    typename string_map::const_iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia constante al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type &operator*() const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    const value_type* operator->() const;

    /**
     * @brief Comparación entre iteradores 
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const string_map<T>::const_iterator &other) const;

    /**
     * @brief Comparación entre iteradores 
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const string_map<T>::const_iterator &other) const;

private:
    friend class string_map<T>;

    /**
     * @brief El iterador es puntero a Nodo.
     */
    const_iterator (Nodo* n) : _nodo (n) {};
    Nodo* _nodo;


};

template<class T>
class string_map<T>::iterator {
public:
    using value_type = string_map<T>::value_type;
    using reference = value_type&;
    using pointer = value_type*;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    iterator(const typename string_map<T>::iterator&);

    /**
     * @brief Constructor por refernecia a Nodo.
     *
     * \complexity{\O(1)}
     */
    iterator(const typename string_map<T>::Nodo*);

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    iterator& operator=(const typename string_map<T>::iterator&);

    /**
     * @brief Avanza el iterador una posición.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post \P{res} es una referencia a \P{this}. \P{this} apunta a la posición
     * siguiente.
     *
     * \complexity{\O(1)}
     */
    iterator &operator++();

    /**
     * @brief Desreferencia el puntero
     *
     * El valor devuelto tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es una referencia al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    value_type &operator*();

    bool definido()const;

    /**
     * @brief Operador flechita
     *
     * El valor devuelvo tiene aliasing dentro de la colección.
     *
     * \pre El iterador no debe estar en la posición pasando-el-último.
     * \post El valor resultado es un puntero al valor apuntado.
     *
     * \complexity{\O(1)}
     */
    value_type* operator->() ;

    /**
     * @brief Comparación entre iteradores 
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator==(const string_map<T>::iterator &other) const;

    /**
     * @brief Comparación entre iteradores 
     *
     * \pre ambos iteradores refieren a la misma colección
     * \post true sii los iteradores no apuntan al mismo elemento
     *
     * \complexity{\O(1)}
     */
    bool operator!=(const string_map<T>::iterator &other) const;

private:
    friend class string_map<T>;
    friend class BaseDeDatos;

    /**
     * @brief El iterador es puntero a Nodo.
     */
    iterator (Nodo* n) : _nodo (n) {};
    Nodo* _nodo;
};




// const_iterator methods

template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::const_iterator &other) : _nodo(other._nodo) {}
template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::Nodo* nodo) : _nodo(nodo) {};


template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::iterator &other) : _nodo(other._nodo) {}


//.end() esta contenido en el trie
template<typename T>
typename string_map<T>::const_iterator &string_map<T>::const_iterator::operator++() {
    if(_nodo->_hijos.size() > 0) {
        _nodo = _nodo->_hijos[0];
        while (_nodo->_hijos.size() > 0 && !_nodo->_definido) {
            _nodo = _nodo->_hijos[0];
        }
    }else{
        while(_nodo->padre != nullptr){
            if (_nodo->padre->_hijos.size()-1 > _nodo->posEnPadre()){
                _nodo = this->_nodo->padre->_hijos[_nodo->posEnPadre() + 1];
                break;
            }
            _nodo = _nodo->padre;
        }
        if (_nodo->padre == nullptr){
            _nodo = this->_nodo->padre;
        }else {
            while (_nodo->_hijos.size() > 0 && !_nodo->_definido) {
                _nodo = _nodo->_hijos[0];
            }
        }
    }
    return *this;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type &string_map<T>::const_iterator::operator*() const {
    return *_nodo->v;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type *string_map<T>::const_iterator::operator->() const {
    return _nodo->v;
}

template<typename T>
bool string_map<T>::const_iterator::operator==(const string_map<T>::const_iterator &other) const {
    return _nodo == other._nodo;
}

template<typename T>
bool string_map<T>::const_iterator::operator!=(const string_map<T>::const_iterator &other) const {
    return not (*this == other);
}

/*template<typename T>
string_map<T>::const_iterator::const_iterator(
        const typename string_map<T>::const_iterator& _it)
        : it(_it) {};
*/






// iterator methods



template<typename T>
string_map<T>::iterator::iterator(
        const typename string_map<T>::iterator& other) : _nodo(other._nodo) {};

template<typename T>
string_map<T>::iterator::iterator(
        const typename string_map<T>::Nodo* nodo) : _nodo(nodo) {};

template<typename T>
typename string_map<T>::iterator& string_map<T>::iterator::operator=(const string_map<T>::iterator& other){
    return *this = other;
}

template<typename T>
typename string_map<T>::iterator::value_type &string_map<T>::iterator::operator*() {
    return *_nodo->v;
}

template<typename T>
typename string_map<T>::iterator::value_type *string_map<T>::iterator::operator->(){
    return _nodo->v;
}

template<typename T>
bool string_map<T>::iterator::operator==(const string_map<T>::iterator &other) const {
    return _nodo == other._nodo;
}

template<typename T>
bool string_map<T>::iterator::operator!=(const string_map<T>::iterator &other) const {
    return not (*this == other);
}

template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator++() {
    if(_nodo->_hijos.size() > 0) {
        _nodo = _nodo->_hijos[0];
        while (_nodo->_hijos.size() > 0 && !_nodo->_definido) {
            _nodo = _nodo->_hijos[0];
        }
    }else{
        bool notRoot = false;
        while(_nodo->padre != nullptr){
            if (_nodo->padre->_hijos.size()-1 > _nodo->posEnPadre()){
                _nodo = _nodo->padre->_hijos[_nodo->posEnPadre() + 1];
                notRoot = true;
                break;
            }
            _nodo = _nodo->padre;
        }
        if (notRoot) {
            while (_nodo->_hijos.size() > 0 && !_nodo->_definido) {
                _nodo = _nodo->_hijos[0];
            }
        } else{
            _nodo = _nodo->padre;
        }
    }
    return *this;
}

template<typename T>
bool string_map<T>::iterator::definido() const{
    return _nodo->_definido;
}

//Hacer interfaz???
template<typename T>
int findpos (vector<T> s, T e){
    int res = 0;
    while (res < s.size()){
        if (s[res] == e){
            return res;
        }
        res++;
    }
    return res;
}


#endif //TP2_STRING_MAP_ITERATORS_H
