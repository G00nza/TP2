#ifndef _BASEDEDATOS_H
#define _BASEDEDATOS_H



#include "Registro.h"
#include "Restriccion.h"
#include "Tabla.h"
#include <utility>
#include <list>
#include <string>
#include <map>
#include "linear_map.h"
#include "linear_set.h"
#include "string_map.h"
#include "utils.h"

using namespace std;

/**
 * @brief Una base de datos es un administrador de tablas con funciones de
 * búsqueda.
 *
 * Una base de datos permite administrar tablas identificadas por registro.
 * Permite saber si se puede agegar un registro a una tabla y luego agregarlo.
 * Permite realizar filtros del contenido de tablas mediante criterios de
 * búsqueda. Además mantiene estadísticas del uso de los criterios.
 *
 * **se explica con** TAD BaseDeDatos
 */
class BaseDeDatos {
    class Join;
    struct join_iterator;

public:
    /** @brief Criterio de búsqueda para una base de datos */
    typedef linear_set<Restriccion> Criterio;
    /** @brief Indice para un campo dado, el bool es false si es sobre int, true si es sobre string */
    typedef tuple<map<int, linear_set<const Registro*> >, string_map<linear_set<const Registro*> >, bool > Indice;


    /**
     * @brief Inicializa una base de datos sin tablas.
     *
     * \pre true
     * \post \P{this} = nuevaDB
     *
     * \complexity{\O(1)}
     */
    BaseDeDatos();

    /**
     * @brief Crea una nueva tabla en la base de datos.
     *
     * @param nombre Nombre identificador de la tabla
     * @param claves Claves de la tabla a crear
     * @param campos Campos de la tabla a crear
     * @param tipos  Tipos para los campos de la tabla a crear
     *
     * \pre db = \P{this} \LAND
     *      \LNOT (nombre \IN tablas(\P{this})) \LAND
     *      \LAND \LNOT \EMPTYSET?(claves) \LAND
     *      \FORALL (c: campo) c \IN claves \IMPLICA c \IN campos \LAND
     *      long(campos) = long(tipos) \LAND sinRepetidos(campos)
     * \post \P{this} = agregarTabla(nuevaTabla(claves, nuevoRegistro(campos, tipos)), db)
     *
     * \complexity{\O(C)}
     */
    void crearTabla(const string &nombre, const linear_set<string> &claves,
                    const vector<string> &campos, const vector<Dato> &tipos);

    /**
     * @brief Agrega un registro a la tabla parámetro
     *
     * @param r Registro a agregar
     * @param nombre Nombre de la tabla donde se agrega el registro
     *
     * \pre db = \P{this} \LAND nombre \IN tablas(\P{this}) \LAND
     *      puedoInsertar?(r, dameTabla(\P{this}))
     * \post \P{this} = insertarEntrada(r, nombre, db)
     *
     * \complexity{\O(copy(reg) + C*(L+log(m)))}
     */
    void agregarRegistro(const Registro &r, const string &nombre);

    /**
     * @brief Devuelve el conjunto de tablas existentes en la base.
     *
     * El conjunto de nombres se devuelve por referencia no-modificable.
     *
     * \pre true
     * \post \P{res} = tablas(\P{this})
     *
     * \complexity{\O(1)}
     */
    const linear_set<string> &tablas() const;

    /**
     * @brief Devuelve la tabla asociada al nombre.
     *
     * La tabla se devuelve por referencia no modificable.
     *
     * @param nombre Nombre de la tabla buscada.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = dameTabla(nombre, \P{this})
     *
     * \complexity{O(1)}
     */
    const Tabla &dameTabla(const string &nombre) const;

    /**
     * @brief Devuelve la cantidad de usos que tiene un criterio
     *
     * @param criterio Criterio por el cual se consulta.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = usoCriterio(criterio, \P{this})
     *
     * \complexity{\O(#cs * cmp(Criterio))}
     */
    int uso_criterio(const Criterio &criterio) const;

