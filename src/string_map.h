#ifndef STRING_MAP_STRING_MAP_H
#define STRING_MAP_STRING_MAP_H

#include <cassert>
#include <string>
#include <vector>
//#include "string_map_iterators.h"

using std::string;
using std::vector;
using std::pair;

/**
 * Implementacion de map<string,T> sobre Trie
 * Asume de T:
 * - tiene constructor por copia 
 * - tiene operador ==
 * - solo permite utilizar el operator[] si T tiene constructor por defecto
 */
template < typename T >
class string_map {
    struct Nodo;
public:
    typedef string key_type;
    typedef T mapped_type;
    typedef std::pair<const key_type, mapped_type> value_type;
    typedef size_t size_type;

    class iterator;
    class const_iterator;

    /** @brief Construye mapa vacio
     *
     * \pre true
     * \post \P{this} = vacio
     *
     * \complexity{\O(1)}
     */
    string_map();

    /** @brief Destruye mapa
     *
     * \pre true
     * \post Se libera la memoria ocupada por \P{this}
     *
     * \complexity{\O(sn * S)}
     */
    ~string_map();

    /** @brief Constructor por copia
     *
     * \pre true
     * \post \P{this} es una copia de other. No hay aliasing.
     *
     * \complexity{\O(sn * S)}
     */
    string_map(const string_map &);

    /** @brief Operador de asignacion
     *
     * \pre true
     * \post \P{this} == other \LAND \P{res} es referencia a \P{this}
     *
     * \complexity{\O(sn * S)}
     */
    string_map& operator=(const string_map &);

    /** @brief Operador de comparacion igualdad
     *
     * \pre true
     * \post \P{res} = \P{this} == other
     *
     * \complexity{\O(sn * S)}
     */
    bool operator==(const string_map& otro) const;

    /** @brief Operador de comparacion desigualdad
     *
     * \pre true
     * \post \P{res} = not(\P{this} == other)
     *
     * \complexity{\O(sn * S)}
     */
    bool operator!=(const string_map& otro) const {return !(*this == otro);}

    /** @brief Cantidad de apariciones de la clave (0 o 1)
     *  @param key clave a buscar
     *
     * \pre true
     * \post \IF def?(\P{this}, k) \THEN \P{res} = 1 \ELSE \P{res} = 0 \FI
     *
     *  \complexity{\O(S)}
     */
    bool count(const key_type &key) const;

    /** @brief Devuelve cantidad de claves definidas
     *
     *  \pre true
     *  \post \P{res} = #claves(\P{this})
     *
     *  \complexity{\O(1)}
     */
    size_t size() const;

    /** @brief Devuelve true si no tiene claves definidas
     *
     * \pre true
     * \post \P{res} = (#claves(\P{this})== 0)
     *
     * \complexity{\O(1)}
     */
    bool empty() const;


