#include "BaseDeDatos.h"
#include <list>
#include <tuple>
#include <algorithm>


BaseDeDatos::BaseDeDatos() {_ultimo_join = new Join();};

void BaseDeDatos::crearTabla(const string &nombre,
                             const linear_set<string> &claves,
                             const vector<string> &campos,
                             const vector<Dato> &tipos) {
    _nombres_tablas.fast_insert(nombre);
    _tablas.insert(make_pair(nombre, Tabla(claves, campos, tipos)));
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
        linear_set<const Registro*> nuevo_conj = linear_set<const Registro*>();
        auto it = get<1>(indice).find(valor_en_campo);
        if (it != get<1>(indice).end()) {
            nuevo_conj = get<1>(indice).at(it->first);
        }
        nuevo_conj.fast_insert(&registro);
        get<1>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
    } else {
        int valor_en_campo = registro.dato(campo).valorNat();
        linear_set<const Registro *> nuevo_conj = linear_set<const Registro *>();
        auto it = get<0>(indice).find(valor_en_campo);
        if (it != get<0>(indice).end()) {
            nuevo_conj = get<0>(indice).at(it->first);
        }
        nuevo_conj.fast_insert(&registro);
        get<0>(indice).insert(make_pair(valor_en_campo, nuevo_conj));
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
            mismosTipos &= t.tipoCampo(it).esNat() == r.dato(it).esNat();
        }
    }

    return (camposIguales and _mismos_tipos(r,t) and
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
    const Tabla &t = dameTabla(nombre);

    string_map<Indice> nuevos_indices = string_map<Indice>();
    if (_indices.count(nombre)){ //Si la tabla ya tiene indices los cargo
        nuevos_indices = _indices.at(nombre);
    }
    Indice indice = Indice(); //Creo un indice nuevo
    auto it = t.registros_begin();

    if (t.tipoCampo(campo).esNat()) { //Si es un campo Nat
        while (it != t.registros_end()) { //Itero sobre los registros de la tabla
            int dato_actual = (*it).dato(campo).valorNat(); //Copio el dato del registro actual en el campo param
            linear_set<const Registro*> conj_registros = linear_set<const Registro*>(); //Creo un conjunto nuevo de punteros a registro
            if (get<0>(indice).count(dato_actual) != 0) { //Si ya habia registros con el mismo valor en el campo tomo ese conjunto
                conj_registros = get<0>(indice).at(dato_actual);
            }
            conj_registros.fast_insert(&*it); //Le agrego al conjunto un puntero al registro sobre el que estoy iterando
            get<0>(indice).insert(make_pair(dato_actual, conj_registros)); //Inserto el nuevo conjunto en el indice para ese dato
            ++it;
        }
        get<2>(indice) = false;
    } else { //Si es un campo str, misma idea
        while (it != t.registros_end()) {
            string dato_actual = (*it).dato(campo).valorStr();
            linear_set<const Registro*> conj_registros = linear_set<const Registro*>();
            if (get<1>(indice).count(dato_actual) != 0) {
                conj_registros = get<1>(indice).at(dato_actual);
            }
            conj_registros.fast_insert(&*it);
            get<1>(indice).insert(make_pair(dato_actual, conj_registros));
            ++it;
        }
        get<2>(indice) = true;
    }

    nuevos_indices.insert(make_pair(campo, indice));
    _indices.insert(make_pair(nombre, nuevos_indices));
}

