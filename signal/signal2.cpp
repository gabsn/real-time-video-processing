#include <systemc.h>

int sc_main (int argc, char * argv[])
{
    sc_signal<int> i("i");
    sc_signal<int> j("je m'appelle j");
    sc_signal<int> x;

    cout << "--> " << i.name() << ": " << i << endl;
    cout << "--> " << j.name() << ": " << j << endl;
    cout << "--> " << x.name() << ": " << x << endl;

    return 0;
}
