#ifndef TP2_STRING_MAP_ITERATORS_H
#define TP2_STRING_MAP_ITERATORS_H

#include "string_map.h"
#include "string_map.hpp"
#include <iostream>

using namespace std;


template<class T>
class string_map<T>::const_iterator {
public:
    using value_type = const string_map<T>::value_type;
    using reference = value_type&;
    using pointer = value_type*;

    /**
     * @brief Constructor por copia del iterador.
     *
     * \complexity{\O(1)}
     */
    const_iterator(const typename string_map<T>::const_iterator&);

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
    using value_type = const string_map<T>::value_type;
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
    typename string_map::iterator &operator++();

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
        const typename string_map<T>::iterator &other) : _nodo(other._nodo) {}


//.end() esta contenido en el trie
template<typename T>
typename string_map<T>::const_iterator &string_map<T>::const_iterator::operator++() {
    if(*this._hijos.size() > 0) {
        this = *this._hijos[0];
        while (*this._hijos.size() > 0 && *this._obtener != nullptr) {
            this = *this._hijos[0];
        }
    }else{
        while(*this->padre != nullptr){
            if (*this->padre->_hijos.size()-1 > *this->padre->_hijos.find(*this)){
                this = *this->padre->_hijos.find(*this)+1;
                break;
            }
            this = this->padre;
        }
        while (*this._hijos.size() > 0 && *this._obtener != nullptr) {
            this = *this._hijos[0];
        }
    }
    return this;
}

template<typename T>
const typename string_map<T>::const_iterator::value_type &string_map<T>::const_iterator::operator*() const {
    return *(make_pair(_nodo._camino , _nodo.obtener));
}

template<typename T>
const typename string_map<T>::const_iterator::value_type *string_map<T>::const_iterator::operator->() const {
    return make_pair(_nodo._camino , _nodo.obtener);
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

/*template<typename T>
string_map<T>::iterator::iterator(const typename string_map<T>::iterator& _it)
        : it(_it) {};
*/

//implementar
/*template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator++() {
    it++;
    return *this;
}*/

template<typename T>
const typename string_map<T>::iterator::value_type &string_map<T>::iterator::operator*() const {
    return *(make_pair(_nodo._camino, _nodo._obtener));
}

template<typename T>
const typename string_map<T>::iterator::value_type *string_map<T>::iterator::operator->() const {
    return make_pair(_nodo._camino, _nodo._obtener);
}

template<typename T>
bool string_map<T>::iterator::operator==(const string_map<T>::iterator &other) const {
    return _nodo == other._nodo;
}

template<typename T>
bool string_map<T>::iterator::operator!=(const string_map<T>::iterator &other) const {
    return not (this == other);
}

template<typename T>
typename string_map<T>::iterator &string_map<T>::iterator::operator++() {
    if(*this->_hijos.size() > 0) {
        this = *this->_hijos[0];
        while (*this->_hijos.size() > 0 && *this->_obtener != nullptr) {
            this = *this->_hijos[0];
        }
    }else{
        while(*this->padre != nullptr){
            if (*this->padre->_hijos.size()-1 > *this->padre->_hijos.find(*this)){
                this = *this->padre->_hijos.find(*this)+1;
                break;
            }
            this = this->padre;
        }
        while (*this._hijos.size() > 0 && *this._obtener != nullptr) {
            this = *this._hijos[0];
        }
    }
    return this;
}


#endif //TP2_STRING_MAP_ITERATORS_H