BaseDeDatos::join_iterator BaseDeDatos::join(const string &tabla1, const string &tabla2, const string &campo) {
    *_ultimo_join = Join(); //Limpio el valor de ultimo join
    //Si tabla2 no tiene indices doy vuelta las cosas, tabla1 tendra indice por precondicion
    if (_indices.count(tabla2) == 0  or _indices.at(tabla2).count(campo) == 0) {
        return join(tabla2, tabla1, campo);
    }

    //Ahora si, tabla2 seguro tiene indice
    const Tabla& t1 = dameTabla(tabla1);
    const Tabla& t2 = dameTabla(tabla2);
    Indice indice = _indices.at(tabla2).at(campo);
    //Itero sobre los registros de la tabla 1
    auto it = t1.registros_begin();
    //En caso de estar tratando con un campo nat:
    if (!get<2>(indice)) {
        while (it != t1.registros_end()) {
            int dato_actual = it->dato(campo).valorNat(); //Me fijo que valor tiene el registro actual en el campo
            auto find_dato = get<0>(indice).find(dato_actual); //Busco que registros tienen ese valor en la tabla2
            if (find_dato != get<0>(indice).end()) { //Si hay registros con ese valor en la tabla dos:
                _ultimo_join->tabla1.fast_insert(&*it); //Agrego un puntero al registro de la tabla1 y al conjunto de registros de la tabla2
                _ultimo_join->tabla2.fast_insert(&find_dato->second);
            }
            ++it;
        }
    } else {
        //En caso de estar tratando con un campo string (misma idea):
        while (it != t1.registros_end()) {
            string dato_actual = it->dato(campo).valorStr();
            auto find_dato = get<1>(indice).find(dato_actual);
            if (find_dato != get<1>(indice).end()) {
                _ultimo_join->tabla1.fast_insert(&*it);
                _ultimo_join->tabla2.fast_insert(&find_dato->second);
            }
            ++it;
        }
    }
    return _ultimo_join->begin();
}

BaseDeDatos::join_iterator BaseDeDatos::Join::begin (){
    if (tabla1.empty()) {
        return end();
    }
    auto first = tabla1.begin();
    auto second = (*tabla2.begin())->begin();
    return join_iterator(make_pair(first, second), tabla1.size(), 0,tabla2);
}

BaseDeDatos::join_iterator& BaseDeDatos::join_iterator::operator++(){
    bool esUltimo = false;
    ++v.second;
    auto it = tabla2.begin();
    while (it != tabla2.end()) { //Me fijo si v.second es el end de alguno de los conjuntos de la tabla 2, para pasar al proximo si es asi
        if (v.second == (*it)->end() ) {
            esUltimo = true;
            ++it;
            break;
        }
        ++it;
    }

    if(esUltimo){
        iteraciones++;
        ++v.first;
        if (!termino()) { //it != tabla2.end() &&
            v.second = (*it)->begin();
        } else {
            v.second = linear_set<const Registro*>().begin();
        }
    }
    return *this;
}

//esto es para que el compilador distinga entre ++it e it++
BaseDeDatos::join_iterator BaseDeDatos::join_iterator::operator++(int) {
    join_iterator temp(*this);
    operator++();
    return temp;
}


Registro BaseDeDatos::join_iterator::operator*() const {
    Registro reg1 = *(*v.first);
    Registro reg2 = *(*v.second);
    vector<string> campos;
    vector<Dato> datos;
    for (auto it : reg1.campos()) {
        campos.push_back(it);
        datos.push_back(reg1.dato(it));
    }
    for (auto it : reg2.campos()) {
        campos.push_back(it);
        datos.push_back(reg2.dato(it));
    }
    return Registro(campos, datos);
}

bool BaseDeDatos::join_iterator::operator==(const join_iterator &other) const {
    return (this->termino() && other.termino()) || other.v == this->v;
}

bool BaseDeDatos::join_iterator::operator!=(const join_iterator &other) const {
    return !(*this == other);
}

BaseDeDatos::join_iterator BaseDeDatos::join_iterator::operator=(const BaseDeDatos::join_iterator &other) {
    return join_iterator(other);
}

bool BaseDeDatos::join_iterator::termino() const {
    return iteraciones >= max_iter;
}

BaseDeDatos::join_iterator BaseDeDatos::Join::end() {
    join_iterator res = join_iterator(make_pair(tabla1.end(), linear_set<const Registro *>().end()), tabla1.size(), tabla1.size(), tabla2);
    res.iteraciones = res.max_iter;
    return res;
}

BaseDeDatos::join_iterator BaseDeDatos::join_end() {
    return _ultimo_join->end();
}


