#include <systemc.h>

#define endl '\n'

using namespace std;

int sc_main (int argc, char * argv[]) {
    /**** LOGIC ****/
    sc_logic u,v,w;
    for (int i=0; i<4; ++i) {
        u = i;
        //cout << u << endl;
    }
    // Runtime error if i >= 4
    
    // double a = 0 throws an error
    int a = 0;
    u = a;
    //cout << u << endl;

    // using bool instead of sc_bit
    bool b = 1;
    v = b;
    //cout << v << endl;

    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            w = ~ sc_logic(j);
            //w = sc_logic(i) | sc_logic(j);
            //w = sc_logic(i) ^ sc_logic(j);
            //w = sc_logic(i) & sc_logic(j);
            //cout << w << " ";
        }
        //cout << endl;
    }

    /**** VECTOR ****/
    sc_bv<8> bv;
    sc_bv<4> c,d;

    for (int i=0; i<8; ++i)
        bv[i] = i%2;

    (c,d) = bv;
    c = 0;
    bv = (c,d);

    bv = 44;
    //cout << "bv---> " << bv << "(" << bv.to_uint() << ")" << endl;
    bv = 300;
    //cout << "bv---> " << bv << "(" << bv.to_uint() << ")" << endl;

    bv = false;
    //cout << bv << endl;

    sc_lv<8> lv;
    lv = false;
    lv(5,3) = true;
    //cout << lv << endl;
    lv = false;
    lv(5,3) = 1111111;
    //cout << lv << endl;
    
    return 0;
}
