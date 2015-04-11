/*
 This code is intended for use with the Max/MSP SDK

 Redistribution and use in source and binary forms, with or without modification, are permitted as long as the following disclaimer is included:

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/


#include "maxcpp6.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//basic TCP client for *nix systems (tested on Mac OSX 10.7)

using namespace std;

class TcpClient : public MaxCpp6<TcpClient> {

    public:
    
        TcpClient( t_symbol * sym, long ac, t_atom * av ) {
            setupIO( 1, 2 ); //inlets / outlets
            _port = 1234; //set default port and hostname
            _hostname = "localhost";
        }
        
        ~TcpClient() {}	
        
        //bang: reads from socket, TO DO: check for disconnection
        void bang( long inlet ) { 
            if ( _socket ) {
                memset( _inBuf, 0, sizeof( _inBuf ) );
                _inMsg = read( _socket, _inBuf, 2047 );
                if ( _inMsg <= 1 ) {
                    return;
                }
                else {
                    //post("%s\n",_inBuf);
                    t_atom argv[1];
                    atom_setsym( argv, gensym( _inBuf ) );
                    outlet_anything( m_outlets[0], gensym("msg"), 1, argv );
                }
            }
            else {
                return;
            }
        }
    
        //send message to server - TO DO: modify to send multipart message
        void send(long inlet, t_symbol * s, long ac, t_atom * av) {
            _outMsg = write( _socket, atom_getsym(av)->s_name, strlen( atom_getsym(av)->s_name ) );
            if ( _outMsg < 0 ) 
                error("ERROR writing to socket");
        }
    
        //connect to server
        void open_conn( long inlet, t_symbol * s, long ac, t_atom * av ) {
            _socket = socket( AF_INET, SOCK_STREAM, 0 ); //streaming socket
            fcntl( _socket, F_SETFL, O_NONBLOCK ); //set socket to non-blocking
            if ( _socket < 0 ) 
                post("ERROR: could not open socket");
            _server = gethostbyname( _hostname.c_str() );
            if ( _server == NULL ) {
                post( "ERROR: no such host" );
                exit(0);
            }
            bzero( ( char * ) &_servAddr, sizeof( _servAddr ) );
            _servAddr.sin_family = AF_INET;
            bcopy( (char *)_server->h_addr, 
                 (char *)&_servAddr.sin_addr.s_addr,
                 _server->h_length);
            _servAddr.sin_port = htons( _port );
            
            //connect to _server - TO DO: better error handling
            connect( _socket, (struct sockaddr *) &_servAddr, sizeof( _servAddr ) );
            //outlet_anything( m_outlets[1], gensym( "" ), ac, av );
        }
    
        //disconnect from _server
        void close_conn( long inlet, t_symbol * s, long ac, t_atom * av ) {
            close(_socket);
            _socket = NULL;
            //outlet_anything(m_outlets[1], gensym(""), ac, av);
        }
    
        void set_port(long inlet, long v) {
            _port = v;
            post( "port set: %i", v );
        }
    
        void set_hostname(long inlet, t_symbol * s, long ac, t_atom * av) {
            _hostname = string( atom_getsym(av)->s_name );
            string output = "hostname set: " + _hostname;
            post( output.c_str() );
        }
    
    private:
    
        int                 _socket;
        int                 _port;
        int                 _inMsg;
        int                 _outMsg;
        string              _hostname;
        struct sockaddr_in  _servAddr;
        struct hostent *    _server;
        char                _inBuf[2048];
        bool                _bIsConnected;

};

C74_EXPORT int main(void) {

    // create a class with the given name:
	TcpClient::makeMaxClass("TcpClient");
	REGISTER_METHOD(TcpClient, bang);
    REGISTER_METHOD_GIMME(TcpClient, send);
	REGISTER_METHOD_GIMME(TcpClient, open_conn);
    REGISTER_METHOD_GIMME(TcpClient, close_conn);
    REGISTER_METHOD_LONG(TcpClient, set_port);
    REGISTER_METHOD_GIMME(TcpClient, set_hostname);
}