    /**
     * @brief Evalúa si un registro puede ingresarse en la tabla parámetro.
     *
     * @param r Registro a ingresar en la tabla.
     * @param nombre Nombre de la tabla.
     *
     * \pre nombre \IN tablas(\P{this})
     * \post \P{res} = puedoInsertar?(r, dameTabla(nombre, \P{this}))
     *
     * \complexity{\O(C + (c*n*L))}
     */
    bool registroValido(const Registro &r, const string &nombre) const;

    /**
     * @brief Evalúa si un criterio puede aplicarse en la tabla parámetro.
     *
     * @param c Criterio a utilizar.
     * @param nombre Nombre de la tabla.
     *
     * \pre tabla \IN tablas(\P{this})
     * \post \P{res} = criterioValido(c, nombre, \P{this})
     *
     * \complexity{\O(cr * C)}
     */
    bool criterioValido(const Criterio &c, const string &nombre) const;

    /**
     * @brief Devuelve el resultado de buscar en una tabla con un criterio.
     *
     * @param c Criterio de búsqueda utilizado.
     * @param nombre Nombre de la tabla.
     *
     * \pre nombre \IN tablas(\P{this}) \LAND criterioValido(c, nombre, \P{this})
     * \post \P{res} = buscar(c, nombre, \P{this})
     *
     * \complexity{\O(T + cs * cmp(Criterio) + cr * n * (C + L + copy(reg)))}
     */
    Tabla busqueda(const Criterio &c, const string &nombre);

    /**
     * @brief Devuelve los criterios de máximo uso.
     *
     * \pre true
     * \post \FORALL (c : Criterio) [c \IN \P{res} \IFF
     *       \FORALL (c' : Criterio) usoCriterio(c, db) >= usoCriterio(c', db)]
     *
     * \complexity{\O(cs * copy(Criterio))}
     */
    linear_set<Criterio> top_criterios() const;

    /**
     * @brief Crea un índice para una tabla de la base de datos en un campo de la misma.
     *
     * @param nombre Nombre de la tabla
     * @param campo Nombre del campo
     *
     * \pre nombre \IN _nombres_tablas \LAND campo \IN campos(dameTabla(nombre,bd)) \LAND bd = bd'
     * \post tieneIndice?(nombre, campo, bd)
     *
     * \complexity{\O(m[L+log(m)])}
     */
    void crearIndice(const string &nombre, const string &campo);


    /**
     * @brief Crea un join entre las dos tablas y devuelve join_begin(res)
     *
     * @param tabla1 Nombre de la tabla 1
     * @param tabla2 Nombre de la tabla 2
     * @param campo Nombre del campo sobre el cual realizar el join
     *
     * \pre tabla1 \IN _nombres_tablas \LAND campo \IN campos(dameTabla(tabla1)) \LAND
     *      tabla2 \IN _nombres_tablas \LAND campo \IN campos(dameTabla(tabla2)) \LAND
     *      (tieneIndice?(tabla1, campo, bd) \OR tieneIndice?(tabla2, campo, bd))
     * \post El iterador itera sobre los registros de join(t,t2,c,db) en un orden no definido
     *
     * \complexity{\O(n*[L+log(m)])}
     */
    join_iterator join(const string &tabla1, const string &tabla2,
                       const string &campo);

