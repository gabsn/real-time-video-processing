#include <systemc.h>

int sc_main (int argc, char * argv[]) {
    sc_time T(0.333, SC_NS);

    cout << "On commence à " << sc_time_stamp() << " " << sc_delta_count() << endl;

    sc_start(T);
    cout << " ... " << sc_time_stamp() << " " << sc_delta_count() << endl;

    sc_start(0.333,SC_NS);
    cout << " ... " << sc_time_stamp() << " " << sc_delta_count() << endl;

    sc_start(SC_ZERO_TIME);
    cout << " ... " << sc_time_stamp() << " " << sc_delta_count() << endl;

    sc_start();
    cout << " ... " << sc_time_stamp() << " " << sc_delta_count() << endl;

    return 0;
}
