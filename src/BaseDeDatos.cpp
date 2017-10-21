#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>

BaseDeDatos::BaseDeDatos() {};

void BaseDeDatos::crearTabla(const string &nombre,
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
    _nombres_tablas.fast_insert(nombre);
    _tablas.insert(pair<string, Tabla>(nombre, Tabla(claves, campos, tipos)));
}

void BaseDeDatos::agregarRegistro(const Registro &r, const string &nombre) {
    Tabla &t = _tablas.at(nombre);
    t.agregarRegistro(r);

    if (_indices.count(nombre) == 1){
        string_map<Indice> indices_tabla = _indices.at(nombre);
        for (auto it : t.campos()){
            if (indices_tabla.count(it) == 1){
                agregarAIndice(indices_tabla.at(it), r, it);
            }
        }
    }
}

void BaseDeDatos::agregarAIndice(BaseDeDatos::Indice &indice, const Registro &registro, const string &campo) {
    if (get<2>(indice)) {
        string valor_en_campo = registro.dato(campo).valorStr();
        auto it = get<1>(indice).find(valor_en_campo);
        if (it != get<1>(indice).end()) {
            linear_set<const Registro&> nuevo_conj = indice.at(it->first);
            nuevo_conj.fast_insert(registro);
            get<1>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
        } else {
            linear_set<const Registro&> nuevo_conj = linear_set<const Registro&>();
            nuevo_conj.fast_insert(registro);
            get<1>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
        }
    } else {
        int valor_en_campo = registro.dato(campo).valorNat();
        auto it = get<0>(indice).find(valor_en_campo);
        if (it != get<0>(indice).end()) {
            linear_set<const Registro&> nuevo_conj = indice.at(it->first);
            nuevo_conj.fast_insert(registro);
            get<0>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
        } else {
            linear_set<const Registro&> nuevo_conj = linear_set<const Registro&>();
            nuevo_conj.fast_insert(registro);
            get<0>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
        }
    }

}


const linear_set<string> &BaseDeDatos::tablas() const { return _nombres_tablas; }

const Tabla &BaseDeDatos::dameTabla(const string &nombre) const {
    return _tablas.at(nombre);
}

int BaseDeDatos::uso_criterio(const BaseDeDatos::Criterio &criterio) const {
    if (_uso_criterios.count(criterio)) {
        return _uso_criterios.at(criterio);
    } else {
        return 0;
    }
}

bool BaseDeDatos::registroValido(const Registro &r,
                                 const string &nombre) const {
    const Tabla &t = _tablas.at(nombre); //ahora O(1)

    bool camposIguales = true;

    for (auto it : t.campos() ) { //O(C)
        camposIguales &= r.campos().count(it);
    }

    for (auto it : r.campos() ) { // O(C)
        camposIguales &= t.tipos().count(it);
    }

    bool mismosTipos = true;
    if (camposIguales) { //O(C)
        for (auto it : t.campos() ) {
            mismosTipos &= t.tipoCampo(it).esNat() && r.dato(it).esNat();
        }
    }

    return (camposIguales and mismosTipos and
            _no_repite(r, t));
}

bool BaseDeDatos::_mismos_tipos(const Registro &r, const Tabla &t) const {
    for (auto c : t.campos()) {
        if (r.dato(c).esNat() != t.tipoCampo(c).esNat()) {
            return false;
        }
    }
    return true;
}

bool BaseDeDatos::_no_repite(const Registro &r, const Tabla &t) const {
    list<Registro> filtrados(t.registros().begin(), t.registros().end());
    for (auto clave : t.claves()) {
        _filtrar_registros(clave, r.dato(clave), filtrados);
    }
    return filtrados.empty();
}

list<Registro> &
BaseDeDatos::_filtrar_registros(const string &campo, const Dato &valor,
                                list<Registro> &registros) const {
    return _filtrar_registros(campo, valor, registros, true);
}

list<Registro> &BaseDeDatos::_filtrar_registros(const string &campo,
                                                const Dato &valor,
                                                list<Registro> &registros,
                                                bool igualdad) const {
    auto iter = registros.begin();
    while (iter != registros.end()) {
        auto now = iter;
        iter++;
        if ((not igualdad) xor now->dato(campo) != valor) {
            registros.erase(now);
        }
    }

    return registros;
}

