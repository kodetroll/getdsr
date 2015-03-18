/* getdsr.c - Reads the DSR pin of the specified port and shows this on
 * screen. Repeats in loop. Requires ezV24 Serial lib to compile.
 *
 * (C) 2014 KB4OID Labs, A Division of Kodetroll Heavy Industries
 * Author: Kodetroll (KB4OID)
 * Date Created: January 13, 2014
 * Project: Serial Tools???
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#ifdef UNINSTALLED
#include "ezV24.h"
#else
#include <ezV24/ezV24.h>
#endif

char PortName[V24_SZ_PORTNAME]={'\0'};
v24_port_t *Port=NULL;
unsigned int OpenFlags=V24_STANDARD;
int DsrCurrState=V24_DSRCTS_UNKNOWN;

void dumpErrorMessage ( int rc );

int main( int argc, char* argv[] )
{
    int HelpOnly=0;
    int TestNr;
    int Done=0;
    int rc;

    fprintf(stderr,"ezV24 library -- simple test application\ncompiled: "__DATE__"\n\n");
    optind=1;     /* start without prog-name */
    do
    {
        switch( getopt(argc,argv,"lnHNrxdhwbp:t:") )
        {
	    case 'p':
		if ( optarg )
		{
		    strncpy(PortName,optarg,V24_SZ_PORTNAME);
		    PortName[V24_SZ_PORTNAME]='\0';
		    fprintf(stderr,"info: use port `%s'\n",PortName);
		}
		break;
	    case 't':
		break;
	    case 'l':
		break;
	    case 'n':
		break;
	    case 'b':
		break;
	    case 'r':
		break;
	    case 'x':
		break;
	    case 'd':
		break;
	    case 'H':
		break;
            case 'h':     // user want's help
            case '?':     // getopt3() reports invalid option
//                usage();
                exit(1);
            default:
                Done=1;
	}
    } while (!Done);

    if ( *PortName=='\0' )
    {
	fputs("error: you have to specify a portname using parm `-p'!\n",stderr);
	return 1;
    }
//    installSignalhandler();

    /* First we have to open the port.
     */
    if ( PortName[0]=='\0' )
    {
	fputs("error: use option -p to specify a valid port!\n",stderr);
	return 1;
    }
    Port=v24OpenPort(PortName,OpenFlags);
    if ( Port==NULL )
    {
	rc = v24QueryErrno(Port);

	fputs("error: sorry, open failed!\n",stderr);
	fprintf(stderr,"error: %d!\n",rc);
	dumpErrorMessage(rc);
	return 1;
    }
    fputs("info: port opened!\n",stderr);

    /* than we have to configure the port.
     */
    rc=v24SetParameters(Port,V24_B9600,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
	dumpErrorMessage(rc);
	v24ClosePort(Port);
	return 1;
    }
    fputs("info: parameter set to 9600,8,N,1\n",stderr);

    while(1)
    {
        rc = v24GetDSR(Port, &DsrCurrState);
	printf("DSR: %d\n",rc);
    }

    /* At the end of all the stuff, we have close the port. ;-)
     */
    rc=v24ClosePort(Port);
    if ( rc!=V24_E_OK )
	dumpErrorMessage(rc);
    else
	fputs("info: port closed!\n",stderr);
    return 0;
}



void dumpErrorMessage ( int rc )
{
    switch ( rc )
    {
	case V24_E_OK: fputs("error-msg: V24_E_OK\n",stderr); break;
	case V24_E_ILLBAUD: fputs("error-msg: V24_E_ILLBAUD\n",stderr); break;
	case V24_E_ILLDATASZ: fputs("error-msg: V24_E_ILLDATASZ\n",stderr); break;
	case V24_E_ILLHANDLE: fputs("error-msg: V24_E_ILLHANDLE\n",stderr); break;
	case V24_E_ILLTIMEOUT: fputs("error-msg: V24_E_ILLTIMEOUT\n",stderr); break;
	case V24_E_OPEN_LOCK: fputs("error-msg: V24_E_OPEN\n",stderr); break;
	case V24_E_CREATE_LOCK: fputs("error-msg: V24_E_CREATE_LOCK\n",stderr); break;
	case V24_E_KILL_LOCK: fputs("error-msg: V24_E_KILL_LOCK\n",stderr); break;
	case V24_E_LOCK_EXIST: fputs("error-msg: V24_E_LOCK_EXIST\n",stderr); break;
	case V24_E_NOMEM: fputs("error-msg: V24_E_NOMEM\n",stderr); break;
	case V24_E_NULL_POINTER: fputs("error-msg: V24_E_NULL_POINTER\n",stderr); break;
	case V24_E_OPEN: fputs("error-msg: V24_E_OPEN\n",stderr); break;
	case V24_E_READ: fputs("error-msg: V24_E_READ\n",stderr); break;
	case V24_E_WRITE: fputs("error-msg: V24_E_WRITE\n",stderr); break;
	case V24_E_NOT_IMPLEMENTED: fputs("error-msg: V24_E_NOT_IMPLEMENTED\n",stderr); break;
	case V24_E_DBG_STALE_LOCK: fputs("debug-msg: V24_E_DBG_STALE_LOCK\n",stderr); break;
	default:  fputs("error-msg undefined?!?!\n",stderr); break;
    }
}


