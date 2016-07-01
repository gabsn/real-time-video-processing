/**********************************************************************
 * File : system.cpp
 * Date : 2008-2016
 * Author :  Alexis Polti/Tarik Graba
 *
 * This program is released under the GNU public license
 * Télécom ParisTECH
 *
 * Testbench pour video_in
 **********************************************************************/

#include <systemc.h>
#include <sstream>
#include "video_in.h"
#include "video_out.h"
#include "filtre.h"
#include "zoom.h"

#define TRACE(x) sc_trace(my_trace_file, x, #x)

#define IS_ZOOM
#define IS_FILTRE
#define EDGE_DETECTION

/***************************************************
 *	MAIN
 **************************************************/

int sc_main (int argc, char *argv[]) {
    int	ncycles;

    if (argc == 2) {
        std::stringstream arg1(argv[1]);
        arg1 >> ncycles;
    } else {
        cout
           << endl
           << "Le nombre de cycles de simulation doit être passé en argument (-1 pour une simulation illimitée)"
           << endl;
        exit(1);
    }

    /******************************************************
     *      Déclaration des signaux
     *****************************************************/

    /* La période de l'horloge du signal vidéo */
    sc_time pix_period(74, SC_NS);

    sc_clock                        signal_clk("Clock", pix_period);
    sc_signal<bool>                 signal_resetn;

    sc_signal<bool>                 signal_vref_1, signal_href_1, signal_vref_2, signal_href_2, signal_vref_3, signal_href_3;
    sc_signal<unsigned char>        signal_pixel_1, signal_pixel_2, signal_pixel_3;

    /********************************************************
     *	Instanciation des modules
     *******************************************************/
    VIDEO_IN video_in("VIDEO_GEN");
    VIDEO_OUT video_out("VIDEO_DISP");

#if defined(IS_ZOOM)
    ZOOM zoom("ZOOM");
#endif

#if defined(IS_FILTRE)
    #if defined(EDGE_DETECTION)
    double edge[R][R] = { {0,1,0},
                          {1,-4,1},
                          {0,1,0}
                        }; 
    FILTRE filtre("EDGE",edge);
    #else
    double moy[R][R] = { {1,1,1},
                         {1,1,1},
                         {1,1,1}
                       }; 
    FILTRE filtre("MOY",moy);
    #endif
#endif

    /*********************************************************
     *	Connexion des composants
     ********************************************************/

    video_in.clk       ( signal_clk     );
    video_in.reset_n   ( signal_resetn  );
    video_in.href      ( signal_href_1  );
    video_in.vref      ( signal_vref_1  );
    video_in.pixel_out ( signal_pixel_1 );

#if !defined (IS_ZOOM) && !defined (IS_FILTRE)
    video_out.clk      ( signal_clk     );
    video_out.reset_n  ( signal_resetn  );
    video_out.pixel_in ( signal_pixel_1 );
    video_out.href     ( signal_href_1  );
    video_out.vref     ( signal_vref_1  );

#elif defined (IS_ZOOM) && !defined (IS_FILTRE)
    zoom.clk           ( signal_clk     );
    zoom.reset_n       ( signal_resetn  );
    zoom.h_in          ( signal_href_1  );
    zoom.v_in          ( signal_vref_1  );
    zoom.p_in          ( signal_pixel_1 );
    zoom.h_out         ( signal_href_2  );
    zoom.v_out         ( signal_vref_2  );
    zoom.p_out         ( signal_pixel_2 );

    video_out.clk      ( signal_clk     );
    video_out.reset_n  ( signal_resetn  );
    video_out.pixel_in ( signal_pixel_2 );
    video_out.href     ( signal_href_2  );
    video_out.vref     ( signal_vref_2  );

#elif !defined (IS_ZOOM) && defined (IS_FILTRE)
    filtre.clk           ( signal_clk     );
    filtre.reset_n       ( signal_resetn  );
    filtre.h_in          ( signal_href_1  );
    filtre.v_in          ( signal_vref_1  );
    filtre.p_in          ( signal_pixel_1 );
    filtre.h_out         ( signal_href_2  );
    filtre.v_out         ( signal_vref_2  );
    filtre.p_out         ( signal_pixel_2 );

    video_out.clk      ( signal_clk     );
    video_out.reset_n  ( signal_resetn  );
    video_out.pixel_in ( signal_pixel_2 );
    video_out.href     ( signal_href_2  );
    video_out.vref     ( signal_vref_2  );

#elif defined (IS_ZOOM) && defined (IS_FILTRE)
    zoom.clk           ( signal_clk     );
    zoom.reset_n       ( signal_resetn  );
    zoom.h_in          ( signal_href_1  );
    zoom.v_in          ( signal_vref_1  );
    zoom.p_in          ( signal_pixel_1 );
    zoom.h_out         ( signal_href_2  );
    zoom.v_out         ( signal_vref_2  );
    zoom.p_out         ( signal_pixel_2 );

    filtre.clk           ( signal_clk     );
    filtre.reset_n       ( signal_resetn  );
    filtre.h_in          ( signal_href_2  );
    filtre.v_in          ( signal_vref_2  );
    filtre.p_in          ( signal_pixel_2);
    filtre.h_out         ( signal_href_3  );
    filtre.v_out         ( signal_vref_3  );
    filtre.p_out         ( signal_pixel_3 );

    video_out.clk      ( signal_clk     );
    video_out.reset_n  ( signal_resetn  );
    video_out.pixel_in ( signal_pixel_3 );
    video_out.href     ( signal_href_3  );
    video_out.vref     ( signal_vref_3  );
#endif

    /*********************************************************
     *	Traces
     ********************************************************/

    /* fichier de traces */
    sc_trace_file * my_trace_file;
    my_trace_file = sc_create_vcd_trace_file ("simulation_trace");
    my_trace_file->set_time_unit(1,SC_NS);

    /* chronogrammes signaux CLK et NRESET */
    TRACE( signal_clk );
    TRACE( signal_resetn );

    /* chronogrammes video */
    TRACE( signal_href_1 );
    TRACE( signal_vref_1 );
    TRACE( signal_pixel_1 );
#if (defined (IS_ZOOM) && !defined (IS_FILTRE)) || (!defined (IS_ZOOM) && defined (IS_FILTRE))
    TRACE( signal_href_2 );
    TRACE( signal_vref_2 );
    TRACE( signal_pixel_2 );
#elif defined (IS_ZOOM) && defined (IS_FILTRE)
    TRACE( signal_href_2 );
    TRACE( signal_vref_2 );
    TRACE( signal_pixel_2 );
    TRACE( signal_href_3 );
    TRACE( signal_vref_3 );
    TRACE( signal_pixel_3 );
    TRACE( zoom.big_width );
    TRACE( zoom.big_width_set );
    TRACE( zoom.nb_p_received );
#endif
    
#undef TRACE

    /*********************************************************
     *	Simulation
     ********************************************************/

    /* Initialisation de la simulation */
    sc_start(SC_ZERO_TIME);
    signal_resetn = true;
    sc_start(10*signal_clk.period());

    /* Génération d'un reset */
    signal_resetn = false;
    sc_start(10*signal_clk.period());
    signal_resetn = true;

    /* Lancement de la simulation */
    if(ncycles >= 0) {
       cout << "Simulation lancée pour " << ncycles << " cycle de " << signal_clk.period() << endl;
       sc_start(ncycles * signal_clk.period());
    } else {
       cout << "Simulation lancée en continu (CTRL-C pour l'arrêter)" << endl;
       sc_start();
    }

    cout << "Fin de la simulation @ " << sc_time_stamp() << endl;

    /* Close trace file */
    sc_close_vcd_trace_file (my_trace_file);

    return EXIT_SUCCESS;
}