    /** @brief Acceso / definición de pares clave/valor
     *  @param key clave a acceder, si no existe, se crea
     *  @returns una referencia a la definicion.
     *
     * \pre d == \P{this}
     * \post \IF not def?(\P{this}, k) \THEN definir(d, k, vacio) \FI
     *          \LAND \P{res} = obtener(k, \P{this})
     *
     *  \complexity{\O(S + copy(value_type)}
     */
    mapped_type &operator[](const key_type &key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia a la definicion.
     *
     * \pre def?(\P{this}, k)
     * \post \P{res} = obtener(k, \P{this})
     *
     *  \complexity{\O(S)}
     */
    mapped_type& at(const key_type& key);

    /** @brief Acceso a una clave sin modificar mapa
     *  @param key clave a acceder que debe existir previamente
     *  @returns una referencia const a la definicion.
     *
     * \pre def?(\P{this}, k)
     * \post \P{res} = obtener(k, \P{this})
     *
     *  \complexity{\O(S)}
     */ 
    const mapped_type& at(const key_type& key) const;

    /** @brief Vacia el mapa
     *
     * \pre true
     * \post \P{this} = vacio
     *
     * \complexity{\O(sn * S)}
     */
    void clear();

    // Accesos con iteradores

    /** @brief iterador al primer par <clave,significado> en orden lexicografico
     *  @returns iterador al elemento o end() si el mapa era vacio
     *
     * \pre true
     * \post \IF #claves(\P{this}) > 0 \THEN \P{res} apunta al inicio de \P{this} \ELSE \P{res} apunta a la posición pasando-el-último  \FI
     *
     *  \complexity{\O(S)}
     */
    iterator begin();

    /**  @brief iterador al fin de la coleccion
     *
     * \pre true
     * \post \P{res} apunta a la posición pasando-el-último
     *
     *  \complexity{\O(S)}
     */
    iterator end();

    /// Versiones const de begin/end

    /** @brief iterador const al primer par <clave,significado> en orden lexicografico
     *  @returns iterador const al elemento o end() si el mapa era vacio
     *
     * \pre true
     * \post \IF #claves(\P{this}) > 0 \THEN \P{res} apunta al inicio de \P{this} \ELSE \P{res} apunta a end \FI
     *
     *  \complexity{\O(S)}
     */
    const_iterator begin() const;

    /**  @brief iterador const al fin de la coleccion
     *
     * \pre true
     * \post \P{res} apunta a la posición pasando-el-último
     *
     *  \complexity{\O(S)}
     */
    const_iterator end() const;
    //const_iterator cbegin() const;
    //const_iterator cend() const;

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador al par <clave, significado> si k esta definido, de otro modo end()
     *
     * \pre true
     * \post \IF def?(\P{this}, k) \THEN \P1(\P{res}) = k \LAND \P2(\P{res}) = obtener(k, \P{this}) \ELSE \P{res} es end \FI
     *
     *  \complexity{\O(S)}
     */
    iterator find(const key_type &key);

    /** @brief busca una clave
     *  @param key clave a buscar
     *  @returns un iterador const al par <clave, significado>
     *
     * \pre true
     * \post \IF def?(\P{this}, k) \THEN \P1(\P{res}) = k \LAND \P2(\P{res}) = obtener(k, \P{this}) \ELSE \P{res} es end \FI
     *
     *  \complexity{\O(S)}
     */
    const_iterator find(const key_type &key) const;

    /** @brief insercion
     *
     * @param value par <clave,significado> a insertar
     * @returns un par con un iterador al par clave-significado agregado o
     * modificado y un bool que indica si la clave se insertó como una clave
     * nueva.
     *
     * \pre d = \P{this}
     * \post \post \P{this} = definir(d, \P1(v), \P2(v)) \LAND \P{res} es una tupla con
     * el primer elemento un iterador a la relación definida y el segúndo elemento
     * indica si \P1(v) no era clave del diccionario
     *
     * \complexity{\O(S + copy(value_type))}
     */
    pair<iterator,bool> insert(const value_type &value);

    /** @brief eliminar una clave
     *  @param key clave a eliminar
     *  @returns cantidad de elementos eliminados
     *
     * \pre d == \P{this} \LAND def?(k, \P{this})
     * \post \P{this} = borrar(k, d) \AND \P{res} = (#claves(d) - #claves(\P{this}))
     *
     *  \complexity{\O(S)}
     */
    size_type erase(const key_type& key);

    /** @brief eliminar una clave mediante irerador
     *  @param pos iterador apuntando a clave a eliminar
     *  @returns iterador apuntando el proximo de la clave eliminada (o end() si era la ultima)
     *
     * \pre d == \P{this} \LAND def?(k, \P{this})
     * \post \P{this} = borrar(k, d) \AND \P{res} apunta a la posicion siguiente al elemento borrado
     *
     *  \complexity{\O(S)}
     */
    iterator erase(iterator pos);


private:

    /**
   * \name Representación:
   *
   * rep: string_map(Clave, Significado) \TO bool\n
   * rep(d) \EQUIV true
     *
   *
   * abs: string_map(Clave, Significado) \TO Dicc(Clave, Significado)\n
   * abs(d) \EQUIV d' \|
   *  * \FORALL (c : Clave) def?(c, d') \IFF \EXISTS
   *    (t : tupla(Clave, Significado)) está?(t, _elems) \LAND P1(t) == c
   *  * \FORALL (t : tupla(Clave, Significado)) está?(t, _elems) \IMPLIES
   *    P2(t) == obtener(P1(t), d')
   */

    Nodo* _raiz;
    Nodo* _end = nullptr; //new Nodo("");


    struct Nodo{
        Nodo(string camino){
            _definido = false;
            _camino = new string(camino);
            _obtener = new T();
            v = new value_type(*_camino, *_obtener);
            padre = nullptr;
        }
        ~Nodo(){
            delete _camino;
            delete _obtener;
            //delete v;
            //delete padre;
        }
        //vector <char> _claves;
        vector <Nodo*> _hijos;
        T* _obtener;//ojo que obtener no se actualiza con v, esta de mas
        Nodo* padre;
        string* _camino;
        bool _definido;
        value_type* v;

        int posEnPadre(){
            for (int i = 0; i < padre->_hijos.size(); ++i) {
                if (padre->_hijos[i] == this)
                    return i;
            }
        }
    };

    size_type _tamano;

    friend class iterator;
    friend class const_iterator;

    /*class iterator{
        Nodo* _valorIt;
        friend class string_map;
    };*/
    /*class const_iterator{
        Nodo* _valorIt;
        friend class string_map;
    };*/
};

#include "string_map.hpp"
#include "string_map_iterators.h"

#endif //STRING_MAP_STRING_MAP_H