    /**
    * @brief Devuelve un iterador "end" del ultimo join realizado en la base de datos
    *
    * \pre True
    *
    * \post res = this._ultimo_join->end()
     *
    * \complexity{\O(1)}
    */
    join_iterator join_end();
private:
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /** \name Representación
     * rep: basededatos \TO bool\n
     * rep(bd) \EQUIV 
     *  * _nombres_tablas = claves(_tablas) \LAND
     *  * \FORALL (c : Criterio) c \IN claves(_uso_criterios) \IMPLIES 
     *     * (
     *       * \EXISTS (n : string) n \IN _nombres_tablas 
     *       * \LAND criterioValido(c, n, db)
     *     * ) \LAND
     *     * obtener(c, _uso_criterios) > 0
     *   * \FORALL (t : string) def?(t , _indices) \IMPLIES def?(t , _tablas) \LAND
     *     * (
     *       * \FORALL (c : string) def? (c , obtener(t , _indices)) \IMPLIES
     *         *  c \IN campos(obtener(t , _tablas)) \LAND
     *         * (
     *           * \FORALL (d : Dato) def?(d, obtener(c, obtener(t , _indices))) \IMPLIES
     *             *  \LNOT vacio?(obtener(d, obtener(c, obtener(t , _indices)))) \LAND
     *             * (
     *               * \FORALL (r : Registro) r \IN obtener(d, obtener(c, obtener(t , _indices))) \IMPLIES
     *                 *  r \IN registros (obtener(t, _tablas)) \LAND valor(c, r) = d
     *             * )
     *         * )
     *     * )
     *
     * abs: basededatos \TO BaseDeDatos\n
     * abs(bd) \EQUIV bd' \|
     *  * _nombres_tablas = tablas(bd') \LAND
     *  * (\FORALL nt : string) nt \IN _nombres_tablas \IMPLIES
     *    * obtener(nt, _tablas) = dameTabla(nt, bd') \LAND
     *  * (\FORALL c : criterio) 
     *    * (usoCriterio(c, bd') == 0 \LAND \LNOT def?(c, _uso_criterios)) \LOR
     *    * (usoCriterio(c, db') == obtener(c, _uso_criterios)) \LAND
     *  * (\FORALL t : string) t \IN tablas(bd') \LAND
     *  * (\FORALL c : string) c \IN campos(dameTabla(t, bd'))
     *    * tieneIndice?(t, c, bd') == def?(c, obtener(t, _indices)) 
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _nombres_tablas;
    string_map<Tabla> _tablas;
    linear_map<Criterio, int> _uso_criterios;
    string_map<string_map<Indice> >_indices;

    //Decidi implementarlo como que se guarda un puntero al último Join hecho asi no se invalida el iterador que devuelve join
    Join* _ultimo_join;
    /** @} */

    /** @{ */
    /**
     * @brief Revisa si los campos del registro y la tabla tienen el mismo tipo.
     *
     * \pre campos(r) == campos(t)
     * \post \P{res} == \FORALL (c : campo) c \IN campos(r) \IMPLIES
     * Nat?(valor(c, r)) == tipoCampo(c, t)
     *
     * \complexity{O(C)}
     */
    bool _mismos_tipos(const Registro &r, const Tabla &t) const;