pair<vector<string>, vector<Dato> > BaseDeDatos::_tipos_tabla(const Tabla &t) {
    vector<string> res_campos;
    vector<Dato> res_tipos;
    for (auto c : t.campos()) {
        res_campos.push_back(c);
        res_tipos.push_back(t.tipoCampo(c));
    }
    return make_pair(res_campos, res_tipos);
}

bool BaseDeDatos::criterioValido(const Criterio &c,
                                 const string &nombre) const {
    const Tabla &t = _tablas.at(nombre);
    for (auto restriccion : c) {
        if (not t.campos().count(restriccion.campo())) {
            return false;
        }
        if (t.tipoCampo(restriccion.campo()).esNat() !=
            restriccion.dato().esNat()) {
            return false;
        }
    }
    return true;
}

Tabla BaseDeDatos::busqueda(const BaseDeDatos::Criterio &c,
                            const string &nombre) {
    if (_uso_criterios.count(c)) {
        _uso_criterios.at(c)++;
    } else {
        _uso_criterios.fast_insert(make_pair(c, 1));
    }

    const Tabla &ref = dameTabla(nombre);
    auto campos_datos = _tipos_tabla(ref);
    Tabla res(ref.claves(), campos_datos.first, campos_datos.second);
    list<Registro> regs(ref.registros().begin(), ref.registros().end());
    for (auto restriccion : c) {
        _filtrar_registros(restriccion.campo(), restriccion.dato(),
                           regs, restriccion.igual());
    }
    for (auto r : regs) {
        res.agregarRegistro(r);
    }
    return res;
}

linear_set<BaseDeDatos::Criterio> BaseDeDatos::top_criterios() const {
    linear_set<Criterio> ret;
    int max = 0;
    for (auto crit_count : _uso_criterios) {
        if (crit_count.second >= max) {
            if (crit_count.second > max) {
                ret = linear_set<Criterio>();
                max = crit_count.second;
            }
            ret.fast_insert(crit_count.first);
        }
    }
    return ret;
}

void BaseDeDatos::crearIndice(const string &nombre, const string &campo) {
    Tabla t = dameTabla(nombre);

    string_map<Indice> nuevos_indices = string_map<Indice>();
    Indice indice = Indice();
    //Testear si sirve pasarle una tabla que ya tiene indice en ese campo
    auto it = t.registros_begin();

    if (t.tipoCampo(campo).esNat()) {
        linear_set<int> datos = linear_set<int>();
        while (it != t.registros_end()) {
            int dato_actual = (*it).dato(campo).valorNat();
            datos.insert(dato_actual);
            if (get<0>(indice).count(dato_actual) == 0) {
                linear_set<const Registro &> nuevo_conj_registros = linear_set<const Registro &>();
                const Registro &actual = *it;
                nuevo_conj_registros.fast_insert(actual);
                get<0>(indice).insert(make_pair(dato_actual, nuevo_conj_registros));
            } else {
                linear_set<const Registro &> conj_registros = get<0>(indice).at(dato_actual);
                conj_registros.fast_insert((*it));
                get<0>(indice).insert(make_pair(dato_actual, conj_registros));
            }
            ++it;
        }
        get<2>(indice) = false;
    } else {
        linear_set<string> datos = linear_set<string>();
        while (it != t.registros_end()) {
            string dato_actual = (*it).dato(campo).valorStr();
            datos.insert(dato_actual);
            if (get<1>(indice).count(dato_actual) == 0) {
                linear_set<const Registro &> nuevo_conj_registros = linear_set<const Registro &>();
                const Registro &actual = *it;
                nuevo_conj_registros.fast_insert(actual);
                get<1>(indice).insert(make_pair(dato_actual, nuevo_conj_registros));
            } else {
                linear_set<const Registro &> conj_registros = get<1>(indice).at(dato_actual);
                conj_registros.fast_insert((*it));
                get<1>(indice).insert(make_pair(dato_actual, conj_registros));
            }
            ++it;
        }
        get<2>(indice) = true;
    }

    nuevos_indices.insert(make_pair(campo, indice));
    _indices.insert(make_pair(nombre, nuevos_indices));

}