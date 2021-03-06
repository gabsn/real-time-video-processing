#include <systemc.h>

#include <cstdlib>

#define METHOD // Implémentation du producteur avec une SC_METHOD
#define CLK_PERIOD 15,SC_NS

struct Pixel {
    sc_uint<5> r;
    sc_uint<6> g;
    sc_uint<6> b;

    // Constructeur
    Pixel (sc_uint<5> _r=0, sc_uint<6> _g=0, sc_uint<5> _b=0): r(_r), g(_g), b(_b) {}

    // Surcharge d'opérateurs
    bool operator == (const Pixel &other) {
        return (r == other.r) && (g == other.g) && (b == other.b);
    }

    friend ostream& operator << (ostream& o, const Pixel& P) {
        o << "[" << P.r << "," << P.g << "," << P.b << "]" ;
        return o;
    }

    Pixel operator + (const Pixel p2) {
        Pixel ps(0,0,0);
        ps.r = (r + p2.r > 31) ? 31 : r + p2.r;
        ps.g = (g + p2.g > 63) ? 63 : g + p2.g;
        ps.b = (b + p2.b > 63) ? 63 : b + p2.b;

        return ps;
    }
};

SC_MODULE(Producteur_Thread) {
    sc_fifo_out<Pixel> out;
    sc_in<bool> clk;
    
    SC_CTOR(Producteur_Thread) { 
        SC_CTHREAD(main,clk.pos()); 
    }

    void main() {
        Pixel pixel(0,0,0);
        const Pixel pixel_3(1,1,1);
        for (int i=0; i<100; ++i) {
            out.write(pixel);
            pixel = pixel + pixel_3;
            wait();
        }
        const int N = rand() % 32;
        wait(N);
    }
};

SC_MODULE(Producteur_Method) {
    sc_fifo_out<Pixel> out;
    sc_in<bool> clk;
    sc_uint<5> N;
    sc_uint<7> count;
    Pixel * pixel;
    const Pixel * pixel_3;
   
    SC_CTOR(Producteur_Method) { 
        N = 0;
        count = 0;
        pixel = new Pixel(0,0,0);
        pixel_3 = new Pixel(3,3,3);

        SC_METHOD(main); 
        sensitive << clk.pos();

        SC_METHOD(wait_N);
        sensitive << clk.pos();
    }

    void main() {
        if (N == 0) {
            if (count < 100) {
                out.nb_write(*pixel);
                *pixel = *pixel + *pixel_3;
                ++count;
                if (count == 99) {
                    N = rand() % 32;
                    cout << "100 pixels sent" << endl;
                }
            }
        }
    }

    void wait_N() {
        if (N < 0)
            N--;
    }
};

SC_MODULE(Consommateur) {
    sc_fifo_in<Pixel> in;
    sc_in<bool> clk;

    SC_CTOR(Consommateur) { 
        SC_CTHREAD(main,clk.pos()); 
    }

    void main() {
        while (1) {
            Pixel pixel = in.read();
            if (pixel.r % 2 == 0)
                wait(2);
            else
                wait(3);
            //cout << pixel << endl;
        }
    }
};

int sc_main(int, char **) {
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",sc_core::SC_DO_NOTHING );
    sc_fifo<Pixel> fifo;

    sc_clock clk("clk",CLK_PERIOD);

    Consommateur conso("module_conso");
    conso.in(fifo);
    conso.clk(clk);

#if defined(METHOD) 
    Producteur_Method prod_method("module_prod_method");
    prod_method.out(fifo);
    prod_method.clk(clk);
#else
    Producteur_Thread prod_thread("module_prod_thread");
    prod_thread.out(fifo);
    prod_thread.clk(clk);
#endif

    sc_start(300,SC_NS);
    cout 
        << "------------------------------------" << endl
        << "contenu de la fifo @" << sc_time_stamp() << " : " << endl
        << fifo 
        << "------------------------------------" << endl << endl;

    sc_start(500,SC_NS);
    cout 
        << "------------------------------------" << endl
        << "contenu de la fifo @" << sc_time_stamp() << " : " <<  endl
        << fifo
        << "------------------------------------" << endl << endl;

    sc_start(1,SC_US);
    cout 
        << "------------------------------------" << endl
        << "contenu de la fifo @" << sc_time_stamp() << " : " << endl
        << fifo
        << "------------------------------------" << endl << endl;

    return 0;
}
