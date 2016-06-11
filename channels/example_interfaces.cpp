#include <systemc.h>

//Une interface pour l'écriture
class tag_write_if : virtual public sc_interface {
    public:
        virtual void write(sc_uint<8> i) =0;
};

// Une interface pour la lecture
class tag_read_if : virtual public sc_interface {
    public:
        virtual sc_uint<16> read() =0;
};

// notre canal personnalisé, implémente les deux interfaces
// ce canal est semblable au sc_signal avec:
//    - des données de taille fixe
//    - un tag ajouté pour chaque nouvelle donnée
class tagged_bus : public tag_write_if, public tag_read_if, public sc_prim_channel {
    sc_uint<8> tag;
    sc_uint<8> cur_d, new_d;
    sc_event   m_ev;

    public:
    SC_CTOR(tagged_bus) {
        tag   = 0;
        cur_d = 0;
        new_d = 0;
    }
    // dans tag_write_if
    virtual void write(sc_uint<8> i) {
        new_d = i;
        // on demande à ce qu'update soit appelé
        request_update();
    }
    // dans tag_read_if
    virtual sc_uint<16> read() {
        return (tag,cur_d); 
    }
    // dans sc_interface
    virtual const sc_event& default_event() const {
        return m_ev;
    }
    // dans sc_prim_channel
    virtual void update() {
        if (cur_d != new_d) {
            cur_d = new_d;
            tag++;
            m_ev.notify(SC_ZERO_TIME);
        }
    }
};

SC_MODULE(W) {
    // un port en sortie n'implémente que l'interface d'écriture
    sc_port<tag_write_if> p_o;

    SC_CTOR(W) {
        SC_THREAD(loop);
    }
    void loop () {
        sc_uint<8> v = 1;
        while(v) {
            // on appelle la méthode write de l'interface
            p_o->write(v);
            v = v<<1;
            wait(10, SC_NS);
        }
    }
};

SC_MODULE(R) {
    // un port en entrée n'implémente que l'interface de lecture
    sc_port<tag_read_if> p_i;

    SC_CTOR(R) {
        SC_METHOD(loop);
        // Utilise le default_event
        sensitive << p_i;
    }
    void loop () {
        // on appelle la méthode read de l'interface
        sc_uint<16> t = p_i->read();
        cout << name() <<" --> tag: " << t(15,8) << " val: " << sc_bv<8>(t(7,0))<< endl;
    }
};

int sc_main(int argc, char * argv[]) {
    tagged_bus b("t_bus");

    W w("writer");
    w.p_o(b);

    R r("reader");
    r.p_i(b);

    sc_start();

    return 0;
}
