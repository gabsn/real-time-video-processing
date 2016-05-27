#include <systemc.h>

//un type utilisateur
struct pt_t {
    int i;
    int j;
    // un constructeur particulier avec des valeurs par défaut
    pt_t( int _i=0, int _j=1): i(_i), j(_j) { }

    bool operator == (const pt_t &other) {
        return (i == other.i) && (j == other.j);
    }
    // Comment ce type est imprimé
    // l'opérateur << est un opérateur de la classe std::ostream
    friend ostream& operator << ( ostream& o, const pt_t& P ) {
        o << "{" << P.i << "," << P.j << "}" ;
        return o;
    }
};

// surcharge de la fonction sc_trace pour le type utilisateur
void sc_trace( sc_trace_file* _f, const pt_t& _foo, const std::string& _s ) {
    sc_trace( _f, _foo.i, _s + "_i" );
    sc_trace( _f, _foo.j, _s + "_j" );
}

// Le test
int sc_main (int argc, char * argv[])
{
    sc_signal<pt_t> P;
    cout << "--> @ " << sc_time_stamp() << " P = " << P << endl;

    // affectation au signal
    P = pt_t(33,22);
    cout << "--> @ " << sc_time_stamp() << " P = " << P << endl;

    sc_start(1,SC_NS);
    cout << "--> @ " << sc_time_stamp() << " P = " << P << endl;

    return 0;
}
