#ifndef _BASEDEDATOS_H
#define _BASEDEDATOS_H

#include "Registro.h"
#include "Restriccion.h"
#include "Tabla.h"
#include <utility>
#include <list>
#include <string>
#include "linear_map.h"
#include "linear_set.h"
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

public:
  /** @brief Criterio de búsqueda para una base de datos */
  typedef linear_set<Restriccion> Criterio;

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
   * \complexity{\O(T + copy(reg))}
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
   * \complexity{O(T)}
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
   * \complexity{\O(T + C^2 + (c * C + c * n * (C + L)))}
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
   * \complexity{\O(T + cr * C)}
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
     *
     * abs: registro \TO Registro\n
     * abs(r) \EQUIV r' \|
     *  * campos(r') = _campos \LAND
     *  * \FORALL (c : string) c \in _campos \IMPLIES valor(c, r') = valor(c,
     *    _datos) 
     */
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /** @{ */
    linear_set<string> _nombres_tablas;
    linear_map<string, Tabla> _tablas;
    linear_map<Criterio, int> _uso_criterios;
    /** @} */

    /** @{ */
    /**
     * @brief Revisa si los campos del registro y la tabla tienen el mismo tipo.
     *
     * \pre campos(r) == campos(t)
     * \post \P{res} == \FORALL (c : campo) c \IN campos(r) \IMPLIES
     * Nat?(valor(c, r)) == tipoCampo(c, t)
     *
     * \complexity{O(C^2)}
     */
    bool _mismos_tipos(const Registro &r, const Tabla &t) const;

    /**
     * @brief Revisa si el registro no repite claves en la tabla.
     *
     * \pre compatible(r, t)
     * \post \P{res} = \FORALL (r' : Registro) r \IN registros(t) \IMPLIES
     *  \EXISTS (c : campo) c \IN claves(t) \LAND valor(c, r') != valor(c, r)
     *
     * \complexity{O(c * C + c * n * (C + L))}
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
    /** @} */
};

#endif
