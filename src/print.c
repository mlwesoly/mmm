/*
 *	  C
 *
 *	  This program is free software; you can redistribute it and/or modify
 *	  it under the terms of the GNU General Public License as published by
 *	  the Free Software Foundation; either version 2 of the License, or
 *	  (at your option) any later version.
 *
 *	  This program is distributed in the hope that it will be useful,
 *	  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	  GNU General Public License for more details.
 *
 *	  You should have received a copy of the GNU General Public License
 *	  along with this program; if not, write to the Free Software
 *	  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


//#ifdef HAVE_CONFIG_H
//# include "config.h"
//#endif

#include <geanyplugin.h>
// #include "mmm.h"
//#include <dislin.h>

// #include <errno.h>
#include <glib/gstdio.h>

#include <stdlib.h>
// #include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <glib-2.0/glib.h>
// #include <signal.h>
// #include <math.h>
// #include <locale.h>

//#include "dislin.h"
// #include "print.h"
#include "matrixreader.h"


//static GtkWidget *main_menu_item = NULL;

static float xray[300], yray[300], y1ray[300], y2ray[300], zmat[50][50];
static char  cl1[500], cl2[500];
static int id_lis1, id_lis2, id_pbut;


static int n_rows = 200;
static int n_cols = 50;
//static float kernel[300][50];

// TODO : falls erste zeile namen hat, einlesen für die legende?
// oder legende standardmäßig mit spalte 1, spalte 2 ...beschriften



static float getMax(int c, int n, float a[200][50])
    {
        //for (int i = 0; i < n; i++)
        //{
        float max = a[1][n];

        for (int j = 1; j <= n; j++)
        {
            // printf("%.f",a[j][c]);
            // fflush(stdout);
            if (a[j][c] > max)
                max = a[j][c];
        }
        // printf("%f ",max);
        //}

        return(max);
    }

static float getMin(int c,int n, float b[200][50])
    {

        //for (int i = 0; i < n; i++) {
        float min = b[1][c];
        for (int j = 1; j <= n; j++)
        {
            if (b[j][c] < min)
                min = b[j][c];
        }
        //  printf("%f ",min);
        // }
        return(min);
    }


int exa_1 (void)
{ 
    // put this part in an seperate thread/pid
    // so geany does not stop to work
    int n = (int) kernel[0][0], i;
    int m = (int) kernel[0][1];
    //if (m<2) return;
    float xsteps,ysteps,xstart,ystart,xstop,ystop;
    float xmax, xmin, ymax, ymin;
    float tempymax,tempymin;
    
    int pid = fork();
    if (pid == 0) {
        // Newly spawned child Process.
        for (i = 0; i < n; i++)
        { xray[i] = (float) kernel[i+1][0];

        // y1ray[i] = (float) kernel[i+1][1];
        //y2ray[i] = (float) ;
        }

        metafl ("cons");
        scrmod ("revers");
        window (20, 20, 1200, 850);
        disini ();
        pagera ();
        complx ();
        axspos (350, 1800); // to the right , down
        axslen (2500, 1500);

        name   ("X-axis", "x");
        name   ("Y-axis", "y");

        labdig (-1, "x");
        ticks  (10, "xy");

        titlin ("Demo", 2);
        //titlin ("SIN(X), COS(X)", 3);

        xmax = getMax(0,n,kernel);
        xmin = getMin(0,n,kernel);
        for (i = 1; i < m; i++){
            tempymax = getMax(i,n,kernel);
            tempymin = getMin(i,n,kernel);
            if(i==1){
                ymin=tempymin;
                ymax=tempymax;
            }else{
                if(ymin>tempymin){
                    ymin = tempymin;
                }
                if(ymax<tempymax){
                    ymax=tempymax;
                }
            }
        }

        xsteps = (xmax-xmin)/8.0;
        ysteps = (ymax-ymin)/8.0;
        xstart = xmin;
        ystart = ymin;
        if(xmin>0){
            xmin=xmin*0.95;
        }
        if(xmax<0){
            xmax=xmax*0.95;
        }else{
            xmax=xmax*1.05;
        }

        if(ymin>0){
            ystart=0.0;
            ymin=0.0;
        }else{
            ymin=ymin*1.05;
        }
        if(ymax>0){
            ymax=ymax*1.05;
        }else{
            ymax=0.0;
        }

        
        graf   (xmin, xmax, xstart, xsteps, ymin, ymax, ystart, ysteps);
        title  ();
        //char *cbuf[50];
        //legini (cbuf,m-1,50);
        
        //legtit (" ");

        color  ("red");
        chncrv ("COLOR");
        inccrv (1);	
        for(int h = 1;h < m; h++)
        {
            // gchar *my_string = g_strdup_printf("%i", h);
            //leglin (cbuf,my_string,h);
            for (i = 0; i < n; i++)
            {
                y1ray[i] = (float) kernel[i+1][h];
            }
            curve  (xray, y1ray, n);
        }
        //legend (cbuf,3);
        color  ("fore");
        dash   ();
        xaxgit ();
        disfin ();
        exit(0);
    }
    else {
        g_printf("This line will be printed\n");
    }

    
}


void printer(GtkMenuItem *menuitem, gpointer user_data)
{
    matrixreader();
	exa_1();
    // msgwin_msg_add(1,1,"MyPlugin loaded on Linux!");
}