    /**
     * @brief Revisa si el registro no repite claves en la tabla.
     *
     * \pre compatible(r, t)
     * \post \P{res} = \FORALL (r' : Registro) r \IN registros(t) \IMPLIES
     *  \EXISTS (c : campo) c \IN claves(t) \LAND valor(c, r') != valor(c, r)
     *
     * \complexity{O(c * n * L)}
     */
    bool _no_repite(const Registro &r, const Tabla &t) const;

    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, igualdad), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                       list<Registro> &registros,
                                       bool igualdad) const;

    /**
     * @brief Filtra la lista de registros parametro según el criterio.
     *
     * El resultado tiene aliasing con el parámetro registros.
     *
     * \pre \FORALL (r : Registro) r \IN registros \IMPLIES campo \IN
     *      campos(r) \LAND tipo?(valor(campo, r)) = tipo?(valor)
     * \post \P{res} = filtrarRegistrosSegunRestriccion(
     *       nueva(campo, valor, true), registros)
     */
    list<Registro> &_filtrar_registros(const string &campo, const Dato &valor,
                                       list<Registro> &registros) const;

    /**
     * @brief Obtiene los campos y tipos de una tabla.
     *
     * \pre true
     * \post (\FORALL (c : Campo) está?(c, \P1(\P{res})) \IFF c \IN campos(t))
     *       \LAND #(campos(t)) = long(\P1(\P{res}))
     *       \LAND \FORALL (i : Nat) 0 \LEQ i < #(campos(t)) \IMPLIES
     *       tipo?(\P2(\P{res})[i]) = tipoCampo(\P1(\P{res})[i], t)
     */
    pair<vector<string>, vector<Dato> > _tipos_tabla(const Tabla &t);


    /**
    * @brief Agrega un registro a un índice dado.
    *
    * @param indice Indice a modificar
    * @param registro Registro a agregar
    * @param campo Nombre del campo del indice
    *
    * \pre (\FORALL r: Registro) ((\EXISTS d: Dato) def?(d, indice) \LAND obtener(d, indice) = r) \IMPLIES
    *      campos(r) = campos(registro) \LAND (\FORALL c: campo) c \IN campos(r) \IMPLIES tipo(valor(c,r)) = tipo(valor(c, registro))
    *
    * \post (def?(valor(campo, registro), indice) \IMPLIES indice = definir(valor(campo, registro), Ag(registro, obtener(valor(campo,registro))), indice) )\LAND
    *       ¬def?(valor(campo, registro), indice) \IMPLIES indice = definir(valor(campo, registro), registro)
    *
    *
    */
    void agregarAIndice(Indice& indice, const Registro &registro, const string &campo);


    /** @brief Join entre dos tablas*/
    class Join {

        /** @brief Devuelve un iterador al primer par de registros del Join*/
        join_iterator begin();

        /** @brief Devuelve un iterador al ultimo par de registros del Join*/
        join_iterator end();

    private:

        /** @brief Conjunto con los registros de la tabla1*/
        linear_set<const Registro*> tabla1;

        /** @brief Conjunto con los registros de la tabla2 que coinciden con cada registro de la tabla1*/
        linear_set<linear_set<const Registro*>* > tabla2;

        /** @brief Me dice cual de las dos tablas fue usada para buscar en indice 0 = tabla1, 1 = tabla2*/
        bool tabla_indexada;


        friend class BaseDeDatos;
    };

    struct join_iterator {

        /** @brief Constructor del iterador*/
        join_iterator(pair<linear_set<const Registro*>::iterator, linear_set<const Registro*>::iterator> v,
                      int max_iter, int iteraciones,
                      linear_set<linear_set<const Registro*>* >& tabla2, bool tabla_indexada):
                v(v), max_iter(max_iter), iteraciones(iteraciones), tabla2(tabla2), tabla2_it(tabla2.begin()), tabla_indexada(tabla_indexada){};

        /** @brief Constructor por copia*/
        join_iterator(const join_iterator &other):
                v(other.v), max_iter(other.max_iter), iteraciones(other.iteraciones), tabla2(other.tabla2), tabla2_it(other.tabla2_it) {
            tabla_indexada = other.tabla_indexada;
        };

        /** @brief Par de registros tabla1,tabla2*/
        pair<linear_set<const Registro*>::iterator, linear_set<const Registro*>::iterator> v;

        /** @brief Cantidad de registros en tabla1*/
        int max_iter;

        /** @brief Indice en el que me encuentro sobre los registros de tabla1*/
        int iteraciones;

        /** @brief Me dice cual de las dos tablas fue usada para buscar en indice 0 = tabla1, 1 = tabla2*/
        bool tabla_indexada;

        /** @brief Referencia a los conjuntos de la tabla2*/
        linear_set<linear_set<const Registro*>* >& tabla2;

        /** @brief Iterador de this->tabla2*/
        linear_set<linear_set<const Registro*>* >::iterator tabla2_it;

        /** @brief Retorna true sii estoy en el end()*/
        bool termino() const;

        /** @brief Operador avanzar (prefix)*/
        join_iterator& operator++();

        /** @brief Operador avanzar (postfix) (El parametro int no se usa porque C++)*/
        join_iterator operator++(int);

        /** @brief Operador asignacion*/
        join_iterator operator=(const join_iterator &other);

        /** @brief Devuelve el registro del Join entre las dos tablas sobre el cual estoy iterando*/
        Registro operator*() const;

        /** @brief Igualdad de iteradores*/
        bool operator==(const join_iterator& other) const;
        /** @brief Desigualdad de iteradores*/
        bool operator!=(const join_iterator& other) const;

        friend class BaseDeDatos;

    };

    /** @} */
};


#endif
