#include <systemc.h>

using namespace std;

int sc_main (int argc, char * argv[]) {
    int N;

    if (argc == 2) {
        stringstream arg1(argv[1]);
        arg1 >> N;
        if (N < 0 || N > 255) {
            cout << "N doit être compris entre 0 et 255" << endl;
            exit(1);
        }
    } else {
        cout
           << endl
           << "Il faut passer une entier N en argument"
           << endl;
        exit(1);
    }

    //Un pointeur sur l'objet qui permet de gérer les traces
    sc_trace_file *trace_f;

    // Cette fonction crée l'objet
    // L'argument est le nom du fichier qui sera créer.
    // L'extension .vcd est ajoutée automatiquement
    trace_f = sc_create_vcd_trace_file ("my_simu_trace");

    // On peut aussi préciser l'unité de temps dans le fichier vcd
    trace_f->set_time_unit(1,SC_NS);

    // On trace la valeur de i
    int i;
    sc_trace(trace_f, i, "i");

    for (i=0; i<N; ++i)
        sc_start(10,SC_NS);
    sc_start(10,SC_NS);

    // Ferme le fichier de trace
    // ne peut êter fait qu'à la fin de la simulation
    sc_close_vcd_trace_file(trace_f);

    return 0;
}
